import os

from conan import ConanFile
from conan.errors import ConanInvalidConfiguration
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.files import apply_conandata_patches, copy, get, rmdir
from conan.tools.microsoft import is_msvc


class FunchookConan(ConanFile):
    name = "funchook"
    description = "Hook function calls by inserting jump instructions at runtime"
    license = "GPL-2.0-or-later WITH funchook-exception"
    url = "https://github.com/EndstoneMC/endstone"
    homepage = "https://github.com/kubo/funchook"
    topics = ("hook", "detour", "function-hooking", "instrumentation")
    package_type = "library"

    exports_sources = "patches/*"

    settings = "os", "arch", "compiler", "build_type"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "disasm": ["distorm", "capstone"],
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "disasm": "distorm",
    }

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC
        if str(self.settings.arch) in ("armv8", "arm64"):
            self.options.disasm = "capstone"

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")
        self.settings.rm_safe("compiler.cppstd")
        self.settings.rm_safe("compiler.libcxx")

    def layout(self):
        cmake_layout(self, src_folder="src")

    def requirements(self):
        if self.options.disasm == "capstone":
            self.requires("capstone/5.0.1")

    def validate(self):
        if str(self.settings.arch) not in ("x86_64", "armv8", "arm64"):
            raise ConanInvalidConfiguration(f"{self.ref} only supports x86_64 and arm64 architectures.")
        if self.options.disasm == "distorm" and str(self.settings.arch) in ("armv8", "arm64"):
            raise ConanInvalidConfiguration("distorm does not support arm64, use disasm=capstone.")

    def source(self):
        sources = self.conan_data["sources"][self.version]
        get(self, **sources["funchook"], strip_root=True)
        get(
            self,
            **sources["distorm"],
            strip_root=True,
            destination=os.path.join(self.source_folder, "distorm"),
        )
        apply_conandata_patches(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.cache_variables["FUNCHOOK_BUILD_SHARED"] = bool(self.options.shared)
        tc.cache_variables["FUNCHOOK_BUILD_STATIC"] = not bool(self.options.shared)
        tc.cache_variables["FUNCHOOK_BUILD_TESTS"] = False
        tc.cache_variables["FUNCHOOK_INSTALL"] = True
        tc.cache_variables["FUNCHOOK_DISASM"] = str(self.options.disasm)
        tc.generate()
        CMakeDeps(self).generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        copy(
            self,
            "LICENSE",
            src=self.source_folder,
            dst=os.path.join(self.package_folder, "licenses"),
        )
        cmake = CMake(self)
        cmake.install()
        # Upstream builds but doesn't install the bundled distorm static lib.
        if self.options.disasm == "distorm" and not self.options.shared:
            for pattern in ("*distorm.lib", "*distorm.a"):
                copy(
                    self,
                    pattern=pattern,
                    src=self.build_folder,
                    dst=os.path.join(self.package_folder, "lib"),
                    keep_path=False,
                )
        for d in ("cmake", "pkgconfig", "share"):
            rmdir(self, os.path.join(self.package_folder, "lib", d))
        rmdir(self, os.path.join(self.package_folder, "share"))

    def package_info(self):
        if self.options.shared and is_msvc(self):
            self.cpp_info.libs = ["funchook_dll"]
        else:
            self.cpp_info.libs = ["funchook"]
        if self.options.disasm == "distorm" and not self.options.shared:
            self.cpp_info.libs.append("distorm")
        if self.options.disasm == "capstone" and not self.options.shared:
            self.cpp_info.requires = ["capstone::capstone"]
        self.cpp_info.set_property("cmake_file_name", "funchook")
        self.cpp_info.set_property("cmake_target_name", "funchook::funchook")
        if self.settings.os == "Windows":
            self.cpp_info.system_libs = ["psapi"]
        elif self.settings.os in ("Linux", "FreeBSD"):
            self.cpp_info.system_libs = ["dl"]
