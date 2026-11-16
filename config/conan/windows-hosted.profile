[settings]
os=Windows
arch=x86_64
build_type=Release
&:build_type=RelWithDebInfo
{% set compiler, version, _ = detect_api.detect_msvc_compiler() %}
compiler={{ compiler }}
compiler.version={{ detect_api.default_compiler_version(compiler, version) }}
compiler.runtime=dynamic
compiler.cppstd=20

[conf]
tools.cmake.cmaketoolchain:generator=Ninja
tools.build:compiler_executables={"c": "clang-cl", "cpp": "clang-cl"}
sentry-native/*:tools.build:compiler_executables={"c": "cl", "cpp": "cl"}
tools.build:jobs=4
