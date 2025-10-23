#include "bedrock/world/level/map_data_manager.h"

#include "endstone/core/map/map_view.h"
#include "endstone/core/server.h"
#include "endstone/event/server/map_initialize_event.h"
#include "endstone/runtime/hook.h"

MapItemSavedData *MapDataManager::_loadMapData(const ActorUniqueID &uuid)
{
    auto *map_data = ENDSTONE_HOOK_CALL_ORIGINAL(&MapDataManager::_loadMapData, this, uuid);
    if (map_data) {
        endstone::MapInitializeEvent e{map_data->getMapView()};
        endstone::core::EndstoneServer::getInstance().getPluginManager().callEvent(e);
    }
    return map_data;
}
