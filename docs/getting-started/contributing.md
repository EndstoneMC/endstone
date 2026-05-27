---
comments: true
toc_depth: 2
---

# Contributing

👍🎉 First off, thanks for taking the time to contribute! 🎉👍

## Where to start?

If you are looking for a good issue to start with, please check the following:

- [good first issue](https://github.com/EndstoneMC/endstone/labels/good%20first%20issue) - issues that
  should be pretty simple to implement,
- [help wanted](https://github.com/EndstoneMC/endstone/labels/help%20wanted) - issues that typically are
  a bit more involved than beginner issues,
- [high priority](https://github.com/EndstoneMC/endstone/labels/high%20priority) - things to fix ASAP
  but often of higher complexity.

## Source

To build from source on your local machine, please follow the instructions below:

### Clone the repository

```shell
git clone https://github.com/EndstoneMC/endstone.git
cd endstone
```

### Install the package manager (conan)

The dependencies needed for the development of **Endstone** are provided by [Conan package manager](https://docs.conan.io/2) (`>=2.0`). To install
the package manager, run the following command in your Python environment.

```shell
pip install conan
```

The repository ships a Jinja-templated default profile at `.conan2/profiles/default` that auto-detects the host compiler and pins the right settings (clang-cl on Windows, clang/libc++ on Linux, Ninja generator). **Do not run `conan profile detect`** — it would overwrite this file.

### Install dependencies

Run the following command to install the project's dependencies:

=== ":fontawesome-brands-windows: Command Prompt / PowerShell"
    ```shell
    conan install . --build=missing
    ```

=== ":fontawesome-brands-linux: Linux"

    ```shell
    conan install . --build=missing
    ```

Now, activate the build virtual environment created by conan (only required for the manual `cmake --preset` path below; the `pip install` path does not need it):

=== ":fontawesome-brands-windows: Command Prompt"
```cmd
.\build\Release\generators\conanbuild.bat
```

=== ":fontawesome-brands-windows: Powershell"
```cmd
.\build\Release\generators\conanbuild.ps1
```

=== ":fontawesome-brands-linux: Linux"

    ```shell
    source ./build/Release/generators/conanbuild.sh
    ```

Run `cmake` and check the version:

```shell
$ cmake --version
cmake version 3.31.2
```

### Build with CMake

```shell
cmake --preset conan-release
cmake --build --preset conan-release
```

### Install

The PEP 517 backend (`conan-py-build`) runs Conan internally, so installing from sources does not need a separate `conan install` step:

```shell
pip install -U .
# or with uv:
uv pip install -U .
```

To keep the C++ build tree persistent across reinstalls (much faster after the first build), pass a build dir via `config_settings`:

```shell
pip install -U . -C build-dir=./build
```

## Documentation

We are building our documentation using [Material for MkDocs](https://squidfunk.github.io/mkdocs-material/).
The easiest way to install all the required dependencies is with `pip`:

```shell
pip install -r docs/requirements.txt
```

After that, you can either:

- [Start a live server to preview the documentation as you write](https://squidfunk.github.io/mkdocs-material/creating-your-site/#previewing-as-you-write), or

```shell
mkdocs serve
```

- [Build the documentation](https://squidfunk.github.io/mkdocs-material/creating-your-site/#building-your-site)

```shell
mkdocs build
```
