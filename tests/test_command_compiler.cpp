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
#include <cstddef>
#include <format>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "endstone/core/command/command_usage_parser.h"
#include "endstone/core/command/compiler/command_compiler.h"

using endstone::core::CommandCompiler;
using endstone::core::CommandOverload;
using endstone::core::CommandParameter;
using endstone::core::CommandUsageParser;
using Kind = CommandParameter::Kind;

namespace {

struct Compiled {
    CommandCompiler compiler;
    std::vector<std::optional<std::string>> errors;
};

Compiled make_compiled(const std::string &label, const std::vector<std::string> &usages,
                       const std::set<std::string> &soft_enums = {})
{
    Compiled result{CommandCompiler(label), {}};
    for (const auto &usage : usages) {
        auto parsed = CommandUsageParser(usage).parse();
        EXPECT_TRUE(parsed.has_value()) << usage;
        result.errors.push_back(parsed ? result.compiler.addUsage(*parsed, soft_enums)
                                       : std::optional<std::string>{parsed.error()});
    }
    return result;
}

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

}  // namespace

TEST(CommandCompilerTest, RegistersEveryUsageAtOnePosition)
{
    const std::vector<std::pair<std::string, std::string>> pairs = {
        {"/t (a|b)<x: TX>", "/t <name: string>"},         {"/t set <value: string>", "/t set reset"},
        {"/t <name: player>", "/t <name: str>"},          {"/t <warp: WarpNames>", "/t <name: string>"},
        {"/t <warp: WarpNames>", "/t <home: HomeNames>"},
    };
    for (const auto &[first, second] : pairs) {
        auto [compiler, errors] = make_compiled("t", {first, second}, {"WarpNames", "HomeNames"});
        EXPECT_EQ(errors[0], std::nullopt) << first;
        EXPECT_EQ(errors[1], std::nullopt) << second << ": " << errors[1].value_or("");
        EXPECT_EQ(compiler.getCompiled().overloads.size(), 2) << second;
    }
}

TEST(CommandCompilerTest, RegistersAKeywordBesideEveryIdClassParameter)
{
    for (const auto *type : {"string", "str", "actor", "entity", "target", "player", "message", "WarpNames"}) {
        for (const auto &usages : {std::vector<std::string>{"/t list", std::format("/t <name: {}>", type)},
                                   std::vector<std::string>{std::format("/t <name: {}>", type), "/t list"}}) {
            auto [compiler, errors] = make_compiled("t", usages, {"WarpNames"});
            EXPECT_EQ(errors[0], std::nullopt) << type;
            EXPECT_EQ(errors[1], std::nullopt) << type << ": " << errors[1].value_or("");
            EXPECT_EQ(compiler.getCompiled().overloads.size(), 2) << type;
        }
    }

    CommandCompiler compiler("t");
    ASSERT_EQ(compiler.addOverloads({{CommandParameter{
                  .kind = Kind::Keyword, .name = "list", .enum_name = "TList", .values = {"list"}}}}),
              std::nullopt);
    EXPECT_EQ(compiler.addOverloads({{CommandParameter{.kind = Kind::RawText, .name = "text"}}}), std::nullopt);
    EXPECT_EQ(compiler.getCompiled().overloads.size(), 2);
}

TEST(CommandCompilerTest, RefusesTwoEnumsSharingAValueAtOnePosition)
{
    auto [compiler, errors] = make_compiled("t", {"/t (a|b)<x: TX>", "/t (b|c)<y: TY>", "/t b"});
    EXPECT_EQ(errors[0], std::nullopt);
    ASSERT_TRUE(errors[1].has_value());
    EXPECT_EQ(*errors[1], "'/t' has <x: TX> and <y: TY> at one position, and both take the word 'b'.");
    ASSERT_TRUE(errors[2].has_value());
    EXPECT_EQ(*errors[2], "'/t' has <x: TX> and b at one position, and both take the word 'b'.");
}

