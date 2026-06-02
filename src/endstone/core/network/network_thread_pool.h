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
#include <functional>
#include <mutex>
#include <optional>
#include <thread>
#include <vector>

#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/strand.hpp>

namespace endstone::core {

/**
 * Global asio runtime for async network packet processing (issue #356).
 *
 * Owns an io_context, a fixed pool of worker threads, and a main-thread completion queue. Each connection gets
 * its own strand (serialized codec) multiplexed onto the shared worker pool. Work that must run on the BDS main
 * thread (e.g. flush completion callbacks) is posted to the main queue and drained once per server tick.
 */
class NetworkThreadPool {
public:
    using Strand = boost::asio::strand<boost::asio::io_context::executor_type>;

    static NetworkThreadPool &getInstance();

    NetworkThreadPool(const NetworkThreadPool &) = delete;
    NetworkThreadPool &operator=(const NetworkThreadPool &) = delete;

    /**
     * Start `workers` threads. 0 = auto = max(1, hardware_concurrency() - 2). Idempotent.
     */
    void start(std::size_t workers = 0);

    /**
     * Reset the work guard, stop the io_context and join all workers. Safe to call when not started.
     */
    void stop();

    [[nodiscard]] bool isRunning() const { return running_; }

    [[nodiscard]] Strand makeStrand() { return boost::asio::make_strand(io_context_); }

    [[nodiscard]] boost::asio::io_context &context() { return io_context_; }

    /**
     * Queue a callback to run on the main thread (drained by drainMainQueue()).
     */
    void postToMain(std::function<void()> task);

    /**
     * Run all queued main-thread callbacks. Must be called from the main thread, once per server tick.
     */
    void drainMainQueue();

private:
    NetworkThreadPool() = default;
    ~NetworkThreadPool();

    boost::asio::io_context io_context_;
    std::optional<boost::asio::executor_work_guard<boost::asio::io_context::executor_type>> work_guard_;
    std::vector<std::thread> workers_;
    std::atomic<bool> running_{false};

    std::mutex main_queue_mutex_;
    std::vector<std::function<void()>> main_queue_;
};

}  // namespace endstone::core
