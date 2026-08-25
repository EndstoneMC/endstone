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

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <memory>
#include <ranges>
#include <regex>
#include <sstream>
#include <thread>
#include <unordered_map>
#include <unordered_set>

#include <boost/algorithm/string.hpp>
#include <entt/locator/locator.hpp>
#include <pybind11/pybind11.h>
#include <toml++/toml.h>

#include "bedrock/network/server_network_handler.h"
#include "bedrock/platform/threading/assigned_thread.h"
#include "bedrock/server/dedicated_server.h"
#include "bedrock/server/server_instance.h"
#include "bedrock/server/server_map_data_manager.h"
#include "bedrock/shared_constants.h"
#include "bedrock/world/item/enchanting/enchant.h"
#include "bedrock/world/level/block/block_descriptor.h"
#include "bedrock/world/scores/server_scoreboard.h"
#include "endstone/actor/actor_type.h"
#include "endstone/color_format.h"
#include "endstone/command/plugin_command.h"
#include "endstone/core/ban/ip_ban_list.h"
#include "endstone/core/ban/player_ban_list.h"
#include "endstone/core/block/biome.h"
#include "endstone/core/block/block_data.h"
#include "endstone/core/block/block_type.h"
#include "endstone/core/boss/boss_bar.h"
#include "endstone/core/command/command_map.h"
#include "endstone/core/command/console_command_sender.h"
#include "endstone/core/enchantments/enchantment.h"
#include "endstone/core/game_rule.h"
#include "endstone/core/inventory/item_factory.h"
#include "endstone/core/inventory/item_type.h"
#include "endstone/core/level/chunk.h"
#include "endstone/core/level/dimension.h"
#include "endstone/core/level/level.h"
#include "endstone/core/logger_factory.h"
#include "endstone/core/map/map_view.h"
#include "endstone/core/message.h"
#include "endstone/core/metrics.h"
#include "endstone/core/permissions/default_permissions.h"
#include "endstone/core/player.h"
#include "endstone/core/plugin/cpp_plugin_loader.h"
#include "endstone/core/plugin/python_plugin_loader.h"
#include "endstone/core/potion/effect_type.h"
#include "endstone/core/potion/potion_type.h"
#include "endstone/core/registry.h"
#include "endstone/core/signal_handler.h"
#include "endstone/core/type.h"
#include "endstone/core/util/uuid.h"
#include "endstone/event/chunk/chunk_load_event.h"
#include "endstone/event/chunk/chunk_unload_event.h"
#include "endstone/event/server/broadcast_message_event.h"
#include "endstone/event/server/map_initialize_event.h"
#include "endstone/event/server/server_load_event.h"
#include "endstone/plugin/plugin.h"
#include "endstone/runtime/runtime.h"
#include "endstone/util/format.h"

namespace fs = std::filesystem;
namespace py = pybind11;

