// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#define ENDSTONE_STRINGIFY(x) #x
#define ENDSTONE_TOSTRING(x)  ENDSTONE_STRINGIFY(x)

#define ENDSTONE_VERSION_MAJOR 0
#define ENDSTONE_VERSION_MINOR 4
#define ENDSTONE_VERSION_PATCH 0
#define ENDSTONE_API_VERSION   ENDSTONE_TOSTRING(ENDSTONE_VERSION_MAJOR) "." ENDSTONE_TOSTRING(ENDSTONE_VERSION_MINOR);

#if !defined(ENDSTONE_EXPORT)
#if defined(WIN32) || defined(_WIN32)
#define ENDSTONE_EXPORT __declspec(dllexport)
#else
#define ENDSTONE_EXPORT __attribute__((visibility("default")))
#endif

#if defined(_MSC_VER)
#define ENDSTONE_NOINLINE __declspec(noinline) inline
#else
#define ENDSTONE_NOINLINE __attribute__((noinline)) inline
#endif

#ifdef _WIN32
#define _WIN32_LINUX_(win32, linux) win32
#elif __linux__
#define _WIN32_LINUX_(win32, linux) linux
#endif

#endif
