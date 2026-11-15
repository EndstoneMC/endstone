{% set cache = os.getenv("CONAN_HOME", os.path.join(os.path.expanduser("~"), ".conan2")) %}
[options]
# Keep these LGPL libraries replaceable in the Linux runtime distribution.
libdwarf/*:shared=True
libelf/*:shared=True

[conf]
# libelf 0.8.13's ELF configure probe calls exit() without including stdlib.h.
# Provide its declaration so modern Clang can execute the actual probe.
libelf/*:tools.build:cflags+=["-include", "stdlib.h"]
# Dependency diagnostics also record source locations in their own object files.
{% for dependency in ["sentry-native", "cpptrace", "raknet", "libcurl"] %}
{{ dependency }}/*:tools.build:cflags+=["-ffile-prefix-map={{ cache }}=/zernyth/dependencies"]
{{ dependency }}/*:tools.build:cxxflags+=["-ffile-prefix-map={{ cache }}=/zernyth/dependencies"]
{% endfor %}
