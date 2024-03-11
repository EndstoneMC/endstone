<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/EndstoneMC/endstone/releases">
    <img src="https://static.wikia.nocookie.net/minecraft_gamepedia/images/4/43/End_Stone_JE3_BE2.png" alt="Logo" width="80" height="80">
  </a>

<h3 align="center">Endstone</h3>

<p align="center">
  Write your plugins in Python and C++ for Bedrock Dedicated Servers.
</p>
</div>

[![Windows](https://github.com/EndstoneMC/endstone/actions/workflows/windows.yml/badge.svg)](https://github.com/EndstoneMC/endstone/actions/workflows/windows.yml)
[![Linux](https://github.com/EndstoneMC/endstone/actions/workflows/linux.yml/badge.svg)](https://github.com/EndstoneMC/endstone/actions/workflows/linux.yml)
[![Wheel](https://github.com/EndstoneMC/endstone/actions/workflows/wheel.yml/badge.svg)](https://github.com/EndstoneMC/endstone/actions/workflows/wheel.yml)
[![Docker](https://github.com/EndstoneMC/endstone/actions/workflows/docker.yml/badge.svg)](https://github.com/EndstoneMC/endstone/actions/workflows/docker.yml)
[![PyPI - Version](https://img.shields.io/pypi/v/endstone)](https://pypi.org/project/endstone)
[![PyPI - Downloads](https://img.shields.io/pypi/dm/endstone)](https://pypi.org/project/endstone)
[![Python](https://img.shields.io/badge/Python-3-blue?logo=python&logoColor=white)](https://www.python.org/)
[![GitHub License](https://img.shields.io/github/license/endstonemc/endstone)](LICENSE)
[![Codacy Badge](https://img.shields.io/codacy/grade/8877402fc70b40f5a8c4b325d890e3f7?logo=codacy)](https://app.codacy.com/gh/EndstoneMC/endstone/dashboard)
[![Code style: black](https://img.shields.io/badge/code%20style-black-000000.svg)](https://github.com/psf/black)

## 📄 Table of Contents

- [📖 Introduction](#-introduction-)
- [🎯 Features](#-features-)
- [🛠️ Get started](#%EF%B8%8F-get-started-)
- [🌟 Plugins](#-plugins-)
- [🙌 Contributing](#-contributing-)
- [🎫 License](#-license-)

## 📖 Introduction [🔝](#-table-of-contents)

We are excited to announce the Endstone project. Endstone offers a plugin API for Bedrock Dedicated Servers, supporting
both Python and C++ languages. This allows developers to enhance Bedrock Dedicated Servers with various features and
functionalities.

## 🎯 Features [🔝](#-table-of-contents)

- **Cross-platform**: We have dedicated efforts to support both Windows and Linux. Proudly, we are the only
  BDS-based server software sporting native support for both platforms without the need for an emulator.
- **Feature-rich**: All up-to-date Minecraft features are at your fingertips! We make sure to keep Endstone aligned with
  the latest game updates.
- **Hassle-free**: Ready to use right out of the box. Start the Endstone server with just one line of command. No extra
  tools required.
- **High-level API**: We provide a high-level friendly API akin to Bukkit plugin developers. Save yourself the hassle of
  dealing with the dot-and-cross of the low-level system API and hooks, we've done the difficult part for you!
- **Starter-friendly**: No C++ knowledge? No problem! Craft your plugins using Python, your favourite.
- **Performance-in-mind**: For the performance savants, you can, of course, write (or rewrite) your plugins in C++ to
  squeeze out
  every bit of performance.

## 🛠️ Get started [🔝](#-table-of-contents)

Before getting started, verify that you are using one of the following compatible operating systems:

- Windows 10 version 10.0.15063 or later
- Windows Server 2016 or later
- Debian 11 or later
- Ubuntu 20.04 or later

Please also ensure you have **Python 3.9 or higher** installed on your system.

There are several ways to install Endstone:

### 🐍 PyPi

Endstone can be installed directly from PyPi using pip. Open your terminal and execute the following command to install
and start
the Endstone server:

```shell
pip install endstone
endstone
```

### 🐳 Docker

To try Endstone from a Docker image, use the following command:

```shell
docker pull endstone/endstone
docker run --rm -it -p 19132:19132/udp endstone/endstone
```

or

```shell
docker pull ghcr.io/endstonemc/endstone
docker run --rm -it -p 19132:19132/udp ghcr.io/endstonemc/endstone
```

This will pull the latest Endstone image and run it interactively in your terminal.

### 🔨 Build locally from Source

For advanced users, you may build Endstone locally from the source.

**Please note that building from source requires toolchains to be installed on your system.**

|                                  | Windows                       | Linux    |
|----------------------------------|-------------------------------|----------|
| Build System                     | CMake                         | CMake    |
| Generator                        | Ninja                         | Ninja    |
| C/C++ Compiler **(Recommended)** | MSVC 193 (Visual Studio 2022) | Clang 15 |
| C/C++ Compiler (Minimum)         | MSVC 191 (Visual Studio 2017) | Clang 5  |
| Standard Library                 | MSVC STL                      | libc++   |

<details>
<summary>Click to toggle the detailed instructions</summary>

First, clone the repository:

```shell
git clone https://github.com/EndstoneMC/endstone.git
cd endstone
```

The Endstone project uses Conan as the package manager. The next step will involve the installation of conan and
dependencies.

```shell
pip install conan
conan export third_party/funchook --version 1.1.3
conan install . --build=missing -s compiler.cppstd=17 -c tools.cmake.cmaketoolchain:generator=Ninja
```

Finally, you can build the wheel and install it on your local machine:

```shell
pip install .
endstone
```

</details>

## 🌟 Plugins [🔝](#-table-of-contents)

Currently, the Endstone project is under active development, and we invite you to review our roadmap. While you wait,
why not take a sneaky peek of our example plugins which illustrate the friendliness of the plugin API. If you have
previous experience with Bukkit, you should find the API particularly intuitive to work with:

- [C++ Example Plugin](https://github.com/EndstoneMC/cpp-plugin-template)
- [Python Example Plugin](https://github.com/EndstoneMC/python-plugin-template)

| Milestone                                | Duration   | Core | C++ API | Python API |
|------------------------------------------|------------|------|---------|------------|
| **🔌 Plugin Loader**                     | 1-2 months | ✅    | ✅       | ✅          |
| **⌨️ Command System**                    | 2-3 months | ✅    | ✅       | ✅          |
| **🔐 Permission System**                 | 2-3 months | 🚧   | ⏳       | ⏳          |
| **🎈 Event System**                      | 2-3 months | 🚧   | ⏳       | ⏳          |
| **🛠 Minecraft Core Features**           | 4-5 months | ⏳    | ⏳       | ⏳          |
| **🖼 GUI & Inventory System**            | 3-4 months | ⏳    | ⏳       | ⏳          |
| **🌟 Advanced Features & Refinements**   | 4-6 months | ⏳    | ⏳       | ⏳          |
| **🧪 Beta Testing & Community Feedback** | 3 months   | ⏳    | ⏳       | ⏳          |
| **🚀 Official Release & Support**        | -          | ⏳    | ⏳       | ⏳          |

Here's a legend to guide you:

- ✅: Task is completed. Woohoo! 🎉
- 🚧: Task is under way. We're on it! 💪
- ⏳: Task is up next. Exciting things are coming! 🌠

## 🙌 Contributing [🔝](#-table-of-contents)

We warmly welcome contributions to the Endstone project! If you're enthusiastic about enhancing Bedrock Dedicated
Servers with Python and C++ and have ideas on how to improve Endstone, here are a few ways you can contribute:

### Reporting Bugs

If you encounter any bugs while using Endstone, please open an [issue](https://github.com/EndstoneMC/endstone/issues) in
our GitHub repository. Ensure to include a detailed description of the bug and steps to reproduce it.

### Submitting a Pull Request

We appreciate code contributions. If you've fixed a bug or implemented a new feature, please submit a pull request!
Please ensure your code follows our coding standards and include tests where possible.

## 🎫 License [🔝](#-table-of-contents)

The Endstone project is licensed under the [Apache-2.0 license](LICENSE).
