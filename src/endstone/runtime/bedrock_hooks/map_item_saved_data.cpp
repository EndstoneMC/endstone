#include "bedrock/world/level/map_data_manager.h"
#include "endstone/core/map/map_view.h"
#include "endstone/core/server.h"
#include "endstone/event/server/map_initialize_event.h"
#include "endstone/runtime/hook.h"

void MapItemSavedData::_deserializeData(const CompoundTag &tag)
{
    endstone::MapInitializeEvent e{getMapView()};
    endstone::core::EndstoneServer::getInstance().getPluginManager().callEvent(e);
    ENDSTONE_HOOK_CALL_ORIGINAL(&MapItemSavedData::_deserializeData, this, tag);
}
