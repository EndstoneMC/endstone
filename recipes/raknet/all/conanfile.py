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
    replace_in_file,
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
        "protocol_version": ["ANY"],
        "max_internal_ids": ["ANY"],
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "protocol_version": 6,
        "max_internal_ids": 10,
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
        for opt in ("protocol_version", "max_internal_ids"):
            value = str(getattr(self.options, opt))
            if not value.isdigit() or int(value) <= 0:
                raise ConanInvalidConfiguration(f"raknet:{opt} must be a positive integer (got {value!r}).")

    def source(self):
        get(self, **self.conan_data["sources"][self.version], strip_root=True)
        apply_conandata_patches(self)
        file_map = {
            "CMakeLists.txt": ".",
            "raknetConfig.cmake.in": ".",
            "NativeFeatureIncludesOverrides.h": "Source",
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

    def _patch_sources_for_options(self):
        protocol_version = str(self.options.protocol_version)
        max_internal_ids = str(self.options.max_internal_ids)
        replace_in_file(
            self,
            os.path.join(self.source_folder, "Source", "RakNetVersion.h"),
            "#define RAKNET_PROTOCOL_VERSION 6",
            f"#define RAKNET_PROTOCOL_VERSION {protocol_version}",
        )
        defines_path = os.path.join(self.source_folder, "Source", "RakNetDefines.h")
        replace_in_file(
            self,
            defines_path,
            "#define MAXIMUM_NUMBER_OF_INTERNAL_IDS 10",
            f"#define MAXIMUM_NUMBER_OF_INTERNAL_IDS {max_internal_ids}",
        )
        replace_in_file(
            self,
            defines_path,
            "#define RAKNET_SUPPORT_IPV6 0",
            "#define RAKNET_SUPPORT_IPV6 1",
        )

    def build(self):
        self._patch_sources_for_options()
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        copy(
            self,
            "LICENSE",
            src=self.source_folder,
            dst=os.path.join(self.package_folder, "licenses")
        )
        cmake = CMake(self)
        cmake.install()
        rmdir(self, os.path.join(self.package_folder, "lib", "pkgconfig"))
        rmdir(self, os.path.join(self.package_folder, "share"))

    def package_info(self):
        self.cpp_info.libs = ["raknet"]
        self.cpp_info.includedirs = ["include", "include/raknet"]
        self.cpp_info.set_property("cmake_file_name", "raknet")
        self.cpp_info.set_property("cmake_target_name", "raknet::raknet")
        if self.settings.os == "Windows":
            self.cpp_info.system_libs = ["ws2_32"]
        else:
            self.cpp_info.system_libs = ["pthread"]
