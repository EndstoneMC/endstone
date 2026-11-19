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

#include <algorithm>
#include <map>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "endstone/command/brigadier/argument/argument_types.h"
#include "endstone/command/brigadier/commands.h"
#include "endstone/command/command_sender.h"
#include "endstone/core/command/compiler/command_compiler.h"
#include "endstone/level/location.h"
#include "endstone/util/vector.h"

using endstone::ArgumentTypes;
using endstone::Commands;
using endstone::CommandSource;
using endstone::brigadier::BoolArgumentType;
using endstone::brigadier::CommandContext;
using endstone::brigadier::DoubleArgumentType;
using endstone::brigadier::FloatArgumentType;
using endstone::brigadier::IntegerArgumentType;
using endstone::brigadier::StringArgumentType;
using endstone::core::CommandCompiler;
using endstone::core::CommandOverload;
using endstone::core::CommandParameter;
using endstone::core::CommandSlot;
using Kind = CommandParameter::Kind;
using Command = endstone::brigadier::Command<CommandSource>;

namespace {

class TestSource : public endstone::CommandSourceStack {
public:
    [[nodiscard]] endstone::NotNull<endstone::CommandSender> getSender() const override
    {
        throw std::logic_error("no sender in tests");
    }
    [[nodiscard]] endstone::Nullable<endstone::Actor> getExecutor() const override { return nullptr; }
    [[nodiscard]] endstone::Location getLocation() const override { return {nullptr, 0, 0, 0}; }
};

class TestArgumentType : public endstone::brigadier::ArgumentType {
public:
    [[nodiscard]] endstone::brigadier::ArgumentValue parse(endstone::brigadier::StringReader &reader) const override
    {
        return reader.readInt();
    }
    [[nodiscard]] std::string toString() const override { return "test()"; }
};

const Command one = [](const CommandContext<CommandSource> &) {
    return 1;
};
const Command two = [](const CommandContext<CommandSource> &) {
    return 2;
};

std::vector<std::vector<std::pair<Kind, bool>>> make_shapes(const CommandCompiler &compiler)
{
    std::vector<std::vector<std::pair<Kind, bool>>> shapes;
    for (const auto &overload : compiler.getCompiled().overloads) {
        auto &shape = shapes.emplace_back();
        for (const auto &parameter : overload) {
            shape.emplace_back(parameter.kind, parameter.optional);
        }
    }
    return shapes;
}

std::vector<CommandSlot> make_slots(const std::vector<std::optional<std::string>> &values)
{
    std::vector<CommandSlot> slots;
    for (const auto &value : values) {
        slots.push_back({.string_value = value.value_or(""), .is_set = value.has_value()});
    }
    return slots;
}

}  // namespace

TEST(CommandTreeTest, RootOnlyYieldsOneEmptyOverload)
{
    CommandCompiler compiler("ping");
    EXPECT_EQ(compiler.addTree(*Commands::literal("ping").executes(one).build()), std::nullopt);
    EXPECT_EQ(make_shapes(compiler), (std::vector<std::vector<std::pair<Kind, bool>>>{{}}));
}

TEST(CommandTreeTest, MakesOneEnumPerKeywordSharedByEveryOverload)
{
    const auto root =
        Commands::literal("warp")
            .then(Commands::literal("set").then(Commands::argument("name", StringArgumentType::word()).executes(one)))
            .then(Commands::literal("list").executes(one))
            .then(Commands::literal("go").then(Commands::literal("set").executes(one)))
            .build();
    CommandCompiler compiler("warp");
    ASSERT_EQ(compiler.addTree(*root), std::nullopt);
    const auto &compiled = compiler.getCompiled();
    EXPECT_EQ(compiled.overloads.size(), 3);
    std::vector<std::string> names;
    for (const auto &compiled_enum : compiled.enums) {
        names.push_back(compiled_enum.name);
        EXPECT_EQ(compiled_enum.values.size(), 1);
    }
    std::ranges::sort(names);
    EXPECT_EQ(names, (std::vector<std::string>{"WarpGo", "WarpList", "WarpSet"}));
}

TEST(CommandTreeTest, LowercasesKeywords)
{
    CommandCompiler compiler("t");
    ASSERT_EQ(compiler.addTree(*Commands::literal("t").then(Commands::literal("GoHome").executes(one)).build()),
              std::nullopt);
    const auto &parameter = compiler.getCompiled().overloads.front().front();
    EXPECT_EQ(parameter.kind, Kind::Keyword);
    EXPECT_EQ(parameter.name, "gohome");
    EXPECT_EQ(parameter.enum_name, "TGoHome");
    EXPECT_EQ(parameter.values, std::vector<std::string>{"gohome"});
}

