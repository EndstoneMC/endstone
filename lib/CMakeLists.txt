add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)

file(
        GLOB_RECURSE ENDSTONE_SOURCES
        LIST_DIRECTORIES false
        RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
        "src/*.cpp"
)

add_library(libendstone SHARED ${ENDSTONE_SOURCES})

include_directories(libendstone PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(
        ENDSTONE_LIBRARIES
        pybind11::embed
        tomlplusplus::tomlplusplus
        indicators::indicators
)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    FetchContent_Declare(
            minhook
            GIT_REPOSITORY https://github.com/TsudaKageyu/minhook.git
            GIT_TAG master
    )
    FetchContent_MakeAvailable(minhook)

    list(APPEND ENDSTONE_LIBRARIES minhook dbghelp.lib)
endif ()

target_link_libraries(libendstone PRIVATE ${ENDSTONE_LIBRARIES})

set(ENDSTONE_PYBIND11_MODULES _logger _server _plugin _plugin_manager)

foreach (ENDSTONE_PYBIND11_MODULE ${ENDSTONE_PYBIND11_MODULES})
    pybind11_add_module(${ENDSTONE_PYBIND11_MODULE} MODULE ${ENDSTONE_SOURCES})
    target_link_libraries(${ENDSTONE_PYBIND11_MODULE} PRIVATE ${ENDSTONE_LIBRARIES})
endforeach ()

install(TARGETS libendstone ${ENDSTONE_PYBIND11_MODULE}
        LIBRARY DESTINATION endstone
        RUNTIME DESTINATION endstone)