from endstone._logger import Logger


class PluginLogger(Logger):

    def __init__(self, name: str):
        Logger.__init__(self, name)

    def log(self, level, msg):
        Logger.log(self, level, f"[{self.get_name()}] {msg}")
