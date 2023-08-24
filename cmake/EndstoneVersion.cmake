include(FetchContent)
FetchContent_Declare(
        CMakeExtraUtils
        GIT_REPOSITORY https://github.com/LecrisUT/CMakeExtraUtils.git
        GIT_SHALLOW TRUE
        GIT_TAG v0.3.0
)
FetchContent_MakeAvailable(CMakeExtraUtils)
include(DynamicVersion)
dynamic_version(
        PROJECT_PREFIX Endstone_
        OUTPUT_VERSION MINECRAFT_VERSION
        OUTPUT_DESCRIBE ENDSTONE_VERSION
)

# Remove the leading "v"
string(REGEX REPLACE "^v" "" ENDSTONE_VERSION ${ENDSTONE_VERSION})

# Extract and increment the pre-release segment if the dev segment is present
if (ENDSTONE_VERSION MATCHES "-[0-9]+-")
    if (ENDSTONE_VERSION MATCHES "([a|b|rc][0-9]+)")
        # Extract the numerical part of the pre-release segment
        string(REGEX REPLACE "[a|b|rc]([0-9]+)" "\\1" PRE_RELEASE_NUM ${CMAKE_MATCH_1})
        # Increment the number
        math(EXPR PRE_RELEASE_INCREMENTED "${PRE_RELEASE_NUM} + 1")
        # Replace the original pre-release segment
        string(REGEX REPLACE "([a|b|rc])[0-9]+" "\\1${PRE_RELEASE_INCREMENTED}" ENDSTONE_VERSION ${ENDSTONE_VERSION})
    endif ()

    # Extract the dev segment and format it
    string(REGEX REPLACE "-([0-9]+)-.*" ".dev\\1" ENDSTONE_VERSION ${ENDSTONE_VERSION})
endif ()

MESSAGE(STATUS "Minecraft version: ${MINECRAFT_VERSION}")
MESSAGE(STATUS "Endstone version: ${ENDSTONE_VERSION}")

set(PROJECT_VERSION ${MINECRAFT_VERSION})
