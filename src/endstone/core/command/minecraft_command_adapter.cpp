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

#include "endstone/core/command/minecraft_command_adapter.h"

#include <cassert>
#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <typeinfo>
#include <utility>
#include <vector>

#include <entt/locator/locator.hpp>

#include "bedrock/server/commands/command_selector.h"
#include "bedrock/world/actor/actor.h"
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/level/dimension/dimension.h"
#include "endstone/actor/actor_type.h"
#include "endstone/block/block_type.h"
#include "endstone/brigadier/exceptions.h"
#include "endstone/check.h"
#include "endstone/command/brigadier/argument/resolvers/block_position_resolver.h"
#include "endstone/command/brigadier/argument/resolvers/fine_position_resolver.h"
#include "endstone/command/brigadier/argument/resolvers/selector/actor_selector_argument_resolver.h"
#include "endstone/command/brigadier/argument/resolvers/selector/player_selector_argument_resolver.h"
#include "endstone/core/command/command_origin_wrapper.h"
#include "endstone/core/command/command_source_stack.h"
#include "endstone/core/command/commands.h"
#include "endstone/core/level/level.h"
#include "endstone/inventory/item_type.h"
#include "endstone/lang/language.h"
#include "endstone/lang/translatable.h"
#include "endstone/player.h"
#include "endstone/plugin/plugin.h"
#include "endstone/plugin/plugin_manager.h"
#include "endstone/util/vector.h"

