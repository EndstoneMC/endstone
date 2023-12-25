from conan import ConanFile
from conan.tools.build import check_min_cppstd
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conan.tools.scm import Git
from conans.errors import ConanInvalidConfiguration
from conans.model.version import Version


class EndstoneRecipe(ConanFile):
    name = "endstone"
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
    default_options = {"shared": False, "fPIC": True}

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "cmake/*", "endstone_api/*", "endstone_core/*", "endstone_python/*"

    def set_version(self):
        if self.version:
            return

        git = Git(self)
        tag = git.run("describe --tags --long")

        import re

        tag, num_commits, commit_hash = re.match(r"^(\S+)-(\d+)-g([a-f0-9]+)$", tag).groups()
        version = Version(tag)
        value = ".".join(str(i) for i in version.main)
        if version.pre:
            value += f"-{version.pre}"

        if (num_commits := int(num_commits)) > 0:
            value += f".dev{num_commits}"

        if version.build:
            value += f"+{version.build}"

        self.version = value

    @property
    def _min_cppstd(self):
        return 17

    @property
    def _min_msvc_compiler_version(self):
        # NOTE: the latest bedrock server for Windows is compiled with MSVC 2022 (v193),
        # but it should be ABI compatible with MSVC 2017 (v191)
        return 191  # Visual Studio 17

    @property
    def _min_gcc_compiler_version(self):
        # NOTE: the latest bedrock server for Linux is compiled with GCC 7.5.0,
        # but we enforce the GCC version >= 8
        return 8

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
        self.requires("fmt/10.1.1", transitive_headers=True, transitive_libs=True)
        self.requires("spdlog/1.12.0")
        self.requires("pybind11/2.11.1@pybind11/smart_holder", transitive_headers=True)
        self.test_requires("gtest/1.14.0")

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
        tc.variables["ENDSTONE_VERSION"] = self.version
        tc.variables["ENDSTONE_MINECRAFT_VERSION"] = (lambda v: f"{v[0]}.{v[1]}.{v[2]}.{v[3]:02}")(
            [int(a.value) for a in Version(self.version).main]
        )
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
        self.cpp_info.components["core"].requires = ["api", "spdlog::spdlog", "pybind11::pybind11"]
        if self.settings.os == "Linux":
            self.cpp_info.components["core"].system_libs.extend(["dl", "stdc++fs"])
