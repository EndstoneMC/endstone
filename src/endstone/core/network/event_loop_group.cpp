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

EventLoopGroup::EventLoopGroup(std::size_t threads) : work_guard_(boost::asio::make_work_guard(io_context_))
{
    if (threads == 0) {
        const auto hardware = std::thread::hardware_concurrency();
        threads = hardware > 3 ? static_cast<std::size_t>(hardware) - 2 : 1;
    }
    workers_.reserve(threads);
    for (std::size_t i = 0; i < threads; ++i) {
        workers_.emplace_back([this] { io_context_.run(); });
    }
}

EventLoopGroup::~EventLoopGroup()
{
    work_guard_.reset();  // let run() return once outstanding work drains
    io_context_.stop();   // ... and stop promptly regardless
    for (auto &worker : workers_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

}  // namespace endstone::core
