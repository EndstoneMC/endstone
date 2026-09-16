# endstone-test

An Endstone plugin that runs pytest-based tests against a live Bedrock Dedicated Server to verify API functionality.

## Overview

This plugin is used for runtime testing of the Endstone API. Unlike the unit tests under `tests/`, which run in
isolation, these tests execute inside a running server, so they can verify actual API behaviour against the Bedrock
Dedicated Server.

## Installation

Install the plugin as a Python package from the project root:

```shell
pip install -e tests/plugin
```

The plugin will be automatically discovered and loaded by Endstone on server startup.

## Usage

| Suite | When it runs |
|-------|--------------|
| `tests/on_enable` | Automatically, when the plugin is enabled |
| `tests/on_player_join` | Each time a player joins |

Event listeners are registered at startup and log the first time each event fires. A boss bar shows how many of the
tracked events have fired.

Interactive probes that need a real client are `/test` subcommands:

```
/test form <message|action|modal>
/test sender
/test player <toast|title|kick|particle|sound>
/test block <block: block> [blockStates: block_states]
/test broadcast
/test inv <mainhand|offhand|meta>
/test spawn <entity: entity_type>
```

## Project Structure

```
tests/plugin/
├── pyproject.toml
├── README.md
└── src/
    └── endstone_test/
        ├── plugin.py                 # Plugin entry point
        ├── command_executor.py       # /test subcommands
        ├── image_renderer.py         # Map renderer used by the inventory tests
        ├── test_helper.py            # Injects server, plugin and player fixtures
        ├── listeners/                # Event listeners
        └── tests/
            ├── on_enable/
            └── on_player_join/
```

## Writing Tests

Tests use pytest and have access to the live `server` and `plugin` fixtures, plus `player` in `on_player_join`:

```python
from endstone import Server

def test_server_name(server: Server) -> None:
    assert server.name == "Endstone"
```
