#!/bin/sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
export LD_LIBRARY_PATH="$ROOT/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
if [ ! -x "$ROOT/.venv/bin/python" ] || [ ! -f "$ROOT/.venv/.zernyth-install-complete" ]; then
    echo 'Run ./install.sh first.' >&2
    exit 1
fi
exec "$ROOT/.venv/bin/python" "$ROOT/bin/run.py" "$@"
