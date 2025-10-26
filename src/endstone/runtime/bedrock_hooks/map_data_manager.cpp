#include "bedrock/world/level/map_data_manager.h"

#include "endstone/core/map/map_view.h"
#include "endstone/core/server.h"
#include "endstone/event/server/map_initialize_event.h"
#include "endstone/runtime/hook.h"

MapItemSavedData *MapDataManager::_loadMapData(const ActorUniqueID &uuid)
{
    auto *map_data = ENDSTONE_HOOK_CALL_ORIGINAL(&MapDataManager::_loadMapData, this, uuid);
    if (map_data) {
        auto &server = endstone::core::EndstoneServer::getInstance();
        auto &map = map_data->getMapView();
        // Map data may be loaded from a non-server thread (e.g., during chunk loading).
        // Schedule the event to run on the main server thread.
        server.getEndstoneScheduler().runTask([&]() {
            endstone::MapInitializeEvent e{map};
            server.getPluginManager().callEvent(e);
        });
    }
    return map_data;
}
