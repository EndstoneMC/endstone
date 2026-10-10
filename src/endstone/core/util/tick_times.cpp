// Copyright (c) 2026, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include "endstone/core/util/tick_times.h"

#include <algorithm>
#include <numeric>

namespace endstone::core {

TickTimes::TickTimes(const std::size_t size) : times_(size) {}

void TickTimes::add(const std::chrono::nanoseconds time)
{
    times_[index_] = time;
    index_ = (index_ + 1) % times_.size();
    count_ = std::min(count_ + 1, times_.size());
}

std::chrono::duration<double, std::milli> TickTimes::getAverage() const
{
    if (count_ == 0) {
        return {};
    }
    const auto total = std::accumulate(times_.begin(), times_.begin() + static_cast<std::ptrdiff_t>(count_),
                                       std::chrono::nanoseconds::zero());
    return std::chrono::duration<double, std::milli>(total) / static_cast<double>(count_);
}

}  // namespace endstone::core
