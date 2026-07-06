import os

from conan import ConanFile
from conan.tools.build import can_run
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout


class RakNetTestConan(ConanFile):
    settings = "os", "arch", "compiler", "build_type"
    generators = "CMakeDeps", "VirtualRunEnv"
    test_type = "explicit"

    def requirements(self):
        self.requires(self.tested_reference_str)

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

    @property
    def _is_mojang(self):
        # The mojang variant adds pure virtuals whose RakPeer overrides are left
        # undefined, so a concrete RakPeer cannot be linked into an executable.
        return "mojang" in str(self.tested_reference_str)

    def build(self):
        if self._is_mojang:
            return
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def test(self):
        if self._is_mojang:
            return
        if can_run(self):
            exe = os.path.join(self.cpp.build.bindir, "test_package")
            self.run(exe, env="conanrun")
