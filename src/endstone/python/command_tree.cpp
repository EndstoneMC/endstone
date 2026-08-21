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

#include "endstone/command/arguments.h"
#include "endstone/command/command_builder.h"
#include "endstone/command/command_context.h"
#include "endstone/command/command_error.h"
#include "endstone/command/command_node.h"
#include "endstone_python.h"

namespace endstone::python {

namespace {

py::handle command_error_type;

py::object toPython(const ArgumentValue &value)
{
    if (const auto *v = value.get<bool>()) {
        return py::cast(*v);
    }
    if (const auto *v = value.get<int>()) {
        return py::cast(*v);
    }
    if (const auto *v = value.get<float>()) {
        return py::cast(*v);
    }
    if (const auto *v = value.get<std::string>()) {
        return py::cast(*v);
    }
    if (const auto *v = value.get<NotNull<Player>>()) {
        return py::cast(*v);
    }
    if (const auto *v = value.get<std::vector<NotNull<Player>>>()) {
        return py::cast(*v);
    }
    if (const auto *v = value.get<NotNull<Actor>>()) {
        return py::cast(*v);
    }
    if (const auto *v = value.get<std::vector<NotNull<Actor>>>()) {
        return py::cast(*v);
    }
    throw py::type_error(std::format("Argument of type '{}' cannot be read from Python.", value.getTypeName()));
}

CommandHandler wrapHandler(py::function handler)
{
    return [handler = std::move(handler)](const CommandContext &ctx) {
        const py::gil_scoped_acquire gil{};
        try {
            handler(py::cast(&ctx, py::return_value_policy::reference));
        }
        catch (py::error_already_set &e) {
            if (command_error_type && e.matches(command_error_type)) {
                throw CommandError(py::str(e.value()).cast<std::string>());
            }
            throw;
        }
    };
}

CommandRequirement wrapRequirement(py::function predicate)
{
    return [predicate = std::move(predicate)](const NotNull<CommandSender> &sender) {
        const py::gil_scoped_acquire gil{};
        return predicate(sender).cast<bool>();
    };
}

template <typename Builder>
void bindBuilder(py::class_<Builder> &cls)
{
    cls.def(
           "then", [](Builder &self, const LiteralArgumentBuilder &child) { return self.then(child.build()); },
           py::arg("child"), "Adds a child branch.")
        .def(
            "then", [](Builder &self, const RequiredArgumentBuilder &child) { return self.then(child.build()); },
            py::arg("child"), "Adds a child branch.")
        .def(
            "executes", [](Builder &self, py::function handler) { return self.executes(wrapHandler(std::move(handler))); },
            py::arg("handler"), R"doc(
    Sets the handler run when a command ends at this node.

    Returning normally reports the command as successful; raise a `CommandError` to report a
    failure to the sender.
)doc")
        .def(
            "permission", [](Builder &self, std::string permission) { return self.permission(std::move(permission)); },
            py::arg("permission"), R"doc(
    Requires a permission to use this branch.

    A sender holding any one of the permissions added here passes. A sender without one is told
    they lack permission, and the branch is hidden from their client.
)doc")
        .def(
            "requires",
            [](Builder &self, py::function predicate) { return self.requires_(wrapRequirement(std::move(predicate))); },
            py::arg("predicate"), R"doc(
    Requires a predicate to pass to use this branch.

    Unlike `permission`, a sender that fails is not told why, and the branch is simply absent.
    Keep the predicate cheap: it runs once per node per parse, and again for every node each time
    the command tree is sent to a player.
)doc")
        .def("build", &Builder::build, "Returns the node this builder has shaped.");
}

}  // namespace

void init_command_tree(py::module_ &m)
{
    command_error_type = py::register_exception<CommandError>(m, "CommandError").ptr();

    py::enum_<ArgumentKind>(m, "ArgumentKind", "The built-in argument types Endstone can register with the client.")
        .value("CUSTOM", ArgumentKind::Custom)
        .value("BOOLEAN", ArgumentKind::Boolean)
        .value("INTEGER", ArgumentKind::Integer)
        .value("FLOAT", ArgumentKind::Float)
        .value("STRING", ArgumentKind::String)
        .value("MESSAGE", ArgumentKind::Message)
        .value("RAW_TEXT", ArgumentKind::RawText)
        .value("PLAYER", ArgumentKind::Player)
        .value("PLAYERS", ArgumentKind::Players)
        .value("ENTITY", ArgumentKind::Entity)
        .value("ENTITIES", ArgumentKind::Entities)
        .value("BLOCK_POSITION", ArgumentKind::BlockPosition)
        .value("POSITION", ArgumentKind::Position)
        .value("BLOCK_TYPE", ArgumentKind::BlockType)
        .value("BLOCK_STATES", ArgumentKind::BlockStates)
        .value("ENTITY_TYPE", ArgumentKind::EntityType)
        .value("JSON", ArgumentKind::Json)
        .value("INTEGER_RANGE", ArgumentKind::IntegerRange)
        .value("ENUMERATION", ArgumentKind::Enumeration)
        .value("SOFT_ENUM", ArgumentKind::SoftEnum);

    py::classh<ArgumentType>(m, "ArgumentType", "Describes how a command argument is parsed.")
        .def_property_readonly("kind", &ArgumentType::getKind, "Which built-in type this is.");

    py::classh<BuiltinArgumentType, ArgumentType>(m, "BuiltinArgumentType",
                                                  "An argument type provided by Endstone.");

    py::classh<CommandNode>(m, "CommandNode", "Represents a node in a command tree.")
        .def_property_readonly("name", &CommandNode::getName, "The name of this node.")
        .def_property_readonly("children", &CommandNode::getChildren, "The children of this node.")
        .def_property_readonly("is_executable", &CommandNode::isExecutable,
                               "Whether a command may end at this node.")
        .def_property_readonly("permissions", &CommandNode::getPermissions,
                               "The permissions that allow this branch to be used.");

    py::classh<LiteralCommandNode, CommandNode>(m, "LiteralCommandNode",
                                                "A node matched by typing a fixed word.")
        .def_property_readonly("description", &LiteralCommandNode::getDescription,
                               "A brief description of this command.")
        .def_property_readonly("aliases", &LiteralCommandNode::getAliases,
                               "The alternative names this command is registered under.");

    py::classh<ArgumentCommandNode, CommandNode>(m, "ArgumentCommandNode",
                                                 "A node matched by parsing a value.");

    auto literal_builder = py::class_<LiteralArgumentBuilder>(m, "LiteralArgumentBuilder",
                                                            "Builds a node matched by typing a fixed word.");
    auto required_builder = py::class_<RequiredArgumentBuilder>(m, "RequiredArgumentBuilder",
                                                              "Builds a node matched by parsing a value.");

    literal_builder
        .def(
            "description",
            [](LiteralArgumentBuilder &self, std::string description) {
                return self.description(std::move(description));
            },
            py::arg("description"), "Sets a brief description of this command.")
        .def(
            "aliases",
            [](LiteralArgumentBuilder &self, const std::vector<std::string> &aliases) {
                auto builder = self;
                for (const auto &alias : aliases) {
                    builder = builder.aliases(alias);
                }
                return builder;
            },
            py::arg("aliases"), "Adds alternative names this command may be typed as.")
        .def("register_to", &LiteralArgumentBuilder::registerTo, py::arg("command_map"), py::arg("owner"), R"doc(
    Registers this command tree.

    Args:
        command_map: The `CommandMap` to register to.
        owner: The plugin the command belongs to.

    Returns:
        `True` on success, `False` if a command with the same name is already registered.
)doc");

    bindBuilder(literal_builder);
    bindBuilder(required_builder);

    py::class_<Commands>(m, "Commands", "Entry points for building a command tree.")
        .def_static("literal", &Commands::literal, py::arg("name"),
                    "Begins a branch matched by typing a fixed word.")
        .def_static("argument", &Commands::argument, py::arg("name"), py::arg("type"),
                    "Begins a branch matched by parsing a value.");

    py::class_<CommandContext>(m, "CommandContext", "The arguments and sender a command handler is invoked with.")
        .def_property_readonly("sender", &CommandContext::getSender, "The source of this command.")
        .def_property_readonly("command", &CommandContext::getCommand, py::return_value_policy::reference,
                               "The command being executed.")
        .def_property_readonly("argument_names", &CommandContext::getArgumentNames,
                               "The names of the arguments bound on the branch that ran.")
        .def(
            "__contains__",
            [](const CommandContext &self, const std::string &name) { return self.has(name); }, py::arg("name"))
        .def(
            "__getitem__",
            [](const CommandContext &self, const std::string &name) {
                const auto *value = self.find(name);
                if (!value) {
                    throw py::key_error(name);
                }
                return toPython(*value);
            },
            py::arg("name"))
        .def(
            "get",
            [](const CommandContext &self, const std::string &name, py::object default_value) {
                const auto *value = self.find(name);
                return value ? toPython(*value) : std::move(default_value);
            },
            py::arg("name"), py::arg("default") = py::none(),
            "Returns the value of the named argument, or the default if it was omitted.");

    auto arguments = m.def_submodule("arguments", "Factories for the argument types Endstone provides.");
    arguments.def("boolean", &Arguments::boolean, "A boolean, suggested as true or false.");
    arguments.def("integer", &Arguments::integer, py::arg("minimum") = std::numeric_limits<int>::min(),
                  py::arg("maximum") = std::numeric_limits<int>::max(), "An integer, optionally bounded.");
    arguments.def("floating", &Arguments::floating, py::arg("minimum") = -std::numeric_limits<float>::infinity(),
                  py::arg("maximum") = std::numeric_limits<float>::infinity(),
                  "A floating-point number, optionally bounded.");
    arguments.def("string", &Arguments::string, "A single word, or a quoted string if it contains spaces.");
    arguments.def("message", &Arguments::message, "Everything up to the end of the line.");
    arguments.def("raw_text", &Arguments::rawText, "Everything up to the end of the line, taken verbatim.");
    arguments.def("player", &Arguments::player, "A target selector resolving to exactly one player.");
    arguments.def("players", &Arguments::players, "A target selector resolving to any number of players.");
    arguments.def("entity", &Arguments::entity, "A target selector resolving to exactly one actor.");
    arguments.def("entities", &Arguments::entities, "A target selector resolving to any number of actors.");
    arguments.def("block_position", &Arguments::blockPosition, "A block position.");
    arguments.def("position", &Arguments::position, "A precise position.");
    arguments.def("block_type", &Arguments::blockType, "A block type.");
    arguments.def("block_states", &Arguments::blockStates, "A list of block states.");
    arguments.def("entity_type", &Arguments::entityType, "An actor type.");
    arguments.def("json", &Arguments::json, "A JSON object.");
    arguments.def("integer_range", &Arguments::integerRange, "An integer range, such as 3, 3.., ..7 or 3..7.");
    arguments.def("enumeration", &Arguments::enumeration, py::arg("name"), py::arg("values"),
                  "One of a fixed set of values, validated by the server.");
    arguments.def("soft_enum", &Arguments::softEnum, py::arg("name"), R"doc(
    A string completed from a named set of suggestions that can change at runtime.

    Update the set with `CommandMap.set_suggestions()`. The values are only a hint to the client:
    the server accepts any string, so validate the value in your handler.
)doc");
}

}  // namespace endstone::python
