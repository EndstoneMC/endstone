# Third-party attribution and distribution notes

Zernyth Engine is a modified fork of [Endstone](https://github.com/EndstoneMC/endstone),
based on its 0.11.15 source release. The original Apache-2.0 [LICENSE](LICENSE),
source copyright headers, contribution guide, and release history are retained.
Zernyth-specific changes include the Java framework, JNI adapters, SDK, and
associated build, test, and documentation infrastructure. No original root
`NOTICE` file was present in the inherited baseline.

## Java runtime dependencies

The locally available POM metadata declares:

| Dependency | Version | Declared license |
|---|---|---|
| Gson | 2.14.0 | Apache-2.0 |
| SnakeYAML | 2.4 | Apache-2.0 |

The wheel includes each dependency's original POM (including its existing
copyright notices), all license/notice files extracted by the Java build, and
the complete Apache-2.0 text for each dependency under `zernyth/licenses/java`.
Native dependency license files are copied from the matching cached Conan graph
under `zernyth/licenses/native`, with a path-free inventory. Missing native
license directories fail packaging. `opengl/system` identifies an operating
system component and is not bundled. These checks establish file presence,
not independent legal certification of every transitive dependency.

Build and test tools have their own license terms. See `conanfile.py`,
`java/runtime/build.gradle.kts`, and accompanying dependency materials for the
actual dependency definitions. No dependency binaries are committed here.

The wheel also includes 19 original Sentry/Crashpad source license and copyright
files under `zernyth/licenses/sentry-source-notices`. Its inventory records their
source recipe and SHA-256 hashes, covering nested Mini Chromium, ICU, zlib, mpack,
getopt and other dependency notices.

The Windows build uses Cpptrace's DbgHelp backend. Packaging verifies its linker
map and rejects linked libdwarf objects or symbols. The Conan recipe graph still
lists libdwarf and its license materials are included conservatively. The wheel
contains a path-free Windows linkage audit. Python dependencies are installed as
separate distributions and retain the license files in their own wheels.

## Ubuntu archive

The Ubuntu 24.04 / CPython 3.12 archive uses shared libelf and libdwarf libraries.
Their full cached license materials, corresponding source trees and Conan recipe
exports accompany the archive. The shared libraries remain replaceable with
interface-compatible builds. Packaging checks the runtime's actual ELF dependency
entries. Upstream source test fixtures retain their original contents and are
included solely as corresponding source materials.

Bundled libc++, libc++abi, libunwind and libpython files retain the matching Ubuntu
package copyright notices, including referenced common license texts. Hash-pinned
Python dependency wheels retain their original license/notice files. The archive
includes the same complete Gson, SnakeYAML, Sentry and font notices described above.
See the artifact's `LICENSES`, `sources`, `requirements.lock` and checksum manifest.

The Linux OpenSSL 3.6.5 build uses Zernyth's hash-checked Conan source hook to
normalize private prefixes in generated diagnostic compiler-flags metadata before
compilation. The original source license header is retained. This modifies the
metadata generator; actual compiler arguments are preserved. The hook is included
under `sources/hook_zernyth_build_metadata.py` for inspection and reproduction.

## Bundled font

The unmodified JetBrains Mono font retains its embedded copyright:
Copyright 2020 The JetBrains Mono Project Authors
(https://github.com/JetBrains/JetBrainsMono). The wheel places its complete SIL
Open Font License 1.1 alongside it at `endstone/fonts/OFL.txt`. The source copy
is `packaging/licenses/JetBrainsMono-OFL.txt`, obtained from the font project's
`OFL.txt` (Git blob `5ceee0025da23d2cd5fd14b28569708a8db4db2b`).

## Minecraft and BDS

Minecraft and Bedrock Dedicated Server are owned by their respective rights
holders. No proprietary BDS executable, world, player data, or JDK is included.
Use a separately obtained BDS installation. Apache-2.0 does not grant rights to
third-party products or trademarks.
