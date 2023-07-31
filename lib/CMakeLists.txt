add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)

file(
        GLOB_RECURSE SOURCE_FILES
        LIST_DIRECTORIES false
        RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
        "*.cpp"
)

add_library(libendstone SHARED ${SOURCE_FILES} src/plugin/plugin_manager.cpp include/plugin_manager.h)

include_directories(libendstone PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    FetchContent_Declare(
            minhook
            GIT_REPOSITORY https://github.com/TsudaKageyu/minhook.git
            GIT_TAG master
    )
    FetchContent_MakeAvailable(minhook)

    target_link_libraries(libendstone PRIVATE minhook dbghelp.lib)


endif ()

pybind11_add_module(plugin MODULE src/plugin/plugin.cpp)

target_link_libraries(libendstone PRIVATE
        pybind11::embed
        tomlplusplus::tomlplusplus)


install(TARGETS libendstone plugin
        LIBRARY DESTINATION endstone
        RUNTIME DESTINATION endstone)