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

#include "bedrock/core/utility/pub_sub/connector.h"
#include "bedrock/core/utility/pub_sub/detail/dispatching_publisher_base.h"
#include "bedrock/core/utility/pub_sub/return_policy_type.h"

namespace Bedrock::PubSub {

template <typename Signature, typename ThreadingModel, ReturnPolicyType PolicyType = ReturnPolicyType::Aggregate>
class Publisher
    : public Detail::DispatchingPublisherBase<ThreadingModel, Detail::SubscriptionBody<Signature, PolicyType>>,
      public Connector<Signature> {
public:
    Publisher() = default;
    Publisher(Publisher &&) noexcept = default;
    ~Publisher() override = default;
    Publisher(const Publisher &) = delete;
    Publisher &operator=(const Publisher &) = delete;

private:
    using FunctionType = Connector<Signature>::FunctionType;
    Subscription _connectInternal(FunctionType &&fn, ConnectPosition at, ContextType &&context,
                                  std::optional<int> group) override;
};

template <typename Signature, typename ThreadingModel>
using PublisherPtr = std::unique_ptr<Publisher<Signature, ThreadingModel>>;

}  // namespace Bedrock::PubSub
