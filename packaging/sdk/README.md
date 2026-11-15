# Zernyth Engine Developer SDK

This SDK contains the Java API, sources, Javadocs, a local Maven-compatible
repository, a plugin template and the first-party HelloZernyth example.
Use JDK 25, preferably Eclipse Temurin, and Gradle 9.1 or newer.

## IntelliJ IDEA and Gradle

Open `zernyth-plugin-template` as a Gradle project and select JDK 25. Build with:

```sh
gradle -PzernythSdkRepository=/absolute/path/to/sdk-repository build
```

An installed Oracle JDK 25 requires `-PzernythJavaVendor=any`. Add `--offline`
when your Gradle dependencies are already available locally. The template uses
`dev.zernyth:api:1.0.0` as a compile-only dependency. Install the resulting JAR
in the separately managed BDS server's `plugins` directory.

## Maven

Install the included API into your local Maven repository using one command:

```sh
mvn install:install-file -Dfile=api/api-1.0.0.jar -DpomFile=sdk-repository/dev/zernyth/api/1.0.0/api-1.0.0.pom
```

Declare `dev.zernyth:api:1.0.0` with `provided` scope and set
`maven.compiler.release` to `25`. Maven and its plugins must already be installed
or provisioned explicitly. This archive performs no publication.

## API reference and runtime rules

Attach `api/api-1.0.0-sources.jar` in your IDE. Extract
`api/api-1.0.0-javadoc.jar` and open `index.html` for the generated API reference.
The Hello example demonstrates lifecycle-owned listeners, tasks and explicit
diagnostic commands using the public API.

Native gameplay access requires the documented server thread and a valid native
object lifetime. Hand independent I/O work to the managed async runtime and
schedule native interactions back onto the server thread. Cancellation is
cooperative. Plugin callbacks and resources belong to the current plugin generation.
Java plugins run inside the server process and require operator trust.

Install the matching Zernyth runtime separately. Provide an original compatible
BDS installation and JDK 25. Server configuration, backups, startup and shutdown
remain operator-controlled. See the project release guide for tested platforms.
