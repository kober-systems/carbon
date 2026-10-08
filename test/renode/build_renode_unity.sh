#!/bin/sh
#
# For our renode tests to run we need to compile first all unity tests
# into binaries and then add them to our robot framework test suite
set -eu

cd "$(dirname "$0")/../.."
python3 test/renode/generate_teensy41_repl.py
mkdir -p .pio/build/teensy41
found=0
for dir in test/unity/test_*; do
    found=1
    [ -d "$dir" ] || continue
    suite=${dir##*/}
    flags='-D PLATFORM_ARDUINO_ENABLED -D PLATFORM_TEENSY_ENABLED -D PLATFORM_RENODE_ENABLED -D USB_RAWHID -I include'
    # Suite-specific linker/compiler options live next to the suite, if needed.
    if [ -f "$dir/build_flags" ]; then
        flags="$flags $(tr '\n' ' ' < "$dir/build_flags")"
    fi
    PLATFORMIO_BUILD_FLAGS="$flags" pio test -e teensy41 -f "unity/${suite}" --without-uploading --without-testing
    [ -f .pio/build/teensy41/firmware.elf ] || {
        echo "PlatformIO did not build $suite" >&2
        exit 1
    }
    mv .pio/build/teensy41/firmware.elf ".pio/build/teensy41/$suite.elf"

    # Renode loads a fresh machine for each suite. Keep generated scripts beside
    # their ELF so new suites need no hand-written .resc file.
    cat > ".pio/build/teensy41/$suite.resc" <<EOF
\$name="teensy41-$suite"
\$bin=\$ORIGIN/$suite.elf
\$repl?=\$ORIGIN/../../../test/renode/teensy41.repl

using sysbus
mach create \$name
mach set \$name
machine LoadPlatformDescription \$repl
sysbus LoadELF \$bin
sysbus WriteDoubleWord 0x40080000 0x80000000
sysbus WriteWord \`sysbus GetSymbolAddress "configure_cache"\` 0x4770
sysbus WriteWord \`sysbus GetSymbolAddress "usb_pll_start"\` 0x4770
sysbus WriteWord \`sysbus GetSymbolAddress "configure_external_ram"\` 0x4770
cpu0 PC \`sysbus GetSymbolAddress "ResetHandler"\`
cpu0 SP \`sysbus GetSymbolAddress "_estack"\`
EOF
done
[ "$found" -eq 1 ] || { echo 'No Unity suites found' >&2; exit 1; }
