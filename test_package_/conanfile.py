import os
import sys

from conan import ConanFile
from conan.tools.build import can_run
from conan.tools.cmake import CMake, cmake_layout, CMakeToolchain, CMakeDeps
from conan.tools.env import Environment, VirtualRunEnv
from conan.tools.files import copy
from conans.errors import ConanInvalidConfiguration


class EndstoneTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"

    # generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires(self.tested_reference_str)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

        env = Environment()
        env.prepend_path("PATH", os.pathsep.join(sys.path))
        # env.append_path("PYTHONPATH", os.path.join(self.build_folder, self.cpp.build.libdirs[0]))
        env.vars(self, scope="run").save_script("testrun")

        run = VirtualRunEnv(self)
        run.generate()

    @property
    def _python_interpreter(self):
        if getattr(sys, "frozen", False):
            return "python"
        return sys.executable

    def test(self):
        if can_run(self):
            copy(  # python package source
                self,
                "*",
                os.path.join(self.source_path, "..", "endstone_python", "python"),
                self.cpp.build.bindir,
            )
            copy(  # python bindings
                self,
                "*",
                os.path.join(self.dependencies["endstone"].package_folder, "endstone"),
                os.path.join(self.cpp.build.bindir, "endstone"),
            )

            if self.settings.os == "Windows":
                copy(self, "test_plugin.dll", self.cpp.build.bindir, "plugins")
            elif self.settings.os == "Linux":
                copy(self, "libtest_plugin.so", self.cpp.build.bindir, "plugins")
            else:
                raise ConanInvalidConfiguration(f"{self.settings.os} is not supported.")

            self.run(os.path.join(self.cpp.build.bindir, "test_server"), env="conanrun")
