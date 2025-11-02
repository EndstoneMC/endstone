include(FetchContent)
FetchContent_Declare(
        funchook
        GIT_REPOSITORY https://github.com/kubo/funchook.git
        GIT_TAG v1.1.3
        GIT_SHALLOW 1
)
set(FUNCHOOK_BUILD_SHARED OFF CACHE BOOL "Build funchook shared library")
set(FUNCHOOK_BUILD_TESTS OFF CACHE BOOL "Build funchook tests")
set(FUNCHOOK_INSTALL OFF CACHE BOOL "Install funchook")
# set(FUNCHOOK_DISASM capstone CACHE STRING "funchook disassembler engine")
FetchContent_MakeAvailable(funchook)
add_library(funchook::funchook ALIAS funchook-static)
