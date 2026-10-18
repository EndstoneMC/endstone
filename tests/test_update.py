import importlib
import logging

import pytest


@pytest.fixture
def base():
    return importlib.import_module("endstone.cli.base")


@pytest.fixture
def bootstrap(base, tmp_path, monkeypatch):
    cls = type("Bootstrap", (base.Bootstrap,), {"executable_filename": "bedrock_server"})
    bootstrap = cls.__new__(cls)
    bootstrap._server_path = tmp_path
    bootstrap._no_confirm = False
    bootstrap._logger = logging.getLogger("test")
    bootstrap.downloads = 0

    def download(dst):
        bootstrap.downloads += 1

    monkeypatch.setattr(bootstrap, "_download", download)
    return bootstrap


@pytest.fixture
def prompts(base, monkeypatch):
    prompts = []

    def confirm(text, default):
        prompts.append(text)
        return False

    monkeypatch.setattr(base.click, "confirm", confirm)
    return prompts


def test_unknown_version_is_not_reported_as_older(bootstrap, prompts, caplog):
    with pytest.raises(SystemExit) as exc_info:
        bootstrap._update()

    assert exc_info.value.code == 1
    assert len(prompts) == 1
    assert "unknown version" in prompts[0]
    assert "0.0.0" not in prompts[0]
    assert f"only runs Bedrock Dedicated Server v{bootstrap.minecraft_version}" in caplog.text
    assert bootstrap.downloads == 0


def test_older_version_asks_to_update(bootstrap, prompts, caplog):
    (bootstrap.server_path / "version.txt").write_text("0.1", encoding="utf-8")

    with pytest.raises(SystemExit) as exc_info:
        bootstrap._update()

    assert exc_info.value.code == 1
    assert len(prompts) == 1
    assert "An older version of Bedrock Dedicated Server (v0.1)" in prompts[0]
    assert "install the Endstone release that supports it" in caplog.text


def test_newer_version_raises(bootstrap, prompts):
    (bootstrap.server_path / "version.txt").write_text("999.0", encoding="utf-8")

    with pytest.raises(RuntimeError, match="A newer version"):
        bootstrap._update()

    assert prompts == []


def test_supported_version_starts(bootstrap, prompts):
    (bootstrap.server_path / "version.txt").write_text(str(bootstrap.minecraft_version), encoding="utf-8")

    bootstrap._update()

    assert prompts == []
    assert bootstrap.downloads == 0


def test_unknown_version_installs_without_confirm(bootstrap, prompts):
    bootstrap._no_confirm = True

    bootstrap._update()

    assert prompts == []
    assert bootstrap.downloads == 1
