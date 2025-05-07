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
#include <functional>

#include "bedrock/core/container/intrusive_list.h"
#include "bedrock/core/utility/pub_sub/publisher_config.h"
#include "bedrock/core/utility/pub_sub/return_policy_type.h"

namespace Bedrock::PubSub::Detail {

class PublisherDisconnector {
public:
    virtual ~PublisherDisconnector() = 0;
};

class SubscriptionBodyBase : public Intrusive::list_base_hook<SubscriptionBodyBase> {
public:
    virtual ~SubscriptionBodyBase() = 0;

private:
    std::shared_ptr<SubscriptionBodyBase> strong_self_;  // +24
    PublisherDisconnector *disconnector_;                // +40
    ContextType context_;                                // +48
    GroupType group_;                                    // +56
};

template <typename Signature, ReturnPolicyType PolicyType>
class SubscriptionBody : public SubscriptionBodyBase {
    using FunctionType = std::function<Signature>;

protected:
    FunctionType function_;  // +64
};

}  // namespace Bedrock::PubSub::Detail
