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

#include "endstone/core/server.h"

#include <filesystem>
#include <memory>

namespace fs = std::filesystem;

#include <boost/algorithm/string.hpp>

#include "bedrock/network/server_network_handler.h"
#include "bedrock/platform/threading/assigned_thread.h"
#include "bedrock/shared_constants.h"
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/level/block/block_descriptor.h"
#include "bedrock/world/scores/server_scoreboard.h"
#include "endstone/color_format.h"
#include "endstone/command/plugin_command.h"
#include "endstone/core/block/block_data.h"
#include "endstone/core/boss/boss_bar.h"
#include "endstone/core/command/command_map.h"
#include "endstone/core/command/console_command_sender.h"
#include "endstone/core/event/handlers/block_gameplay_handler.h"
#include "endstone/core/event/handlers/level_gameplay_handler.h"
#include "endstone/core/event/handlers/player_gameplay_handler.h"
#include "endstone/core/event/handlers/scripting_event_handler.h"
#include "endstone/core/event/handlers/server_network_event_handler.h"
#include "endstone/core/level/level.h"
#include "endstone/core/logger_factory.h"
#include "endstone/core/message.h"
#include "endstone/core/permissions/default_permissions.h"
#include "endstone/core/plugin/cpp_plugin_loader.h"
#include "endstone/core/plugin/python_plugin_loader.h"
#include "endstone/core/signal_handler.h"
#include "endstone/core/util/error.h"
#include "endstone/event/server/broadcast_message_event.h"
#include "endstone/event/server/server_load_event.h"
#include "endstone/plugin/plugin.h"

#if !defined(ENDSTONE_VERSION)
#error ENDSTONE_VERSION is not defined
#endif