namespace endstone::core {

std::unordered_set<const ::Command *> CommandAdapter::instances_;

CommandAdapter::CommandAdapter()
{
    assert(!entt::locator<EndstoneServer>::has_value() || EndstoneServer::getInstance().isPrimaryThread());
    instances_.insert(this);
}

CommandAdapter::~CommandAdapter()
{
    assert(!entt::locator<EndstoneServer>::has_value() || EndstoneServer::getInstance().isPrimaryThread());
    instances_.erase(this);
}

void CommandAdapter::execute(const CommandOrigin &origin, CommandOutput &output) const
{
    const std::shared_ptr<CommandOutput> output_ref(&output, [](CommandOutput *) {});
    if (run(origin, origin.getEndstoneSender(output_ref), "")) {
        output.success();
    }
}

const CommandAdapter *CommandAdapter::find(const ::Command *command)
{
    if (!instances_.contains(command)) {
        return nullptr;
    }
    return static_cast<const CommandAdapter *>(command);
}

MinecraftCommandAdapter::MinecraftCommandAdapter(std::string label, RegisteredOverload registered)
    : label_(std::move(label)), registered_(std::move(registered))
{
}

bool MinecraftCommandAdapter::run(const CommandOrigin & /*origin*/, const NotNull<CommandSender> &sender,
                                  const std::string & /*input*/) const
{
    const auto &server = EndstoneServer::getInstance();
    try {
        const auto command = server.getCommandMap().getCommand(label_);
        if (!command) {
            sender->sendErrorMessage(Translatable("commands.generic.unknown", {label_}));
            return false;
        }
        const auto chosen = CommandCompiler::chooseSource(registered_, args_);
        if (!chosen) {
            std::string before = "/" + label_ + " ";
            for (std::size_t i = 0; i < chosen.error(); ++i) {
                before += args_[i].string_value + " ";
            }
            std::string after;
            for (std::size_t i = chosen.error() + 1; i < args_.size(); ++i) {
                after += " " + args_[i].string_value;
            }
            sender->sendErrorMessage(
                Translatable("commands.generic.syntax",
                             {before, chosen.error() < args_.size() ? args_[chosen.error()].string_value : "", after}));
            return false;
        }
        std::vector<std::string> args;
        for (const auto &arg : args_) {
            args.push_back(arg.string_value);
        }
        return command->execute(sender, args);
    }
    catch (const std::exception &e) {
        if (std::string_view(typeid(e).name()) == typeid(brigadier::CommandSyntaxException).name()) {
            sender->sendErrorMessage(e.what());
            return false;
        }
        server.getLogger().error("Unhandled exception executing '{}': {}", label_, e.what());
        return false;
    }
}

BrigadierCommandAdapter::BrigadierCommandAdapter(std::string label, RegisteredOverload registered)
    : label_(std::move(label)), registered_(std::move(registered)), slots_(registered_.parameters.size()),
      native_slots_(registered_.parameters.size())
{
    for (std::size_t i = 0; i < registered_.parameters.size(); ++i) {
        switch (registered_.parameters[i].kind) {
        case CommandParameter::Kind::Actor:
            native_slots_[i] = std::make_shared<CommandSelector<::Actor>>();
            break;
        case CommandParameter::Kind::Player:
            native_slots_[i] = std::make_shared<CommandSelector<::Player>>();
            break;
        case CommandParameter::Kind::BlockPos:
            native_slots_[i].emplace<CommandPosition>();
            break;
        case CommandParameter::Kind::Pos:
            native_slots_[i].emplace<CommandPositionFloat>();
            break;
        default:
            break;
        }
    }
}

bool BrigadierCommandAdapter::run(const CommandOrigin &origin, const NotNull<CommandSender> &sender,
                                  const std::string &input) const
{
    const auto &server = EndstoneServer::getInstance();
    try {
        const auto command = server.getCommandMap().getCommand(label_);
        const auto tree = server.getCommandMap().getTree(label_, registered_);
        if (!tree || !command.is<TreeCommand>()) {
            sender->sendErrorMessage(Translatable("commands.generic.unknown", {label_}));
            return false;
        }
        const auto owner = server.getPluginManager().getPlugin(command.cast<TreeCommand>()->getOwner());
        if (!owner) {
            sender->sendMessage("Cannot execute command '{}' - plugin is disabled.", command->getName());
            return false;
        }
        if (!owner->isEnabled()) {
            sender->sendMessage("Cannot execute command '{}' in plugin {} - plugin is disabled.", command->getName(),
                                owner->getDescription().getFullName());
            return false;
        }
        Nullable<Actor> executor;
        if (const auto *entity = origin.getEntity()) {
            executor = entity->getEndstoneActor<Actor>();
        }
        Nullable<Dimension> dimension;
        if (const auto *level = server.getEndstoneLevel(); level != nullptr && origin.getDimension() != nullptr) {
            dimension = level->getDimension(origin.getDimension()->getDimensionId());
        }
        const auto position = origin.getWorldPosition();
        const auto rotation = origin.getRotation().value_or(Vec2{});
        const CommandSource source = std::make_shared<EndstoneCommandSourceStack>(
            sender, executor, Location(dimension, position.x, position.y, position.z, rotation.x, rotation.y),
            origin.clone());
        auto slots = slots_;
        for (std::size_t i = 0; i < slots.size(); ++i) {
            const CommandSelectorBase *selector = nullptr;
            if (const auto *player = std::get_if<std::shared_ptr<CommandSelector<::Player>>>(&native_slots_[i])) {
                selector = player->get();
            }
            else if (const auto *actor = std::get_if<std::shared_ptr<CommandSelector<::Actor>>>(&native_slots_[i])) {
                selector = actor->get();
            }
            if (selector != nullptr && selector->is_explicit_id_selector_ && !selector->name_filters_.empty()) {
                slots[i].string_value = selector->name_filters_.front().value;
            }
        }
        const auto chosen = CommandCompiler::chooseSource(registered_, slots);
        if (!chosen) {
            sender->sendErrorMessage(Translatable("commands.generic.unknown", {label_}));
            return false;
        }
        const auto &overload = registered_.sources[*chosen];
        std::vector<std::string> names;
        for (std::size_t i = 0; i < overload.size() && i < slots.size() && slots[i].is_set; ++i) {
            names.push_back(overload[i].name);
        }
        if (CommandCompiler::findUsable(*tree, names, source) != names.size()) {
            sender->sendErrorMessage(Translatable("commands.generic.unknown", {label_}));
            return false;
        }
        for (std::size_t i = 0; i < overload.size() && i < slots.size(); ++i) {
            using Kind = CommandParameter::Kind;
            const auto &parameter = overload[i];
            auto &slot = slots[i];
            if (!slot.is_set) {
                continue;
            }
            switch (parameter.kind) {
            case Kind::Player: {
                const std::shared_ptr<const CommandSelector<::Player>> selector =
                    std::get<std::shared_ptr<CommandSelector<::Player>>>(native_slots_[i]);
                slot.value = PlayerSelectorArgumentResolver([selector, single = parameter.single](
                                                                const CommandSourceStack &stack) {
                    static const brigadier::SimpleCommandExceptionType not_found{
                        EndstoneServer::getInstance().getLanguage().translate("commands.generic.noTargetMatch")};
                    static const brigadier::SimpleCommandExceptionType too_many{
                        EndstoneServer::getInstance().getLanguage().translate("commands.generic.tooManyTargets")};
                    const auto *endstone_stack = dynamic_cast<const EndstoneCommandSourceStack *>(&stack);
                    Preconditions::checkArgument(endstone_stack != nullptr && endstone_stack->getOrigin() != nullptr,
                                                 "The command source was not created by the server.");
                    std::vector<NotNull<Player>> players;
                    const auto results = selector->results(*endstone_stack->getOrigin());
                    for (auto *actor : *results.targets_) {
                        players.emplace_back(static_cast<::Player *>(actor)->getEndstoneActor<Player>());
                    }
                    if (single && players.empty()) {
                        throw not_found.create();
                    }
                    if (single && players.size() > 1) {
                        throw too_many.create();
                    }
                    return players;
                });
                break;
            }
            case Kind::Actor: {
                const std::shared_ptr<const CommandSelector<::Actor>> selector =
                    std::get<std::shared_ptr<CommandSelector<::Actor>>>(native_slots_[i]);
                slot.value = ActorSelectorArgumentResolver([selector, single = parameter.single](
                                                               const CommandSourceStack &stack) {
                    static const brigadier::SimpleCommandExceptionType not_found{
                        EndstoneServer::getInstance().getLanguage().translate("commands.generic.noTargetMatch")};
                    static const brigadier::SimpleCommandExceptionType too_many{
                        EndstoneServer::getInstance().getLanguage().translate("commands.generic.tooManyTargets")};
                    const auto *endstone_stack = dynamic_cast<const EndstoneCommandSourceStack *>(&stack);
                    Preconditions::checkArgument(endstone_stack != nullptr && endstone_stack->getOrigin() != nullptr,
                                                 "The command source was not created by the server.");
                    std::vector<NotNull<Actor>> actors;
                    const auto results = selector->results(*endstone_stack->getOrigin());
                    for (const auto *actor : *results.targets_) {
                        actors.emplace_back(actor->getEndstoneActor<Actor>());
                    }
                    if (single && actors.empty()) {
                        throw not_found.create();
                    }
                    if (single && actors.size() > 1) {
                        throw too_many.create();
                    }
                    return actors;
                });
                break;
            }
            case Kind::BlockPos:
                slot.value = BlockPositionResolver([position = std::get<CommandPosition>(native_slots_[i]),
                                                    version = version_](const CommandSourceStack &stack) {
                    const auto *endstone_stack = dynamic_cast<const EndstoneCommandSourceStack *>(&stack);
                    Preconditions::checkArgument(endstone_stack != nullptr && endstone_stack->getOrigin() != nullptr,
                                                 "The command source was not created by the server.");
                    const auto block_pos =
                        position.getBlockPos(version, *endstone_stack->getOrigin(), Vec3{0.0F, 0.0F, 0.0F});
                    return Vector(block_pos.x, block_pos.y, block_pos.z);
                });
                break;
            case Kind::Pos:
                slot.value = FinePositionResolver([position = std::get<CommandPositionFloat>(native_slots_[i]),
                                                   version = version_](const CommandSourceStack &stack) {
                    const auto *endstone_stack = dynamic_cast<const EndstoneCommandSourceStack *>(&stack);
                    Preconditions::checkArgument(endstone_stack != nullptr && endstone_stack->getOrigin() != nullptr,
                                                 "The command source was not created by the server.");
                    const auto vec =
                        position.getPosition(version, *endstone_stack->getOrigin(), Vec3{0.0F, 0.0F, 0.0F});
                    return Vector(vec.x, vec.y, vec.z);
                });
                break;
            case Kind::Dimension: {
                static const brigadier::SimpleCommandExceptionType unknown_dimension{
                    server.getLanguage().translate("commands.generic.dimension.notFound")};
                Nullable<Dimension> found;
                if (const auto *level = server.getEndstoneLevel()) {
                    found = level->getDimension(DimensionId(slot.string_value));
                }
                if (!found) {
                    throw unknown_dimension.create();
                }
                slot.value = NotNull<Dimension>(found);
                break;
            }
            case Kind::BlockType: {
                static const brigadier::DynamicCommandExceptionType unknown{[](const std::string &value) {
                    return EndstoneServer::getInstance().getLanguage().translate("commands.setblock.notFound", {value});
                }};
                const auto *type = server.getRegistry<BlockType>().get(BlockTypeId(slot.string_value));
                if (type == nullptr) {
                    throw unknown.create(slot.string_value);
                }
                slot.value = type;
                break;
            }
            case Kind::ItemType: {
                static const brigadier::DynamicCommandExceptionType unknown{[](const std::string &value) {
                    return EndstoneServer::getInstance().getLanguage().translate("commands.give.item.notFound",
                                                                                 {value});
                }};
                const auto *type = server.getRegistry<ItemType>().get(ItemTypeId(slot.string_value));
                if (type == nullptr) {
                    throw unknown.create(slot.string_value);
                }
                slot.value = type;
                break;
            }
            case Kind::ActorType: {
                static const brigadier::DynamicCommandExceptionType unknown{[](const std::string &value) {
                    return EndstoneServer::getInstance().getLanguage().translate("commands.generic.entity.invalidType",
                                                                                 {value});
                }};
                const auto *type = server.getRegistry<ActorType>().get(ActorTypeId(slot.string_value));
                if (type == nullptr) {
                    throw unknown.create(slot.string_value);
                }
                slot.value = type;
                break;
            }
            default:
                break;
            }
        }
        const auto context = CommandCompiler::bindArguments(tree, overload, slots, source, input);
        if (!context) {
            sender->sendErrorMessage(Translatable("commands.generic.unknown", {label_}));
            return false;
        }
        return context->getCommand()(*context) > 0;
    }
    catch (const std::exception &e) {
        if (std::string_view(typeid(e).name()) == typeid(brigadier::CommandSyntaxException).name()) {
            sender->sendErrorMessage(e.what());
            return false;
        }
        server.getLogger().error("Unhandled exception executing '{}': {}", label_, e.what());
        return false;
    }
}

void *BrigadierCommandAdapter::getSlot(::Command *command, int index)
{
    auto &self = static_cast<BrigadierCommandAdapter &>(*command);
    auto &slot = self.slots_.at(index);
    const auto &parameter = self.registered_.parameters.at(index);
    switch (parameter.kind) {
    case CommandParameter::Kind::Actor:
        return std::get<std::shared_ptr<CommandSelector<::Actor>>>(self.native_slots_.at(index)).get();
    case CommandParameter::Kind::Player:
        return std::get<std::shared_ptr<CommandSelector<::Player>>>(self.native_slots_.at(index)).get();
    case CommandParameter::Kind::BlockPos:
        return &std::get<CommandPosition>(self.native_slots_.at(index));
    case CommandParameter::Kind::Pos:
        return &std::get<CommandPositionFloat>(self.native_slots_.at(index));
    default:
        if (parameter.isParsedAsText()) {
            return &slot;
        }
        return &slot.string_value;
    }
}

bool *BrigadierCommandAdapter::isSlotSet(::Command *command, int index)
{
    return &static_cast<BrigadierCommandAdapter &>(*command).slots_.at(index).is_set;
}

}  // namespace endstone::core

