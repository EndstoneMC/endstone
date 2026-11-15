# Zernyth Java plugin template
The local dependency is `dev.zernyth:api:1.0.0`. For managed CPU/I/O tasks, cancellation,
server handoffs and profiling, see the [runtime guide](../docs/zernyth-runtime-0.6.md)
and the existing HelloZernyth `/zernythasync` example.
Java 25 LTS is required; Eclipse Temurin 25 is preferred. This directory belongs to the existing fork.

1. In the fork's `java/` directory, run the already installed Gradle >=9.1:
   `gradle --offline --no-daemon :api:publishApiPublicationToSdkRepository`.
   This writes only to `java/build/sdk-repository`; it does not publish publicly.
2. Open this template directory in IntelliJ IDEA as a Gradle project. Select Temurin 25 as project SDK and Gradle JVM.
3. Run `gradle --offline --no-daemon build`. Without local Temurin, explicitly add
   `-PzernythJavaVendor=any` to BOTH commands to use an installed alternative JDK 25.
4. The JAR is in `build/libs/`. Only in an independently authorized server test,
   install it in the existing plugins folder. API/runtime JARs belong in the JVM classpath.
5. Check plugin lifecycle logs for `example_plugin`. `/example 3` and `/ex 3` require
   `example.use`; no world changes occur automatically.

Do not shade the Zernyth API into your JAR. The runtime rejects duplicate core classes.
Rename package, plugin name (lowercase Endstone rules), commands and permissions before reuse.
Data is stored in the native plugin data directory; reload is explicit and never autosaves on disable.

`pom.xml` demonstrates Maven with release 25 and the same local API repository. Use
`mvn -o package` only when Maven and its plugins are already available locally;
no automatic Maven installation/download is part of the SDK. Maven validation is separate from Gradle.

Common failures: missing SDK repository ÃƒÂ¢Ã¢â‚¬Â Ã¢â‚¬â„¢ build the local API artifact first; missing Temurin ÃƒÂ¢Ã¢â‚¬Â Ã¢â‚¬â„¢
select the installed JDK or explicit override; UnsupportedClassVersionError ÃƒÂ¢Ã¢â‚¬Â Ã¢â‚¬â„¢ runtime must be >=25;
ClassNotFound ÃƒÂ¢Ã¢â‚¬Â Ã¢â‚¬â„¢ declare local libraries/imports; duplicate API classes ÃƒÂ¢Ã¢â‚¬Â Ã¢â‚¬â„¢ use compileOnly/provided.
Never block the server thread or access native views from async tasks.
See [ecosystem guide](../docs/zernyth-ecosystem-0.5.md) for manifest, services, libraries and lifecycle.

0.7: /example [count] accepts 1..100, default 3; uses TypedCommand and Component.text.
See ../docs/zernyth-developer-0.7.md for the new APIs and local debugging.
