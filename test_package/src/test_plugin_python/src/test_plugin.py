from endstone.plugin import Plugin


class TestPlugin(Plugin):
    def on_load(self) -> None:
        self.logger.info("on_load is called!")

    def on_enable(self) -> None:
        self.logger.info("on_enable is called!")

    def on_disable(self) -> None:
        self.logger.info("on_disable is called!")
