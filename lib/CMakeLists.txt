add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)

file(
        GLOB_RECURSE SOURCE_FILES
        LIST_DIRECTORIES false
        RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
        "src/*.cpp"
)

add_library(libendstone SHARED ${SOURCE_FILES})

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

target_link_libraries(libendstone PRIVATE
        pybind11::embed
        tomlplusplus::tomlplusplus
        indicators::indicators)

pybind11_add_module(_server MODULE src/python/server.cpp)
pybind11_add_module(_plugin MODULE src/python/plugin.cpp)
pybind11_add_module(_plugin_manager MODULE src/python/plugin_manager.cpp src/python/server.cpp)

install(TARGETS libendstone _plugin _plugin_manager _server
        LIBRARY DESTINATION endstone
        RUNTIME DESTINATION endstone)