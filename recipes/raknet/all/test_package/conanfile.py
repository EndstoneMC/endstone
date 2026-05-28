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
        raknet_opts = self.dependencies["raknet"].options
        tc.cache_variables["RAKNET_TEST_EXPECTED_PROTOCOL"] = str(raknet_opts.protocol_version)
        tc.cache_variables["RAKNET_TEST_EXPECTED_MAX_INTERNAL_IDS"] = str(raknet_opts.max_internal_ids)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def test(self):
        if can_run(self):
            exe = os.path.join(self.cpp.build.bindir, "test_package")
            self.run(exe, env="conanrun")