namespace {
std::pair<std::string_view, bool> removeQuotes(const std::string_view &str)
{
    if (str.size() < 2) {
        return {str, false};
    }
    if (str.front() == '"' && str.back() == '"') {
        return {str.substr(1, str.size() - 2), true};
    }
    return {str, false};
}

const CommandRegistry::ParseToken *findFirstWithText(const CommandRegistry::ParseToken *node)
{
    if (!node) {
        return nullptr;
    }

    // Check current node first (pre-order)
    if (node->text) {
        return node;
    }

    // Recurse into child
    if (const CommandRegistry::ParseToken *found = findFirstWithText(node->child.get())) {
        return found;
    }

    // Recurse into next sibling
    return findFirstWithText(node->next.get());
}

std::string_view rstrip(std::string_view str, std::string_view chars = " \t\n\r\f\v")
{
    size_t end = str.size();
    while (end > 0 && chars.find(str[end - 1]) != std::string_view::npos) {
        --end;
    }
    return str.substr(0, end);
}

std::optional<std::pair<std::string_view, bool>> parseNode(const CommandRegistry::ParseToken &root)
{
    // If there are no children, this is a leaf text node.
    if (!root.child) {
        if (root.length == 0) {
            return std::nullopt;
        }
        if (root.type == CommandRegistry::HardNonTerminal::Id) {
            return removeQuotes({root.text, root.length});
        }
        return std::pair{std::string_view{root.text, root.length}, false};
    }

    // Otherwise:
    const auto &child = *root.child;
    const auto *begin = findFirstWithText(&child);
    if (!begin) {
        return std::nullopt;
    }

    // (1) If this node has no next sibling, it's the last argument: consume all remaining text until the end
    if (!root.next) {
        const auto view = rstrip(begin->text);
        if (child.type == CommandRegistry::HardNonTerminal::Id) {
            return removeQuotes(view);
        }
        return std::pair{view, false};
    }

    // (2) Otherwise, find the start of the next argument and extract the substring between begin and end, then trim
    // trailing whitespace.
    const auto *end = findFirstWithText(root.next.get());
    if (!end) {
        const auto view = rstrip(begin->text);
        if (child.type == CommandRegistry::HardNonTerminal::Id) {
            return removeQuotes(view);
        }
        return std::pair{view, false};
    }

    const auto view = rstrip({begin->text, end->text});
    if (child.type == CommandRegistry::HardNonTerminal::Id) {
        return removeQuotes(view);
    }
    return std::pair{view, false};
}
}  // namespace