TEST(CommandTreeTest, CollapsesAnArgumentUnderAnExecutingParent)
{
    CommandCompiler compiler("t");
    ASSERT_EQ(
        compiler.addTree(*Commands::literal("t")
                              .executes(one)
                              .then(Commands::argument("a", StringArgumentType::word())
                                        .executes(one)
                                        .then(Commands::argument("b", StringArgumentType::string()).executes(one)))
                              .build()),
        std::nullopt);
    EXPECT_EQ(make_shapes(compiler),
              (std::vector<std::vector<std::pair<Kind, bool>>>{{{Kind::String, true}, {Kind::String, true}}}));
}

TEST(CommandTreeTest, KeepsAKeywordUnderAnExecutingParentRequired)
{
    CommandCompiler compiler("t");
    ASSERT_EQ(
        compiler.addTree(*Commands::literal("t").executes(one).then(Commands::literal("go").executes(one)).build()),
        std::nullopt);
    EXPECT_EQ(make_shapes(compiler), (std::vector<std::vector<std::pair<Kind, bool>>>{{}, {{Kind::Keyword, false}}}));
}

TEST(CommandTreeTest, KeepsAGreedyStringRequiredAndDropsThePrefixBeforeIt)
{
    CommandCompiler compiler("t");
    ASSERT_EQ(compiler.addTree(*Commands::literal("t")
                                    .executes(one)
                                    .then(Commands::argument("args", StringArgumentType::greedyString()).executes(one))
                                    .build()),
              std::nullopt);
    EXPECT_EQ(make_shapes(compiler), (std::vector<std::vector<std::pair<Kind, bool>>>{{{Kind::Message, false}}}));
}

TEST(CommandTreeTest, MapsASoftEnumArgument)
{
    CommandCompiler compiler("t");
    ASSERT_EQ(compiler.addTree(*Commands::literal("t")
                                    .then(Commands::argument("w", ArgumentTypes::softEnum("WarpNames")).executes(one))
                                    .build()),
              std::nullopt);
    const auto &parameter = compiler.getCompiled().overloads.front().front();
    EXPECT_EQ(parameter.kind, Kind::SoftEnum);
    EXPECT_EQ(parameter.enum_name, "WarpNames");
}

TEST(CommandTreeTest, RefusesARedirect)
{
    const auto target = Commands::literal("t").executes(one).build();
    CommandCompiler compiler("t");
    const auto error =
        compiler.addTree(*Commands::literal("t").then(Commands::literal("again").redirect(target)).build());
    EXPECT_EQ(error, "'/t again' redirects to another node, which a Bedrock command cannot do.");
}

TEST(CommandTreeTest, RefusesAFork)
{
    const auto target = Commands::literal("t").executes(one).build();
    CommandCompiler compiler("t");
    const auto error = compiler.addTree(
        *Commands::literal("t")
             .then(Commands::literal("each").fork(target,
                                                  [](const CommandContext<CommandSource> &context) {
                                                      return std::vector<CommandSource>{context.getSource()};
                                                  }))
             .build());
    EXPECT_EQ(error, "'/t each' redirects to another node, which a Bedrock command cannot do.");
}

TEST(CommandTreeTest, RefusesAGreedyStringThatIsNotLast)
{
    CommandCompiler compiler("t");
    const auto error = compiler.addTree(
        *Commands::literal("t")
             .then(
                 Commands::argument("m", StringArgumentType::greedyString()).then(Commands::literal("x").executes(one)))
             .build());
    EXPECT_EQ(error, "'/t <m: message>' is a greedy string and is not last.");
}

TEST(CommandTreeTest, KeepsALiteralBesideAnArgument)
{
    for (const auto &argument :
         {Commands::argument("name", StringArgumentType::word()), Commands::argument("p", ArgumentTypes::player()),
          Commands::argument("d", ArgumentTypes::dimension()), Commands::argument("b", BoolArgumentType::bool_())}) {
        CommandCompiler compiler("t");
        auto node = argument;
        const auto error = compiler.addTree(
            *Commands::literal("t").then(Commands::literal("all").executes(one)).then(node.executes(one)).build());
        EXPECT_EQ(error, std::nullopt) << error.value_or("");
        EXPECT_EQ(compiler.getCompiled().overloads.size(), 2);
    }
}

