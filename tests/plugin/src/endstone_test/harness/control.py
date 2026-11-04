import json
import queue
import socket
import threading


class Control:
    """JSON-lines link to the out-of-process runner. Reads on a thread, never blocks the server thread."""

    def __init__(self, address: str):
        host, port = address.rsplit(":", 1)
        self._socket = socket.create_connection((host, int(port)), timeout=10)
        self._socket.settimeout(None)
        self._inbox: queue.Queue = queue.Queue()
        self._next_id = 0
        self.replies: dict[int, dict] = {}
        self.closed = False
        threading.Thread(
            target=self._read, name="endstone-test-control", daemon=True
        ).start()

    def send(self, message: dict) -> None:
        if self.closed:
            return
        try:
            self._socket.sendall((json.dumps(message, default=str) + "\n").encode())
        except OSError:
            self.closed = True

    def send_raw(self, data: bytes) -> None:
        """Write bytes to the runner as they are; only for checking how the runner treats a bad line."""
        self._socket.sendall(data)

    def call(self, op: str, **payload) -> int:
        self._next_id += 1
        self.send({"type": "call", "id": self._next_id, "op": op, **payload})
        return self._next_id

    def pump(self) -> list[dict]:
        messages = []
        while True:
            try:
                message = self._inbox.get_nowait()
            except queue.Empty:
                return messages
            if message["type"] == "reply":
                self.replies[message["id"]] = message
            elif message["type"] == "closed":
                self.closed = True
            else:
                messages.append(message)

    def _read(self) -> None:
        try:
            with self._socket.makefile("r", encoding="utf-8") as stream:
                for line in stream:
                    self._inbox.put(json.loads(line))
        except OSError:
            pass
        self._inbox.put({"type": "closed"})
