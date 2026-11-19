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

#include <filesystem>
#include <memory>

#include <gtest/gtest.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include "endstone/brigadier/builder.h"
#include "endstone/brigadier/tree.h"

namespace endstone::brigadier {
namespace {

TEST(CrossModuleNarrowingTest, NodesAndArgumentTypesBuiltInAnotherModuleStayUsable)
{
    const auto dir = std::filesystem::current_path() / "probes";
#ifdef _WIN32
    const auto path = dir / "endstone_brigadier_probe.dll";
    const auto library = LoadLibraryW(path.c_str());
    ASSERT_NE(library, nullptr);
    const auto symbol = reinterpret_cast<void *>(GetProcAddress(library, "brigadier_probe_tree"));
#else
    const auto path = dir / "endstone_brigadier_probe.so";
    void *library = dlopen(path.c_str(), RTLD_NOW);
    ASSERT_NE(library, nullptr) << dlerror();
    const auto symbol = dlsym(library, "brigadier_probe_tree");
#endif
    ASSERT_NE(symbol, nullptr);

    const std::unique_ptr<std::shared_ptr<RootCommandNode<int>>> holder{
        reinterpret_cast<std::shared_ptr<RootCommandNode<int>> *(*)()>(symbol)()};
    const auto &root = *holder;

    EXPECT_TRUE(root->is<RootCommandNode<int>>());
    EXPECT_TRUE(root->is<CommandNode<int>>());
    EXPECT_FALSE(root->is<LiteralCommandNode<int>>());

    const auto probe = root->getChild("probe");
    ASSERT_NE(probe, nullptr);
    EXPECT_TRUE(probe->is<LiteralCommandNode<int>>());
    EXPECT_FALSE(probe->is<ArgumentCommandNode<int>>());
    EXPECT_NE(probe->as<LiteralCommandNode<int>>(), nullptr);

    const auto word = probe->getChild("word");
    ASSERT_NE(word, nullptr);
    ASSERT_TRUE(word->is<ArgumentCommandNode<int>>());
    const auto &word_type = word->as<ArgumentCommandNode<int>>()->getType();
    EXPECT_TRUE(word_type->is<StringArgumentType>());
    EXPECT_TRUE(word_type->is<ArgumentType>());
    EXPECT_NE(word_type->as<StringArgumentType>(), nullptr);

    const auto custom = probe->getChild("custom");
    ASSERT_NE(custom, nullptr);
    ASSERT_TRUE(custom->is<ArgumentCommandNode<int>>());
    const auto &custom_type = custom->as<ArgumentCommandNode<int>>()->getType();
    EXPECT_TRUE(custom_type->is<ArgumentType>());
    EXPECT_FALSE(custom_type->is<StringArgumentType>());
}

}  // namespace
}  // namespace endstone::brigadier
