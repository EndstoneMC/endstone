---
comments: true
toc_depth: 2
---

# Create your first plugin

## Prerequisites

=== ":fontawesome-brands-python: Python"

    To develop your first plugin with the Python API, you will have to install the following prerequisites:
    
    -   [JetBrains PyCharm]
    -   [Python] (>= 3.9)
    -   [endstone Python package]

=== ":simple-cplusplus: C++"
    
    To develop your first plugin with the C++ API, you will have to install the following prerequisites:

    -   [JetBrains CLion]
    -   [CMake] (>= 3.15)
    -   C++ Compiler
        -   **:fontawesome-brands-windows: Windows**: [Visual Studio] 2017 or newer
        -   **:fontawesome-brands-linux: Linux**: [LLVM Toolchain] 5 or newer with Clang and libc++

## Create a new project

=== ":fontawesome-brands-python: Python"

    When you start up PyCharm, you will see this welcome screen:
    ![Welcome to PyCharm](screenshots/pycharm-welcome.png)

    Click on **New Project**

    ![New PyCharm Project](screenshots/pycharm-create-project.png)

    Let's say you want to name your plugin **MyPlugin**. In Endstone plugin development, we follow the following 
    naming convention:

    1. Use `lower-case-with-dash` for project name
    2. Prefix your project name with `endstone-`

    Hence, you should put `endstone-my-plugin` in the **Name** field.

    Then, in the **Interpreter type**, select **Custom environment**. Select **Select existing**, and set the path to 
    where you previously installed `endstone` as stated in the prerequisites.

    Finally, click on **Create**. The PyCharm workspace will pop up and you will see this.
    ![PyCharm Workspace](screenshots/pycharm-workspace.png)

    !!! tip
        Endstone server requires its plugins to be installed in the same Python environment. A virtual environment is
        **strongly recommanded**.

    ### Check your dependencies

    In the last step, you selected the existing interpreter where `endstone` package is installed. For now, that's the
    only dependency we need for a simple plugin. To check its installation, click on the :octicons-terminal-16: icon on
    the side bar to open the terminal and type:
    ```
    pip show endstone
    ```

    You show see something like this:
    ```
    Name: endstone
    Version: 0.4.2
    Summary: Endstone offers a plugin API for Bedrock Dedicated Servers, supporting both Python and C++.
    Home-page:
    Author:
    Author-email: Vincent Wu <magicdroidx@gmail.com>
    License: Apache License
    ```

    ### Create `pyproject.toml`

    Modern Python packages can contain a `pyproject.toml` file, first introduced in [PEP 518]. This file contains build 
    system requirements and information, which are used by pip to build the package.
    
    Now, right click on the project folder and select **New > File** to create a `pyproject.toml`. 
    ![Create pyproject.toml](screenshots/pycharm-create-pyproject-toml.png)    

    Copy the following content and paste into the file.
    ``` toml title="pyproject.toml" linenums="1"
    [build-system]
    requires = ["setuptools >= 61.0"]
    build-backend = "setuptools.build_meta"
    
    [project]
    name = "endstone-my-plugin"
    version = "0.1.0"
    ```

    !!! notice
        The name field should always be the project name. It must start with `endstone-` which is **enforced** by the 
        plugin loader. The name should also use `lower-case-with-dash` style.
    