TEST(CommandCompilerTest, ExpandsAnOptionalBeforeARequiredIntoItsForms)
{
    auto [compiler, errors] = make_compiled("t", {"/t [list] <name: str>"});
    EXPECT_EQ(errors[0], std::nullopt) << errors[0].value_or("");
    EXPECT_EQ(make_shapes(compiler), (std::vector<std::vector<std::pair<Kind, bool>>>{
                                         {{Kind::String, false}},
                                         {{Kind::Keyword, false}, {Kind::String, false}},
                                     }));

    auto both = make_compiled("t", {"/t [a: int] [b: float] <c: int> [d: int]"});
    EXPECT_EQ(both.errors[0], std::nullopt) << both.errors[0].value_or("");
    EXPECT_EQ(make_shapes(both.compiler),
              (std::vector<std::vector<std::pair<Kind, bool>>>{
                  {{Kind::Int, false}, {Kind::Int, true}},
                  {{Kind::Int, false}, {Kind::Int, false}, {Kind::Int, true}},
                  {{Kind::Float, false}, {Kind::Int, false}, {Kind::Int, true}},
                  {{Kind::Int, false}, {Kind::Float, false}, {Kind::Int, false}, {Kind::Int, true}},
              }));
}

TEST(CommandCompilerTest, RefusesAUsageWithMoreThanSixOptionalsToExpand)
{
    auto [compiler, errors] =
        make_compiled("t", {"/t [a: int] [b: int] [c: int] [d: int] [e: int] [f: int] [g: int] <h: int>",
                            "/t [a: int] [b: int] [c: int] [d: int] [e: int] [f: int] <g: float>",
                            "/t <a: string> [b: int] [c: int] [d: int] [e: int] [f: int] [g: int] [h: int]"});
    ASSERT_TRUE(errors[0].has_value());
    EXPECT_EQ(*errors[0], "It has 7 optional parameters before a required one, more than the 6 a usage can expand.");
    EXPECT_EQ(errors[1], std::nullopt) << errors[1].value_or("");
    EXPECT_EQ(errors[2], std::nullopt) << errors[2].value_or("");
    EXPECT_EQ(compiler.getCompiled().overloads.size(), 8);
}

TEST(CommandCompilerTest, CapturesAKeywordBesideAString)
{
    auto [compiler, errors] = make_compiled("warp", {"/warp list", "/warp <name: str>"});
    EXPECT_EQ(errors[1], std::nullopt) << errors[1].value_or("");
    const auto &compiled = compiler.getCompiled();
    ASSERT_EQ(compiled.registered.size(), 2);
    const auto &capture = *std::ranges::find_if(
        compiled.registered, [](const auto &registered) { return registered.parameters.front().kind == Kind::String; });
    EXPECT_TRUE(capture.parameters.front().capture);
    ASSERT_EQ(capture.sources.size(), 2);
    const auto choose = [&](const std::string &word, bool quoted) {
        return CommandCompiler::chooseSource(capture, {{.string_value = word, .is_set = true, .quoted = quoted}});
    };
    ASSERT_TRUE(choose("list", false).has_value());
    EXPECT_EQ(capture.sources[*choose("list", false)].front().kind, Kind::Keyword);
    EXPECT_EQ(capture.sources[*choose("LIST", false)].front().kind, Kind::Keyword);
    EXPECT_EQ(capture.sources[*choose("spawn", false)].front().kind, Kind::String);
    EXPECT_EQ(capture.sources[*choose("list", true)].front().kind, Kind::String);
}