TEST(CommandTreeTest, BindsOnlyThePathOfTheSourceItIsGiven)
{
    int checked = 0;
    const auto root = Commands::literal("warp")
                          .then(Commands::argument("name", StringArgumentType::word()).executes(two))
                          .then(Commands::literal("list")
                                    .requires_([&checked](const CommandSource &) {
                                        ++checked;
                                        return true;
                                    })
                                    .executes(one))
                          .build();
    CommandCompiler compiler("warp");
    ASSERT_EQ(compiler.addTree(*root), std::nullopt);
    const CommandSource source = std::make_shared<TestSource>();
    const auto &argument = *std::ranges::find_if(compiler.getCompiled().overloads, [](const CommandOverload &overload) {
        return overload.front().kind == Kind::String;
    });
    const auto name = CommandCompiler::bindArguments(root, argument, make_slots({"list"}), source, "warp list");
    ASSERT_TRUE(name.has_value());
    EXPECT_EQ(name->getCommand()(*name), 2);
    EXPECT_EQ(name->getArgument<std::string>("name"), "list");
    EXPECT_EQ(checked, 0);
    const auto &literal = *std::ranges::find_if(compiler.getCompiled().overloads, [](const CommandOverload &overload) {
        return overload.front().kind == Kind::Keyword;
    });
    const auto list = CommandCompiler::bindArguments(root, literal, make_slots({"list"}), source, "warp list");
    ASSERT_TRUE(list.has_value());
    EXPECT_EQ(list->getCommand()(*list), 1);
    EXPECT_FALSE(list->getArguments().contains("name"));
    EXPECT_EQ(checked, 1);
}

TEST(CommandTreeTest, BindsTheFirstOfTwoSiblingsOfOneKindAsBrigadierDoes)
{
    const auto root = Commands::literal("t")
                          .then(Commands::argument("x", IntegerArgumentType::integer()).executes(one))
                          .then(Commands::argument("y", IntegerArgumentType::integer()).executes(two))
                          .build();
    CommandCompiler compiler("t");
    ASSERT_EQ(compiler.addTree(*root), std::nullopt);
    const auto &registered = compiler.getCompiled().registered;
    ASSERT_EQ(registered.size(), 1);
    const auto slots = make_slots({"5"});
    const auto chosen = CommandCompiler::chooseSource(registered.front(), slots);
    ASSERT_TRUE(chosen.has_value());
    const CommandSource source = std::make_shared<TestSource>();
    const auto context =
        CommandCompiler::bindArguments(root, registered.front().sources[*chosen], slots, source, "t 5");
    ASSERT_TRUE(context.has_value());
    EXPECT_EQ(context->getCommand()(*context), 1);
    EXPECT_EQ(context->getArgument<int>("x"), 5);
    EXPECT_FALSE(context->getArguments().contains("y"));
}

TEST(CommandTreeTest, CapturesALiteralBesideAWordAndPicksTheLiteralFirst)
{
    const auto root = Commands::literal("warp")
                          .then(Commands::literal("list").executes(one))
                          .then(Commands::argument("name", StringArgumentType::word()).executes(two))
                          .build();
    CommandCompiler compiler("warp");
    ASSERT_EQ(compiler.addTree(*root), std::nullopt);
    const auto &registered = compiler.getCompiled().registered;
    const auto &capture = *std::ranges::find_if(
        registered, [](const auto &entry) { return entry.parameters.front().kind == Kind::String; });
    ASSERT_EQ(capture.sources.size(), 2);
    const CommandSource source = std::make_shared<TestSource>();
    for (const auto &[word, quoted, result] :
         {std::tuple{"list", false, 1}, std::tuple{"spawn", false, 2}, std::tuple{"list", true, 2}}) {
        const std::vector<CommandSlot> slots{{.string_value = word, .is_set = true, .quoted = quoted}};
        const auto chosen = CommandCompiler::chooseSource(capture, slots);
        ASSERT_TRUE(chosen.has_value()) << word;
        const auto context = CommandCompiler::bindArguments(root, capture.sources[*chosen], slots, source, "warp");
        ASSERT_TRUE(context.has_value()) << word;
        EXPECT_EQ(context->getCommand()(*context), result) << word << quoted;
    }
}

TEST(CommandTreeTest, CapturesALiteralWithMoreParametersBesideAWord)
{
    CommandCompiler compiler("t");
    ASSERT_EQ(compiler.addTree(*Commands::literal("t")
                                    .then(Commands::literal("list").then(
                                        Commands::argument("page", IntegerArgumentType::integer()).executes(one)))
                                    .then(Commands::argument("name", StringArgumentType::word()).executes(two))
                                    .build()),
              std::nullopt);
    const auto &registered = compiler.getCompiled().registered;
    ASSERT_EQ(registered.size(), 3);
    const auto &capture = *std::ranges::find_if(registered, [](const auto &entry) {
        return entry.parameters.size() == 2 && entry.parameters.front().kind == Kind::String;
    });
    EXPECT_EQ(capture.parameters[1].kind, Kind::Int);
    const auto list = CommandCompiler::chooseSource(
        capture, {{.string_value = "list", .is_set = true}, {.string_value = "3", .is_set = true}});
    ASSERT_TRUE(list.has_value());
    EXPECT_EQ(capture.sources[*list].front().kind, Kind::Keyword);
    EXPECT_FALSE(CommandCompiler::chooseSource(
                     capture, {{.string_value = "foo", .is_set = true}, {.string_value = "3", .is_set = true}})
                     .has_value());
    const auto &name = *std::ranges::find_if(registered, [](const auto &entry) {
        return entry.parameters.size() == 1 && entry.parameters.front().kind == Kind::String;
    });
    const auto incomplete = CommandCompiler::chooseSource(name, {{.string_value = "list", .is_set = true}});
    ASSERT_FALSE(incomplete.has_value());
    EXPECT_EQ(incomplete.error(), 1);
    EXPECT_TRUE(CommandCompiler::chooseSource(name, {{.string_value = "foo", .is_set = true}}).has_value());
}

