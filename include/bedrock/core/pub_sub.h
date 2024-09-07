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

#include <mutex>

namespace Bedrock::PubSub {

namespace ThreadModel {
struct MultiThreaded {
    using MutexType = std::recursive_mutex;
};
}  // namespace ThreadModel

namespace Detail {

class PublisherDisconnector {
public:
    virtual ~PublisherDisconnector() = 0;
};

class PublisherBase : public PublisherDisconnector {
    char subscriptions_[16];  // +8 Bedrock::PubSub::Detail::PublisherBase::ConnectionList
    int subscriber_count_;    // +24
};

template <typename T>
class FastDispatchPublisherBase : public PublisherBase, public T::MutexType {
    std::atomic<std::size_t> fast_dispatch_info_;
};

template <typename T>
class ThreadingPublisherBase : public FastDispatchPublisherBase<T> {};

template <typename T>
class DispatchingPublisherBase : public ThreadingPublisherBase<T> {};

}  // namespace Detail

template <typename Func>
class Connector {
public:
    virtual ~Connector() = default;
};

template <typename Func, typename T>
class Publisher : public Detail::DispatchingPublisherBase<T>, Connector<Func> {};

}  // namespace Bedrock::PubSub
