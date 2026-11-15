plugins { `java-library` }
group="dev.example"
version="1.0.0"
val jdkVendor=providers.gradleProperty("zernythJavaVendor").orElse("temurin").get()
require(jdkVendor in setOf("temurin","any")) { "Expected zernythJavaVendor=temurin or any" }
java {
    toolchain {
        languageVersion.set(JavaLanguageVersion.of(25))
        if(jdkVendor=="temurin") vendor.set(JvmVendorSpec.ADOPTIUM)
    }
}
tasks.withType<JavaCompile>().configureEach { options.release.set(25) }
repositories { maven { url=uri(providers.gradleProperty("zernythSdkRepository").orElse("../java/build/sdk-repository").get()) } }
dependencies { compileOnly("dev.zernyth:api:1.0.0") }
