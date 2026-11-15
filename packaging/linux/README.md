# Zernyth Engine 1.0.0 for Ubuntu 24.04

This archive targets Ubuntu 24.04 x86_64, CPython 3.12 and Java 25 LTS. The Windows
pip distribution uses CPython 3.15. Install each platform's matching artifact.

Provide your own original Linux BDS 1.26.60.6 installation and an installed JDK 25.
Eclipse Temurin 25 is the preferred JDK. BDS, a JDK, worlds and plugin data are
operator-managed prerequisites.

## Installation

Extract the archive into a new directory, then run:

```sh
./install.sh
./start.sh --server /absolute/path/to/bds --java-home /absolute/path/to/jdk25
```

Installation creates `.venv` using CPython 3.12 and installs hash-pinned dependencies
from the included wheelhouse. It operates offline. Inspection checks artifact hashes,
the exact validated BDS executable, Java 25 and native Python imports.

Start the server explicitly:

```sh
./start.sh --server /absolute/path/to/bds --java-home /absolute/path/to/jdk25 --run
```

Place your Java plugin JARs in the BDS directory's `plugins` folder. Stop the server
with `stop` in its console. Configure network access and the BDS allowlist yourself.
The validated NetherNet configuration uses TCP signaling and UDP gameplay. The
server operator determines which ports are exposed.

The launcher reuses Endstone's Linux startup backend and selects the system
allocator. Explicit Mimalloc preloading produced native startup crashes in validation.
Set `JAVA_TOOL_OPTIONS` to select JVM memory limits appropriate for your plugins.
The tested small-plugin configuration used a 64 MiB initial heap and 256 MiB maximum.

## Updates and rollback

Stop BDS before replacing runtime components. Extract an update into a new directory,
run its installer and inspect the existing BDS before starting. Keep the previous
archive and environment available for rollback. Back up plugin data before applying
plugin-specific migrations. The installer operates inside its extracted directory.

## Libraries and licenses

Complete project, native, Java and bundled-system-library notices are in `LICENSES`.
Python dependency wheels retain their original license files. Corresponding source
and Conan recipe exports for libelf and libdwarf are in `sources`.
That directory also contains the reviewed dependency lock and Linux release
profile, including the configure compatibility flag used for libelf.

libelf and libdwarf are dynamically linked. Interface-compatible replacements can
be installed in `lib`; the runtime launcher permits replacement of these shared
libraries. `manifest.json` records the original release files for integrity checks.
Use an unmodified extracted archive for the initial installation verification.

Java plugins share the embedded JVM and have access to the server process. Plugin
ClassLoaders provide class identity and lifecycle controls. Process-level isolation
requires a separately managed operating-system environment.
