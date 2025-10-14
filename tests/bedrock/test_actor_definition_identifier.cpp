// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include <gtest/gtest.h>

#include "bedrock/world/actor/actor_definition_identifier.h"

static void expect_fields(const ActorDefinitionIdentifier &id, const std::string &ns, const std::string &ident,
                          const std::string &event, const std::string &full_name, bool is_vanilla)
{
    EXPECT_EQ(id.getNamespace(), ns);
    EXPECT_EQ(id.getIdentifier(), ident);
    EXPECT_EQ(id.getInitEvent(), event);
    EXPECT_EQ(id.getFullName(), full_name);
    EXPECT_EQ(id.isVanilla(), is_vanilla);
}

TEST(ActorDefinitionIdentifierTest, BareIdentifierDefaultsToMinecraft)
{
    ActorDefinitionIdentifier id{"zombie"};
    expect_fields(id, "minecraft", "zombie", "", "minecraft:zombie<>", true);
}

TEST(ActorDefinitionIdentifierTest, NamespacedIdentifier)
{
    ActorDefinitionIdentifier id{"minecraft:zombie"};
    expect_fields(id, "minecraft", "zombie", "", "minecraft:zombie<>", true);
}

TEST(ActorDefinitionIdentifierTest, BareIdentifierWithEvent)
{
    ActorDefinitionIdentifier id{"zombie<spawn>"};
    expect_fields(id, "minecraft", "zombie", "spawn", "minecraft:zombie<spawn>", true);
}

TEST(ActorDefinitionIdentifierTest, NamespacedIdentifierWithEvent)
{
    ActorDefinitionIdentifier id{"minecraft:zombie<spawn>"};
    expect_fields(id, "minecraft", "zombie", "spawn", "minecraft:zombie<spawn>", true);
}

TEST(ActorDefinitionIdentifierTest, NonVanillaNamespace)
{
    ActorDefinitionIdentifier id{"my_mod:thing<ev>"};
    expect_fields(id, "my_mod", "thing", "ev", "my_mod:thing<ev>", false);
}

TEST(ActorDefinitionIdentifierTest, EmptyEventIsAllowed)
{
    ActorDefinitionIdentifier id{"minecraft:blaze<>"};
    expect_fields(id, "minecraft", "blaze", "", "minecraft:blaze<>", true);
}

TEST(ActorDefinitionIdentifierTest, MalformedUnclosedEventIsIgnored)
{
    ActorDefinitionIdentifier id{"skeleton<spawn"};
    expect_fields(id, "minecraft", "skeleton", "", "minecraft:skeleton<>", true);
}

TEST(ActorDefinitionIdentifierTest, ReinitializeFromDefault)
{
    ActorDefinitionIdentifier id;
    id.initialize("mod:boss<enrage>");
    expect_fields(id, "mod", "boss", "enrage", "mod:boss<enrage>", false);
    id.initialize("creeper");
    expect_fields(id, "minecraft", "creeper", "", "minecraft:creeper<>", true);
}
