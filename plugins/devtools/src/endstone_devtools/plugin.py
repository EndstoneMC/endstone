"""Endstone-side plugin: snapshot the registries, then open the GUI out-of-process."""

from __future__ import annotations

import multiprocessing
import os

from endstone.command import Command, CommandSender, ConsoleCommandSender
from endstone.plugin import Plugin

from endstone_devtools.snapshot import collect


class DevToolsPlugin(Plugin):
    api_version = "0.11"
    prefix = "DevTools"

    commands = {
        "devtools": {
            "description": "Open the Endstone DevTools window.",
            "usages": ["/devtools"],
            "aliases": ["dev"],
            "permissions": ["endstone_devtools.command.devtools"],
        }
    }

    permissions = {
        "endstone_devtools.command.devtools": {
            "description": "Allow the use of the /devtools command.",
            "default": "op",
        }
    }

    def on_command(self, sender: CommandSender, command: Command, args: list[str]) -> bool:
        if command.name != "devtools":
            return False

        # The window opens on the server host, so a remote sender could never see it.
        if not isinstance(sender, ConsoleCommandSender):
            sender.send_error_message("This command can only be run from the console.")
            return True

        sender.send_message("Collecting registry data...")
        snapshot = collect(self.server)
        _launch(snapshot)
        sender.send_message("DevTools window opened on the server host.")
        return True


def _launch(snapshot: dict) -> None:
    """Spawn the GUI in its own clean Python process and hand it the snapshot."""
    # Local import keeps the GUI stack (imgui-bundle) out of the server process.
    from endstone_devtools import gui

    ctx = multiprocessing.get_context("spawn")  # never fork the live, multi-threaded server
    preload = os.environ.pop("LD_PRELOAD", None)  # don't inject the server runtime into the GUI (Linux)
    try:
        ctx.Process(target=gui.main, args=(snapshot,), daemon=True).start()
    finally:
        if preload is not None:
            os.environ["LD_PRELOAD"] = preload
