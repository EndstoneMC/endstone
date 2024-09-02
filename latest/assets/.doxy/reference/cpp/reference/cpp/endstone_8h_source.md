

# File endstone.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**endstone.h**](endstone_8h.md)

[Go to the documentation of this file](endstone_8h.md)


```C++
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
#define ENDSTONE_VERSION_MINOR 5
#define ENDSTONE_VERSION_PATCH 2
#define ENDSTONE_API_VERSION   ENDSTONE_TOSTRING(ENDSTONE_VERSION_MAJOR) "." ENDSTONE_TOSTRING(ENDSTONE_VERSION_MINOR)

#define ENDSTONE_MAX_PLAYERS 200

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
#define ENDSTONE_VARIANT_WIN32_LINUX(win32, linux) win32
#elif __linux__
#define ENDSTONE_VARIANT_WIN32_LINUX(win32, linux) linux
#endif

namespace endstone {
template <typename... Func>
struct overloaded : Func... {
  using Func::operator()...;
};

template <typename... Func>
overloaded(Func...) -> overloaded<Func...>;
}  // namespace endstone
```


