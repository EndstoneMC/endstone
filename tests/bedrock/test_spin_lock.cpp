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

#include <atomic>
#include <chrono>
#include <thread>
#include <vector>

#include <gtest/gtest.h>

#include "bedrock/platform/threading/mutex_details.h"

// Basic test: lock and unlock
TEST(SpinLockTest, BasicLockUnlock)
{
    SpinLockImpl lock;
    EXPECT_TRUE(lock.try_lock());
    lock.unlock();
}

// Test reentrant locking by the same thread
TEST(SpinLockTest, ReentrantLocking)
{
    SpinLockImpl lock;
    EXPECT_TRUE(lock.try_lock());
    EXPECT_TRUE(lock.try_lock());  // Reentrant locking should work
    lock.unlock();
    lock.unlock();  // Should fully unlock
}

// Test unlock without locking should throw an exception
TEST(SpinLockTest, UnlockWithoutLock)
{
    SpinLockImpl lock;
    EXPECT_THROW(lock.unlock(), std::system_error);
}

// Test unlocking from a different thread should throw an exception
TEST(SpinLockTest, UnlockFromDifferentThread)
{
    SpinLockImpl lock;
    lock.lock();
    std::thread t([&]() { EXPECT_THROW(lock.unlock(), std::system_error); });
    t.join();
    lock.unlock();  // Unlock from the original thread
}

// Test contention: multiple threads trying to acquire the lock
TEST(SpinLockTest, Contention)
{
    SpinLockImpl lock;
    std::atomic<int> counter = 0;
    constexpr int num_threads = 10;

    auto worker = [&]() {
        for (int i = 0; i < 100; ++i) {
            lock.lock();
            ++counter;
            lock.unlock();
        }
    };

    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(worker);
    }

    for (auto &t : threads) {
        t.join();
    }

    EXPECT_EQ(counter, num_threads * 100);
}

// Test try_lock() failing when another thread owns the lock
TEST(SpinLockTest, TryLockFailsWhenOwned)
{
    SpinLockImpl lock;
    lock.lock();

    std::thread t([&]() { EXPECT_FALSE(lock.try_lock()); });

    t.join();
    lock.unlock();
}

// Test multiple threads using the lock with reentrant locking
TEST(SpinLockTest, ReentrantLockInThread)
{
    SpinLockImpl lock;
    bool success = false;

    std::thread t([&]() {
        lock.lock();
        EXPECT_TRUE(lock.try_lock());  // Reentrant locking should work
        lock.unlock();
        lock.unlock();
        success = true;
    });

    t.join();
    EXPECT_TRUE(success);
}

// Test that unlock reduces reference count and properly resets ownership
TEST(SpinLockTest, UnlockResetsOwnership)
{
    SpinLockImpl lock;
    lock.lock();
    lock.unlock();
    EXPECT_TRUE(lock.try_lock());  // Should be lockable again
    lock.unlock();
}
