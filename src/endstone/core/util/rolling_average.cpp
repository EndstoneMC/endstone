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

#include "endstone/core/util/rolling_average.h"

namespace endstone::core {

RollingAverage::RollingAverage(const std::size_t size, const double initial)
    : samples_(size, initial), times_(size, std::chrono::seconds(1)), time_(std::chrono::seconds(size)),
      total_(initial * static_cast<double>(time_.count()))
{
}

void RollingAverage::add(const double value, const std::chrono::nanoseconds time)
{
    time_ -= times_[index_];
    total_ -= samples_[index_] * static_cast<double>(times_[index_].count());
    samples_[index_] = value;
    times_[index_] = time;
    time_ += time;
    total_ += value * static_cast<double>(time.count());
    index_ = (index_ + 1) % samples_.size();
}

double RollingAverage::getAverage() const
{
    return total_ / static_cast<double>(time_.count());
}

}  // namespace endstone::core
