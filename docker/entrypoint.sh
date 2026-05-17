#!/usr/bin/env bash
#
# Container entrypoint for the Endstone server.
#
# Remaps the unprivileged "endstone" user to the host-provided PUID/PGID so that
# files written to the bind-mounted /data volume stay owned by the right user,
# then drops privileges and runs the given command.
set -euo pipefail

DATA_DIR="/data"

# If the container was started as a non-root user (e.g. `docker run --user`),
# we cannot remap ids or fix ownership — just run the command as that user.
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

# Take ownership of the data directory. Only chown files that are not already
# owned by the service user, so this stays fast even for large worlds.
mkdir -p "$DATA_DIR"
find "$DATA_DIR" \! -user endstone -exec chown endstone:endstone {} + || true

exec gosu endstone "$@"
