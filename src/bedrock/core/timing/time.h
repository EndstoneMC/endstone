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

#include <chrono>
#include <optional>

class launch_time_clock {
public:
    using time_point = std::chrono::steady_clock::time_point;
    using duration = std::chrono::steady_clock::time_point::duration;
};
static_assert(sizeof(launch_time_clock) == 1);

class Stopwatch {
public:
    Stopwatch();
    virtual ~Stopwatch() = default;
    virtual double stop();
    virtual double stopContinue();
    virtual void print(const std::string &);

private:
    bool operator==(const Stopwatch &) const;
    std::optional<std::chrono::steady_clock::time_point> st_;
    launch_time_clock::duration tt_;
    launch_time_clock::duration last_;
    launch_time_clock::duration max_;
    int count_;
};
static_assert(sizeof(Stopwatch) == 56);
