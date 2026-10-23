#pragma once

#include "bedrock/bedrock.h"
#include "bedrock/world/level/pathfinder/path.h"

class Actor;

class PathFinder {
public:
    ENDSTONE_HOOK_LINUX std::unique_ptr<Path> findPath(Actor &actor, int x, int y, int z, float maximum_distance);
};
