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

#include <cmath>
#include <cstdint>
#include <exception>
#include <limits>
#include <string_view>
#include <type_traits>
#include <typeinfo>
#include <variant>

#include "endstone_python.h"

namespace endstone::python {

void init_brigadier(py::module_ &m)
{
    using Source = CommandSource;
    using LiteralBuilder = brigadier::LiteralArgumentBuilder<Source>;
    using RequiredBuilder = brigadier::RequiredArgumentBuilder<Source>;
    using Node = brigadier::CommandNode<Source>;

    const auto &syntax_exception =
        py::register_exception<brigadier::CommandSyntaxException>(m, "CommandSyntaxException", PyExc_Exception);
    PyObject *syntax_type = syntax_exception.ptr();
    py::register_exception_translator([](std::exception_ptr exception) {
        try {
            if (exception) {
                std::rethrow_exception(exception);
            }
        }
        catch (const std::exception &e) {
            if (std::string_view(typeid(e).name()) != typeid(brigadier::CommandSyntaxException).name()) {
                throw;
            }
            PyErr_SetString(
                py::module_::import("endstone._python").attr("brigadier").attr("CommandSyntaxException").ptr(),
                e.what());
        }
    });

    py::classh<brigadier::ArgumentType>(m, "ArgumentType", "Turns the text of an argument into its value.")
        .def_property_readonly("examples", &brigadier::ArgumentType::getExamples, "Examples of valid input.")
        .def("__str__", &brigadier::ArgumentType::toString);

    py::classh<brigadier::StringArgumentType, brigadier::ArgumentType>(m, "StringArgumentType",
                                                                       "An argument whose value is a string.")
        .def_static("word", &brigadier::StringArgumentType::word, "A single word of letters, digits and `_-.+`.")
        .def_static("string", &brigadier::StringArgumentType::string, "A single word, or a phrase in quotes.")
        .def_static("greedy_string", &brigadier::StringArgumentType::greedyString,
                    "The rest of the input, which must be the last argument.");

    py::classh<brigadier::BoolArgumentType, brigadier::ArgumentType>(m, "BoolArgumentType",
                                                                     "An argument whose value is `True` or `False`.")
        .def_static("bool", &brigadier::BoolArgumentType::bool_, "Either `true` or `false`.");

    py::classh<brigadier::IntegerArgumentType, brigadier::ArgumentType>(m, "IntegerArgumentType",
                                                                        "An argument whose value is an integer.")
        .def_static("integer", &brigadier::IntegerArgumentType::integer,
                    py::arg("min") = std::numeric_limits<int>::min(), py::arg("max") = std::numeric_limits<int>::max(),
                    R"doc(
    An integer between `min` and `max`, both included.

    Args:
        min: The smallest value accepted.
        max: The largest value accepted.

    Returns:
        The argument type.
)doc")
        .def_property_readonly("minimum", &brigadier::IntegerArgumentType::getMinimum, "The smallest value accepted.")
        .def_property_readonly("maximum", &brigadier::IntegerArgumentType::getMaximum, "The largest value accepted.")
        .def(py::self == py::self)
        .def("__hash__", [](const brigadier::IntegerArgumentType &self) {
            return static_cast<std::int32_t>(31U * static_cast<std::uint32_t>(self.getMinimum()) +
                                             static_cast<std::uint32_t>(self.getMaximum()));
        });

    py::classh<brigadier::FloatArgumentType, brigadier::ArgumentType>(m, "FloatArgumentType",
                                                                      "An argument whose value is a float.")
        .def_static("float_arg", &brigadier::FloatArgumentType::floatArg,
                    py::arg_v("min", -std::numeric_limits<float>::max(), "..."),
                    py::arg_v("max", std::numeric_limits<float>::max(), "..."), R"doc(
    A float between `min` and `max`, both included.

    Args:
        min: The smallest value accepted.
        max: The largest value accepted.

    Returns:
        The argument type.
)doc")
        .def_property_readonly("minimum", &brigadier::FloatArgumentType::getMinimum, "The smallest value accepted.")
        .def_property_readonly("maximum", &brigadier::FloatArgumentType::getMaximum, "The largest value accepted.")
        .def(py::self == py::self)
        .def("__hash__", [](const brigadier::FloatArgumentType &self) {
            const float hash = 31 * self.getMinimum() + self.getMaximum();
            if (std::isnan(hash)) {
                return std::int32_t{0};
            }
            if (hash >= 2147483648.0F) {
                return std::numeric_limits<std::int32_t>::max();
            }
            if (hash <= -2147483648.0F) {
                return std::numeric_limits<std::int32_t>::min();
            }
            return static_cast<std::int32_t>(hash);
        });

    py::classh<brigadier::DoubleArgumentType, brigadier::ArgumentType>(m, "DoubleArgumentType",
                                                                       "An argument whose value is a double.")
        .def_static("double_arg", &brigadier::DoubleArgumentType::doubleArg,
                    py::arg_v("min", -std::numeric_limits<double>::max(), "..."),
                    py::arg_v("max", std::numeric_limits<double>::max(), "..."), R"doc(
    A double between `min` and `max`, both included.

    Args:
        min: The smallest value accepted.
        max: The largest value accepted.

    Returns:
        The argument type.
)doc")
        .def_property_readonly("minimum", &brigadier::DoubleArgumentType::getMinimum, "The smallest value accepted.")
        .def_property_readonly("maximum", &brigadier::DoubleArgumentType::getMaximum, "The largest value accepted.")
        .def(py::self == py::self)
        .def("__hash__", [](const brigadier::DoubleArgumentType &self) {
            const double hash = 31 * self.getMinimum() + self.getMaximum();
            if (std::isnan(hash)) {
                return std::int32_t{0};
            }
            if (hash >= 2147483648.0) {
                return std::numeric_limits<std::int32_t>::max();
            }
            if (hash <= -2147483648.0) {
                return std::numeric_limits<std::int32_t>::min();
            }
            return static_cast<std::int32_t>(hash);
        });

    py::class_<brigadier::CommandContext<Source>>(m, "CommandContext",
                                                  "What a command receives: its source and its parsed arguments.")
        .def_property_readonly("source", &brigadier::CommandContext<Source>::getSource,
                               "The source the command runs with.")
        .def_property_readonly("input", &brigadier::CommandContext<Source>::getInput, "The input of the command.")
        .def(
            "get_argument",
            [](const brigadier::CommandContext<Source> &self, const std::string &name) {
                const auto argument = self.getArguments().find(name);
                if (argument == self.getArguments().end()) {
                    throw std::invalid_argument("No such argument '" + name + "' exists on this command");
                }
                return std::visit(
                    []<typename T>(const T &value) {
                        if constexpr (std::is_pointer_v<T>) {
                            return py::cast(value, py::return_value_policy::reference);
                        }
                        else {
                            return py::cast(value);
                        }
                    },
                    argument->second.getResult());
            },
            py::arg("name"), R"doc(
    Gets the value of an argument.

    Args:
        name: The name of the argument.

    Returns:
        The value of the argument.

    Raises:
        ValueError: If the command has no argument with that name.
)doc");

    py::classh<Node>(m, "CommandNode", "A node of a command tree.")
        .def_property_readonly("name", &Node::getName, "The name of this node.")
        .def_property_readonly("children", &Node::getChildren, "The nodes below this node.")
        .def_property_readonly(
            "command",
            [](const Node &self) -> std::optional<brigadier::Command<Source>> {
                if (!self.getCommand()) {
                    return std::nullopt;
                }
                return self.getCommand();
            },
            "The code that runs when this node ends the input, or `None`.")
        .def("can_use", &Node::canUse, py::arg("source"), R"doc(
    Checks whether a source passes the requirement of this node.

    Args:
        source: The source of the command.

    Returns:
        `True` if the source can use this node.
)doc");

    py::classh<brigadier::LiteralCommandNode<Source>, Node>(m, "LiteralCommandNode", "A node that matches a keyword.")
        .def_property_readonly("literal", &brigadier::LiteralCommandNode<Source>::getLiteral, "The keyword.");

    py::classh<brigadier::ArgumentCommandNode<Source>, Node>(m, "ArgumentCommandNode", "A node that reads an argument.")
        .def_property_readonly("type", &brigadier::ArgumentCommandNode<Source>::getType, "The type of the argument.");

    py::classh<brigadier::RootCommandNode<Source>, Node>(m, "RootCommandNode", "The root node of a dispatcher.");

    auto literal_builder = py::class_<LiteralBuilder>(m, "LiteralArgumentBuilder", "Builds a literal node.");
    auto required_builder = py::class_<RequiredBuilder>(m, "RequiredArgumentBuilder", "Builds an argument node.");

    literal_builder
        .def(
            "then",
            [](LiteralBuilder &self, const LiteralBuilder &argument) -> LiteralBuilder & {
                return self.then(argument);
            },
            py::arg("argument"), py::return_value_policy::reference_internal, R"doc(
    Adds a node below this one.

    Args:
        argument: The builder of the node to add.

    Returns:
        This builder.
)doc")
        .def(
            "then",
            [](LiteralBuilder &self, const RequiredBuilder &argument) -> LiteralBuilder & {
                return self.then(argument);
            },
            py::arg("argument"), py::return_value_policy::reference_internal)
        .def(
            "then",
            [](LiteralBuilder &self, const std::shared_ptr<Node> &argument) -> LiteralBuilder & {
                return self.then(argument);
            },
            py::arg("argument"), py::return_value_policy::reference_internal)
        .def(
            "executes",
            [syntax_type](
                LiteralBuilder &self,
                std::function<py::object(const brigadier::CommandContext<Source> &)> command) -> LiteralBuilder & {
                return self.executes(
                    [command = std::move(command), syntax_type](const brigadier::CommandContext<Source> &context) {
                        try {
                            py::gil_scoped_acquire gil;
                            const auto result = command(context);
                            return result.is_none() ? brigadier::SINGLE_SUCCESS : result.cast<int>();
                        }
                        catch (py::error_already_set &e) {
                            py::gil_scoped_acquire gil;
                            if (!e.matches(syntax_type)) {
                                throw;
                            }
                            static const brigadier::SimpleCommandExceptionType type{""};
                            throw brigadier::CommandSyntaxException(type, py::str(e.value()).cast<std::string>());
                        }
                    });
            },
            py::arg("command"), py::return_value_policy::reference_internal, R"doc(
    Sets the code that runs when the input ends at this node.

    Args:
        command: Takes the `CommandContext` and returns a result above 0 for success, or `None` for success. A `CommandSyntaxException` it raises is sent to the sender.

    Returns:
        This builder.
)doc")
        .def(
            "requires",
            [](LiteralBuilder &self, brigadier::Predicate<Source> requirement) -> LiteralBuilder & {
                return self.requires_(std::move(requirement));
            },
            py::arg("requirement"), py::return_value_policy::reference_internal, R"doc(
    Sets the requirement a source must pass to see and use this node.

    Args:
        requirement: Takes the `CommandSourceStack` and returns whether it can use this node.

    Returns:
        This builder.
)doc")
        .def_property_readonly("literal", &LiteralBuilder::getLiteral, "The keyword of the node.")
        .def("build", &LiteralBuilder::build, R"doc(
    Builds the node.

    Returns:
        The node.
)doc");

    required_builder
        .def(
            "then",
            [](RequiredBuilder &self, const LiteralBuilder &argument) -> RequiredBuilder & {
                return self.then(argument);
            },
            py::arg("argument"), py::return_value_policy::reference_internal, R"doc(
    Adds a node below this one.

    Args:
        argument: The builder of the node to add.

    Returns:
        This builder.
)doc")
        .def(
            "then",
            [](RequiredBuilder &self, const RequiredBuilder &argument) -> RequiredBuilder & {
                return self.then(argument);
            },
            py::arg("argument"), py::return_value_policy::reference_internal)
        .def(
            "then",
            [](RequiredBuilder &self, const std::shared_ptr<Node> &argument) -> RequiredBuilder & {
                return self.then(argument);
            },
            py::arg("argument"), py::return_value_policy::reference_internal)
        .def(
            "executes",
            [syntax_type](
                RequiredBuilder &self,
                std::function<py::object(const brigadier::CommandContext<Source> &)> command) -> RequiredBuilder & {
                return self.executes(
                    [command = std::move(command), syntax_type](const brigadier::CommandContext<Source> &context) {
                        try {
                            py::gil_scoped_acquire gil;
                            const auto result = command(context);
                            return result.is_none() ? brigadier::SINGLE_SUCCESS : result.cast<int>();
                        }
                        catch (py::error_already_set &e) {
                            py::gil_scoped_acquire gil;
                            if (!e.matches(syntax_type)) {
                                throw;
                            }
                            static const brigadier::SimpleCommandExceptionType type{""};
                            throw brigadier::CommandSyntaxException(type, py::str(e.value()).cast<std::string>());
                        }
                    });
            },
            py::arg("command"), py::return_value_policy::reference_internal, R"doc(
    Sets the code that runs when the input ends at this node.

    Args:
        command: Takes the `CommandContext` and returns a result above 0 for success, or `None` for success. A `CommandSyntaxException` it raises is sent to the sender.

    Returns:
        This builder.
)doc")
        .def(
            "requires",
            [](RequiredBuilder &self, brigadier::Predicate<Source> requirement) -> RequiredBuilder & {
                return self.requires_(std::move(requirement));
            },
            py::arg("requirement"), py::return_value_policy::reference_internal, R"doc(
    Sets the requirement a source must pass to see and use this node.

    Args:
        requirement: Takes the `CommandSourceStack` and returns whether it can use this node.

    Returns:
        This builder.
)doc")
        .def_property_readonly("name", &RequiredBuilder::getName, "The name of the argument.")
        .def_property_readonly("type", &RequiredBuilder::getType, "The type of the argument.")
        .def("build", &RequiredBuilder::build, R"doc(
    Builds the node.

    Returns:
        The node.
)doc");

    py::classh<brigadier::CommandDispatcher<Source>>(m, "CommandDispatcher",
                                                     "Parses and runs input against command trees.")
        .def(py::init<>())
        .def("register", &brigadier::CommandDispatcher<Source>::registerCommand, py::arg("command"), R"doc(
    Adds a command tree to this dispatcher.

    Args:
        command: The builder of the root node.

    Returns:
        The root node of the command.
)doc")
        .def(
            "execute",
            [](brigadier::CommandDispatcher<Source> &self, const std::string &input, const Source &source) {
                return self.execute(input, source);
            },
            py::arg("input"), py::arg("source"), R"doc(
    Parses and runs a command.

    Args:
        input: The command, without a leading slash.
        source: The source to run it with.

    Returns:
        The result of the command.

    Raises:
        CommandSyntaxException: If the input does not parse or the command fails.
)doc")
        .def_property_readonly("root", &brigadier::CommandDispatcher<Source>::getRoot, "The root node.");
}

}  // namespace endstone::python
