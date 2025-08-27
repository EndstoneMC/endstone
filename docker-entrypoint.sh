#!/bin/bash
set -e

# Fix permissions of /home/endstone/bedrock_server if needed
if [ -d /home/endstone/bedrock_server ]; then
    chown -R endstone:endstone /home/endstone/bedrock_server
fi

# Exec as non-root user
exec gosu endstone "$@"