TEST(CommandCompilerTest, ChoosesTheExpandedFormTheWordsSelect)
{
    auto [compiler, errors] = make_compiled("t", {"/t [list] <name: str>"});
    EXPECT_EQ(errors[0], std::nullopt) << errors[0].value_or("");
    const auto &registered = compiler.getCompiled().registered;
    ASSERT_EQ(registered.size(), 3);
    const auto &two = *std::ranges::find_if(registered, [](const auto &entry) {
        return entry.parameters.size() == 2 && entry.parameters.front().kind == Kind::String;
    });
    ASSERT_EQ(two.sources.size(), 1);
    const auto chosen = CommandCompiler::chooseSource(
        two, {{.string_value = "list", .is_set = true}, {.string_value = "foo", .is_set = true}});
    ASSERT_TRUE(chosen.has_value());
    EXPECT_EQ(two.sources[*chosen].front().kind, Kind::Keyword);
    const auto refused = CommandCompiler::chooseSource(
        two, {{.string_value = "foo", .is_set = true}, {.string_value = "bar", .is_set = true}});
    ASSERT_FALSE(refused.has_value());
    EXPECT_EQ(refused.error(), 0);
    const auto &one = *std::ranges::find_if(registered, [](const auto &entry) { return entry.parameters.size() == 1; });
    const auto name = CommandCompiler::chooseSource(one, {{.string_value = "foo", .is_set = true}});
    ASSERT_TRUE(name.has_value());
    EXPECT_EQ(one.sources[*name].size(), 1);
    const auto missing = CommandCompiler::chooseSource(one, {{.string_value = "list", .is_set = true}});
    ASSERT_FALSE(missing.has_value());
    EXPECT_EQ(missing.error(), 1);
    const auto quoted = CommandCompiler::chooseSource(one, {{.string_value = "list", .is_set = true, .quoted = true}});
    ASSERT_TRUE(quoted.has_value());
    EXPECT_EQ(one.sources[*quoted].size(), 1);
}

TEST(CommandCompilerTest, CapturesAKeywordAForeignEnumTakesAtRuntime)
{
    auto [compiler, errors] = make_compiled("t", {"/t stone <n: int>", "/t <b: block>", "/t <b: block> <n: int>"});
    EXPECT_EQ(errors[2], std::nullopt) << errors[2].value_or("");
    const auto &compiled = compiler.getCompiled();
    auto registered = compiled.registered;
    ASSERT_EQ(CommandCompiler::captureWords(
                  registered, compiled.overloads,
                  [](const CommandParameter &word, const CommandParameter &argument) {
                      return word.isWord() && argument.enum_name == "Block" && word.values.front() == "stone";
                  },
                  "/t", false),
              std::nullopt);
    const auto &block = *std::ranges::find_if(registered, [](const auto &entry) {
        return entry.parameters.size() == 2 && entry.parameters.front().enum_name == "Block";
    });
    ASSERT_EQ(block.sources.size(), 2);
    const auto stone = CommandCompiler::chooseSource(
        block, {{.string_value = "stone", .is_set = true}, {.string_value = "1", .is_set = true}});
    ASSERT_TRUE(stone.has_value());
    EXPECT_EQ(block.sources[*stone].front().kind, Kind::Keyword);
    const auto dirt = CommandCompiler::chooseSource(
        block, {{.string_value = "dirt", .is_set = true}, {.string_value = "1", .is_set = true}});
    ASSERT_TRUE(dirt.has_value());
    EXPECT_EQ(block.sources[*dirt].front().enum_name, "Block");
    const auto &single = *std::ranges::find_if(registered, [](const auto &entry) {
        return entry.parameters.size() == 1 && entry.parameters.front().enum_name == "Block";
    });
    const auto incomplete = CommandCompiler::chooseSource(single, {{.string_value = "stone", .is_set = true}});
    ASSERT_FALSE(incomplete.has_value());
    EXPECT_EQ(incomplete.error(), 1);
}

TEST(CommandCompilerTest, KeepsTrailingOptionalsOmittable)
{
    auto [message, message_errors] = make_compiled("t", {"/t <a: int> [b: int] [m: message]"});
    EXPECT_EQ(message_errors[0], std::nullopt) << message_errors[0].value_or("");
    EXPECT_EQ(make_shapes(message), (std::vector<std::vector<std::pair<Kind, bool>>>{
                                        {{Kind::Int, false}},
                                        {{Kind::Int, false}, {Kind::Int, false}, {Kind::Message, false}},
                                    }));
    const auto &full = *std::ranges::find_if(message.getCompiled().registered,
                                             [](const auto &entry) { return entry.parameters.size() == 3; });
    EXPECT_TRUE(CommandCompiler::chooseSource(
                    full, {{.string_value = "1", .is_set = true}, {.string_value = "2", .is_set = true}})
                    .has_value());
    EXPECT_TRUE(CommandCompiler::chooseSource(full, {{.string_value = "1", .is_set = true},
                                                     {.string_value = "2", .is_set = true},
                                                     {.string_value = "x y", .is_set = true}})
                    .has_value());

    auto [plain, plain_errors] = make_compiled("t", {"/t <a: int> [b: int] [c: string]"});
    EXPECT_EQ(plain_errors[0], std::nullopt) << plain_errors[0].value_or("");
    EXPECT_EQ(make_shapes(plain), (std::vector<std::vector<std::pair<Kind, bool>>>{
                                      {{Kind::Int, false}, {Kind::Int, true}, {Kind::String, true}},
                                  }));
    const auto &only = plain.getCompiled().registered.front();
    EXPECT_TRUE(CommandCompiler::chooseSource(only, {{.string_value = "1", .is_set = true}}).has_value());
}

