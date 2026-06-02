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

#include "endstone/core/network/network_thread_pool.h"

#include <boost/asio/post.hpp>

namespace endstone::core {

NetworkThreadPool &NetworkThreadPool::getInstance()
{
    static NetworkThreadPool instance;
    return instance;
}

NetworkThreadPool::~NetworkThreadPool()
{
    stop();
}

void NetworkThreadPool::start(std::size_t workers)
{
    if (running_.exchange(true)) {
        return;  // already running
    }

    if (workers == 0) {
        const auto hardware = std::thread::hardware_concurrency();
        workers = hardware > 3 ? static_cast<std::size_t>(hardware) - 2 : 1;
    }

    io_context_.restart();
    work_guard_.emplace(boost::asio::make_work_guard(io_context_));
    workers_.reserve(workers);
    for (std::size_t i = 0; i < workers; ++i) {
        workers_.emplace_back([this] { io_context_.run(); });
    }
}

void NetworkThreadPool::stop()
{
    if (!running_.exchange(false)) {
        return;  // not running
    }

    work_guard_.reset();
    io_context_.stop();
    for (auto &worker : workers_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
    workers_.clear();

    std::lock_guard lock(main_queue_mutex_);
    main_queue_.clear();
}

void NetworkThreadPool::postToMain(std::function<void()> task)
{
    std::lock_guard lock(main_queue_mutex_);
    main_queue_.push_back(std::move(task));
}

void NetworkThreadPool::drainMainQueue()
{
    std::vector<std::function<void()>> tasks;
    {
        std::lock_guard lock(main_queue_mutex_);
        tasks.swap(main_queue_);
    }
    for (auto &task : tasks) {
        task();
    }
}

}  // namespace endstone::core