namespace endstone::core {

namespace {
std::thread::id &mainThread()
{
    static std::thread::id thread_id;
    return thread_id;
}

// Only archives are auto-activated; an extracted pack still goes through world_resource_packs.json
bool isArchivePack(const ResourceLocation &location)
{
    static const std::regex archive(R"(\.(mcpack|zip)$)", std::regex::icase);
    return std::regex_search(location.getRelativePath().getContainer(), archive);
}

// An encrypted pack carries its content key in a sidecar file next to it, e.g. my_pack.mcpack.key
std::string readContentKey(const ResourceLocation &location, Logger &logger)
{
    const auto path = location.getRelativePath().getContainer() + ".key";
    if (!fs::exists(path)) {
        return {};
    }

    try {
        std::ifstream file(path);
        if (!file.is_open()) {
            logger.error("Could not open encryption key file: '{}'.", path);
            return {};
        }

        std::ostringstream ss;
        ss << file.rdbuf();
        auto key = ss.str();
        boost::algorithm::trim(key);

        if (key.length() != 32) {
            logger.error(
                "Could not open encryption key file: '{}'. Invalid encryption key length, must be exactly 32 bytes.",
                path);
            return {};
        }
        return key;
    }
    catch (const std::exception &e) {
        logger.error("Could not open encryption key file: '{}'. {}.", path, e.what());
        return {};
    }
}
}  // namespace

EndstoneServer::EndstoneServer() : logger_(LoggerFactory::getLogger(""))
{
    registerTypes();
    EndstoneServer::getLogger().info("{}This server is running {} version: {} (Minecraft: {})",
                                     ColorFormat::DarkAqua + ColorFormat::Bold, EndstoneServer::getName(),
                                     EndstoneServer::getVersion(), EndstoneServer::getMinecraftVersion());

    crash_handler_ = std::make_unique<CrashHandler>();
    signal_handler_ = std::make_unique<SignalHandler>();
    player_ban_list_ = std::make_unique<EndstonePlayerBanList>("banned-players.json");
    if (auto result = player_ban_list_->load(); !result) {
        EndstoneServer::getLogger().error(result.error());
    }
    ip_ban_list_ = std::make_unique<EndstoneIpBanList>("banned-ips.json");
    if (auto result = ip_ban_list_->load(); !result) {
        EndstoneServer::getLogger().error(result.error());
    }
    language_ = std::make_unique<EndstoneLanguage>();
    plugin_manager_ = std::make_unique<EndstonePluginManager>(*this);
    service_manager_ = std::make_unique<EndstoneServiceManager>();
    scheduler_ = std::make_unique<EndstoneScheduler>(*this);
    start_time_ = std::chrono::system_clock::now();

    try {
        toml::table tbl = toml::parse_file("endstone.toml");
        log_commands_ = tbl.at_path("commands.log").value_or(true);
        allow_client_packs_ = tbl.at_path("settings.allow-client-packs").value_or(false);
    }
    catch (const toml::parse_error &err) {
        EndstoneServer::getLogger().error("Failed to parse config file: {}", err.what());
    }

    loadPlugins();
}

EndstoneServer::~EndstoneServer() = default;

void EndstoneServer::init(ServerInstance &server_instance)
{
    if (server_instance_) {
        throw std::runtime_error("Server instance already initialized.");
    }
    server_instance_ = &server_instance;
    command_sender_ = std::make_shared<EndstoneConsoleCommandSender>();
    command_sender_->recalculatePermissions();
    enablePlugins(PluginLoadOrder::Startup);
}

// #blameMojang - MCPE-240610: BDS skips the server advertisement when the level's LANBroadcast flag is off,
// leaving RakNet's offline ping response empty. Clients refuse to start the handshake without a valid pong.
void EndstoneServer::fixServerAnnouncement()
{
    const auto handler = getServer().getMinecraft()->getServerNetworkHandler();
    if (!handler || !handler->server_name_.empty()) {
        return;
    }

    auto server_name = getServer().server_name_;
    if (server_name.empty()) {
        server_name = "Endstone Server";
    }

    handler->server_name_ = server_name;
    handler->updateServerAnnouncement();
}

void EndstoneServer::setLevel(::Level &level)
{
    if (level_) {
        throw std::runtime_error("Level already initialized.");
    }
    level_ = std::make_unique<EndstoneLevel>(level);
    level_->loadDimensions();
    scoreboard_ = EndstoneScoreboard::create(level.getScoreboard());
    command_map_ = std::make_unique<EndstoneCommandMap>(*this);
    metrics_ = std::make_unique<Metrics>(*this, "endstone._metrics", "EndstoneMetrics", std::ref<Server>(*this));
    loadResourcePacks();
    initRegistries();

    // enable packet rate limiter
    (void)dispatchCommand(getCommandSender(), "reloadpacketlimitconfig");

    // prevent BDS from sending these messages by default - we allow plugin to override these messages
    auto &text_settings =
        const_cast<ServerTextSettingsBitset &>(server_instance_->getServerTextSettings()->getEnabledServerTextEvents());
    text_settings_ = text_settings;
    text_settings.reset(static_cast<std::underlying_type_t<ServerTextEvent>>(ServerTextEvent::PlayerConnection));
    text_settings.reset(static_cast<std::underlying_type_t<ServerTextEvent>>(ServerTextEvent::PlayerChangedSkin));
    level._getPlayerDeathManager()->sender_.reset();  // player death

    // #blameMojang - MapItemSavedData tracks players for map updates but never cleans up on disconnect.
    // Each stale tracker pins a 256x256 chunk area in memory. Players come and go, memory only goes up.
    // Fix: actually remove players from map trackers when they leave.
    on_gameplay_user_removed_ = level.getGameplayUserManager()->getGameplayUserRemovedConnector().connect(
        [&](EntityContext &entity) {
            if (auto *player = ::Player::tryGetFromEntity(entity, true); player) {
                for (const auto &data : level.getMapDataManager()->getMapDataMap() | std::ranges::views::values) {
                    data->removeTrackedMapEntity(*player);
                }
            }
        },
        Bedrock::PubSub::ConnectPosition::AtBack, nullptr);

    on_chunk_load_ = level.getLevelChunkEventManager()->getOnChunkLoadedConnector().connect(
        [&](ChunkSource & /*chunk_source*/, LevelChunk &lc, int /*closest_player_distance_squared*/) -> void {
            if (lc.getState() >= ChunkState::Loaded) {
                const auto chunk = std::make_unique<EndstoneChunk>(lc);
                ChunkLoadEvent e(*chunk);
                getPluginManager().callEvent(e);
            }
        },
        Bedrock::PubSub::ConnectPosition::AtBack, nullptr);

    on_chunk_unload_ = level.getLevelChunkEventManager()->getOnChunkDiscardedConnector().connect(
        [&](LevelChunk &lc) -> void {
            const auto chunk = std::make_unique<EndstoneChunk>(lc);
            ChunkUnloadEvent e(*chunk);
            getPluginManager().callEvent(e);
        },
        Bedrock::PubSub::ConnectPosition::AtBack, nullptr);

    on_map_created_ = static_cast<ServerMapDataManager *>(level.getMapDataManager().get().access())
                          ->getOnCreateMapSavedDataConnector()
                          .connect(
                              [&](const MapItemSavedData &map_data) {
                                  // The map origin isn't initialized yet at this point.
                                  // Defer the event to the next tick to ensure all data is fully set.
                                  auto &map = map_data.getMapView();
                                  getEndstoneScheduler().runTask([&]() {
                                      MapInitializeEvent e{map};
                                      getPluginManager().callEvent(e);
                                  });
                              },
                              Bedrock::PubSub::ConnectPosition::AtBack, nullptr);

    fixServerAnnouncement();
    enablePlugins(PluginLoadOrder::PostWorld);
    ServerLoadEvent event{ServerLoadEvent::LoadType::Startup};
    getPluginManager().callEvent(event);
}

void EndstoneServer::initRegistries()
{
    registries_[typeid(Ability)] = EndstoneRegistry<Ability, std::string>::create();
    registries_[typeid(ActorType)] = EndstoneRegistry<ActorType, std::string>::create();
    registries_[typeid(Biome)] = EndstoneRegistry<Biome, ::Biome>::create();
    registries_[typeid(BlockType)] = EndstoneRegistry<BlockType, ::BlockType>::create();
    registries_[typeid(EffectType)] = EndstoneRegistry<EffectType, ::MobEffect>::create();
    registries_[typeid(Enchantment)] = EndstoneRegistry<Enchantment, ::Enchant>::create();
    registries_[typeid(GameRule)] = EndstoneRegistry<GameRule, ::GameRule>::create();
    registries_[typeid(ItemType)] = EndstoneRegistry<ItemType, ::Item>::create();
    registries_[typeid(PotionType)] = EndstoneRegistry<PotionType, ::Potion>::create();
    BlockStateRegistry::get().unregisterBlockStates();
    ::BlockState::forEachState([](const auto &state) {
        BlockStateRegistry::get().registerBlockState(state);
        return true;
    });
}

void EndstoneServer::setResourcePackRepository(IResourcePackRepository &repo)
{
    if (resource_pack_repository_) {
        throw std::runtime_error("Resource pack repository already set.");
    }
    resource_pack_repository_ = &repo;
}

const std::string *EndstoneServer::getContentKey(const PackIdVersion &pack_id) const
{
    const auto it = content_keys_.find(pack_id);
    return it == content_keys_.end() ? nullptr : &it->second;
}

bool EndstoneServer::getAllowClientPacks() const
{
    return allow_client_packs_;
}

bool EndstoneServer::logCommands() const
{
    return log_commands_;
}

bool EndstoneServer::isServerTextEnabled(ServerTextEvent event) const
{
    return text_settings_.getEnabledServerTextEvents().test(
        static_cast<std::underlying_type_t<ServerTextEvent>>(event));
}

void EndstoneServer::loadResourcePacks()
{
    auto &repo = *resource_pack_repository_;
    const auto *manager = level_->getHandle().getClientResourcePackManager();
    auto &level_stack = const_cast<ResourcePackStack &>(manager->getStack(ResourcePackStackType::LEVEL));

    // Packs listed in world_resource_packs.json are already on the stack
    std::unordered_set<PackIdVersion> seen;
    for (const auto &pack_instance : level_stack.stack) {
        seen.insert(pack_instance.getManifest().getIdentity());
    }

    // Activate the archives dropped into resource_packs, which BDS discovers but leaves off the stack
    std::vector<PackInstanceId> pack_ids;
    for (const auto *pack : repo.getPacksByResourceLocation(PackOrigin::User)) {
        const auto &manifest = pack->getManifest();

        // Behavior packs live in the same origin, but belong to the server, not the client stack
        if (manifest.getPackType() != PackType::Resources) {
            continue;
        }

        // An extracted pack is left to world_resource_packs.json, as it is in vanilla
        if (!isArchivePack(manifest.getLocation())) {
            continue;
        }

        // Already activated, either by world_resource_packs.json or by a duplicate of this pack
        if (!seen.insert(manifest.getIdentity()).second) {
            continue;
        }

        if (auto key = readContentKey(manifest.getLocation(), getLogger()); !key.empty()) {
            content_keys_[manifest.getIdentity()] = std::move(key);
        }
        pack_ids.emplace_back(manifest.getIdentity(), std::string{});
    }

    std::vector<PackInstance> pack_instances;
    pack_instances.reserve(pack_ids.size());
    repo.getResourcePacksByPackId(pack_ids, pack_instances);

    for (auto &pack_instance : pack_instances) {
        const auto &manifest = pack_instance.getManifest();
        const bool encrypted = content_keys_.contains(manifest.getIdentity());
        getLogger().info("Loading resource pack {} v{}{}", manifest.getName(),
                         manifest.getIdentity().version.asString(),
                         encrypted ? ColorFormat::Green + " [encrypted]" : "");
        level_stack.stack.push_back(std::move(pack_instance));
    }
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
    return std::format("{}.{}", SharedConstants::MinorVersion, SharedConstants::PatchVersion);
}

int EndstoneServer::getProtocolVersion() const
{
    return SharedConstants::NetworkProtocolVersion;
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

PluginManager &EndstoneServer::getPluginManager() const
{
    return *plugin_manager_;
}

EndstonePluginManager &EndstoneServer::getEndstonePluginManager() const
{
    return *plugin_manager_;
}

Nullable<PluginCommand> EndstoneServer::getPluginCommand(std::string name) const
{
    if (auto command = command_map_->getCommand(name); command && command->is<PluginCommand>()) {
        return command.cast<PluginCommand>();
    }
    return nullptr;
}

NotNull<ConsoleCommandSender> EndstoneServer::getCommandSender() const
{
    return command_sender_;
}

bool EndstoneServer::dispatchCommand(const NotNull<CommandSender> &sender, std::string command_line) const
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
        MinecraftDefaultPermissions::registerCorePermissions();
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
        if (!plugin_manager_->getPermission(perm.getName())) {
            plugin_manager_->addPermission(std::make_shared<Permission>(perm));
        }
        else {
            getLogger().error("Plugin {} tried to register permission '{}' that was already registered.",
                              plugin.getDescription().getFullName(), perm.getName());
        }
    }
    plugin_manager_->dirtyPermissibles(PermissionLevel::Default);
    plugin_manager_->dirtyPermissibles(PermissionLevel::Operator);
    plugin_manager_->dirtyPermissibles(PermissionLevel::Console);
    plugin_manager_->enablePlugin(plugin);
}

