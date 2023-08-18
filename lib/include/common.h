//
// Created by Vincent on 07/08/2023.
//

// common.h - Intended to be used as a Precompiled Header (PCH)
// Contains commonly used and stable headers to improve compilation times.

#ifndef ENDSTONE_COMMON_H
#define ENDSTONE_COMMON_H

// C++ Standard Library headers
#include <bitset>
#include <codecvt>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <regex>
#include <string>
#include <system_error>
#include <thread>
#include <vector>

// Third-party library headers (pybind11)
#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// Namespace alias for pybind11 (used frequently across the project)
namespace py = pybind11;

// Platform-specific macros for exporting symbols
#if defined(_WIN32)
#define BEDROCK_API __declspec(dllexport)
#define ENDSTONE_API __declspec(dllexport)
#else
#define BEDROCK_API
#define ENDSTONE_API
#endif

#endif // ENDSTONE_COMMON_H
