__all__ = ["EndstoneTest"]


def __getattr__(name: str):
    if name == "EndstoneTest":
        from endstone_test.plugin import EndstoneTest

        return EndstoneTest
    raise AttributeError(name)