void EndstoneServer::disablePlugins() const
{
    plugin_manager_->disablePlugins();
}

Scheduler &EndstoneServer::getScheduler() const
{
    return *scheduler_;
}

NotNull<MetricsBase> EndstoneServer::createMetrics(Plugin &plugin, int service_id)
{
    auto it = plugin_metrics_.find(service_id);
    if (it == plugin_metrics_.end()) {
        plugin_metrics_.emplace(service_id,
                                std::make_shared<Metrics>(*this, "endstone.metrics", "Metrics", &plugin, service_id));
    }
    return plugin_metrics_.at(service_id);
}

EndstoneScheduler &EndstoneServer::getEndstoneScheduler() const
{
    return *scheduler_;
}

Level &EndstoneServer::getLevel() const
{
    if (!level_) {
        throw std::runtime_error("Level has not been loaded yet.");
    }
    return *level_;
}

std::vector<NotNull<Recipe>> EndstoneServer::getRecipes() const
{
    return getLevel().getRecipes();
}

EndstoneLevel *EndstoneServer::getEndstoneLevel() const
{
    return level_.get();
}

std::vector<NotNull<Player>> EndstoneServer::getOnlinePlayers() const
{
    std::vector<NotNull<Player>> result;
    level_->getHandle().forEachPlayer([&](const ::Player &player) {
        result.emplace_back(player.getEndstoneActor<EndstonePlayer>());
        return true;
    });
    return result;
}