=== ":simple-cplusplus: C++"

     When you start up CLion, you will see this welcome screen:
    ![Welcome to CLion](screenshots/clion-welcome.png)

    Click on **New Project**

    ![New CLion Project](screenshots/clion-create-project.png)

    In the side bar, select **C++ Library**. Select **C++ 17** for **Language standard**. 
    Select **shared** for **Library type**. Click on **Create**. The CLion workspace will pop up and you will see this.
    ![CLion Workspace](screenshots/clion-workspace.png)

    ### File structure
    From the project view in the side bar, you will notice that CLion created a few files for us.

    -   `.clang-format`: The configuration file for ClangFormat
    -   `CMakeLists.txt`: The manifest file for CMake build system
    -   `library.cpp`: Source file
    -   `library.h`: Header file
    
    **Delete** `library.cpp` and `library.h` as they are not required for Endstone plugins. You can keep the 
    `.clang-format` and `CMakeLists.txt`.

    ### Update `CMakeLists.txt`

    Now, open the `CMakeLists.txt` in the side bar and delete all the existing content. 
    Then, copy and paste the following into your `CMakeLists.txt`.

    ``` CMake title="CMakeLists.txt" linenums="1"
    cmake_minimum_required(VERSION 3.15)
    
    project(endstone_example_plugin CXX)
    
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    
    include(FetchContent)
    FetchContent_Declare(
        endstone
        GIT_REPOSITORY https://github.com/EndstoneMC/endstone.git
        GIT_TAG main #(1)!
    )
    FetchContent_MakeAvailable(endstone)

    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC") #(2)!
        add_compile_definitions(_ITERATOR_DEBUG_LEVEL=0)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -fPIC")
        link_libraries(-static-libgcc -static-libstdc++ libc++.a libc++abi.a)
    endif ()
    ```

    1.  :warning: This will use the latest development version of Endstone. Consider use a release tag (e.g. `v0.4.0`) instead of `main`.
    2.  :information_source: The following lines are necessary for ensuring ABI compatibility with the Bedrock Dedicated Server (BDS).

    [JetBrains CLion]: https://www.jetbrains.com/clion/

## Create the main plugin class

=== "Python"

    Now, right click on the project folder and select **New > Directory** to create a `src` directory. 

    Right click on the `src` directory you just created and select **Mark Directory as > Sources Root**. You will notice
    the colour of the icon changes to blue.

    Right click again on the `src` directory and select **New > Python Package** to create a package for our plugin. 
    Since my project name is `endstone-my-plugin`, I will name the package `endstone_my_plugin`.
    
    You should have something similar to this:
    ![Create Python Pcakge](screenshots/pycharm-create-package.png)
    
    !!! tip
        For Python packages, it is a common practice to use `lower-case-with-dash` for project name and 
        `lower_case_with_underscore` for the package name. See [PEP 8] for the style guide for Python.

    Right click on the package you just created and select **New > Python File** to create a `my_plugin.py`. Create a
    class named `MyPlugin` which extends the `Plugin` class from `endstone.plugin`.

    ``` python title="src/endstone_my_plugin/my_plugin.py" linenums="1" 
    from endstone.plugin import Plugin

    class MyPlugin(Plugin):
        pass
    ```

    Then, open the `__init__.py` under the same folder and import the `MyPlugin` class from the Python file and add it 
    to the `__all__` variable.

    ``` python title="src/enstone_my_plugin/__init__.py" linenums="1"
    from enstone_my_plugin.my_plugin import MyPlugin

    __all__ = ["MyPlugin"]
    ```

=== "C++"

    ``` c++ title="include/example_plugin.h" linenums="1" 
    #include "endstone/plugin/plugin.h"

    class ExamplePlugin : public endstone::Plugin {}
    ```

    ``` c++ title="src/example_plugin.cpp" linenums="1"
    #include "example_plugin.h"
    ```

    ``` CMake title="CMakeLists.txt" linenums="1" hl_lines="23-25"
    cmake_minimum_required(VERSION 3.15)
    
    project(endstone_example_plugin CXX)
    
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    
    include(FetchContent)
    FetchContent_Declare(
        endstone
        GIT_REPOSITORY https://github.com/EndstoneMC/endstone.git
        GIT_TAG main
    )
    FetchContent_MakeAvailable(endstone)

    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        add_compile_definitions(_ITERATOR_DEBUG_LEVEL=0)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -fPIC")
        link_libraries(-static-libgcc -static-libstdc++ libc++.a libc++abi.a)
    endif ()

    add_library(${PROJECT_NAME} SHARED src/example_plugin.cpp)
    target_include_directories(${PROJECT_NAME} PUBLIC include)
    target_link_libraries(${PROJECT_NAME} PRIVATE endstone::headers)
    ```