TEST(CommandCompilerTest, RefusesALiteralWordWithMoreTextBesideAMessage)
{
    auto [compiler, errors] = make_compiled("t", {"/t list", "/t <text: message>"});
    EXPECT_EQ(errors[1], std::nullopt) << errors[1].value_or("");
    const auto &registered = *std::ranges::find_if(compiler.getCompiled().registered, [](const auto &entry) {
        return entry.parameters.front().kind == Kind::Message;
    });
    const auto list = CommandCompiler::chooseSource(registered, {{.string_value = "list", .is_set = true}});
    ASSERT_TRUE(list.has_value());
    EXPECT_EQ(registered.sources[*list].front().kind, Kind::Keyword);
    EXPECT_FALSE(CommandCompiler::chooseSource(registered, {{.string_value = "list all", .is_set = true}}).has_value());
    const auto text = CommandCompiler::chooseSource(registered, {{.string_value = "hello world", .is_set = true}});
    ASSERT_TRUE(text.has_value());
    EXPECT_EQ(registered.sources[*text].front().kind, Kind::Message);
}

TEST(CommandCompilerTest, RefusesAMessageOrRawTextThatIsNotLast)
{
    auto [compiler, errors] = make_compiled("t", {"/t <m: message> <n: int>"});
    ASSERT_TRUE(errors[0].has_value());
    EXPECT_EQ(*errors[0], "'/t <m: message>' is a message and is not last.");

    CommandCompiler raw("t");
    const auto error = raw.addOverloads(
        {{CommandParameter{.kind = Kind::RawText, .name = "text"}, CommandParameter{.kind = Kind::Int, .name = "n"}}});
    ASSERT_TRUE(error.has_value());
    EXPECT_EQ(*error, "'/t <text: raw_text>' is raw text and is not last.");
}

TEST(CommandCompilerTest, RefusesATypeWithNoBedrockSymbol)
{
    auto [compiler, errors] = make_compiled("t", {"/t <n: long>", "/t <n: int>"});
    ASSERT_TRUE(errors[0].has_value());
    EXPECT_EQ(*errors[0], "Unsupported type 'long'.");
    EXPECT_EQ(errors[1], std::nullopt);
}

TEST(CommandCompilerTest, AcceptsTheCleanShapes)
{
    const std::vector<std::vector<std::string>> shapes = {
        {"/t give <p: player> <n: int>", "/t give <p: player> all"},
        {"/t (a|b)<x: TX>", "/t (c|d)<y: TY>"},
        {"/t <a: int>", "/t <a: int> <b: int>"},
        {"/t <n: int>", "/t list"},
        {"/t <p: block_pos>", "/t <e: actor>"},
        {"/t <p: pos>", "/t here"},
        {"/t <s: string>", "/t <p: pos>"},
        {"/t [n: int] (a|b)<x: TX>"},
        {"/t <p: player> <m: message>"},
        {"/t <b: block> [states: block_states]", "/t <e: entity_type>", "/t <flag: bool>", "/t <j: json>"},
    };
    for (const auto &usages : shapes) {
        auto [compiler, errors] = make_compiled("t", usages);
        for (std::size_t i = 0; i < usages.size(); ++i) {
            EXPECT_EQ(errors[i], std::nullopt) << usages[i] << ": " << errors[i].value_or("");
        }
    }
}

