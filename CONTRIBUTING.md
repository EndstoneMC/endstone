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

The dependencies needed for the development of **Endstone** are provided Conan Package Manager (`>=2.0`). The install
the package manager, run the following commands in your Python environment.

```shell
pip install conan
conan profile detect
```

### Install dependencies

First of all, run the following commands to install dependencies of this project:

=== ":fontawesome-brands-windows: Command Prompt"
    ```shell
    conan install . --build=missing -s compiler.cppstd=20 -c tools.cmake.cmaketoolchain:generator=Ninja
    ```

=== ":fontawesome-brands-windows: Powershell"
    ```shell
    conan install . --build=missing -s compiler.cppstd=20 -c tools.cmake.cmaketoolchain:generator=Ninja -c tools.env.virtualenv:powershell=True
    ```

=== ":fontawesome-brands-linux: Linux"

    ```shell
    conan install . --build=missing -s compiler.cppstd=20 -s compiler.libcxx=libc++ -c tools.cmake.cmaketoolchain:generator=Ninja
    ```

Now, activate the build virtual environment create by conan.

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

To install **Endstone** from your local sources, simply run:

```shell
pip install -U .
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