TEST(CommandTreeTest, RefusesALiteralWithMoreParametersBesideAGreedyString)
{
    CommandCompiler compiler("t");
    const auto error =
        compiler.addTree(*Commands::literal("t")
                              .then(Commands::literal("list").then(
                                  Commands::argument("page", IntegerArgumentType::integer()).executes(one)))
                              .then(Commands::argument("rest", StringArgumentType::greedyString()).executes(two))
                              .build());
    ASSERT_TRUE(error.has_value());
    EXPECT_EQ(*error, "'/t' has list beside the greedy <rest: message>, which reads the rest of the line.");
}

TEST(CommandTreeTest, ResolvesArgumentSiblingsInChildOrderAsBrigadierDoes)
{
    CommandCompiler compiler("t");
    ASSERT_EQ(compiler.addTree(*Commands::literal("t")
                                    .then(Commands::argument("flag", BoolArgumentType::bool_()).executes(one))
                                    .then(Commands::argument("d", ArgumentTypes::dimension()).executes(one))
                                    .then(Commands::argument("name", StringArgumentType::word()).executes(two))
                                    .build()),
              std::nullopt);
    const auto &capture = *std::ranges::find_if(compiler.getCompiled().registered, [](const auto &entry) {
        return entry.parameters.front().kind == Kind::String;
    });
    ASSERT_EQ(capture.sources.size(), 3);
    for (const auto &[word, kind] : {std::pair{"true", Kind::Enum}, std::pair{"overworld", Kind::Dimension}}) {
        const auto chosen = CommandCompiler::chooseSource(capture, {{.string_value = word, .is_set = true}});
        ASSERT_TRUE(chosen.has_value()) << word;
        EXPECT_EQ(capture.sources[*chosen].front().kind, kind) << word;
    }
}

TEST(CommandTreeTest, RefusesATypeWithNoBedrockCounterpart)
{
    CommandCompiler compiler("t");
    const auto error =
        compiler.addTree(*Commands::literal("t")
                              .then(Commands::argument("n", std::make_shared<TestArgumentType>()).executes(one))
                              .build());
    EXPECT_EQ(error, "'/t <n: test()>' has a type with no Bedrock counterpart.");
}

TEST(CommandTreeTest, RefusesATreeThatNeverExecutes)
{
    CommandCompiler compiler("t");
    EXPECT_EQ(compiler.addTree(*Commands::literal("t").then(Commands::literal("a")).build()),
              "'/t' has no node that executes.");
}

TEST(CommandTreeTest, RefusesChildrenEqualOnceLowercased)
{
    CommandCompiler compiler("t");
    EXPECT_EQ(compiler.addTree(*Commands::literal("t")
                                    .then(Commands::literal("Go").executes(one))
                                    .then(Commands::literal("go").executes(one))
                                    .build()),
              "'/t' has two children named 'go' once lowercased.");
}

TEST(CommandTreeTest, RefusesTheOverloadPastTheBudget)
{
    auto builder = Commands::literal("t");
    for (int i = 0; i < 251; ++i) {
        builder.then(Commands::literal("k" + std::to_string(i)).executes(one));
    }
    CommandCompiler compiler("t");
    EXPECT_EQ(compiler.addTree(*builder.build()),
              "'/t' would have 251 overloads, more than the 250 a command can have.");
}

TEST(CommandTreeTest, BindsSlotsByIndexAlongThePath)
{
    const auto root = Commands::literal("t")
                          .then(Commands::literal("a").then(
                              Commands::argument("n", StringArgumentType::word())
                                  .then(Commands::argument("m", StringArgumentType::greedyString()).executes(two))))
                          .build();
    CommandCompiler compiler("t");
    ASSERT_EQ(compiler.addTree(*root), std::nullopt);
    const CommandSource source = std::make_shared<TestSource>();
    const auto context = CommandCompiler::bindArguments(root, compiler.getCompiled().overloads.front(),
                                                        make_slots({"a", "foo", "bar baz"}), source, "t a foo bar baz");
    ASSERT_TRUE(context.has_value());
    EXPECT_EQ(context->getArgument<std::string>("n"), "foo");
    EXPECT_EQ(context->getArgument<std::string>("m"), "bar baz");
    EXPECT_FALSE(context->getArguments().contains("a"));
    EXPECT_EQ(context->getNodes().size(), 4);
    EXPECT_EQ(context->getInput(), "t a foo bar baz");
    EXPECT_EQ(context->getCommand()(*context), 2);
}

