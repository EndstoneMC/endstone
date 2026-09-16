import pytest


class FixtureInjection:
    def __init__(self, **kwargs):
        for name, obj in kwargs.items():
            setattr(
                self, name, pytest.fixture(scope="session")(self._create_fixture(obj))
            )

    @staticmethod
    def _create_fixture(obj):
        def fixture_func():
            return obj

        return fixture_func
