import importlib
import logging
import socket

import pytest


@pytest.fixture
def check_server_port(tmp_path):
    cls = importlib.import_module("endstone.cli.base").Bootstrap
    bootstrap = cls.__new__(cls)
    bootstrap._server_path = tmp_path
    bootstrap._logger = logging.getLogger("test")

    def check(text):
        (tmp_path / "server.properties").write_text(text, encoding="utf-8")
        bootstrap._check_server_port()

    return check


@pytest.fixture
def occupied_port():
    dualstack = socket.has_dualstack_ipv6()
    family = socket.AF_INET6 if dualstack else socket.AF_INET
    with socket.create_server(("", 0), family=family, dualstack_ipv6=dualstack) as server:
        yield server.getsockname()[1]


def test_exits_when_nethernet_port_is_in_use(check_server_port, occupied_port, caplog):
    with pytest.raises(SystemExit) as exc_info:
        check_server_port(f"server-port={occupied_port}\ntransport=nethernet\n")

    assert exc_info.value.code == 1
    assert f"Port [{occupied_port}] may be in use by another process." in caplog.text


def test_exits_when_server_ip_port_is_in_use(check_server_port, caplog):
    with socket.create_server(("127.0.0.1", 0)) as server:
        port = server.getsockname()[1]
        with pytest.raises(SystemExit):
            check_server_port(f"server-port={port}\nserver-ip=127.0.0.1\ntransport=nethernet\n")


def test_starts_when_nethernet_port_is_free(check_server_port):
    with socket.create_server(("", 0)) as server:
        port = server.getsockname()[1]

    check_server_port(f"server-port={port}\ntransport=nethernet\n")


def test_raknet_is_left_to_the_server(check_server_port, occupied_port):
    check_server_port(f"server-port={occupied_port}\ntransport=raknet\n")