TEST(CommandTreeTest, MapsThePrimitivesOntoBedrockTypes)
{
    const auto root =
        Commands::literal("t")
            .then(Commands::argument("i", IntegerArgumentType::integer(1, 64))
                      .then(Commands::argument("f", FloatArgumentType::floatArg())
                                .then(Commands::argument("d", DoubleArgumentType::doubleArg())
                                          .then(Commands::argument("b", BoolArgumentType::bool_()).executes(one)))))
            .build();
    CommandCompiler compiler("t");
    ASSERT_EQ(compiler.addTree(*root), std::nullopt);
    const auto &overload = compiler.getCompiled().overloads.front();
    ASSERT_EQ(overload.size(), 4);
    EXPECT_EQ(overload[0].kind, Kind::Int);
    EXPECT_EQ(overload[1].kind, Kind::Float);
    EXPECT_EQ(overload[2].kind, Kind::Float);
    EXPECT_EQ(overload[3].kind, Kind::Enum);
    EXPECT_EQ(overload[3].enum_name, "Boolean");
    EXPECT_TRUE(overload[3].foreign);
}

TEST(CommandTreeTest, ParsesEachPrimitiveFromItsSlot)
{
    const auto root =
        Commands::literal("t")
            .then(Commands::argument("i", IntegerArgumentType::integer())
                      .then(Commands::argument("f", FloatArgumentType::floatArg())
                                .then(Commands::argument("d", DoubleArgumentType::doubleArg())
                                          .then(Commands::argument("b", BoolArgumentType::bool_()).executes(one)))))
            .build();
    CommandCompiler compiler("t");
    ASSERT_EQ(compiler.addTree(*root), std::nullopt);
    const CommandSource source = std::make_shared<TestSource>();
    const auto context =
        CommandCompiler::bindArguments(root, compiler.getCompiled().overloads.front(),
                                       make_slots({"42", "1.5", "0.1", "true"}), source, "t 42 1.5 0.1 true");
    ASSERT_TRUE(context.has_value());
    EXPECT_EQ(context->getArgument<int>("i"), 42);
    EXPECT_EQ(context->getArgument<float>("f"), 1.5F);
    EXPECT_EQ(context->getArgument<double>("d"), 0.1);
    EXPECT_EQ(context->getArgument<bool>("b"), true);
}

TEST(CommandTreeTest, ThrowsTheRangeErrorOfABoundedArgument)
{
    const auto root = Commands::literal("t")
                          .then(Commands::argument("n", IntegerArgumentType::integer(1, 64)).executes(one))
                          .then(Commands::literal("f").then(
                              Commands::argument("x", DoubleArgumentType::doubleArg(0, 1)).executes(one)))
                          .build();
    CommandCompiler compiler("t");
    ASSERT_EQ(compiler.addTree(*root), std::nullopt);
    const CommandSource source = std::make_shared<TestSource>();
    const auto &exceptions = endstone::brigadier::CommandSyntaxException::BUILT_IN_EXCEPTIONS;
    for (const auto &overload : compiler.getCompiled().overloads) {
        const bool integer = overload.front().kind == Kind::Int;
        try {
            static_cast<void>(CommandCompiler::bindArguments(
                root, overload, integer ? make_slots({"65"}) : make_slots({"f", "-0.5"}), source, "t"));
            ADD_FAILURE() << "expected a CommandSyntaxException";
        }
        catch (const endstone::brigadier::CommandSyntaxException &e) {
            if (integer) {
                EXPECT_EQ(&e.getType(), &exceptions.integerTooHigh());
                EXPECT_STREQ(e.what(), "Integer must not be more than 64, found 65");
            }
            else {
                EXPECT_EQ(&e.getType(), &exceptions.doubleTooLow());
                EXPECT_STREQ(e.what(), "Double must not be less than 0.0, found -0.5");
            }
        }
    }
}

