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

// The ENDSTONE_NOINLINE macro is for function DEFINITIONS.
// FORWARD DECLARATIONS should never use this macro:
// https://stackoverflow.com/questions/9317473/forward-declaration-of-inline-functions
#if defined(ENDSTONE_NOINLINE_DISABLED)
#define ENDSTONE_NOINLINE inline
#elif defined(_MSC_VER)
#define ENDSTONE_NOINLINE __declspec(noinline) inline
#else
#define ENDSTONE_NOINLINE __attribute__((noinline)) inline
#endif