int EndstoneServer::getMaxPlayers() const
{
    return getServer().getMinecraft()->getServerNetworkHandler()->getMaxNumPlayers();
}

void EndstoneServer::setMaxPlayers(int max_players)
{
    Preconditions::checkArgument(max_players >= 0, "Max number of players must be >= 0");
    getServer().getMinecraft()->getServerNetworkHandler()->setMaxNumPlayers(max_players);
}

Nullable<Player> EndstoneServer::getPlayer(UUID id) const
{
    if (auto *player = level_->getHandle().getPlayer(EndstoneUUID::toMinecraft(id))) {
        return player->getEndstoneActor<EndstonePlayer>();
    }
    return nullptr;
}

Nullable<Player> EndstoneServer::getPlayer(std::string name) const
{
    Nullable<Player> result;
    level_->getHandle().forEachPlayer([&](const ::Player &player) {
        auto endstone_player = player.getEndstoneActor<EndstonePlayer>();
        if (boost::iequals(endstone_player->getName(), name)) {
            result = std::move(endstone_player);
            return false;  // found a match; stop iterating
        }
        return true;
    });
    return result;
}

int EndstoneServer::getPort() const
{
    return getRemoteConnector().getIPv4Port();
}

bool EndstoneServer::getOnlineMode() const
{
    return getServer().getMinecraft()->getServerNetworkHandler()->network_server_config_.require_trusted_authentication;
}

