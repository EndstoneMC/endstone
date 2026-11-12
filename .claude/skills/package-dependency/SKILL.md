---
name: package-dependency
description: Adds a Conan recipe for a third-party CMake library under recipes/, validates it on Windows and Linux, and uploads it recipe-only to the endstone Cloudsmith remote. Use when a C or C++ dependency is missing from conancenter or needs project patches, or when asked to "add a recipe for X", "package X for conan" or "replace the X FetchContent".
---

# Package Dependency

Read first: `.claude/rules/build.md`.

1. Collect the name, version, source tarball URL and upload scope (default recipe-only); stop and tell the user when the library does not build with CMake.
2. Create `recipes/<name>/config.yml` and `recipes/<name>/all/{conandata.yml,conanfile.py,test_package/}`, with `patches/` only when upstream needs one.
3. Hash each source: `curl -sL -o <scratch>/src.tar.gz "<URL>"` then `sha256sum <scratch>/src.tar.gz`; write `url` and `sha256` into `conandata.yml`.
4. Write `conanfile.py` and any patches from `recipes/funchook/all/` as the model.
5. Write `test_package`; stop and ask the user for an end-to-end check when none is possible.
6. On Windows from a Visual Studio developer prompt run `conan create recipes/<name>/all --version=<v> -pr default --build=missing`, repeated with `-o '<name>/*:<option>=<value>'` per non-default option.
7. On Linux run the same `conan create` in an `ubuntu:20.04` container with a throwaway Conan home and the repo mounted at `/work`.
8. Commit only when the user asked.
9. Export from the committed tree: `git -c core.autocrlf=false archive HEAD recipes/<name>/all | tar -x -C <scratch>`, then `conan export <scratch>/recipes/<name>/all --version=<v>`.
10. Stop and ask the user to confirm the scope and run `conan remote login endstone <username>`.
11. Run `conan upload "<name>/<v>" --only-recipe -r=endstone -c`, then `conan list "<name>/*" -r=endstone`, retrying once after 30 seconds when empty.
12. Wire the consumer: `self.requires("<name>/<v>")`, `find_package(<name> CONFIG REQUIRED)` and `<name>::<name>` in its `CMakeLists.txt`, delete the replaced `FetchContent` block, then run `conan install . --build=missing`.
