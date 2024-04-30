# Create your first plugin

## Create a new project

=== "Python"

    TODO

    ``` toml title="pyproject.toml" linenums="1" hl_lines="9-10"
    [build-system]
    requires = ["setuptools >= 61.0"]
    build-backend = "setuptools.build_meta"
    
    [project]
    name = "endstone-example"
    version = "0.4.0"
    ```

=== "C++"

    TODO

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

## Create the main plugin class

=== "Python"

    ``` python title="src/endstone_example/example_plugin.py" linenums="1" 
    from endstone.plugin import Plugin

    class ExamplePlugin(Plugin):
        pass
    ```

    ``` python title="src/endstone_example/__init__.py" linenums="1"
    from endstone_example.example_plugin import ExamplePlugin

    __all__ = ["ExamplePlugin"]
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

    ``` python title="src/endstone_example/example_plugin.py" linenums="1" hl_lines="4-7"
    from endstone.plugin import Plugin

    class ExamplePlugin(Plugin):
        name = "PythonExamplePlugin"
        version = "0.4.0"
        api_version = "0.4"
        description = "Python example plugin for Endstone servers"

        def on_load(self) -> None:
            self.logger.info("on_load is called!")

        def on_enable(self) -> None:
            self.logger.info("on_enable is called!")

        def on_disable(self) -> None:
            self.logger.info("on_disable is called!")
    ```

    ``` toml title="pyproject.toml" linenums="1" hl_lines="9-10"
    [build-system]
    requires = ["setuptools >= 61.0"]
    build-backend = "setuptools.build_meta"
    
    [project]
    name = "endstone-example"
    version = "0.4.0"

    [project.entry-points."endstone"]
    example = "endstone_example:ExamplePlugin"
    ```

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