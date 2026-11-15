# Zernyth Engine 1.0.0

Java 25 plugins for Endstone and the original Minecraft Bedrock Dedicated Server.
This first stable release targets Windows x64, CPython 3.15 and BDS 1.26.60.6.
It includes the JNI thread-diagnostics correction confirmed in real BDS/client testing.

## Requirements

- Windows x64 and CPython 3.15.
- JDK 25 LTS. Eclipse Temurin is preferred. The local release build uses Oracle JDK 25.
- A separately obtained BDS 1.26.60.6 installation.
- The Microsoft Visual C++ runtime required by the native components.

## Install

Use a fresh Python environment. Zernyth retains the `endstone` import namespace and
replaces the upstream Endstone distribution in that environment.

For a locally built release wheel, run from the directory containing the wheel:

```powershell
py -3.15 -m venv .venv
.\.venv\Scripts\python.exe -m pip install .\zernyth_engine-1.0.0-cp315-cp315-win_amd64.whl
.\.venv\Scripts\python.exe -m pip check
.\.venv\Scripts\zernyth.exe --version
```

Once the owner has published this version on PyPI, its package specification is
`zernyth-engine==1.0.0`. Public upload is a separate owner-controlled action.

Inspect an existing server using the actual paths to your BDS and installed JDK:

```powershell
.\.venv\Scripts\zernyth.exe --server "D:\BDS-Test" --java-home "D:\JDK25"
```

Append `--run` when ready to start. Stop with `stop` in the server console.
The operator manages networking, server configuration, worlds and allowlist settings.
The launcher starts the existing native engine with Endstone and the matching Java runtime.

## Included components

The wheel contains the Python compatibility modules, native runtime and JNI bridge,
matching Java API/runtime JARs, default resources and third-party license texts.
BDS, JDK, active example plugins, worlds and local test data are supplied separately.
The native Endstone API retains its upstream identity, 0.11.15.

## Validation and limits

Real owner-run BDS 1.26.60.6 tests confirmed Java lifecycle, command execution,
services, configuration, delayed tasks/cancellation, player/world/inventory reads,
movement events, read-only packet observation, form submission and native cleanup.
The diagnostics correction was retested in BDS. The 1.0.0 artifact is rebuilt from
that corrected source and receives a fresh installation check.

The current 1.0.0 native/JNI suite passed 249 cases through the first-party Windows
DLL test host, with nine JVM processes and zero checker diagnostics. The host uses
the standard Windows loader. Application-control restrictions on standalone test
executables remain enabled.

Linux/Temurin build and runtime evidence is recorded separately in the repository's
VPS validation report. The Linux archive targets Ubuntu 24.04 x86_64 and CPython 3.12.
Long-duration load, additional JDK combinations, every gameplay mutation,
all event-cancellation cases and every disconnect/failure scenario remain outside
the completed validation. Packet access is read-only metadata. Bukkit/Paper porting
support is limited. Embedded Java plugins share the server process and require trust.
No player-capacity, uptime or complete sandbox guarantee is provided.

## License

Apache-2.0 with retained Endstone attribution. Full applicable notices and dependency
license texts are included. Minecraft, BDS and Java distributions have their own terms.

Source and release documentation are maintained at
https://github.com/MakisImperium/endstone-zernyth.
