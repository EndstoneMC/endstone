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

#include <cstddef>
#include <thread>
#include <vector>

#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/strand.hpp>

namespace endstone::core {

/**
 * The worker pool that drives async network packet processing (issue #356), modelled on Netty's EventLoopGroup:
 * a boost::asio io_context backed by a fixed pool of worker threads. RAII -- the threads start in the constructor
 * and are joined in the destructor -- so EndstoneServer just holds it in a std::unique_ptr (its lifetime is the
 * pool's lifetime). Each connection binds to one EventLoop (a serialized asio strand) obtained via next(); the
 * EventLoop is handed to the AsyncBatchedNetworkPeer at construction and owns that connection's inner-chain codec.
 */
class EventLoopGroup {
public:
    using EventLoop = boost::asio::strand<boost::asio::io_context::executor_type>;

    /**
     * Start `threads` worker threads. 0 = auto = max(1, hardware_concurrency() - 2).
     */
    explicit EventLoopGroup(std::size_t threads = 0);
    ~EventLoopGroup();

    EventLoopGroup(const EventLoopGroup &) = delete;
    EventLoopGroup &operator=(const EventLoopGroup &) = delete;

    /**
     * Return an EventLoop to bind a new connection to. (Each call creates a fresh strand multiplexed onto the
     * shared worker threads, so per-connection ops are serialized while different connections run in parallel.)
     */
    [[nodiscard]] EventLoop next() { return boost::asio::make_strand(io_context_); }

private:
    boost::asio::io_context io_context_;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard_;
    std::vector<std::thread> workers_;
};

}  // namespace endstone::core
