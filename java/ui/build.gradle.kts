plugins { `java-library` }

dependencies {
    compileOnly(project(":api"))
    testImplementation(project(":api"))
}

tasks.withType<JavaCompile>().configureEach {
    options.compilerArgs.addAll(listOf("-Xlint:all", "-Werror"))
}

sourceSets {
    create("example") {
        compileClasspath += sourceSets.main.get().output + project(":api").sourceSets.main.get().output
        runtimeClasspath += output + compileClasspath
    }
}
tasks.named("compileExampleJava") { dependsOn(":api:classes") }

// Plain Java test runner, like the other Zernyth modules: fails the build on the first assertion error.
val uiTest by tasks.registering(JavaExec::class) {
    dependsOn(tasks.testClasses)
    classpath = sourceSets.test.get().runtimeClasspath
    mainClass.set("dev.zernyth.ui.UiTests")
    workingDir = projectDir
}
tasks.named("check") { dependsOn(uiTest, "compileExampleJava") }
// Tests run through uiTest (no JUnit, like the other Zernyth modules).
tasks.named<Test>("test") { enabled = false }

tasks.withType<Javadoc>().configureEach {
    (options as StandardJavadocDocletOptions).addStringOption("Xdoclint:all", "-quiet")
    (options as StandardJavadocDocletOptions).addBooleanOption("Werror", true)
    options.encoding = "UTF-8"
}

// Installable test plugin for the showcase resource pack: the UI library plus UiShowcasePlugin and its plugin.yml.
val showcasePluginJar by tasks.registering(Jar::class) {
    group = "build"
    description = "Builds the UI showcase test plugin (build/showcase/zernyth-ui-showcase.jar)."
    dependsOn("exampleClasses")
    archiveFileName.set("zernyth-ui-showcase.jar")
    destinationDirectory.set(layout.buildDirectory.dir("showcase"))
    from(sourceSets.main.get().output)
    from(sourceSets["example"].output) { include("dev/zernyth/ui/example/UiShowcasePlugin*.class") }
    from("showcase/plugin.yml") { expand("version" to project.version.toString()) }
    inputs.property("version", project.version.toString())
}
