from conan import ConanFile
from conan.tools.build import check_min_cppstd
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conans.errors import ConanInvalidConfiguration
from conans.model.version import Version


class EndstoneRecipe(ConanFile):
    name = "endstone"
    version = "0.1.0"
    package_type = "library"

    # Optional metadata
    license = "Apache-2.0"
    url = "https://github.com/EndstoneMC/endstone"
    homepage = "https://github.com/EndstoneMC/endstone"
    description = "Endstone offers a plugin API for Bedrock Dedicated Servers, supporting both Python and C++."
    topics = ("plugin", "python", "c++", "minecraft", "bedrock")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True, "fmt/*:header_only": True}

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "endstone_api/*", "endstone_core/*", "endstone_python/*"

    @property
    def _min_cppstd(self):
        return 17

    @property
    def _min_msvc_compiler_version(self):
        # NOTE: the latest bedrock server for Windows is compiled with MSVC 2022 (193),
        # but it should be ABI compatible with MSVC 2017 (191)
        return 191  # Visual Studio 17

    @property
    def _min_gcc_compiler_version(self):
        # NOTE: the latest bedrock server of Linux is compiled with GCC 7.5.0
        return 7

    def validate(self):
        check_min_cppstd(self, self._min_cppstd)

        compiler = self.settings.compiler
        compiler_version = self.settings.compiler.version

        if self.settings.os == "Windows":
            if not compiler == "msvc" or Version(compiler_version) < self._min_msvc_compiler_version:
                raise ConanInvalidConfiguration(
                    f"{self.ref} requires MSVC compiler version >= {self._min_msvc_compiler_version} on Windows."
                )
        elif self.settings.os == "Linux":
            if not compiler == "gcc" or Version(compiler_version) < self._min_gcc_compiler_version:
                raise ConanInvalidConfiguration(
                    f"{self.ref} requires GCC compiler version >= {self._min_gcc_compiler_version} on Linux."
                )
        else:
            raise ConanInvalidConfiguration(
                f"{self.ref} can only be built on Windows or Linux. {self.settings.os} is not supported."
            )

    def requirements(self):
        self.requires("fmt/10.1.1", transitive_headers=True)
        self.requires("spdlog/1.12.0")
        self.requires("pybind11/2.11.1")

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.test()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.components["api"].libs = []
        self.cpp_info.components["api"].libdirs = []
        self.cpp_info.components["api"].set_property("cmake_target_name", "endstone::api")
        self.cpp_info.components["api"].requires = ["fmt::fmt"]

        self.cpp_info.components["core"].libs = ["endstone_core"]
        self.cpp_info.components["core"].set_property("cmake_target_name", "endstone::core")
        self.cpp_info.components["core"].requires = ["api", "spdlog::spdlog", "pybind11::embed"]
        if self.settings.os == "Linux":
            self.cpp_info.components["core"].system_libs.extend(["dl", "stdc++fs"])
