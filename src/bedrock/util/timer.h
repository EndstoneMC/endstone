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

#include <functional>

class Timer {
public:
    Timer(float, std::function<std::int64_t()>);
    [[nodiscard]] std::uint64_t getTicks() const;
    [[nodiscard]] float getAlpha() const;
    [[nodiscard]] float getTimeScale() const;
    [[nodiscard]] float getTime() const;
    void setTimeScale(float time_scale);
    void advanceTime(float preferred_frame_step);
    void advanceTimeQuickly();
    void skipTime();
    void stepTick(int num_steps);
    [[nodiscard]] bool stepping() const;
    void resetTimePassed();
    [[nodiscard]] float getMaxTimePerUpdate() const;
    [[nodiscard]] float getLastTimestep() const;
    [[nodiscard]] float getTicksPerSecond() const;
    int popOverflowTicks();

private:
    float ticks_per_second_;
    int ticks_;
    float alpha_;
    float time_scale_;
    float passed_time_;
    float frame_step_alignment_remainder_;
    float last_time_seconds_;
    float last_time_step_;
    float overflow_time_;
    int64_t last_ms_;
    int64_t last_ms_sys_time_;
    float adjust_time_;
    int stepping_tick_;
    std::function<std::int64_t()> get_time_ms_callback_;
    static constexpr int MAX_TICKS_PER_UPDATE = 10;
    static constexpr float MAX_TIME_PER_UPDATE = 1.0 / MAX_TICKS_PER_UPDATE;
};
