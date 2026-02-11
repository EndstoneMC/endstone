import pytest

# =============================================================================
# Runtime context
# =============================================================================

_runtime_context = {}


def set_runtime_context(**kwargs):
    _runtime_context.update(kwargs)


def clear_runtime_context():
    _runtime_context.clear()


# =============================================================================
# Session-scoped fixtures
# =============================================================================


def _make_fixture(name):
    @pytest.fixture(scope="session", name=name)
    def _fixture():
        if name not in _runtime_context:
            pytest.skip(f"{name.capitalize()} not available")
        return _runtime_context[name]

    return _fixture


server = _make_fixture("server")
plugin = _make_fixture("plugin")
player = _make_fixture("player")


# =============================================================================
# Pytest hooks
# =============================================================================


def pytest_configure(config):
    config.addinivalue_line(
        "markers", "player: tests that require a player to be connected"
    )


def pytest_collection_modifyitems(items):
    for item in items:
        if "endstone_test.tests.player." in item.module.__name__:
            item.add_marker("player")
