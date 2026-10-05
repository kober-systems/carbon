# Minimal CCM_ANALOG_PLL_ARM register for Teensy Arduino clock initialization.
# The lock bit is hardware-generated; report it immediately in this test model.
if request.IsInit:
    control = 0
elif request.IsWrite:
    if request.Offset == 0:
        control = request.Value
    elif request.Offset == 4:  # SET
        control |= request.Value
    elif request.Offset == 8:  # CLR
        control &= ~request.Value
    elif request.Offset == 12:  # TOG
        control ^= request.Value
elif request.IsRead:
    request.Value = (control | (1 << 31)) if request.Offset == 0 else 0
