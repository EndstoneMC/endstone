if (NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "Only 64-bit is supported")
endif ()

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    if (NOT MSVC)
        message(FATAL_ERROR "Only MSVC is supported on Windows")
    else ()
        message(STATUS "Building on Windows ${CMAKE_SYSTEM_PROCESSOR} with MSVC")
    endif ()
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if (NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        message(FATAL_ERROR "Only Clang is supported on Linux")
    else ()
        message(STATUS "Building on Linux ${CMAKE_SYSTEM_PROCESSOR} with Clang")
    endif ()
else ()
    message(FATAL_ERROR "${CMAKE_SYSTEM_NAME} is not a supported platform. Only Windows and Linux are supported.")
endif ()