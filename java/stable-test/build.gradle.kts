import java.time.Duration

plugins {
    `java-library`
}

dependencies {
    compileOnly(project(":api"))
    testImplementation(project(":api"))
}

tasks.processResources {
    inputs.property("zernythVersion", project.version.toString())
    filesMatching("plugin.yml") {
        expand("version" to project.version.toString())
    }
}

tasks.withType<JavaCompile>().configureEach {
    options.compilerArgs.addAll(listOf("-Xlint:all", "-Werror"))
}

tasks.javadoc {
    options {
        this as StandardJavadocDocletOptions
        addBooleanOption("Xdoclint:all", true)
        addBooleanOption("Werror", true)
    }
}

val selfTest = tasks.register<JavaExec>("selfTest") {
    dependsOn(tasks.testClasses)
    classpath = sourceSets.test.get().runtimeClasspath
    mainClass.set("dev.zernyth.stable.SafetyTest")
    jvmArgs("-Xms32m", "-Xmx128m", "-XX:ActiveProcessorCount=2")
    timeout.set(Duration.ofSeconds(30))
}

tasks.check {
    dependsOn(selfTest)
}

// The dependency-free JavaExec suite runs all tests without a JUnit download.
tasks.test {
    enabled = false
}
