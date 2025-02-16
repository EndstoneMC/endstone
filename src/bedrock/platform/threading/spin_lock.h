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
#include <system_error>
#include <thread>

class SpinLockImpl {
    static const uint32_t LOOP_LIMIT_BEFORE_YIELD = 3000;

public:
    SpinLockImpl() : no_thread_id_(thread_hasher_(std::thread::id())), owner_thread_(no_thread_id_) {}

    ~SpinLockImpl() = default;

    bool try_lock()
    {
        std::size_t current_thread_id = thread_hasher_(std::this_thread::get_id());

        if (owner_thread_ == no_thread_id_) {
            owner_thread_ = current_thread_id;
            owner_ref_count_ = 1;
            return true;
        }

        if (owner_thread_ != current_thread_id || owner_ref_count_ == -2) {
            return false;
        }

        ++owner_ref_count_;
        return true;
    }

    void lock()
    {
        std::size_t current_thread_id = thread_hasher_(std::this_thread::get_id());
        int i = LOOP_LIMIT_BEFORE_YIELD;
        while (true) {
            if (owner_thread_ == no_thread_id_) {
                owner_thread_ = current_thread_id;
                break;
            }

            if (owner_thread_ == current_thread_id && owner_ref_count_ != -2) {
                ++owner_ref_count_;
                return;
            }

            if (i > 0) {
                --i;
            }
            else {
                std::this_thread::yield();
            }
        }
        owner_ref_count_ = 1;
    }

    void unlock()
    {
        std::size_t current_thread_id = thread_hasher_(std::this_thread::get_id());

        if (owner_thread_ != current_thread_id || owner_ref_count_ == 0) {
            throw std::system_error(std::make_error_code(std::errc::operation_not_permitted));
        }

        if (owner_ref_count_ == 1) {
            owner_ref_count_ = 0;
            owner_thread_ = no_thread_id_;
        }
        else {
            --owner_ref_count_;
        }
    }

private:
    std::hash<std::thread::id> thread_hasher_{};  // +0
    const std::size_t no_thread_id_;              // +8
    std::atomic<std::size_t> owner_thread_;       // +16
    std::uint32_t owner_ref_count_{0};            // +24
};
