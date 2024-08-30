# EndstoneHeaders.cmake -- Build system for endstone plugins
#
# Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# ===================
# Compiler Check
# ===================
if (WIN32)
    if (NOT CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        message(FATAL_ERROR "Endstone: MSVC is required on Windows.")
    endif ()
    # Force set _ITERATOR_DEBUG_LEVEL to 0 for ABI compatibility with BDS
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_ITERATOR_DEBUG_LEVEL=0" CACHE STRING "" FORCE)
elseif (UNIX)
    if (NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        message(FATAL_ERROR "Endstone: Clang is required on Linux.")
    endif ()
    # Force use libc++ for ABI compatibility with BDS
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -fPIC" CACHE STRING "" FORCE)
else ()
    message(FATAL_ERROR "Endstone: ${CMAKE_SYSTEM_NAME} (${CMAKE_SYSTEM_PROCESSOR}) is not supported")
endif ()


# ===================
# Dependencies
# ===================
include(FetchContent)
FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 10.2.1
)
FetchContent_MakeAvailable(fmt)


# ===================
# Endstone API
# ===================
add_library(endstone_headers INTERFACE)
add_library(endstone::headers ALIAS endstone_headers)
target_include_directories(endstone_headers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(endstone_headers INTERFACE fmt::fmt)


# ===================
# Endstone Add Plugin
# ===================
function(endstone_add_plugin target_name)
    cmake_parse_arguments(PARSE_ARGV 1 ARG "" "" "")

    add_library(${target_name} SHARED ${ARG_UNPARSED_ARGUMENTS})
    target_include_directories(${target_name} PUBLIC include)
    target_link_libraries(${target_name} PRIVATE endstone::headers)
    set_target_properties(${target_name} PROPERTIES PREFIX "endstone_")

    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_link_libraries(${target_name} PRIVATE -static-libgcc -static-libstdc++ libc++.a libc++abi.a)
    endif ()
endfunction()
