# Project Structure

This chapter provides a high-level overview of the project to make it easier to navigate, build, and use.

## CMake Projects and Dependencies

The [GitHub repository](https://github.com/EndstoneMC/endstone) contains the following CMake projects:

- **Endstone API** (`include/endstone`)
    - This is the **header-only** API layer that developers use to create plugins.
    - It provides a standardized set of methods and abstractions to interact with Minecraft, ensuring plugins work
      across different Minecraft versions with minimal adjustments.
    - It abstracts Minecraft internals, allowing plugins to interact with the game in a version-agnostic way without
      directly interfacing with Mojang's code.

- **Endstone Python Bindings** (`src/endstone_python`)
    - This is the Python binding of the Endstone API.
    - It allows developers to create plugins in Python by translating API calls from Python to the underlying C++ calls.

- **Endstone Core** (`src/endstone_core`)
    - This is the implementation of the Endstone API. It sits between the API and the game itself.
    - It provides the actual behavior for API methods by interacting directly with the Bedrock Dedicated Server (BDS),
      translating abstracted API calls from plugins into specific BDS internal calls.

- **Endstone Runtime** (`src/endstone_runtime`)
    - This includes hooks that are applied to the executable binary of the Bedrock Dedicated Server to modify or extend
      functionality, providing bug fixes and features that are not possible through the API alone.
    - These hooks are necessary because the original software is compiled into binary, and updates to Minecraft
      frequently break compatibility with existing code.

- **Endstone DevTools** (`src/endstone_devtools`)
    - This provides a Graphical User Interface (GUI) that allows advanced users to dump useful data from the vanilla
      software for other purposes, such as third-party server software.
    - It is currently enabled only on Windows and requires OpenGL functionality to be available on the system.

## Python Project

Endstone is released as a Python package on PyPI, which encapsulates several utilities written in pure Python, in
addition to the compiled libraries of the CMake projects and the Python bindings of the Endstone API. These Python codes
are located under the `python/src` folder, and unit tests are located under `python/tests`.
