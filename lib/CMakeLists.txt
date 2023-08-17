add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
add_definitions(-DENDSTONE_VERSION="${PROJECT_VERSION}")

file(
        GLOB_RECURSE ENDSTONE_SOURCES
        LIST_DIRECTORIES false
        RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
        "src/*.cpp"
)

file(
        GLOB_RECURSE ENDSTONE_PYBIND11_MODULE_SOURCES
        LIST_DIRECTORIES false
        RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
        "src/endstone/*.cpp"
)

add_library(libendstone SHARED ${ENDSTONE_SOURCES})
target_include_directories(libendstone PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_precompile_headers(libendstone PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/common.h)

set(
        ENDSTONE_LIBRARIES
        pybind11::embed
        nlohmann_json::nlohmann_json
)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    FetchContent_Declare(
            minhook
            GIT_REPOSITORY https://github.com/TsudaKageyu/minhook.git
            GIT_TAG master
            GIT_PROGRESS TRUE
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(minhook)

    list(APPEND ENDSTONE_LIBRARIES minhook dbghelp.lib)
endif ()

target_link_libraries(libendstone PRIVATE ${ENDSTONE_LIBRARIES})

set(ENDSTONE_PYBIND11_MODULES _logger _server _plugin_manager)

foreach (ENDSTONE_PYBIND11_MODULE ${ENDSTONE_PYBIND11_MODULES})
    pybind11_add_module(${ENDSTONE_PYBIND11_MODULE} MODULE ${ENDSTONE_PYBIND11_MODULE_SOURCES})
    target_link_libraries(${ENDSTONE_PYBIND11_MODULE} PRIVATE libendstone)
endforeach ()

install(TARGETS libendstone ${ENDSTONE_PYBIND11_MODULES}
        LIBRARY DESTINATION endstone
        RUNTIME DESTINATION endstone)