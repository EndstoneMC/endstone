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

#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

#include <gtest/gtest.h>
#include <pybind11/embed.h>
#include <pybind11/eval.h>
#include <pybind11/functional.h>

#include "endstone/runtime/python_thread_state.h"

namespace py = pybind11;
using namespace std::chrono_literals;

TEST(PythonThreadStateTest, ReusesRetainedStateWithoutHoldingGil)
{
    py::scoped_interpreter interpreter{};

    std::mutex mutex;
    std::condition_variable condition;
    std::vector<std::uintptr_t> callback_states;
    std::vector<int> listener_calls;
    std::uintptr_t primary_state = 0;
    std::uintptr_t async_state = 0;
    bool callback_ready = false;
    bool contender_started = false;
    bool contender_acquired = false;
    bool acquired_before_release = false;
    bool retained_without_gil = false;
    bool released_without_gil = false;
    bool second_cycle_without_gil = false;
    bool exception_seen = false;
    bool listener_unregistered = false;

    py::dict scope;
    scope["record"] = py::cpp_function([&](int listener) {
        listener_calls.push_back(listener);
        callback_states.push_back(reinterpret_cast<std::uintptr_t>(PyThreadState_Get()));
    });
    auto first_executor = py::eval("lambda: record(1)", scope, scope).cast<std::function<void()>>();
    auto second_executor = py::eval("lambda: record(2)", scope, scope).cast<std::function<void()>>();
    auto failing_executor = py::eval("lambda: 1 / 0").cast<std::function<void()>>();
    auto removable_executor = py::eval("lambda: None").cast<std::function<void()>>();

    {
        py::gil_scoped_release release;

        std::thread server_thread([&]() {
            endstone::runtime::python::retainThreadState();
            endstone::runtime::python::retainThreadState();
            retained_without_gil = PyGILState_Check() == 0;

            first_executor();
            second_executor();
            try {
                failing_executor();
            }
            catch (const py::error_already_set &) {
                exception_seen = true;
            }
            first_executor();
            removable_executor();
            removable_executor = {};
            listener_unregistered = true;

            primary_state = callback_states.front();
            {
                std::lock_guard lock(mutex);
                callback_ready = true;
            }
            condition.notify_all();

            {
                std::unique_lock lock(mutex);
                condition.wait(lock, [&]() { return contender_started; });
                acquired_before_release = condition.wait_for(lock, 5s, [&]() { return contender_acquired; });
            }

            endstone::runtime::python::releaseThreadState();
            released_without_gil = PyGILState_Check() == 0;

            endstone::runtime::python::retainThreadState();
            second_executor();
            endstone::runtime::python::releaseThreadState();
            second_cycle_without_gil = PyGILState_Check() == 0;
        });

        {
            std::unique_lock lock(mutex);
            condition.wait(lock, [&]() { return callback_ready; });
        }

        std::thread async_thread([&]() {
            {
                std::lock_guard lock(mutex);
                contender_started = true;
            }
            condition.notify_all();

            py::gil_scoped_acquire gil;
            async_state = reinterpret_cast<std::uintptr_t>(PyThreadState_Get());

            {
                std::lock_guard lock(mutex);
                contender_acquired = true;
            }
            condition.notify_all();
        });

        async_thread.join();
        server_thread.join();
    }

    ASSERT_EQ(callback_states.size(), 4);
    EXPECT_EQ(listener_calls, (std::vector<int>{1, 2, 1, 2}));
    EXPECT_EQ(callback_states[0], callback_states[1]);
    EXPECT_EQ(callback_states[1], callback_states[2]);
    EXPECT_EQ(primary_state, callback_states[0]);
    EXPECT_NE(primary_state, async_state);
    EXPECT_TRUE(retained_without_gil);
    EXPECT_TRUE(acquired_before_release);
    EXPECT_TRUE(exception_seen);
    EXPECT_TRUE(listener_unregistered);
    EXPECT_TRUE(released_without_gil);
    EXPECT_TRUE(second_cycle_without_gil);
}
