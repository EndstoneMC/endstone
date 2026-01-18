# endstone-test

An Endstone plugin that runs pytest-based tests against a live Bedrock Dedicated Server to verify API functionality.

## Overview

This plugin is used for runtime testing of the Endstone API. Unlike unit tests that run in isolation, these tests execute within a running server environment, allowing verification of actual API behavior against the Bedrock Dedicated Server.

## Installation

Install the plugin as a Python package from the project root:

```shell
pip install -e tests/endstone_test
```

The plugin will be automatically discovered and loaded by Endstone on server startup.

## Usage

Tests run automatically when the plugin is enabled (after the world loads). Test results are output to the server console.

## Project Structure

```
tests/endstone_test/
├── pyproject.toml                    # Package configuration
├── README.md
└── src/
    └── endstone_test/
        ├── __init__.py
        ├── plugin.py                 # Plugin entry point
        └── tests/
            ├── __init__.py
            ├── conftest.py           # Pytest fixtures (server, plugin)
            └── test_server.py        # Server API tests
```

## Writing Tests

Tests use pytest and have access to the live `server` and `plugin` fixtures:

```python
from endstone import Server

def test_server_name(server: Server) -> None:
    """Verify server name is 'Endstone'."""
    assert server.name == "Endstone"
```

### Available Fixtures

- `server` - The Endstone `Server` instance
- `plugin` - The `EndstoneTest` plugin instance

### Test Categories

Current test coverage includes:

- **Server Information** - Version, protocol, and metadata
- **Server Components** - Logger, language, plugin manager, scheduler
- **Level Access** - World/dimension access
- **Player Management** - Online players, max players
- **Network Configuration** - Ports, online mode
- **Performance Metrics** - TPS, MSPT, tick usage
- **Item Factory** - Item creation APIs
- **Map System** - Map creation and retrieval
- **Broadcasting** - Message broadcast functionality

## Dependencies

- `pytest` - Test framework
- `babel` - Internationalization support
- `pillow` - Image processing (for map tests)
