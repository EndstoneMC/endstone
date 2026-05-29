import os
import sys

from conan import ConanFile
from conan.errors import ConanInvalidConfiguration
from conan.tools.build import check_min_cppstd
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout


class EndstoneRecipe(ConanFile):
    build_policy = "never"
    name = "endstone"
    package_type = "library"
    license = "Apache-2.0"
    url = "https://github.com/EndstoneMC/endstone"
    homepage = "https://github.com/EndstoneMC/endstone"
    description = "Endstone offers a plugin API for Bedrock Dedicated Servers, supporting both Python and C++."
    topics = ("plugin", "python", "c++", "minecraft", "bedrock")

    settings = "os", "arch", "compiler", "build_type"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {
        "shared": False,
        "fPIC": True,
        "boost/*:header_only": True,
        "date/*:header_only": True,
        "raknet/*:minecraft_version": "r26u3",
    }

    exports_sources = "CMakeLists.txt", "src/*", "include/*", "tests/*"

    @property
    def _min_cppstd(self):
        return 20

    @property
    def _with_devtools(self):
        return self.settings.os == "Windows"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)

    def requirements(self):
        self.requires("base64/0.5.2")
        self.requires("boost/1.86.0")
        self.requires("concurrentqueue/1.0.4")
        self.requires("cpptrace/1.0.4")
        self.requires("date/3.0.4")
        self.requires("entt/3.15.0")
        self.requires("expected-lite/0.8.0")
        self.requires("funchook/1.1.3")
        self.requires("glm/1.0.1")
        self.requires("magic_enum/0.9.7")
        self.requires("ms-gsl/4.2.0")
        self.requires("nlohmann_json/3.12.0")
        self.requires("pybind11/3.0.1")
        self.requires("raknet/4.081-mojang")
        self.requires("replxx/0.0.4")
        self.requires("sentry-native/0.14.0")
        self.requires("spdlog/1.15.3")
        self.requires("tomlplusplus/3.3.0")
        self.requires("zstr/1.0.7")

        if self.settings.os == "Windows":
            self.requires("detours/cci.20220630")
        elif self.settings.os == "Linux":
            self.requires("libelf/0.8.13")

        if self._with_devtools:
            self.requires("glfw/3.4")
            self.requires("imgui/1.92.8-docking")

    def build_requirements(self):
        self.test_requires("gtest/1.17.0")

    def validate(self):
        check_min_cppstd(self, self._min_cppstd)

        if self.settings.arch != "x86_64":
            raise ConanInvalidConfiguration(
                f"{self.ref} can only be built on x86_64. {self.settings.arch} is not supported."
            )

        if self.settings.os not in ("Windows", "Linux"):
            raise ConanInvalidConfiguration(
                f"{self.ref} can only be built on Windows or Linux. {self.settings.os} is not supported."
            )

        if self.settings.os == "Linux" and self.settings.compiler.libcxx != "libc++":
            raise ConanInvalidConfiguration(f"{self.ref} requires libc++ on Linux.")

    def generate(self):
        CMakeDeps(self).generate()

        tc = CMakeToolchain(self)
        if self.version:
            tc.variables["ENDSTONE_VERSION_FULL"] = str(self.version)
        if self._with_devtools:
            tc.variables["ENDSTONE_ENABLE_DEVTOOLS"] = True
        sentry_bin = os.path.join(self.dependencies["sentry-native"].package_folder, "bin")
        tc.variables["SENTRY_NATIVE_BIN_DIR"] = sentry_bin.replace("\\", "/")
        # Pin pybind11's Python search to the interpreter running conan-py-build
        python_root = sys.base_exec_prefix.replace("\\", "/")
        tc.variables["Python_ROOT_DIR"] = python_root
        tc.variables["Python3_ROOT_DIR"] = python_root
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install(component="endstone_wheel")