## Add methods

=== "Python"

    Now we want to add a few methods:
    
    - `on_load`: this will be called when the plugin is loaded by the server
    - `on_enable`: this will be called when the plugin is enabled
    - `on_disable`: this will be called when the plugin is disabled (e.g. during server shutdown)

    You can use the logger to log a message when the plugin is loaded, enabled and disabled like below:

    ``` python title="src/endstone_example/example_plugin.py" linenums="1" hl_lines="4-5 7-8 10-11"
    from endstone.plugin import Plugin

    class ExamplePlugin(Plugin):
        def on_load(self) -> None:
            self.logger.info("on_load is called!")

        def on_enable(self) -> None:
            self.logger.info("on_enable is called!")

        def on_disable(self) -> None:
            self.logger.info("on_disable is called!")
    ```

=== "C++"

    ``` c++ title="include/example_plugin.h" linenums="1" hl_lines="4-8 10-13 15-18"
    #include "endstone/plugin/plugin.h"

    class ExamplePlugin : public endstone::Plugin {
    public:
        void onLoad() override
        {
            getLogger().info("onLoad is called");
        }
    
        void onEnable() override
        {
            getLogger().info("onEnable is called");
        }
    
        void onDisable() override
        {
            getLogger().info("onDisable is called");
        }
    }
    ```

## Configure plugin metadata

=== "Python"

    Now, the plugin is almost finished. One more thing, we must tell the server multiple things including name, version,
    description and the compatible API version.

    ``` python title="src/endstone_my_plugin/my_plugin.py" linenums="1" hl_lines="4-7"
    from endstone.plugin import Plugin

    class MyPlugin(Plugin):
        name = "MyPlugin"
        version = "0.1.0"
        api_version = "0.4"
        description = "My first plugin for Endstone servers!"

        def on_load(self) -> None:
            self.logger.info("on_load is called!")

        def on_enable(self) -> None:
            self.logger.info("on_enable is called!")

        def on_disable(self) -> None:
            self.logger.info("on_disable is called!")
    ```
    
    Lastly, to have the plugin discoverable by the server, you must specify an entry point in `pyproject.toml`.

    ``` toml title="pyproject.toml" linenums="1" hl_lines="9-10"
    [build-system]
    requires = ["setuptools >= 61.0"]
    build-backend = "setuptools.build_meta"
    
    [project]
    name = "endstone-my-plugin"
    version = "0.1.0"

    [project.entry-points."endstone"]
    my-plugin = "endstone_my_plugin:MyPlugin"
    ```

    !!! notice
    
        For the entry point, the name **must** be the name of your project **without** the `endstone-` prefix. For example,
        our project name is `endstone-my-plugin` so the entry point's name should be `my-plugin`. The value is simply 
        `{module}:{class}`.

=== "C++"

    ``` c++ title="src/example_plugin.cpp" linenums="1" hl_lines="3-6"
    #include "example_plugin.h"

    ENDSTONE_PLUGIN(/*(1)!*/"CppExamplePlugin", /*(2)!*/"0.4.0", /*(3)!*/ExamplePlugin)
    {
        description = "C++ example plugin for Endstone servers";
    }
    ```

    1.  :abc: This is the plugin name!
    2.  :hash: This is the plugin version!
    3.  :white_check_mark: This is the main class of the plugin!


[JetBrains PyCharm]: https://www.jetbrains.com/pycharm/
[JetBrains CLion]: https://www.jetbrains.com/clion/
[Python]: https://www.python.org/downloads/
[endstone Python package]: ../getting-started/installation.md#with-pip
[CMake]: https://cmake.org/
[PEP 8]: https://peps.python.org/pep-0008/
[PEP 518]: https://peps.python.org/pep-0518/
[Visual Studio]: https://visualstudio.microsoft.com/
[LLVM Toolchain]: https://apt.llvm.org/
