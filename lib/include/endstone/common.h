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
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <regex>
#include <string>
#include <system_error>
#include <thread>
#include <vector>

// Platform-specific macros for exporting symbols
#ifdef _WIN32
#define BEDROCK_API __declspec(dllexport)
#define ENDSTONE_API __declspec(dllexport)
#else
#define BEDROCK_API
#define ENDSTONE_API
#endif

#define ENDSTONE_PLUGIN_CLASS(ClassName)                                                                               \
    extern "C" ENDSTONE_API CppPlugin *createPlugin()                                                                  \
    {                                                                                                                  \
        return new ClassName();                                                                                        \
    }

#endif // ENDSTONE_COMMON_H
