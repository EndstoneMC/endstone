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

#include <gtest/gtest.h>

#include "endstone/command/arguments.h"
#include "endstone/core/command/tree/argument_types.h"

using endstone::ArgumentKind;
using endstone::Arguments;
using endstone::ArgumentType;
using endstone::ArgumentValue;
using endstone::CommandSender;
using endstone::NotNull;
using endstone::Nullable;
using endstone::core::effectiveArgumentKind;
using endstone::core::isSelectorArgument;

namespace {

class WrappingArgument : public ArgumentType {
public:
    explicit WrappingArgument(NotNull<ArgumentType> native) : native_(std::move(native)) {}

    [[nodiscard]] Nullable<ArgumentType> getNativeType() const override { return native_.get(); }

private:
    NotNull<ArgumentType> native_;
};

}  // namespace

TEST(ArgumentTypesTest, BuiltinKindsAreReportedDirectly)
{
    EXPECT_EQ(effectiveArgumentKind(*Arguments::integer()), ArgumentKind::Integer);
    EXPECT_EQ(effectiveArgumentKind(*Arguments::string()), ArgumentKind::String);
    EXPECT_EQ(effectiveArgumentKind(*Arguments::players()), ArgumentKind::Players);
}

TEST(ArgumentTypesTest, OnlySelectorKindsAreSelectors)
{
    EXPECT_TRUE(isSelectorArgument(ArgumentKind::Player));
    EXPECT_TRUE(isSelectorArgument(ArgumentKind::Players));
    EXPECT_TRUE(isSelectorArgument(ArgumentKind::Entity));
    EXPECT_TRUE(isSelectorArgument(ArgumentKind::Entities));

    EXPECT_FALSE(isSelectorArgument(ArgumentKind::String));
    EXPECT_FALSE(isSelectorArgument(ArgumentKind::Integer));
    EXPECT_FALSE(isSelectorArgument(ArgumentKind::EntityType));
    EXPECT_FALSE(isSelectorArgument(ArgumentKind::Custom));
}

TEST(ArgumentTypesTest, CustomTypeReportsTheKindItIsPresentedAs)
{
    // A custom type borrows a built-in type's grammar, so it has to be stored and parsed as that
    // built-in - a custom type over player() still needs selector storage.
    const auto over_player = std::make_shared<WrappingArgument>(Arguments::player());
    EXPECT_EQ(effectiveArgumentKind(*over_player), ArgumentKind::Player);
    EXPECT_TRUE(isSelectorArgument(effectiveArgumentKind(*over_player)));

    const auto over_string = std::make_shared<WrappingArgument>(Arguments::string());
    EXPECT_EQ(effectiveArgumentKind(*over_string), ArgumentKind::String);
    EXPECT_FALSE(isSelectorArgument(effectiveArgumentKind(*over_string)));
}

TEST(ArgumentTypesTest, NestedCustomTypesFollowTheWholeChain)
{
    const auto inner = std::make_shared<WrappingArgument>(Arguments::entities());
    const auto outer = std::make_shared<WrappingArgument>(inner);
    EXPECT_EQ(effectiveArgumentKind(*outer), ArgumentKind::Entities);
}

TEST(ArgumentTypesTest, CustomTypeWithoutANativeTypeStaysCustom)
{
    class Bare : public ArgumentType {};
    const Bare bare;
    EXPECT_EQ(effectiveArgumentKind(bare), ArgumentKind::Custom);
}
