#!/usr/bin/env bash
# Container entrypoint: remap the endstone user to PUID/PGID so /data stays host-owned, then drop privileges.
set -euo pipefail

DATA_DIR="/data"

# started as non-root: can't remap ids, so run the command as-is
if [ "$(id -u)" -ne 0 ]; then
    exec "$@"
fi

PUID="${PUID:-1000}"
PGID="${PGID:-1000}"

if [ "$(id -g endstone)" != "$PGID" ]; then
    groupmod -o -g "$PGID" endstone
fi
if [ "$(id -u endstone)" != "$PUID" ]; then
    usermod -o -u "$PUID" endstone
fi

# chown only files not already owned by endstone, so large worlds stay fast
mkdir -p "$DATA_DIR"
find "$DATA_DIR" \! -user endstone -exec chown endstone:endstone {} + || true

exec gosu endstone "$@"