TEST(CommandCompilerTest, SharesOneEnumPerKeyword)
{
    auto [compiler, errors] = make_compiled(
        "Test", {"/Test set <a: int>", "/Test set <a: int> <b: string>", "/Test reset", "/Test reset set <a: int>"});
    for (const auto &error : errors) {
        EXPECT_EQ(error, std::nullopt) << error.value_or("");
    }
    const auto &enums = compiler.getCompiled().enums;
    ASSERT_EQ(enums.size(), 2);
    EXPECT_EQ(enums[0].name, "TestSet");
    EXPECT_EQ(enums[0].values, std::vector<std::string>{"set"});
    EXPECT_EQ(enums[1].name, "TestReset");
    EXPECT_EQ(enums[1].values, std::vector<std::string>{"reset"});
    for (const auto &overload : compiler.getCompiled().overloads) {
        for (const auto &parameter : overload) {
            if (parameter.kind == Kind::Keyword) {
                EXPECT_TRUE(parameter.enum_name == "TestSet" || parameter.enum_name == "TestReset")
                    << parameter.enum_name;
            }
        }
    }
}

TEST(CommandCompilerTest, LowercasesKeywordsAndEnumValues)
{
    auto [compiler, errors] = make_compiled("t", {"/t (Add|REMOVE)<action: TAction>", "/t Reset", "/t [All]"});
    for (const auto &error : errors) {
        EXPECT_EQ(error, std::nullopt) << error.value_or("");
    }
    const auto &enums = compiler.getCompiled().enums;
    ASSERT_EQ(enums.size(), 3);
    EXPECT_EQ(enums[0].values, (std::vector<std::string>{"add", "remove"}));
    EXPECT_EQ(enums[1].name, "TReset");
    EXPECT_EQ(enums[1].values, std::vector<std::string>{"reset"});
    EXPECT_EQ(enums[2].name, "TAll");
    EXPECT_EQ(enums[2].values, std::vector<std::string>{"all"});
}

TEST(CommandCompilerTest, RefusesValuesEqualOnceLowercased)
{
    auto [compiler, errors] = make_compiled("t", {"/t (yes|YES)<answer: TAnswer>"});
    ASSERT_TRUE(errors[0].has_value());
    EXPECT_EQ(*errors[0], "'/t' has the value 'yes' twice in 'TAnswer' once lowercased.");
}

TEST(CommandCompilerTest, RefusesOneEnumNameWithTwoValueSets)
{
    auto [compiler, errors] = make_compiled("t", {"/t (a|b)<x: TX>", "/t <n: int> (c|d)<x: TX>"});
    EXPECT_EQ(errors[0], std::nullopt);
    ASSERT_TRUE(errors[1].has_value());
    EXPECT_EQ(*errors[1], "'/t' declares the enum 'TX' with the values 'a|b' and also 'c|d'.");
}

TEST(CommandCompilerTest, ExpandsOptionalsBeforeAMessage)
{
    auto spark = make_compiled("spark", {"/spark (profiler|sampler)<module: SparkProfilerModule> "
                                         "(start|stop|upload|info|cancel|open|trust-viewer)[action: "
                                         "SparkProfilerAction] [flags: message]"});
    EXPECT_EQ(spark.errors[0], std::nullopt);
    EXPECT_EQ(make_shapes(spark.compiler), (std::vector<std::vector<std::pair<Kind, bool>>>{
                                               {{Kind::Enum, false}},
                                               {{Kind::Enum, false}, {Kind::Enum, false}, {Kind::Message, false}},
                                           }));

    auto message_only = make_compiled("testoptmsg", {"/testoptmsg [msg: message]"});
    EXPECT_EQ(make_shapes(message_only.compiler),
              (std::vector<std::vector<std::pair<Kind, bool>>>{{{Kind::Message, false}}}));

    auto ban = make_compiled("ban", {"/ban <name: str> [reason: message]"});
    EXPECT_EQ(make_shapes(ban.compiler),
              (std::vector<std::vector<std::pair<Kind, bool>>>{{{Kind::String, false}, {Kind::Message, false}}}));

    auto three = make_compiled("t", {"/t [a: int] [b: float] <m: message>"});
    EXPECT_EQ(make_shapes(three.compiler), (std::vector<std::vector<std::pair<Kind, bool>>>{
                                               {},
                                               {{Kind::Int, false}},
                                               {{Kind::Int, false}, {Kind::Float, false}, {Kind::Message, false}},
                                           }));
}

