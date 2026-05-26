#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
cd "$SCRIPT_DIR"

WHEEL=$(ls -1 endstone-*.whl 2>/dev/null | head -n 1 || true)
if [[ -z "$WHEEL" ]]; then
    echo "Couldn't find an endstone wheel next to $SCRIPT_DIR/start.sh."
    echo "Please re-extract the portable archive without removing the .whl file."
    exit 1
fi

if ! command -v uv >/dev/null 2>&1; then
    if [[ -x "$HOME/.local/bin/uv" ]]; then
        export PATH="$HOME/.local/bin:$PATH"
    else
        echo "Installing uv..."
        if command -v curl >/dev/null 2>&1; then
            curl -LsSf https://astral.sh/uv/install.sh | sh
        elif command -v wget >/dev/null 2>&1; then
            wget -qO- https://astral.sh/uv/install.sh | sh
        else
            echo "Neither curl nor wget is available. See https://docs.astral.sh/uv/getting-started/installation/ for manual installation."
            exit 1
        fi
        export PATH="$HOME/.local/bin:$PATH"
    fi
fi

exec uv run --python 3.13 --with "./$WHEEL" endstone -s . -i "$@"
