import os

from conan import ConanFile
from conan.tools.build import can_run
from conan.tools.cmake import CMake, cmake_layout
from conan.tools.files import copy


class EndstoneTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires(self.tested_reference_str)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def layout(self):
        cmake_layout(self)

    def test(self):
        if can_run(self):
            if self.settings.os == "Windows":
                copy(self, "test_plugin.dll", self.cpp.build.bindir, "plugins")
            elif self.settings.os == "Linux":
                copy(self, "libtest_plugin.so", self.cpp.build.bindir, "plugins")
            else:
                raise NotImplementedError(f"{self.settings.os} is not supported.")

            self.run(os.path.join(self.cpp.build.bindir, "test_server"), env="conanrun")
