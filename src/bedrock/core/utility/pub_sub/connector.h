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

#include <functional>
#include <optional>

#include "bedrock/core/utility/pub_sub/publisher_config.h"
#include "bedrock/core/utility/pub_sub/subscription.h"

namespace Bedrock::PubSub {

template <typename Signature>
class Connector {
public:
    using FunctionType = std::function<Signature>;

    template <typename Fn>
    Subscription connect(const Fn &fn, ConnectPosition at, ContextType &&context)
    {
        return _connectInternal(fn, at, std::move(context), std::nullopt);
    }

protected:
    Connector() = default;
    virtual ~Connector() = default;

private:
    virtual Subscription _connectInternal(FunctionType &&fn, ConnectPosition at, ContextType &&context,
                                          std::optional<int> group) = 0;
};

}  // namespace Bedrock::PubSub
