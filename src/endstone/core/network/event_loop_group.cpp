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

#include "endstone/core/network/event_loop_group.h"

namespace endstone::core {

EventLoopGroup::EventLoopGroup(std::size_t threads)
{
    if (threads == 0) {
        const auto hardware = std::thread::hardware_concurrency();
        threads = hardware > 3 ? static_cast<std::size_t>(hardware) - 2 : 1;
    }
    loops_.reserve(threads);
    for (std::size_t i = 0; i < threads; ++i) {
        auto loop = std::make_unique<Loop>();
        auto *io = &loop->io_context;
        loop->thread = std::thread([io] { io->run(); });
        loops_.push_back(std::move(loop));
    }
}

EventLoopGroup::~EventLoopGroup()
{
    for (auto &loop : loops_) {
        loop->work_guard.reset();  // let run() return once outstanding work drains
        loop->io_context.stop();   // ... and stop promptly regardless
    }
    for (auto &loop : loops_) {
        if (loop->thread.joinable()) {
            loop->thread.join();
        }
    }
}

EventLoopGroup::EventLoop EventLoopGroup::next()
{
    const auto index = next_.fetch_add(1, std::memory_order_relaxed) % loops_.size();
    return loops_[index]->io_context.get_executor();
}

}  // namespace endstone::core
