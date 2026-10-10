import pytest


class FixtureInjection:
    def __new__(cls, **kwargs):
        fixtures = {
            name: staticmethod(pytest.fixture(scope="session", name=name)(cls._create_fixture(obj)))
            for name, obj in kwargs.items()
        }
        return super().__new__(type(cls.__name__, (cls,), fixtures))

    @staticmethod
    def _create_fixture(obj):
        def fixture_func():
            return obj

        return fixture_func
