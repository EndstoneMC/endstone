# ------------------------
# Endstone API Pybind
# ------------------------
file(
        GLOB_RECURSE ENDSTONE_PYBIND11_MODULES CONFIGURE_DEPENDS
        LIST_DIRECTORIES false
        RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
        "pybind/*.cpp"
)

FetchContent_Declare(
        pybind11
        URL https://github.com/pybind/pybind11/archive/b6444460eeddc2965ab1a49c6c50c83073779489.zip # smart_holder branch v2.11.1
)
FetchContent_MakeAvailable(pybind11)

pybind11_add_module(_bindings MODULE ${ENDSTONE_PYBIND11_MODULES})
target_include_directories(_bindings PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(_bindings PRIVATE endstone::api)
install(TARGETS _bindings
        LIBRARY DESTINATION ${PROJECT_NAME}
        RUNTIME DESTINATION ${PROJECT_NAME}
)

# ------------------------
# Endstone Core
# ------------------------
file(
        GLOB_RECURSE ENDSTONE_SOURCES CONFIGURE_DEPENDS
        LIST_DIRECTORIES false
        RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
        "endstone/*.cpp"
)

add_library(core STATIC ${ENDSTONE_SOURCES})
add_library(endstone::core ALIAS core)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)
FetchContent_MakeAvailable(json)
target_link_libraries(core PUBLIC endstone::api pybind11::embed nlohmann_json::nlohmann_json)

# ------------------------
# Endstone Dynamic Library
# ------------------------
file(
        GLOB_RECURSE ENDSTONE_LIB_SOURCES CONFIGURE_DEPENDS
        LIST_DIRECTORIES false
        RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
        "lib/*.cpp"
)
add_library(libendstone SHARED ${ENDSTONE_LIB_SOURCES})
target_compile_definitions(libendstone PRIVATE BEDROCK_EXPORTS)
target_include_directories(libendstone PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib)

if (WIN32)
    FetchContent_Declare(
            minhook
            GIT_REPOSITORY https://github.com/TsudaKageyu/minhook.git
            GIT_TAG master
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(minhook)
    target_link_libraries(libendstone PRIVATE minhook dbghelp.lib)
endif ()

target_link_libraries(libendstone PUBLIC endstone::api endstone::core)

install(TARGETS libendstone
        LIBRARY DESTINATION ${PROJECT_NAME}
        RUNTIME DESTINATION ${PROJECT_NAME}
)