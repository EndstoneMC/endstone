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

#include <atomic>
#include <cstddef>
#include <memory>
#include <thread>
#include <vector>

#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/io_context.hpp>

namespace endstone::core {

/**
 * The async network worker pool (issue #356), modelled on Netty's EventLoopGroup. Unlike a single shared io_context
 * with a strand per connection, each EventLoop here is its OWN single-threaded io_context: a connection binds to one
 * via next() and keeps it for life, so all of that connection's async work runs on the same thread. That makes the
 * work both serialized AND thread-affine -- the connection's cipher/compression/buffer state stays hot on one core,
 * with none of a strand's cross-thread hand-off or dispatch overhead. Many connections share one EventLoop (round-
 * robin), exactly like Netty channels sharing an event loop.
 */
class EventLoopGroup {
public:
    // A connection's bound executor. Posting to it runs the work on that loop's single dedicated thread.
    using EventLoop = boost::asio::io_context::executor_type;

    // 0 threads = auto = max(1, hardware_concurrency() - 2).
    explicit EventLoopGroup(std::size_t threads = 0);
    ~EventLoopGroup();

    EventLoopGroup(const EventLoopGroup &) = delete;
    EventLoopGroup &operator=(const EventLoopGroup &) = delete;

    // Round-robin the next EventLoop. The returned executor stays valid until this group is destroyed.
    [[nodiscard]] EventLoop next();

private:
    // io_context + thread are non-movable, so each loop lives behind a stable heap pointer (the thread captures it).
    struct Loop {
        boost::asio::io_context io_context;
        boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard;
        std::thread thread;

        Loop() : work_guard(boost::asio::make_work_guard(io_context)) {}
    };

    std::vector<std::unique_ptr<Loop>> loops_;
    std::atomic<std::size_t> next_{0};
};

}  // namespace endstone::core
