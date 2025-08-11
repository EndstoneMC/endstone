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

#include <sstream>
#include <string>
#include <variant>

#include "bedrock/bedrock.h"
#include "bedrock/core/utility/binary_stream.h"

template <typename Tag, typename RawIdT = int, RawIdT RawInvalid = 0>
class TypedServerNetId {
public:
    [[nodiscard]] std::string toString() const
    {
        std::stringstream ss;
        ss << "[TypedServerNetId: " << raw_id << "]";
        return ss.str();
    }

    RawIdT raw_id = RawInvalid;
};

template <typename Tag, typename RawIdT = int, RawIdT RawInvalid = 0>
class TypedClientNetId {
public:
    virtual ~TypedClientNetId() = default;
    [[nodiscard]] std::string toString() const
    {
        std::stringstream ss;
        ss << "[TypedClientNetId: " << raw_id << "]";
        return ss.str();
    }

    RawIdT raw_id = RawInvalid;
};

struct ItemStackNetIdVariant {
    void serialize(BinaryStream &) const;
    [[nodiscard]] std::string toString() const;

private:
    std::variant<TypedServerNetId<struct ItemStackNetIdTag>, TypedClientNetId<struct ItemStackRequestIdTag>,
                 TypedClientNetId<struct ItemStackLegacyRequestIdTag>>
        variant_;
};
BEDROCK_STATIC_ASSERT_SIZE(ItemStackNetIdVariant, 24, 24);
