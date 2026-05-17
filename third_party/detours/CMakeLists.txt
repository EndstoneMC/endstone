include(FetchContent)

FetchContent_Declare(
        detours
        URL https://github.com/microsoft/Detours/archive/9764cebcb1a75940e68fa83d6730ffaf0f669401.zip
)
FetchContent_MakeAvailable(detours)

add_library(detours STATIC
        "${detours_SOURCE_DIR}/src/creatwth.cpp"
        "${detours_SOURCE_DIR}/src/detours.cpp"
        "${detours_SOURCE_DIR}/src/disasm.cpp"
        "${detours_SOURCE_DIR}/src/disolarm.cpp"
        "${detours_SOURCE_DIR}/src/disolarm64.cpp"
        "${detours_SOURCE_DIR}/src/disolia64.cpp"
        "${detours_SOURCE_DIR}/src/disolx64.cpp"
        "${detours_SOURCE_DIR}/src/disolx86.cpp"
        "${detours_SOURCE_DIR}/src/image.cpp"
        "${detours_SOURCE_DIR}/src/modules.cpp"
)

target_include_directories(detours PUBLIC "${detours_SOURCE_DIR}/src")
# Detours is third-party (MSVC-targeted) code; clang-cl does not compile it
# warning-clean, so /WX is dropped here. /Gm- is an MSVC-only flag clang-cl rejects.
target_compile_options(detours PRIVATE /W4 /Zi /MT /Gy /Zl /Od)

include(GNUInstallDirs)
install(TARGETS detours)
install(
        FILES
        "${detours_SOURCE_DIR}/src/detours.h"
        "${detours_SOURCE_DIR}/src/detver.h"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)