namespace endstone::core {

EndstoneServer::EndstoneServer() : logger_(LoggerFactory::getLogger("Server"))
{
    crash_handler_ = std::make_unique<CrashHandler>();
    signal_handler_ = std::make_unique<SignalHandler>();
    player_ban_list_ = std::make_unique<EndstonePlayerBanList>("banned-players.json");
    ip_ban_list_ = std::make_unique<EndstoneIpBanList>("banned-ips.json");
    language_ = std::make_unique<EndstoneLanguage>();
    plugin_manager_ = std::make_unique<EndstonePluginManager>(*this);
    command_sender_ = EndstoneConsoleCommandSender::create();
    scheduler_ = std::make_unique<EndstoneScheduler>(*this);
    start_time_ = std::chrono::system_clock::now();
}

void EndstoneServer::init(ServerInstance &server_instance)
{
    server_instance_ = &server_instance;
    getLogger().info("{}This server is running {} version: {} (Minecraft: {})",
                     ColorFormat::DarkAqua + ColorFormat::Bold, getName(), getVersion(), getMinecraftVersion());
    command_sender_->init();
    player_ban_list_->load();
    ip_ban_list_->load();
}

EndstonePackSource &EndstoneServer::createResourcePackSource(Bedrock::NotNullNonOwnerPtr<IResourcePackRepository> repo)
{
    resource_pack_repository_ = repo;
    if (!resource_pack_source_) {
        resource_pack_source_ =
            std::make_unique<EndstonePackSource>(repo->getResourcePacksPath().getContainer(), PackType::Resources);
    }
    return *resource_pack_source_;
}

EndstonePackSource &EndstoneServer::getResourcePackSource() const
{
    return *resource_pack_source_;
}

Bedrock::NotNullNonOwnerPtr<const IResourcePackRepository> EndstoneServer::getResourcePackRepository() const
{
    return Bedrock::NonOwnerPointer<const IResourcePackRepository>(*resource_pack_repository_);
}

std::string EndstoneServer::getName() const
{
    return "Endstone";
}

std::string EndstoneServer::getVersion() const
{
    return ENDSTONE_VERSION;
}

std::string EndstoneServer::getMinecraftVersion() const
{
    static auto minecraft_version = [] {
        auto game_version = Common::getGameVersionString();
        if (game_version[0] == 'v') {
            game_version = game_version.substr(1);  // Removes the initial 'v'
        }
        return game_version;
    }();
    return minecraft_version;
}

Logger &EndstoneServer::getLogger() const
{
    return logger_;
}

Language &EndstoneServer::getLanguage() const
{
    return *language_;
}

EndstoneCommandMap &EndstoneServer::getCommandMap() const
{
    return *command_map_;
}

void EndstoneServer::setCommandMap(std::unique_ptr<EndstoneCommandMap> command_map)
{
    command_map_ = std::move(command_map);
}

PluginManager &EndstoneServer::getPluginManager() const
{
    return *plugin_manager_;
}

PluginCommand *EndstoneServer::getPluginCommand(std::string name) const
{
    if (auto *command = command_map_->getCommand(name)) {
        return command->asPluginCommand();
    }
    return nullptr;
}

ConsoleCommandSender &EndstoneServer::getCommandSender() const
{
    return *command_sender_;
}

bool EndstoneServer::dispatchCommand(CommandSender &sender, std::string command_line) const
{
    return command_map_->dispatch(sender, std::move(command_line));
}

void EndstoneServer::loadPlugins()
{
    plugin_manager_->registerLoader(std::make_unique<CppPluginLoader>(*this));
    plugin_manager_->registerLoader(std::make_unique<PythonPluginLoader>(*this));

    auto plugin_dir = fs::current_path() / "plugins";

    if (exists(plugin_dir)) {
        plugin_manager_->loadPlugins(plugin_dir.string());
    }
    else {
        create_directories(plugin_dir);
    }
}

void EndstoneServer::enablePlugins(PluginLoadOrder type)
{
    if (type == PluginLoadOrder::PostWorld) {
        command_map_->setPluginCommands();
        DefaultPermissions::registerCorePermissions();
    }

    auto plugins = plugin_manager_->getPlugins();
    for (auto *plugin : plugins) {
        if (!plugin->isEnabled() && plugin->getDescription().getLoad() == type) {
            enablePlugin(*plugin);
        }
    }
}

void EndstoneServer::enablePlugin(Plugin &plugin)
{
    auto perms = plugin.getDescription().getPermissions();
    for (const auto &perm : perms) {
        if (plugin_manager_->addPermission(std::make_unique<Permission>(perm)) == nullptr) {
            getLogger().warning("Plugin {} tried to register permission '{}' that was already registered.",
                                plugin.getDescription().getFullName(), perm.getName());
        }
    }
    plugin_manager_->dirtyPermissibles(true);
    plugin_manager_->dirtyPermissibles(false);
    plugin_manager_->enablePlugin(plugin);
}

void EndstoneServer::registerEndstoneEventHandlers(::Level &level)
{
    level.getBlockEventCoordinator().block_gameplay_handler_ = std::make_unique<EndstoneBlockGameplayHandler>(
        std::move(level.getBlockEventCoordinator().block_gameplay_handler_));
    level.getLevelEventCoordinator().level_gameplay_handler_ = std::make_unique<EndstoneLevelGameplayHandler>(
        std::move(level.getLevelEventCoordinator().level_gameplay_handler_));
    level.getServerPlayerEventCoordinator().player_gameplay_handler_ = std::make_unique<EndstonePlayerGameplayHandler>(
        std::move(level.getServerPlayerEventCoordinator().player_gameplay_handler_));
    level.getScriptingEventCoordinator().scripting_event_handler_ = std::make_unique<EndstoneScriptingEventHandler>(
        std::move(level.getScriptingEventCoordinator().scripting_event_handler_));
    level.getServerNetworkEventCoordinator().server_network_event_handler_ =
        std::make_unique<EndstoneServerNetworkEventHandler>(
            std::move(level.getServerNetworkEventCoordinator().server_network_event_handler_));
}

void EndstoneServer::disablePlugins() const
{
    plugin_manager_->disablePlugins();
}

Scheduler &EndstoneServer::getScheduler() const
{
    return *scheduler_;
}

Level *EndstoneServer::getLevel() const
{
    return level_.get();
}

void EndstoneServer::setLevel(std::unique_ptr<EndstoneLevel> level)
{
    level_ = std::move(level);
    registerEndstoneEventHandlers(level_->getHandle());
}

std::vector<Player *> EndstoneServer::getOnlinePlayers() const
{
    std::vector<Player *> result;
    result.reserve(players_.size());
    for (const auto &[id, player] : players_) {
        result.push_back(player);
    }
    return result;
}

int EndstoneServer::getMaxPlayers() const
{
    return getServer().getMinecraft()->getServerNetworkHandler()->max_num_players_;
}

Result<void> EndstoneServer::setMaxPlayers(int max_players)
{
    if (max_players < 0) {
        return nonstd::make_unexpected(make_error("Max number of players must not be negative."));
    }
    if (max_players > MaxPlayers) {
        return nonstd::make_unexpected(
            make_error("Max number of players must not exceed the hard limit {}", MaxPlayers));
    }
    getServer().getMinecraft()->getServerNetworkHandler()->max_num_players_ = max_players;
    getServer().getMinecraft()->getServerNetworkHandler()->updateServerAnnouncement();
    return {};
}

Player *EndstoneServer::getPlayer(UUID id) const
{
    auto it = players_.find(id);
    if (it != players_.end()) {
        return it->second;
    }
    return nullptr;
}

Player *EndstoneServer::getPlayer(std::string name) const
{
    for (const auto &[_, player] : players_) {
        if (boost::iequals(player->getName(), name)) {
            return player;
        }
    }
    return nullptr;
}

Player *EndstoneServer::getPlayer(const NetworkIdentifier &network_id, SubClientId sub_id) const
{
    for (const auto &[uuid, player] : players_) {
        if (const auto component = player->getHandle().getPersistentComponent<UserEntityIdentifierComponent>();
            component->network_id == network_id && component->client_sub_id == sub_id) {
            return player;
        }
    }
    return nullptr;
}

bool EndstoneServer::getOnlineMode() const
{
    return getServer().getMinecraft()->getServerNetworkHandler()->network_server_config_.require_trusted_authentication;
}

void EndstoneServer::shutdown()
{
    static_cast<EndstoneScheduler &>(getScheduler()).runTask([this]() {
        server_instance_->getMinecraft()->requestServerShutdown("");
    });
}

void EndstoneServer::reload()
{
    plugin_manager_->clearPlugins();
    command_map_->clearCommands();
    reloadData();

    // TODO(server): Wait for at most 2.5 seconds for all async tasks to finish, otherwise issue a warning
    loadPlugins();
    enablePlugins(PluginLoadOrder::Startup);
    enablePlugins(PluginLoadOrder::PostWorld);
    ServerLoadEvent event{ServerLoadEvent::LoadType::Reload};
    getPluginManager().callEvent(event);

    // sync commands
    for (const auto &[uuid, player] : players_) {
        player->updateCommands();
    }
}

void EndstoneServer::reloadData()
{
    server_instance_->getMinecraft()->requestResourceReload();
    level_->getHandle().loadFunctionManager();
}

void EndstoneServer::broadcast(const Message &message, const std::string &permission) const
{
    std::unordered_set<const CommandSender *> recipients;
    for (const auto *permissible : getPluginManager().getPermissionSubscriptions(permission)) {
        if (const auto *sender = permissible->asCommandSender(); sender && sender->hasPermission(permission)) {
            recipients.insert(sender);
        }
    }

    BroadcastMessageEvent event{!isPrimaryThread(), EndstoneMessage::toString(message), recipients};
    getPluginManager().callEvent(event);

    if (event.isCancelled()) {
        return;
    }

    for (const auto &recipient : recipients) {
        recipient->sendMessage(event.getMessage());
    }
}

void EndstoneServer::broadcastMessage(const Message &message) const
{
    broadcast(message, BroadcastChannelUser);
}

bool EndstoneServer::isPrimaryThread() const
{
    return Bedrock::Threading::getServerThread().isOnThread();
}

Scoreboard *EndstoneServer::getScoreboard() const
{
    return scoreboard_.get();
}

void EndstoneServer::setScoreboard(std::unique_ptr<EndstoneScoreboard> scoreboard)
{
    scoreboard_ = std::move(scoreboard);
}

std::shared_ptr<Scoreboard> EndstoneServer::createScoreboard()
{
    auto registry = CommandSoftEnumRegistry();
    auto board = std::make_unique<ServerScoreboard>(registry, nullptr, level_->getHandle().getGameplayUserManager());
    board->setPacketSender(level_->getHandle().getPacketSender());
    auto result = std::make_shared<EndstoneScoreboard>(std::move(board));
    scoreboards_.emplace_back(result);
    return result;
}

float EndstoneServer::getCurrentMillisecondsPerTick()
{
    return current_mspt_;
}

float EndstoneServer::getAverageMillisecondsPerTick()
{
    return std::accumulate(average_mspt_, average_mspt_ + SharedConstants::TicksPerSecond, 0.0F) /
           SharedConstants::TicksPerSecond;
}

float EndstoneServer::getCurrentTicksPerSecond()
{
    return current_tps_;
}

float EndstoneServer::getAverageTicksPerSecond()
{
    return std::accumulate(average_tps_, average_tps_ + SharedConstants::TicksPerSecond, 0.0F) /
           SharedConstants::TicksPerSecond;
}

float EndstoneServer::getCurrentTickUsage()
{
    return current_usage_;
}

float EndstoneServer::getAverageTickUsage()
{
    return std::accumulate(average_usage_, average_usage_ + SharedConstants::TicksPerSecond, 0.0F) /
           SharedConstants::TicksPerSecond;
}

std::chrono::system_clock::time_point EndstoneServer::getStartTime()
{
    return start_time_;
}

std::unique_ptr<BossBar> EndstoneServer::createBossBar(std::string title, BarColor color, BarStyle style) const
{
    return std::make_unique<EndstoneBossBar>(std::move(title), color, style);
}

std::unique_ptr<BossBar> EndstoneServer::createBossBar(std::string title, BarColor color, BarStyle style,
                                                       std::vector<BarFlag> flags) const
{
    return std::make_unique<EndstoneBossBar>(std::move(title), color, style, flags);
}

Result<std::shared_ptr<BlockData>> EndstoneServer::createBlockData(std::string type) const
{
    return createBlockData(type, {});
}

Result<std::shared_ptr<BlockData>> EndstoneServer::createBlockData(std::string type, BlockStates block_states) const
{
    std::unordered_map<std::string, std::variant<int, std::string, bool>> states;
    for (const auto &state : block_states) {
        std::visit(overloaded{[&](auto &&arg) {
                       states.emplace(state.first, arg);
                   }},
                   state.second);
    }

    const auto block_descriptor = ScriptModuleMinecraft::ScriptBlockUtils::createBlockDescriptor(type, states);
    const auto *block = block_descriptor.tryGetBlockNoLogging();
    if (!block) {
        return nonstd::make_unexpected(make_error("Block type {} cannot be found in the registry.", type));
    }

    return std::make_shared<EndstoneBlockData>(const_cast<::Block &>(*block));
}

PlayerBanList &EndstoneServer::getBanList() const
{
    return *player_ban_list_;
}

IpBanList &EndstoneServer::getIpBanList() const
{
    return *ip_ban_list_;
}

EndstoneScoreboard &EndstoneServer::getPlayerBoard(const EndstonePlayer &player) const
{
    auto it = player_boards_.find(&player);
    if (it == player_boards_.end()) {
        return *scoreboard_;
    }
    return *it->second;
}

void EndstoneServer::setPlayerBoard(EndstonePlayer &player, Scoreboard &scoreboard)
{
    auto &old_board = getPlayerBoard(player).getHandle();
    auto &new_board = static_cast<EndstoneScoreboard &>(scoreboard).getHandle();

    if (&old_board == &new_board) {
        return;
    }

    // remove player from the old board
    getPlayerBoard(player).resetScores(&player);

    // add player to the new board
    new_board.onPlayerJoined(player.getHandle());

    // update tracking records
    if (&scoreboard == scoreboard_.get()) {
        player_boards_.erase(&player);
    }
    else {
        player_boards_[&player] = std::static_pointer_cast<EndstoneScoreboard>(scoreboard.shared_from_this());
    }
}

void EndstoneServer::removePlayerBoard(EndstonePlayer &player)
{
    player_boards_.erase(&player);
}

void EndstoneServer::tick(std::uint64_t current_tick, const std::function<void()> &tick_function)
{
    using namespace std::chrono;

    const auto tick_time = steady_clock::now();

    scheduler_->mainThreadHeartbeat(current_tick);
    tick_function();

    current_mspt_ = static_cast<float>(duration_cast<milliseconds>(steady_clock::now() - tick_time).count());
    current_tps_ =
        std::min(static_cast<float>(SharedConstants::TicksPerSecond), 1000.0F / std::max(1.0F, current_mspt_));
    current_usage_ = std::min(1.0F, current_mspt_ / SharedConstants::MilliSecondsPerTick);
    const auto idx = current_tick % SharedConstants::TicksPerSecond;
    average_mspt_[idx] = current_mspt_;
    average_tps_[idx] = current_tps_;
    average_usage_[idx] = current_usage_;
}

ServerInstance &EndstoneServer::getServer() const
{
    return *server_instance_;
}

}  // namespace endstone::core