TEST(CommandTreeTest, EndsAtTheLastSetSlot)
{
    const auto root = Commands::literal("t")
                          .executes(one)
                          .then(Commands::argument("n", StringArgumentType::word()).executes(two))
                          .build();
    CommandCompiler compiler("t");
    ASSERT_EQ(compiler.addTree(*root), std::nullopt);
    const auto &overload = compiler.getCompiled().overloads.front();
    const CommandSource source = std::make_shared<TestSource>();
    const auto unset = CommandCompiler::bindArguments(root, overload, make_slots({std::nullopt}), source, "t");
    ASSERT_TRUE(unset.has_value());
    EXPECT_EQ(unset->getCommand()(*unset), 1);
    const auto set = CommandCompiler::bindArguments(root, overload, make_slots({"x"}), source, "t x");
    ASSERT_TRUE(set.has_value());
    EXPECT_EQ(set->getCommand()(*set), 2);
}

TEST(CommandTreeTest, EndsAnEmptyMessageAtAnExecutingParent)
{
    const auto root = Commands::literal("t")
                          .executes(one)
                          .then(Commands::argument("args", StringArgumentType::greedyString()).executes(two))
                          .build();
    CommandCompiler compiler("t");
    ASSERT_EQ(compiler.addTree(*root), std::nullopt);
    const CommandSource source = std::make_shared<TestSource>();
    const auto context =
        CommandCompiler::bindArguments(root, compiler.getCompiled().overloads.front(), make_slots({""}), source, "t");
    ASSERT_TRUE(context.has_value());
    EXPECT_EQ(context->getCommand()(*context), 1);
}

TEST(CommandTreeTest, DeniesAPathWhoseRequirementFails)
{
    const auto root =
        Commands::literal("t")
            .then(Commands::literal("open").executes(one))
            .then(Commands::literal("admin").requires_([](const CommandSource &) { return false; }).executes(one))
            .build();
    CommandCompiler compiler("t");
    ASSERT_EQ(compiler.addTree(*root), std::nullopt);
    const CommandSource source = std::make_shared<TestSource>();
    for (const auto &overload : compiler.getCompiled().overloads) {
        const auto context =
            CommandCompiler::bindArguments(root, overload, make_slots({overload.front().name}), source, "t");
        EXPECT_EQ(context.has_value(), overload.front().name == "open") << overload.front().name;
    }
    EXPECT_EQ(CommandCompiler::findUsable(*root, {"open"}, source), 1);
    EXPECT_EQ(CommandCompiler::findUsable(*root, {"admin"}, source), 0);
    EXPECT_EQ(CommandCompiler::findUsable(*root, {"missing"}, source), 0);
}

TEST(CommandTreeTest, HidesTheCommandWhenTheRootRequirementFails)
{
    const auto root =
        Commands::literal("t").requires_([](const CommandSource &) { return false; }).executes(one).build();
    const CommandSource source = std::make_shared<TestSource>();
    EXPECT_EQ(CommandCompiler::findUsable(*root, {}, source), std::nullopt);
    EXPECT_EQ(CommandCompiler::bindArguments(root, {}, {}, source, "t"), std::nullopt);
}

TEST(CommandTreeTest, MapsSelectorPositionDimensionAndResourceArguments)
{
    const auto root =
        Commands::literal("t")
            .then(Commands::literal("p").then(Commands::argument("v", ArgumentTypes::player()).executes(one)))
            .then(Commands::literal("ps").then(Commands::argument("v", ArgumentTypes::players()).executes(one)))
            .then(Commands::literal("a").then(Commands::argument("v", ArgumentTypes::actor()).executes(one)))
            .then(Commands::literal("as").then(Commands::argument("v", ArgumentTypes::actors()).executes(one)))
            .then(Commands::literal("b").then(Commands::argument("v", ArgumentTypes::blockPosition()).executes(one)))
            .then(Commands::literal("f").then(Commands::argument("v", ArgumentTypes::finePosition()).executes(one)))
            .then(Commands::literal("d").then(Commands::argument("v", ArgumentTypes::dimension()).executes(one)))
            .then(Commands::literal("bt").then(
                Commands::argument("v", ArgumentTypes::resource<endstone::BlockType>()).executes(one)))
            .then(Commands::literal("it").then(
                Commands::argument("v", ArgumentTypes::resource<endstone::ItemType>()).executes(one)))
            .then(Commands::literal("at").then(
                Commands::argument("v", ArgumentTypes::resource<endstone::ActorType>()).executes(one)))
            .build();
    CommandCompiler compiler("t");
    ASSERT_EQ(compiler.addTree(*root), std::nullopt);
    std::map<std::string, CommandParameter> parameters;
    for (const auto &overload : compiler.getCompiled().overloads) {
        ASSERT_EQ(overload.size(), 2);
        parameters.emplace(overload[0].name, overload[1]);
    }
    ASSERT_EQ(parameters.size(), 10);
    EXPECT_EQ(parameters.at("p").kind, Kind::Player);
    EXPECT_TRUE(parameters.at("p").single);
    EXPECT_EQ(parameters.at("ps").kind, Kind::Player);
    EXPECT_FALSE(parameters.at("ps").single);
    EXPECT_EQ(parameters.at("a").kind, Kind::Actor);
    EXPECT_TRUE(parameters.at("a").single);
    EXPECT_EQ(parameters.at("as").kind, Kind::Actor);
    EXPECT_FALSE(parameters.at("as").single);
    EXPECT_EQ(parameters.at("b").kind, Kind::BlockPos);
    EXPECT_EQ(parameters.at("f").kind, Kind::Pos);
    const std::vector<std::tuple<std::string, Kind, std::string>> enums{{"d", Kind::Dimension, "Dimension"},
                                                                        {"bt", Kind::BlockType, "Block"},
                                                                        {"it", Kind::ItemType, "Item"},
                                                                        {"at", Kind::ActorType, "EntityType"}};
    for (const auto &[name, kind, enum_name] : enums) {
        const auto &parameter = parameters.at(name);
        EXPECT_EQ(parameter.kind, kind) << name;
        EXPECT_EQ(parameter.enum_name, enum_name) << name;
        EXPECT_TRUE(parameter.foreign) << name;
        EXPECT_TRUE(parameter.isEnum()) << name;
        EXPECT_TRUE(parameter.values.empty()) << name;
    }
    std::vector<std::string> compiled_enums;
    for (const auto &compiled_enum : compiler.getCompiled().enums) {
        compiled_enums.push_back(compiled_enum.name);
    }
    EXPECT_EQ(std::ranges::count(compiled_enums, "Dimension"), 0);
    EXPECT_EQ(std::ranges::count(compiled_enums, "Block"), 0);
}

