import os
from abc import ABC, abstractmethod


class AbstractLoader(ABC):
    def __init__(self, exec_path):
        self.exec_path = os.path.abspath(exec_path)
        self.lib_path = os.path.abspath(self._find_lib())

        assert os.path.exists(self.exec_path), f"Executable does not exist {self.exec_path}"
        assert os.path.exists(self.lib_path), f"Library does not exist {self.lib_path}"

    @abstractmethod
    def start(self):
        pass

    @abstractmethod
    def stop(self, code: int):
        pass

    @abstractmethod
    def _find_lib(self) -> str:
        pass
