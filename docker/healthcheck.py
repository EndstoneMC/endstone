#!/usr/bin/env python3
"""Docker healthcheck for the Endstone server.

Sends a RakNet unconnected-ping to the local Bedrock server port and exits 0
only when a valid unconnected-pong is received back. Set the ``SERVER_PORT``
environment variable if the server does not listen on the default port.
"""

import os
import socket
import struct
import sys
import time

# RakNet packet IDs.
ID_UNCONNECTED_PING = 0x01
ID_UNCONNECTED_PONG = 0x1C

# RakNet OFFLINE_MESSAGE_DATA_ID magic.
OFFLINE_MAGIC = bytes.fromhex("00ffff00fefefefefdfdfdfd12345678")

HOST = "127.0.0.1"
PORT = int(os.environ.get("SERVER_PORT", "19132"))
TIMEOUT = 4.0


def main() -> int:
    ping = (
        struct.pack(">B", ID_UNCONNECTED_PING)
        + struct.pack(">Q", int(time.monotonic() * 1000) & 0xFFFFFFFFFFFFFFFF)
        + OFFLINE_MAGIC
        + struct.pack(">Q", 0)  # client GUID
    )
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
            sock.settimeout(TIMEOUT)
            sock.sendto(ping, (HOST, PORT))
            data, _ = sock.recvfrom(4096)
    except OSError:
        return 1
    return 0 if data[:1] == bytes([ID_UNCONNECTED_PONG]) else 1


if __name__ == "__main__":
    sys.exit(main())
