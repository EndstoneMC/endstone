from conan import ConanFile
from conan.errors import ConanInvalidConfiguration
from conan.tools.build import check_min_cppstd
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
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
        "boost/*:header_only": True,
        "date/*:header_only": True,
        "sentry-native/*:backend": "crashpad",
    }

    exports_sources = "CMakeLists.txt", "src/*", "include/*", "tests/*"

    @property
    def _min_cppstd(self):
        return 20

    @property
    def _min_msvc_compiler_version(self):
        # NOTE: the latest bedrock server for Windows is compiled with MSVC 2022 (v193),
        # but it should be ABI compatible with MSVC 2017 (v191)
        return 191  # Visual Studio 17

    @property
    def _min_clang_compiler_version(self):
        # NOTE: the latest bedrock server for Linux is compiled with Clang 15.0.7,
        # but it should be ABI compatible with Clang 16 (required by crashpad).
        return 16

    @property
    def _is_dev_build(self):
        return "dev" in self.version

    @property
    def _should_enable_devtools(self):
        return self.settings.os == "Windows"

    def validate(self):
        check_min_cppstd(self, self._min_cppstd)

        compiler = self.settings.compiler
        compiler_version = self.settings.compiler.version

        if self.settings.arch != "x86_64":
            raise ConanInvalidConfiguration(
                f"{self.ref} can only be built on x86_64. {self.settings.arch} is not supported."
            )

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
        self.requires("base64/0.5.2")
        self.requires("boost/1.86.0")
        self.requires("concurrentqueue/1.0.4")
        self.requires("cpptrace/0.7.5")
        self.requires("date/3.0.3")
        self.requires("entt/3.14.0")
        self.requires("expected-lite/0.8.0")
        self.requires("fmt/[~10]", transitive_headers=True, transitive_libs=True)
        # self.requires("funchook/1.1.3")
        self.requires("glm/1.0.1")
        self.requires("magic_enum/0.9.7")
        self.requires("ms-gsl/4.1.0")
        self.requires("nlohmann_json/3.11.3")
        self.requires("pybind11/2.13.6")
        self.requires("sentry-native/0.7.17")
        self.requires("spdlog/1.14.1")
        self.requires("tomlplusplus/3.3.0")

        if self.settings.os == "Linux":
            self.requires("libelf/0.8.13")

        if self._should_enable_devtools:
            self.requires("glew/2.2.0")
            self.requires("glfw/3.4")
            self.requires("imgui/1.91.5-docking")
            self.requires("zstr/1.0.7")

        self.test_requires("gtest/1.15.0")

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
        if self.version:
            tc.variables["ENDSTONE_VERSION"] = self.version
        if self._should_enable_devtools:
            tc.variables["ENDSTONE_ENABLE_DEVTOOLS"] = True
        tc.generate()