void EndstoneServer::shutdown()
{
    static_cast<EndstoneScheduler &>(getScheduler()).runTask([this]() {
        server_instance_->getMinecraft()->requestServerShutdown();
    });
}

void EndstoneServer::reload()
{
    command_map_->clearCommands();

    // Wait for at most 2.5 seconds for plugins to close their async tasks
    plugin_manager_->disablePlugins();
    auto &scheduler = static_cast<EndstoneScheduler &>(getScheduler());
    for (int poll_count = 0; poll_count < 50 && !scheduler.getActiveWorkers().empty(); ++poll_count) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    for (const auto &worker : scheduler.getActiveWorkers()) {
        const auto &description = worker.owner_->getDescription();
        getLogger().error("Nag author(s): '{}' of '{}' about the following: {}",
                          detail::join(description.getAuthors(), ", "), description.getFullName(),
                          "This plugin is not properly shutting down its async tasks when it is being reloaded. This "
                          "may cause conflicts with the newly loaded version of the plugin");
    }
    scheduler.removeCancelledTasks();

    plugin_metrics_.clear();
    plugin_manager_->clearPlugins();
    reloadData();
    loadPlugins();
    enablePlugins(PluginLoadOrder::Startup);
    enablePlugins(PluginLoadOrder::PostWorld);
    ServerLoadEvent event{ServerLoadEvent::LoadType::Reload};
    getPluginManager().callEvent(event);

    // sync commands
    for (const auto &player : getOnlinePlayers()) {
        player->updateCommands();
    }
}

