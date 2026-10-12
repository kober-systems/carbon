#!/bin/sh
# Manual only: sh fuzz/run.sh (Ctrl-C to stop, rerun to resume).
set -eu
cd "$(dirname "$0")/.."
[ -x .pio/fuzz/delimiter_framed_io ] || { echo 'Run sh fuzz/build.sh first' >&2; exit 1; }
command -v afl-fuzz >/dev/null 2>&1 || { echo 'afl-fuzz not found' >&2; exit 1; }
# AFL++ automatically resumes an existing output directory with -i -.
if [ -d .pio/fuzz/afl-out/default/queue ]; then
    exec afl-fuzz -m none -i - -o .pio/fuzz/afl-out -- .pio/fuzz/delimiter_framed_io
fi
exec afl-fuzz -m none -i .pio/fuzz/corpus -o .pio/fuzz/afl-out -- .pio/fuzz/delimiter_framed_io
