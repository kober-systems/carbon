# Minimal FlexSPI IP-command model for Teensy 4.1's EEPROM emulation.
# AHB reads and IP writes share the mapped flash at 0x60000000.
from Antmicro.Renode.Core import EmulationManager

FLASH_BASE = 0x60000000
EEPROM_BASE = 0x607C0000
SECTORS = 63
SECTOR_SIZE = 4096
IPCMDDONE = 1
IPTXWE = 1 << 6

if request.IsInit:
    bus = EmulationManager.Instance.CurrentEmulation.Machines[0].SystemBus
    # The EEPROM scan stops at the first erased halfword in each sector.
    # Firmware is loaded below this reserved region after platform creation.
    for sector in range(SECTORS):
        bus.WriteWord(EEPROM_BASE + sector * SECTOR_SIZE, 0xFFFF)
    ipcr0 = 0
    ipcr1 = 0
    lut60 = 0
    intr = 0
    status = 0
    enabled = False
    pending = 0
    written = 0
elif request.IsWrite:
    offset = request.Offset
    value = request.Value
    if offset == 0xA0:
        ipcr0 = value
    elif offset == 0xA4:
        ipcr1 = value
    elif offset == 0x2F0:
        lut60 = value
    elif offset == 0x14:
        intr &= ~value
    elif offset == 0xB0:  # IPCMD: execute LUT sequence 15
        opcode = lut60 & 0xFF
        intr = IPCMDDONE
        if opcode == 0x06:  # write enable
            enabled = True
        elif opcode == 0x05:  # read status
            status = 0
        elif opcode == 0x20 and enabled:  # erase 4K sector
            start = FLASH_BASE + (ipcr0 & 0xFFF000)
            for i in range(SECTOR_SIZE):
                bus.WriteByte(start + i, 0xFF)
            enabled = False
        elif opcode == 0x32 and enabled:  # quad page program
            pending = ipcr1 & 0xFFFF
            written = 0
            intr = IPTXWE if pending else IPCMDDONE
    elif offset == 0x180 and pending:  # TFDR0: 16-bit or 32-bit FIFO write
        width = 2 if request.Length == 2 else 4
        for i in range(min(width, pending - written)):
            address = FLASH_BASE + (ipcr0 & 0xFFFFFF) + written
            old = bus.ReadByte(address)
            bus.WriteByte(address, old & ((value >> (8 * i)) & 0xFF))
            written += 1
        if written == pending:
            pending = 0
            enabled = False
            intr = IPCMDDONE
elif request.IsRead:
    if request.Offset == 0x14:
        request.Value = intr
    elif request.Offset == 0x100:  # RFDR0, read-status result
        request.Value = status
    elif request.Offset == 0xA0:
        request.Value = ipcr0
    elif request.Offset == 0xA4:
        request.Value = ipcr1
    else:
        request.Value = 0
