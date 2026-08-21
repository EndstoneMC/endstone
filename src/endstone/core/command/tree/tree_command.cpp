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

#include "endstone/core/command/tree/tree_command.h"

#include <algorithm>
#include <unordered_map>

#include "bedrock/server/commands/command_selector.h"
#include "endstone/command/command_context.h"
#include "endstone/command/command_error.h"
#include "endstone/core/command/parse_token_text.h"
#include "endstone/core/command/tree/argument_types.h"
#include "endstone/core/server.h"

namespace endstone::core {

namespace {

class TreeCommandContext : public CommandContext {
public:
    TreeCommandContext(const Command &command, NotNull<CommandSender> sender)
        : command_(command), sender_(std::move(sender))
    {
    }

    [[nodiscard]] const NotNull<CommandSender> &getSender() const override { return sender_; }
    [[nodiscard]] const Command &getCommand() const override { return command_; }

    [[nodiscard]] std::vector<std::string> getArgumentNames() const override
    {
        std::vector<std::string> names;
        names.reserve(order_.size());
        for (const auto &name : order_) {
            names.push_back(name);
        }
        return names;
    }

    [[nodiscard]] const ArgumentValue *find(std::string_view name) const override
    {
        const auto it = values_.find(std::string(name));
        return it == values_.end() ? nullptr : &it->second;
    }

    void bind(const std::string &name, ArgumentValue value)
    {
        if (!values_.contains(name)) {
            order_.push_back(name);
        }
        values_.insert_or_assign(name, std::move(value));
    }

private:
    const Command &command_;
    NotNull<CommandSender> sender_;
    std::vector<std::string> order_;
    std::unordered_map<std::string, ArgumentValue> values_;
};

std::unique_ptr<ArgumentStorage> makeStorage(const CommandTreeSlot &slot)
{
    if (!slot.is_literal) {
        const auto &type = *static_cast<const ArgumentCommandNode *>(slot.node.get().get())->getArgumentType();
        switch (effectiveArgumentKind(type)) {
        case ArgumentKind::Player:
        case ArgumentKind::Players:
            return std::make_unique<TypedArgumentStorage<CommandSelector<::Player>>>();
        case ArgumentKind::Entity:
        case ArgumentKind::Entities:
            return std::make_unique<TypedArgumentStorage<CommandSelector<::Actor>>>();
        default:
            break;
        }
    }
    return std::make_unique<TypedArgumentStorage<std::string>>();
}

}  // namespace

TreeCommand::TreeCommand(NotNull<LiteralCommandNode> root, std::vector<CommandTreeOverload> overloads)
    : Command(root->getName(), root->getDescription()), root_(std::move(root)), overloads_(std::move(overloads))
{
    setAliases(root_->getAliases());
    setPermissions(root_->getPermissions());
}

const char *TreeCommand::intern(const std::string &name)
{
    interned_.push_back(name);
    return interned_.back().c_str();
}

bool TreeCommand::testOverloadSilently(const CommandTreeOverload &overload, const NotNull<CommandSender> &sender)
{
    return std::ranges::all_of(overload.path, [&sender](const auto &node) { return node->testSilently(sender); });
}

bool TreeCommand::execute(const NotNull<CommandSender> &sender, const std::vector<std::string> & /*args*/) const
{
    sender->sendErrorMessage(Translatable("commands.generic.unknown", {getName()}));
    return false;
}

bool TreeCommand::run(const CommandTreeOverload &overload, const NotNull<CommandSender> &sender,
                      const std::vector<std::unique_ptr<ArgumentStorage>> &slots, const CommandOrigin &origin) const
{
    if (!testOverloadSilently(overload, sender)) {
        sender->sendErrorMessage(Translatable("commands.generic.error.permissions", {getName()}));
        return false;
    }

    TreeCommandContext context(*this, sender);
    for (std::size_t i = 0; i < overload.slots.size() && i < slots.size(); ++i) {
        const auto &slot = overload.slots[i];
        if (slot.is_literal) {
            continue;
        }
        const auto *argument = static_cast<const ArgumentCommandNode *>(slot.node.get().get());
        const auto &type = *argument->getArgumentType();
        const auto kind = effectiveArgumentKind(type);

        if (isSelectorArgument(kind)) {
            const auto results =
                kind == ArgumentKind::Player || kind == ArgumentKind::Players
                    ? static_cast<const TypedArgumentStorage<CommandSelector<::Player>> *>(slots[i].get())
                          ->get()
                          .newResults(origin)
                    : static_cast<const TypedArgumentStorage<CommandSelector<::Actor>> *>(slots[i].get())
                          ->get()
                          .newResults(origin);
            context.bind(argument->getName(), convertSelector(type, results, sender));
            continue;
        }

        const auto *text = static_cast<const TypedArgumentStorage<std::string> *>(slots[i].get());
        context.bind(argument->getName(), convertArgument(type, text->get(), sender));
    }

    overload.leaf->getHandler()(context);
    return true;
}

TreeCommandAdapter::TreeCommandAdapter(const TreeCommand &command, const CommandTreeOverload &overload)
    : command_(&command), overload_(&overload)
{
    slots_.reserve(overload.slots.size());
    for (const auto &slot : overload.slots) {
        slots_.push_back(makeStorage(slot));
    }
}

void *TreeCommandAdapter::getStorageValue(::Command *command, int index)
{
    auto &slots = static_cast<TreeCommandAdapter *>(command)->slots_;
    if (index < 0 || static_cast<std::size_t>(index) >= slots.size()) {
        return nullptr;
    }
    return slots[index]->data();
}

bool TreeCommandAdapter::runFrom(const NotNull<CommandSender> &sender, const CommandOrigin &origin) const
{
    try {
        return command_->run(*overload_, sender, slots_, origin);
    }
    catch (const CommandError &e) {
        sender->sendErrorMessage(e.getMessage());
        return false;
    }
    catch (const std::exception &e) {
        EndstoneServer::getInstance().getLogger().error("Unhandled exception executing '{}': {}", command_->getName(),
                                                        e.what());
        return false;
    }
}

void TreeCommandAdapter::execute(const CommandOrigin &origin, CommandOutput &output) const
{
    if (runFrom(origin.getEndstoneSender(output), origin)) {
        output.success();
    }
}

}  // namespace endstone::core

template <>
bool CommandRegistry::parse<endstone::core::TreeCommandAdapter>(void *storage, const ParseToken &token,
                                                                const CommandOrigin &origin, int version,
                                                                std::string &error,
                                                                std::vector<std::string> &error_params) const
{
    if (!storage) {
        return false;
    }
    if (auto result = endstone::core::parseNode(token)) {
        static_cast<std::string *>(storage)->assign(*result);
    }
    return true;
}

template <>
const CommandRegistry::ParamParseRule CommandRegistry::ParseRuleFor<endstone::core::TreeCommandAdapter>::instance{
    &CommandRegistry::parse<endstone::core::TreeCommandAdapter>, {}};
