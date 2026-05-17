#include "bedrock/world/level/map_data_manager.h"

#include "endstone/core/map/map_view.h"
#include "endstone/core/server.h"
#include "endstone/event/server/map_initialize_event.h"
#include "endstone/runtime/hook.h"

// MapInitializeEvent fires when a map's data is first loaded from storage. The
// hook target differs by platform: on Windows that load is _loadMapData; on
// Linux _loadMapData is inlined into getMapSavedData, so the hook moves there
// and is gated -- getMapSavedData also returns already-loaded maps, so the
// event fires only on an actual load (uuid absent from the cache before the
// call, call returned non-null). The load may run off the main thread (e.g.
// during chunk loading), so the event is scheduled onto the server thread.
#ifdef _WIN32
MapItemSavedData *MapDataManager::_loadMapData(const ActorUniqueID &uuid)
{
    auto *map_data = ENDSTONE_HOOK_CALL_ORIGINAL(&MapDataManager::_loadMapData, this, uuid);
    if (map_data) {
        auto &server = endstone::core::EndstoneServer::getInstance();
        auto &map = map_data->getMapView();
        server.getEndstoneScheduler().runTask([&]() {
            endstone::MapInitializeEvent e{map};
            server.getPluginManager().callEvent(e);
        });
    }
    return map_data;
}
#elif __linux__
MapItemSavedData *MapDataManager::getMapSavedData(const ActorUniqueID uuid)
{
    const bool loaded = getMapDataMap().contains(uuid);
    auto *map_data = ENDSTONE_HOOK_CALL_ORIGINAL(&MapDataManager::getMapSavedData, this, uuid);
    if (map_data && !loaded) {
        auto &server = endstone::core::EndstoneServer::getInstance();
        auto &map = map_data->getMapView();
        server.getEndstoneScheduler().runTask([&]() {
            endstone::MapInitializeEvent e{map};
            server.getPluginManager().callEvent(e);
        });
    }
    return map_data;
}
#endif