template <>
bool CommandRegistry::parse<endstone::core::MinecraftCommandAdapter>(void *storage, const ParseToken &token,
                                                                     const CommandOrigin &origin, int version,
                                                                     std::string &error,
                                                                     std::vector<std::string> &error_params) const
{
    if (!storage) {
        return false;
    }
    auto &output = static_cast<endstone::core::MinecraftCommandAdapter *>(storage)->args_;
    if (auto result = parseNode(token)) {
        output.push_back({.string_value = std::string(result->first), .is_set = true, .quoted = result->second});
    }
    return true;
}

template <>
bool CommandRegistry::parse<endstone::core::BrigadierCommandAdapter>(void *storage, const ParseToken &token,
                                                                     const CommandOrigin &origin, int version,
                                                                     std::string &error,
                                                                     std::vector<std::string> &error_params) const
{
    if (!storage) {
        return false;
    }
    if (auto result = parseNode(token)) {
        auto &slot = *static_cast<endstone::core::CommandSlot *>(storage);
        slot.string_value = result->first;
        slot.quoted = result->second;
    }
    return true;
}

template <>
const CommandRegistry::ParamParseRule CommandRegistry::ParseRuleFor<endstone::core::MinecraftCommandAdapter>::instance{
    &CommandRegistry::parse<endstone::core::MinecraftCommandAdapter>,
    CommandRegistry::HardNonTerminal::BlockStateArray};
