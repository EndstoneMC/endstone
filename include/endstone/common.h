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
#include <cstdint>
#include <filesystem>
#include <fmt/color.h>
#include <fmt/core.h>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <regex>
#include <set>
#include <string>
#include <system_error>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

// Platform-specific macros for exporting symbols
#ifdef _WIN32
#define BEDROCK_API  __declspec(dllexport)
#define ENDSTONE_API __declspec(dllexport)
#else
#define BEDROCK_API
#define ENDSTONE_API
#endif

#endif // ENDSTONE_COMMON_H
