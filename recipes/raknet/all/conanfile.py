import os
import shutil

from conan import ConanFile
from conan.errors import ConanInvalidConfiguration
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.files import (
    apply_conandata_patches,
    copy,
    export_conandata_patches,
    get,
    rmdir,
)


class RakNetConan(ConanFile):
    name = "raknet"
    description = "Cross-platform C++ networking engine for game programmers (Oculus VR fork, archived)."
    license = "BSD-2-Clause"
    url = "https://github.com/EndstoneMC/endstone"
    homepage = "https://github.com/facebookarchive/RakNet"
    topics = ("networking", "udp", "games", "raknet")
    package_type = "library"

    exports_sources = "cmake/*"

    def export_sources(self):
        export_conandata_patches(self)

    settings = "os", "arch", "compiler", "build_type"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
    }
    default_options = {
        "shared": False,
        "fPIC": True,
    }

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self, src_folder="src")

    def validate(self):
        if str(self.settings.arch) not in ("x86_64", "x86", "armv8", "arm64"):
            raise ConanInvalidConfiguration(f"{self.ref} only supports x86, x86_64 and arm64 architectures.")

    def source(self):
        get(self, **self.conan_data["sources"][self.version], strip_root=True)
        apply_conandata_patches(self)
        file_map = {
            "CMakeLists.txt": ".",
            "raknetConfig.cmake.in": ".",
        }
        for filename, dest_subdir in file_map.items():
            shutil.copy(
                os.path.join(self.export_sources_folder, "cmake", filename),
                os.path.join(self.source_folder, dest_subdir, filename),
            )

    def generate(self):
        tc = CMakeToolchain(self)
        tc.cache_variables["RAKNET_BUILD_SHARED"] = bool(self.options.shared)
        tc.generate()
        CMakeDeps(self).generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        copy(self, "LICENSE", src=self.source_folder, dst=os.path.join(self.package_folder, "licenses"))
        cmake = CMake(self)
        cmake.install()
        rmdir(self, os.path.join(self.package_folder, "lib", "pkgconfig"))
        rmdir(self, os.path.join(self.package_folder, "share"))

    def package_info(self):
        self.cpp_info.libs = ["raknet"]
        self.cpp_info.set_property("cmake_file_name", "raknet")
        self.cpp_info.set_property("cmake_target_name", "raknet::raknet")
        if self.settings.os == "Windows":
            self.cpp_info.system_libs = ["ws2_32"]
        else:
            self.cpp_info.system_libs = ["pthread"]
