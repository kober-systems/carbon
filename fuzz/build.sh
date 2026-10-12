#!/bin/sh
# Manual entry point: sh fuzz/build.sh
set -eu
cd "$(dirname "$0")/.."
# Do not inherit CXX: a normal compiler produces a binary AFL++ cannot fuzz.
CXX=afl-clang-fast++
command -v "$CXX" >/dev/null 2>&1 || { echo "Install AFL++ (afl-clang-fast++ required)" >&2; exit 1; }
mkdir -p .pio/fuzz/corpus
AFL_USE_ASAN=1 AFL_USE_UBSAN=1 "$CXX" -std=c++17 -O1 -g -fno-omit-frame-pointer \
  -I libraries/AbstractIO \
  fuzz/delimiter_framed_io.cpp libraries/AbstractIO/DelimiterFramedIO.cpp \
  -o .pio/fuzz/delimiter_framed_io
# Seed only missing files: never overwrite an evolved corpus.
python3 - <<'PY'
from pathlib import Path
out = Path('.pio/fuzz/corpus')
for seed in Path('fuzz/seeds').glob('*.hex'):
    target = out / seed.stem
    if not target.exists():
        target.write_bytes(bytes.fromhex(seed.read_text()))
PY
printf 'Built .pio/fuzz/delimiter_framed_io; corpus: .pio/fuzz/corpus\n'
