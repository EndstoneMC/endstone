# endstone-test

An Endstone plugin that runs pytest-based tests against a live Bedrock Dedicated Server to verify API functionality.

## Overview

This plugin is used for runtime testing of the Endstone API. Unlike the unit tests under `tests/`, which run in
isolation against the built wheel, these tests execute inside a running server, so they can verify actual API
behaviour against the Bedrock Dedicated Server.

## Installation

Install the plugin as a Python package from the project root:

```shell
pip install -e tests/plugin
```

The plugin will be automatically discovered and loaded by Endstone on server startup.

## Usage

The suite is split into three passes, selected by a pytest marker:

| Pass | Marker | When it runs |
|------|--------|--------------|
| Server | `not player and not event` | Automatically, when the plugin is enabled |
| Player | `player` | When a **player** runs `/test` |
| Event | `event` | When anyone runs `/test events` |

The marker is applied automatically from the package a test lives in, so put a test in the right directory rather
than marking it by hand.

Events cannot be constructed from Python and listeners cannot be unregistered, so event coverage works differently:
a single set of listeners is registered once at startup and records a snapshot of every event it sees. A boss bar
shows how many of the tracked events have fired (`Events: 7/75, Next: PlayerJumpEvent`), and the event tests assert
on the recorded snapshots. An event that has not been triggered yet **skips** rather than fails, so a fresh server
is green; use the boss bar and `/test events` to see what is still unexercised.

### Interactive checks

Some API surface needs a real client and cannot be asserted by pytest. These live behind `/test` subcommands:

```
/test                                            run the pytest suite
/test events                                     event checklist, then the event tests
/test form <message|action|modal>                MessageForm / ActionForm / ModalForm and every control
/test player <toast|title|actionbar|kick|particle|sound>
/test inv <mainhand|offhand|meta>                PlayerInventory hand slots and ItemMeta
/test spawn <actor: entity_type>                 Dimension.spawn_actor
/test map                                        a map drawn by ImageRenderer
/test block <block: block> [states: block_states]
/test chunk [x: int] [z: int]                    Dimension.load_chunk, polled across real ticks
/test sender
/test broadcast
```

`/test chunk` is the only way to prove `load_chunk` actually loads. Chunk generation is tick-driven,
and the pytest pass runs on the blocked server thread, so no tick can land during it — the pytest
cases can only assert that the request was accepted. This subcommand loads a far chunk, then polls
`is_chunk_loaded` once per tick and reports how many ticks it took before releasing it again.

## Project Structure

```
tests/plugin/
├── pyproject.toml                    # Package configuration
├── README.md
└── src/
    └── endstone_test/
        ├── __init__.py
        ├── plugin.py                 # Plugin entry point and the three pytest passes
        ├── command_executor.py       # The interactive /test subcommands
        ├── recorder.py               # EventRecorder: snapshots, boss bar, skip-if-untriggered
        ├── image_renderer.py         # MapRenderer drawing a generated test pattern
        ├── listeners/                # One listener per event category, feeding the recorder
        │   ├── event_listener.py     # Base class; declares the events it tracks
        │   ├── actor_event_listener.py
        │   ├── block_event_listener.py
        │   ├── level_event_listener.py
        │   ├── player_event_listener.py
        │   ├── server_event_listener.py
        │   └── weather_event_listener.py
        └── tests/
            ├── conftest.py           # Runtime-context fixtures and marker assignment
            ├── *.py                  # Server tests, no player required
            ├── player/               # Tests needing a connected player
            └── event/                # Tests asserting on recorded events
```

## Writing Tests

Tests use pytest and have access to the live server through fixtures:

```python
from endstone import Server

def test_server_name(server: Server) -> None:
    """Verify server name is 'Endstone'."""
    assert server.name == "Endstone"
```

### Available Fixtures

- `server` — the Endstone `Server` instance
- `plugin` — the `EndstoneTest` plugin instance
- `player` — the player who ran `/test` (only in the player pass)
- `recorder` — the `EventRecorder` holding what every listener has seen

A fixture that is not available for the current pass skips the test rather than failing it.

### Conventions

- Put a test needing a connected player in `tests/player/`, and one asserting on an event in `tests/event/`.
- Clean up after yourself. Prefer a yield-fixture that restores state before **and** after, and asserts the
  post-state, over restoring inline.
- There is no API to unregister an event listener, so never register one inside a test — each `/test` run would
  stack another copy. Register once at module level, guarded by a flag.
- Use `recorder.require("SomeEvent")` in an event test: it returns the recorded snapshots, or skips when the event
  has not fired yet.

## Dependencies

- `pytest` — test framework