void EndstoneServer::reloadData()
{
    server_instance_->onRequestResourceReload();
    level_->getHandle().loadFunctionManager();
    initRegistries();
}

void EndstoneServer::broadcast(const Message &message, const std::string &permission) const
{
    std::unordered_set<NotNull<CommandSender>> recipients;
    for (const auto &permissible : getPluginManager().getPermissionSubscriptions(permission)) {
        if (permissible->is<CommandSender>() && permissible->hasPermission(permission)) {
            recipients.insert(permissible.cast<CommandSender>());
        }
    }

    BroadcastMessageEvent event{!isPrimaryThread(), message, recipients};
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

void EndstoneServer::setMainThread(std::thread::id thread_id)
{
    mainThread() = thread_id;
}

bool EndstoneServer::isPrimaryThread() const
{
    const std::thread::id invalid;
    const auto main_thread = mainThread();
    if (main_thread == invalid) {
        return false;
    }
    const auto server_thread =
        server_instance_ != nullptr ? server_instance_->server_instance_thread_.get_id() : invalid;
    if (server_thread == invalid) {
        return std::this_thread::get_id() == main_thread;
    }
    return std::this_thread::get_id() == server_thread;
}

ItemFactory &EndstoneServer::getItemFactory() const
{
    return EndstoneItemFactory::instance();
}

Nullable<Scoreboard> EndstoneServer::getScoreboard() const
{
    return scoreboard_;
}

NotNull<Scoreboard> EndstoneServer::createScoreboard()
{
    auto registry = CommandSoftEnumRegistry();
    auto board = std::make_unique<ServerScoreboard>(registry, nullptr, level_->getHandle().getGameplayUserManager());
    board->setPacketSender(level_->getHandle().getPacketSender());
    return EndstoneScoreboard::create(std::move(board));
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

NotNull<BossBar> EndstoneServer::createBossBar(std::string title, BarColor color, BarStyle style)
{
    return createBossBar(std::move(title), color, style, {});
}

NotNull<BossBar> EndstoneServer::createBossBar(std::string title, BarColor color, BarStyle style,
                                               std::vector<BarFlag> flags)
{
    auto boss_bar = std::make_shared<EndstoneBossBar>(std::move(title), color, style, flags);
    boss_bars_.emplace_back(boss_bar);
    return boss_bar;
}

void EndstoneServer::updateBossBars(const NotNull<EndstonePlayer> &player)
{
    std::erase_if(boss_bars_, [&](const auto &boss_bar) {
        const auto bar = boss_bar.lock();
        if (!bar) {
            return true;
        }
        bar->update(player);
        return false;
    });
}

NotNull<BlockData> EndstoneServer::createBlockData(BlockTypeId type) const
{
    return createBlockData(type, {});
}

NotNull<BlockData> EndstoneServer::createBlockData(BlockTypeId type, BlockStates block_states) const
{
    std::unordered_map<std::string, std::variant<int, std::string, bool>> states;
    for (const auto &state : block_states) {
        std::visit(overloaded{[&](auto &&arg) { states.emplace(state.first, arg); }}, state.second);
    }
    const auto block_descriptor =
        ScriptModuleMinecraft::ScriptBlockUtils::createBlockDescriptor(std::string(type), states);
    const auto *block = block_descriptor.tryGetBlockNoLogging();
    Preconditions::checkArgument(block != nullptr, "Block type {} cannot be found in the registry.", type);
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

ServiceManager &EndstoneServer::getServiceManager() const
{
    return *service_manager_;
}

IRegistry *EndstoneServer::_getRegistry(ClassInfo type) const
{
    const auto it = registries_.find(type);
    if (registries_.end() == it) {
        return nullptr;
    }
    return it->second.get();
}

MapView *EndstoneServer::getMap(std::int64_t id) const
{
    const auto *saved_data = level_->getHandle().getMapSavedData(ActorUniqueID(id));
    if (!saved_data) {
        return nullptr;
    }
    return &saved_data->getMapView();
}

MapView &EndstoneServer::createMap(const NotNull<Dimension> &dimension) const
{
    auto &dim = dimension.cast<EndstoneDimension>()->getHandle();
    auto &level = dim.getLevel();
    // TODO: should we use dimension spawn point instead of BlockPos::ZERO?
    // creates a new map at world spawn with the scale of 3, without tracking position and unlimited tracking
    const auto &map = level.createMapSavedData(ActorUniqueID::INVALID_ID, BlockPos::ZERO, dim.getDimensionId(), 3);
    return map.getMapView();
}

NotNull<EndstoneScoreboard> EndstoneServer::getPlayerBoard(const NotNull<EndstonePlayer> &player) const
{
    auto it = player_boards_.find(player->getUniqueId());
    if (it == player_boards_.end()) {
        return scoreboard_;
    }
    return it->second;
}

void EndstoneServer::setPlayerBoard(const NotNull<EndstonePlayer> &player, NotNull<Scoreboard> scoreboard)
{
    auto &old_board = getPlayerBoard(player)->getHandle();
    auto &new_board = scoreboard.cast<EndstoneScoreboard>()->getHandle();

    if (&old_board == &new_board) {
        return;
    }

    // remove player from the old board
    getPlayerBoard(player)->resetScores(player->self());

    // add player to the new board
    new_board.onPlayerJoined(player->getHandle());

    // update tracking records
    if (scoreboard == scoreboard_) {
        player_boards_.erase(player->getUniqueId());
    }
    else {
        player_boards_.insert_or_assign(player->getUniqueId(), scoreboard.cast<EndstoneScoreboard>());
    }
}

void EndstoneServer::removePlayerBoard(const NotNull<EndstonePlayer> &player)
{
    player_boards_.erase(player->getUniqueId());
}

void EndstoneServer::tick(std::uint64_t current_tick, const std::function<void()> &tick_function)
{
    using std::chrono::milliseconds;
    using std::chrono::steady_clock;

    const auto start = steady_clock::now();
    // tick start
    scheduler_->mainThreadHeartbeat(current_tick);
    tick_function();
    for (const auto &p : getOnlinePlayers()) {
        p.cast<EndstonePlayer>()->checkOpStatus();
    }
    // tick end
    const auto end = steady_clock::now();

    current_mspt_ = static_cast<float>(duration_cast<milliseconds>(end - start).count());
    current_tps_ = std::min(1.0F * SharedConstants::TicksPerSecond, 1000.0F / std::max(1.0F, current_mspt_));
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

RemoteConnector &EndstoneServer::getRemoteConnector() const
{
    return *getServer().getMinecraft()->getServerNetworkHandler()->network_.getRemoteConnector();
}

RakNetConnector &EndstoneServer::getRakNetConnector() const
{
    return static_cast<RakNetConnector &>(getRemoteConnector());
}

EndstoneServer &EndstoneServer::getInstance()
{
    return entt::locator<EndstoneServer>::value();
}

}  // namespace endstone::core
