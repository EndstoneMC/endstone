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

#include <memory>
#include <system_error>

namespace Bedrock::Threading {

class AsyncBase : std::enable_shared_from_this<AsyncBase> {};

enum AsyncStatus : unsigned int {
    Started = 0x0,
    Completed = 0x1,
    Canceled = 0x2,
    Error = 0x3,
};

class IAsyncInfo {
public:
    virtual ~IAsyncInfo() = 0;
    [[nodiscard]] virtual AsyncStatus getStatus() const = 0;
    [[nodiscard]] virtual std::error_code getError() const = 0;
    virtual void cancel() = 0;
};

template <typename T>
class IAsyncGetResult : IAsyncInfo {  // size=0x8
public:
    virtual T getResult() const = 0;
};

template <typename T>
class IAsyncResult : public AsyncBase, public IAsyncGetResult<T> {
public:
    using Handle = std::shared_ptr<IAsyncResult>;
    using CompletionHandler = int;

    virtual void addOnComplete(CompletionHandler) = 0;
};

template <typename TResult>
class Async : public std::shared_ptr<IAsyncResult<TResult>> {};

template <typename TResult>
class SharedAsync : public std::shared_ptr<IAsyncResult<TResult>> {};

}  // namespace Bedrock::Threading
