import org.gradle.util.GradleVersion

require(GradleVersion.current() >= GradleVersion.version("9.1")) {
    "Zernyth requires Gradle 9.1+ for Java 25 toolchains and runtime support"
}

rootProject.name = "zernyth"
include("api", "runtime", "example", "stable-test", "ui")
