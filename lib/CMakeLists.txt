# ------------------------
# Compile Definitions
# ------------------------
add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
add_definitions(-DPYBIND11_DETAILED_ERROR_MESSAGES)
add_definitions(-DMINECRAFT_VERSION="${MINECRAFT_VERSION}")
add_definitions(-DENDSTONE_VERSION="${ENDSTONE_VERSION}")

# ------------------------
# Source Files Gathering
# ------------------------
file(
        GLOB_RECURSE ENDSTONE_SOURCES CONFIGURE_DEPENDS
        LIST_DIRECTORIES false
        RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
        "src/*.cpp"
)

file(
        GLOB_RECURSE ENDSTONE_PYBIND11_MODULE_SOURCES CONFIGURE_DEPENDS
        LIST_DIRECTORIES false
        RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
        "src/pybind/*.cpp"
)

# ------------------------
# API Library Setup
# ------------------------
add_library(api INTERFACE)
add_library(endstone::api ALIAS api)
target_include_directories(api INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (NOT ${CMAKE_SOURCE_DIR} STREQUAL ${PROJECT_SOURCE_DIR})
    # endstone is being included as a sub-module.
    return()
endif ()

# ------------------------
# Main Shared Library Setup
# ------------------------
add_library(libendstone SHARED ${ENDSTONE_SOURCES})
target_precompile_headers(libendstone PUBLIC include/endstone/common.h)
target_include_directories(libendstone PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# External Dependencies
set(
        ENDSTONE_PRIVATE_LIBRARIES
        pybind11::embed
        nlohmann_json::nlohmann_json
)

if (WIN32)
    set_target_properties(libendstone PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

    FetchContent_Declare(
            minhook
            GIT_REPOSITORY https://github.com/TsudaKageyu/minhook.git
            GIT_TAG master
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(minhook)
    list(APPEND ENDSTONE_PRIVATE_LIBRARIES minhook dbghelp.lib)
endif ()

target_link_libraries(libendstone PUBLIC endstone::api PRIVATE ${ENDSTONE_PRIVATE_LIBRARIES})

# ------------------------
# Pybind11 Modules Setup
# ------------------------
set(ENDSTONE_PYBIND11_MODULES _plugin _server _logger)

foreach (ENDSTONE_PYBIND11_MODULE ${ENDSTONE_PYBIND11_MODULES})
    pybind11_add_module(
            ${ENDSTONE_PYBIND11_MODULE} MODULE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/pybind/${ENDSTONE_PYBIND11_MODULE}.cpp
    )
    target_link_libraries(${ENDSTONE_PYBIND11_MODULE} PRIVATE libendstone)

    if (WIN32)
        target_link_options(${ENDSTONE_PYBIND11_MODULE} PRIVATE "/DELAYLOAD:libendstone.dll")
        target_link_libraries(${ENDSTONE_PYBIND11_MODULE} PRIVATE delayimp.lib)
    endif ()
endforeach ()

# ------------------------
# Installation Setup
# ------------------------
install(
        TARGETS libendstone ${ENDSTONE_PYBIND11_MODULES}
        LIBRARY DESTINATION endstone
        RUNTIME DESTINATION endstone
)