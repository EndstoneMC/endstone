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

#include <memory>

#include "endstone/brigadier/builder.h"
#include "endstone/brigadier/tree.h"
#include "endstone/plugin/plugin.h"

using namespace endstone::brigadier;

class ProbeArgumentType : public ArgumentType {
public:
    [[nodiscard]] ArgumentValue parse(StringReader &reader) const override { return reader.readString(); }

    [[nodiscard]] std::string toString() const override { return "probe()"; }

    [[nodiscard]] bool isInstanceOf(const std::type_info &target) const override
    {
        return std::string_view(target.name()) == typeid(ProbeArgumentType).name() ||
               ArgumentType::isInstanceOf(target);
    }
};

extern "C" ENDSTONE_EXPORT std::shared_ptr<RootCommandNode<int>> *brigadier_probe_tree()
{
    auto root = std::make_shared<RootCommandNode<int>>();
    root->addChild(LiteralArgumentBuilder<int>::literal("probe")
                       .then(RequiredArgumentBuilder<int>::argument("word", StringArgumentType::word()))
                       .then(RequiredArgumentBuilder<int>::argument("custom", std::make_shared<ProbeArgumentType>()))
                       .build());
    return new std::shared_ptr<RootCommandNode<int>>(std::move(root));
}
