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

#include "bedrock/type_id.h"

#include "bedrock/command/command_registry.h"
#include "endstone_runtime/hook.h"

#define DEFINE_BEDROCK_TYPE_ID(Context, Type)                                  \
    template <>                                                                \
    BEDROCK_API Bedrock::typeid_t<Context> Bedrock::type_id<Context, Type>()   \
    {                                                                          \
        /* IMPORTANT: Use of lambda ensures unique template instantiations. */ \
        static typeid_t<Context> id = []() {                                   \
            Bedrock::typeid_t<Context> result;                                 \
            auto fp = &Bedrock::type_id<Context, Type>;                        \
            result = *ENDSTONE_HOOK_CALL_ORIGINAL_RVO(fp, &result);            \
            return result;                                                     \
        }();                                                                   \
                                                                               \
        return id;                                                             \
    }

DEFINE_BEDROCK_TYPE_ID(CommandRegistry, std::string);
DEFINE_BEDROCK_TYPE_ID(CommandRegistry, bool);
DEFINE_BEDROCK_TYPE_ID(CommandRegistry, int);
DEFINE_BEDROCK_TYPE_ID(CommandRegistry, float);
