#include "bedrock/world/level/map_data_manager.h"

#include "endstone/core/map/map_view.h"
#include "endstone/core/server.h"
#include "endstone/event/server/map_initialize_event.h"
#include "endstone/runtime/hook.h"

MapItemSavedData *MapDataManager::_publishMapData(const ActorUniqueID &uuid,
                                                  std::unique_ptr<MapItemSavedData> loaded_map)
{
    auto *map_data = ENDSTONE_HOOK_CALL_ORIGINAL(&MapDataManager::_publishMapData, this, uuid, std::move(loaded_map));
    if (map_data) {
        auto &server = endstone::core::EndstoneServer::getInstance();
        server.getEndstoneScheduler().runTask([&server, id = uuid.raw_id]() {
            if (auto *map = server.getMap(id)) {
                endstone::MapInitializeEvent e{*map};
                server.getPluginManager().callEvent(e);
            }
        });
    }
    return map_data;
}
