# ===============
# Compiler Check
# ===============
if (WIN32)
    if (NOT MSVC)
        message(FATAL_ERROR "MSVC is required on Windows.")
    endif ()
    add_compile_definitions(_ITERATOR_DEBUG_LEVEL=0)
elseif (UNIX)
    if (NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        message(FATAL_ERROR "Clang is required on Linux.")
    endif ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -fPIC")
    link_libraries(-static-libgcc -static-libstdc++ libc++.a libc++abi.a)
else ()
    message(FATAL_ERROR "${CMAKE_SYSTEM_NAME} (${CMAKE_SYSTEM_PROCESSOR}) is not supported")
endif ()


# ===============
# Dependencies
# ===============
include(FetchContent)
FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 10.2.1
)
FetchContent_MakeAvailable(fmt)


# ===============
# Endstone API
# ===============
add_library(endstone_headers INTERFACE)
add_library(endstone::headers ALIAS endstone_headers)
target_include_directories(endstone_headers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(endstone_headers INTERFACE fmt::fmt)