TEST(CommandTreeTest, KeepsADimensionBesideAString)
{
    CommandCompiler compiler("t");
    const auto error = compiler.addTree(*Commands::literal("t")
                                             .then(Commands::argument("d", ArgumentTypes::dimension()).executes(one))
                                             .then(Commands::argument("name", StringArgumentType::word()).executes(one))
                                             .build());
    EXPECT_EQ(error, std::nullopt) << error.value_or("");
}

TEST(CommandTreeTest, RefusesTwoSelectorsAtOnePosition)
{
    CommandCompiler compiler("t");
    const auto error = compiler.addTree(*Commands::literal("t")
                                             .then(Commands::argument("p", ArgumentTypes::player()).executes(one))
                                             .then(Commands::argument("a", ArgumentTypes::actors()).executes(one))
                                             .build());
    ASSERT_TRUE(error.has_value());
    EXPECT_NE(error->find("two Id-class parameters at one position"), std::string::npos) << *error;
}

TEST(CommandTreeTest, BindsTheResolverInTheSlot)
{
    const auto root =
        Commands::literal("t").then(Commands::argument("who", ArgumentTypes::players()).executes(one)).build();
    const CommandOverload overload{{.kind = Kind::Player, .name = "who"}};
    std::vector<CommandSlot> slots(1);
    const endstone::CommandSourceStack *seen = nullptr;
    slots[0].is_set = true;
    slots[0].value = endstone::PlayerSelectorArgumentResolver([&seen](const endstone::CommandSourceStack &source) {
        seen = &source;
        return std::vector<endstone::NotNull<endstone::Player>>{};
    });
    const CommandSource source = std::make_shared<TestSource>();
    const auto context = CommandCompiler::bindArguments(root, overload, slots, source, "t @a");
    ASSERT_TRUE(context.has_value());
    const auto &resolver = context->getArgument<endstone::PlayerSelectorArgumentResolver>("who");
    EXPECT_TRUE(resolver.resolve(*context->getSource()).empty());
    EXPECT_EQ(seen, context->getSource().get().get());
}

TEST(CommandTreeTest, ResolvesPositionsThroughTheResolverInTheSlot)
{
    const auto root = Commands::literal("t")
                          .then(Commands::argument("b", ArgumentTypes::blockPosition())
                                    .then(Commands::argument("f", ArgumentTypes::finePosition()).executes(one)))
                          .build();
    const CommandOverload overload{{.kind = Kind::BlockPos, .name = "b"}, {.kind = Kind::Pos, .name = "f"}};
    std::vector<CommandSlot> slots(2);
    slots[0].is_set = true;
    slots[0].value =
        endstone::BlockPositionResolver([](const endstone::CommandSourceStack &) { return endstone::Vector(1, 2, 3); });
    slots[1].is_set = true;
    slots[1].value = endstone::FinePositionResolver(
        [](const endstone::CommandSourceStack &) { return endstone::Vector(1.5F, 2.0F, 3.25F); });
    const CommandSource source = std::make_shared<TestSource>();
    const auto context = CommandCompiler::bindArguments(root, overload, slots, source, "t 1 2 3 ~ ~1 ~");
    ASSERT_TRUE(context.has_value());
    EXPECT_EQ(context->getArgument<endstone::BlockPositionResolver>("b").resolve(*source), endstone::Vector(1, 2, 3));
    EXPECT_EQ(context->getArgument<endstone::FinePositionResolver>("f").resolve(*source),
              endstone::Vector(1.5F, 2.0F, 3.25F));
}

