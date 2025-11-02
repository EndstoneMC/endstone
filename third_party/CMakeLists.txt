if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()

add_subdirectory(funchook)
if (MSVC)
    add_subdirectory(detours)
endif ()