TEST(CommandCompilerTest, DropsThePrefixThatStopsBeforeAMessage)
{
    auto [compiler, errors] = make_compiled("t", {"/t", "/t <m: message>"});
    EXPECT_EQ(make_shapes(compiler), (std::vector<std::vector<std::pair<Kind, bool>>>{{{Kind::Message, false}}}));

    auto optional = make_compiled("t", {"/t [a: int]", "/t <a: int> <m: message>"});
    EXPECT_EQ(make_shapes(optional.compiler), (std::vector<std::vector<std::pair<Kind, bool>>>{
                                                  {},
                                                  {{Kind::Int, false}, {Kind::Message, false}},
                                              }));
}

TEST(CommandCompilerTest, DropsCoveredOverloads)
{
    auto [compiler, errors] = make_compiled("version", {"/version", "/version [plugin: PluginNames]"}, {"PluginNames"});
    EXPECT_EQ(make_shapes(compiler), (std::vector<std::vector<std::pair<Kind, bool>>>{{{Kind::SoftEnum, true}}}));
}

TEST(CommandCompilerTest, RefusesTheOverloadPastTheBudget)
{
    std::vector<std::string> usages;
    for (int i = 0; i < 251; ++i) {
        usages.push_back(std::format("/t k{}", i));
    }
    auto [compiler, errors] = make_compiled("t", usages);
    for (int i = 0; i < 250; ++i) {
        EXPECT_EQ(errors[i], std::nullopt) << i;
    }
    ASSERT_TRUE(errors[250].has_value());
    EXPECT_EQ(*errors[250], "'/t' would have 251 overloads, more than the 250 a command can have.");
    EXPECT_EQ(compiler.getCompiled().overloads.size(), 250);
}

TEST(CommandCompilerTest, SuffixesAnEnumNameAForeignEnumHolds)
{
    const std::map<std::string, std::vector<std::string>> taken = {
        {"FormType", {"modal"}},
        {"FormType2", {"other"}},
        {"Shared", {"a", "b"}},
    };
    EXPECT_EQ(CommandCompiler::nameEnum("Fresh", {"x"}, taken), "Fresh");
    EXPECT_EQ(CommandCompiler::nameEnum("Shared", {"a", "b"}, taken), "Shared");
    EXPECT_EQ(CommandCompiler::nameEnum("FormType", {"message", "action"}, taken), "FormType3");
    EXPECT_EQ(CommandCompiler::nameEnum("FormType", {"other"}, taken), "FormType2");
}

TEST(CommandCompilerTest, CompilesEveryBuiltInUsage)
{
    const std::vector<std::pair<std::string, std::vector<std::string>>> commands = {
        {"ban", {"/ban <name: str> [reason: message]"}},
        {"ban-ip", {"/ban-ip <target: str> [reason: message]"}},
        {"banlist", {"/banlist (ips|players)[type: BanListType]"}},
        {"pardon", {"/pardon <name: str>"}},
        {"pardon-ip", {"/pardon-ip <address: str>"}},
        {"plugins", {"/plugins"}},
        {"reload", {"/reload"}},
        {"restart", {"/restart"}},
        {"seed", {"/seed"}},
        {"status", {"/status"}},
        {"version", {"/version", "/version [plugin: PluginNames]"}},
    };
    for (const auto &[label, usages] : commands) {
        auto [compiler, errors] = make_compiled(label, usages, {"PluginNames"});
        for (std::size_t i = 0; i < usages.size(); ++i) {
            EXPECT_EQ(errors[i], std::nullopt) << usages[i] << ": " << errors[i].value_or("");
        }
        EXPECT_FALSE(compiler.getCompiled().overloads.empty()) << label;
    }
}