TEST(CommandTreeTest, PassesTheSyntaxErrorOfAResolverThrough)
{
    static const endstone::brigadier::SimpleCommandExceptionType not_found{"No player was found"};
    const endstone::PlayerSelectorArgumentResolver resolver(
        [](const endstone::CommandSourceStack &) -> std::vector<endstone::NotNull<endstone::Player>> {
            throw not_found.create();
        });
    const TestSource source;
    try {
        static_cast<void>(resolver.resolve(source));
        FAIL() << "resolve did not throw";
    }
    catch (const endstone::brigadier::CommandSyntaxException &e) {
        EXPECT_STREQ(e.what(), "No player was found");
    }
}

TEST(CommandTreeTest, NamesTheResolverAndRegistryTypesInAWrongTypeError)
{
    const auto root =
        Commands::literal("t").then(Commands::argument("who", ArgumentTypes::player()).executes(one)).build();
    const CommandOverload overload{{.kind = Kind::Player, .name = "who"}};
    std::vector<CommandSlot> slots(1);
    slots[0].is_set = true;
    slots[0].value = endstone::PlayerSelectorArgumentResolver(
        [](const endstone::CommandSourceStack &) { return std::vector<endstone::NotNull<endstone::Player>>{}; });
    const CommandSource source = std::make_shared<TestSource>();
    const auto context = CommandCompiler::bindArguments(root, overload, slots, source, "t @s");
    ASSERT_TRUE(context.has_value());
    try {
        static_cast<void>(context->getArgument<std::string>("who"));
        FAIL() << "getArgument did not throw";
    }
    catch (const std::invalid_argument &e) {
        EXPECT_STREQ(e.what(), "Argument 'who' is defined as PlayerSelectorArgumentResolver, not string");
    }
    try {
        static_cast<void>(context->getArgument<endstone::NotNull<endstone::Dimension>>("who"));
        FAIL() << "getArgument did not throw";
    }
    catch (const std::invalid_argument &e) {
        EXPECT_STREQ(e.what(), "Argument 'who' is defined as PlayerSelectorArgumentResolver, not Dimension");
    }
    try {
        static_cast<void>(context->getArgument<const endstone::ActorType *>("who"));
        FAIL() << "getArgument did not throw";
    }
    catch (const std::invalid_argument &e) {
        EXPECT_STREQ(e.what(), "Argument 'who' is defined as PlayerSelectorArgumentResolver, not ActorType");
    }
}

TEST(CommandTreeTest, BuiltInArgumentTypesAreNotParsedByBrigadier)
{
    EXPECT_EQ(ArgumentTypes::player()->toString(), "player()");
    EXPECT_EQ(ArgumentTypes::actors()->toString(), "actors()");
    EXPECT_FALSE(ArgumentTypes::player()->is<endstone::ActorArgumentType>());
    const std::vector<std::shared_ptr<const endstone::brigadier::ArgumentType>> types{
        ArgumentTypes::player(),       ArgumentTypes::actor(),     ArgumentTypes::blockPosition(),
        ArgumentTypes::finePosition(), ArgumentTypes::dimension(), ArgumentTypes::resource<endstone::BlockType>()};
    for (const auto &type : types) {
        endstone::brigadier::StringReader reader("@a");
        EXPECT_THROW(static_cast<void>(type->parse(reader)), endstone::brigadier::CommandSyntaxException)
            << type->toString();
    }
    EXPECT_TRUE(ArgumentTypes::resource<endstone::ItemType>()->is<endstone::ResourceArgumentType>());
}

TEST(CommandTreeTest, ComparesResolversByIdentityAsParsedArgumentsDo)
{
    const endstone::FinePositionResolver first(
        [](const endstone::CommandSourceStack &) { return endstone::Vector(0, 0, 0); });
    const auto copy = first;
    const endstone::FinePositionResolver other(
        [](const endstone::CommandSourceStack &) { return endstone::Vector(0, 0, 0); });
    EXPECT_EQ(endstone::brigadier::ParsedArgument(0, 1, first), endstone::brigadier::ParsedArgument(0, 1, copy));
    EXPECT_NE(endstone::brigadier::ParsedArgument(0, 1, first), endstone::brigadier::ParsedArgument(0, 1, other));
}
