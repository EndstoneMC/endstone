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

#include <cstdint>

#include "bedrock/bedrock.h"

namespace Bedrock {

template <typename Context>
class typeid_t {  // NOLINT(*-identifier-naming)
public:
    uint16_t id;
};

template <typename Context, typename Type>
typeid_t<Context> type_id();

}  // namespace Bedrock

#define DEFINE_BEDROCK_TYPE_ID(Context, Type, Value)  \
    namespace Bedrock {                               \
    template <>                                       \
    inline typeid_t<Context> type_id<Context, Type>() \
    {                                                 \
        return {Value};                               \
    }                                                 \
    }

class CommandRegistry;
DEFINE_BEDROCK_TYPE_ID(CommandRegistry, std::string, 1);
DEFINE_BEDROCK_TYPE_ID(CommandRegistry, bool, 2);
DEFINE_BEDROCK_TYPE_ID(CommandRegistry, int, 5);
DEFINE_BEDROCK_TYPE_ID(CommandRegistry, float, 6);

#undef DEFINE_BEDROCK_TYPE_ID
