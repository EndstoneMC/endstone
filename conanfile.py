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
    default_options = {
        "shared": False,
        "fPIC": True,
        "capstone/*:arm": False,
        "capstone/*:m68k": False,
        "capstone/*:mips": False,
        "capstone/*:ppc": False,
        "capstone/*:sparc": False,
        "capstone/*:sysz": False,
        "capstone/*:xcore": False,
        "capstone/*:tms320c64x": False,
        "capstone/*:m680x": False,
        "capstone/*:evm": False,
        "lief/*:with_frozen": False,
        "lief/*:with_json": False,
        "lief/*:with_c_api": False,
        "lief/*:with_art": False,
        "lief/*:with_dex": False,
        "lief/*:with_macho": False,
        "lief/*:with_oat": False,
        "lief/*:with_pe": False,
        "lief/*:with_vdex": False,
    }

    exports_sources = "CMakeLists.txt", "src/*", "include/*", "tests/*"

    def set_version(self):
        if self.version:
            return

        git = Git(self)
        tag = git.run("describe --tags --long")

        import re

        tag, num_commits, commit_hash = re.match(r"^v?(\S+)-(\d+)-g([a-f0-9]+)$", tag).groups()
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
    def _min_clang_compiler_version(self):
        # NOTE: the latest bedrock server for Linux is compiled with Clang 15.0.7,
        # but it should be ABI compatible with Clang 5
        return 5

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
            if not compiler == "clang" or Version(compiler_version) < self._min_clang_compiler_version:
                raise ConanInvalidConfiguration(
                    f"{self.ref} requires Clang compiler version >= {self._min_clang_compiler_version} on Linux."
                )
            if not compiler.libcxx == "libc++":
                raise ConanInvalidConfiguration(f"{self.ref} requires C++ standard libraries libc++ on Linux.")
        else:
            raise ConanInvalidConfiguration(f"{self.ref} can only not be built on {self.settings.os}.")

    def requirements(self):
        self.requires("spdlog/1.12.0")
        self.requires("fmt/[>=10.1.1]", transitive_headers=True, transitive_libs=True)
        self.requires("pybind11/2.11.1")
        self.requires("funchook/1.1.3")
        self.requires("magic_enum/0.9.5")
        if self.settings.os == "Linux":
            self.requires("lief/0.10.1")

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
        self.cpp_info.components["headers"].libs = []
        self.cpp_info.components["headers"].libdirs = []
        self.cpp_info.components["headers"].set_property("cmake_target_name", "endstone::headers")
        self.cpp_info.components["headers"].requires = ["fmt::fmt"]

        self.cpp_info.components["core"].libs = ["endstone_core"]
        self.cpp_info.components["core"].set_property("cmake_target_name", "endstone::core")
        self.cpp_info.components["core"].requires = ["spdlog::spdlog"]
        self.cpp_info.components["core"].defines = ["PYBIND11_USE_SMART_HOLDER_AS_DEFAULT"]
        if self.settings.os == "Linux":
            self.cpp_info.components["core"].system_libs.extend(["dl", "stdc++fs"])

        self.cpp_info.components["runtime"].libs = ["endstone_runtime"]
        self.cpp_info.components["runtime"].set_property("cmake_target_name", "endstone::runtime")
        self.cpp_info.components["runtime"].requires = [
            "core",
            "funchook::funchook",
            "magic_enum::magic_enum",
            "pybind11::pybind11",
        ]
        if self.settings.os == "Linux":
            self.cpp_info.components["runtime"].requires.extend(["lief::lief"])
        if self.settings.os == "Windows":
            self.cpp_info.components["runtime"].system_libs.extend(["dbghelp"])
