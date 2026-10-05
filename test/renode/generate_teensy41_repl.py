#!/usr/bin/env python3
"""
Generate the Renode platform file with absolute Python peripheral paths.

Currently renode does not support using relative files in PythonPeripheral

See https://github.com/renode/renode/issues/127
"""
from pathlib import Path
import re


RENODE_DIR = Path(__file__).resolve().parent
TEMPLATE = RENODE_DIR / "teensy41.repl.template"
OUTPUT = RENODE_DIR / "teensy41.repl"
FILENAME = re.compile(r'(filename:\s*")([^"]+)(")')


def absolute_filename(match: re.Match[str]) -> str:
    filename = Path(match.group(2))
    if filename.is_absolute():
        return match.group(0)
    return f"{match.group(1)}{(RENODE_DIR / filename).resolve()}{match.group(3)}"


OUTPUT.write_text(FILENAME.sub(absolute_filename, TEMPLATE.read_text()))
