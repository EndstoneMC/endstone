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

#include "bedrock/network/raknet/get_time.h"

static bool initialized = false;  // NOLINT

#ifdef _WIN32
#include <windows.h>
RakNet::TimeUS GetTimeUS_Windows()
{
    if (!initialized) {
        initialized = true;
    }

    RakNet::TimeUS current_time;
    LARGE_INTEGER frequency;
    LARGE_INTEGER pref;

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&pref);

    auto quotient = ((pref.QuadPart) / frequency.QuadPart);
    auto remainder = ((pref.QuadPart) % frequency.QuadPart);
    current_time = static_cast<RakNet::TimeUS>(quotient) * static_cast<RakNet::TimeUS>(1000000) +
                   (remainder * static_cast<RakNet::TimeUS>(1000000) / frequency.QuadPart);
    return current_time;
}
#endif

#ifdef __linux__
#include <unistd.h>

#include <sys/time.h>
RakNet::TimeUS initialTime;

RakNet::TimeUS GetTimeUS_Linux()
{
    timeval tp;
    if (!initialized) {
        gettimeofday(&tp, 0);
        initialized = true;
        initialTime = (tp.tv_sec) * (RakNet::TimeUS)1000000 + (tp.tv_usec);
    }

    RakNet::TimeUS current_time;
    gettimeofday(&tp, 0);
    current_time = (tp.tv_sec) * (RakNet::TimeUS)1000000 + (tp.tv_usec);
    return current_time - initialTime;
}
#endif

RakNet::TimeUS RakNet::GetTimeUS()
{
#if defined(_WIN32)
    return GetTimeUS_Windows();
#else
    return GetTimeUS_Linux();
#endif
}
