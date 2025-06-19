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

#include <optional>

#include "bedrock/core/utility/pub_sub/return_policy_type.h"

namespace Bedrock::PubSub::Detail {

struct ReturnPolicy_NoReturn {  // NOLINT(*-identifier-naming)
    using ReturnType = void;
};
static_assert(sizeof(ReturnPolicy_NoReturn) == 1);

template <typename TReturnType>
struct ReturnPolicy_AggregateUntilCancelled {  // NOLINT(*-identifier-naming)
    using ReturnType =
        std::conditional_t<std::is_default_constructible_v<TReturnType>, TReturnType, std::optional<TReturnType>>;

protected:
    ReturnType return_value_;
};

template <typename ReturnType, ReturnPolicyType PolicyType>
struct ReturnPolicy;

template <typename ReturnType>
struct ReturnPolicy<ReturnType, ReturnPolicyType::Aggregate> : ReturnPolicy_AggregateUntilCancelled<ReturnType> {};

template <>
struct ReturnPolicy<void, ReturnPolicyType::Aggregate> : ReturnPolicy_NoReturn {};

}  // namespace Bedrock::PubSub::Detail
