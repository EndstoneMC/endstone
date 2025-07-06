#pragma once
#include <functional>
#include <string>
#include <unordered_set>

#include <gsl/gsl>

#include "bedrock/forward.h"

class Actor;
class ActorSpawnRuleGroup;
class Biome;
class BlockSource;
class ChunkPos;
class IRandom;
class IWorldRegistriesProvider;
class ItemActor;
class ItemStack;
class LevelChunkVolumeData;
class Mob;
class ResourcePackManager;
class SpawnGroupRegistry;
class Vec3;
struct ActorDefinitionIdentifier;
struct ActorUniqueID;
struct SpawnSettings;
namespace br::spawn {
class EntityTypeCache;
class SpawnPlacements;
struct State;
}  // namespace br::spawn
namespace SpawnCategory {
enum class Type : int {
};
}  // namespace SpawnCategory

class Spawner {
public:
    virtual ~Spawner();
    virtual void initializeServerSide(::ResourcePackManager &, IWorldRegistriesProvider &) = 0;
    [[nodiscard]] virtual SpawnSettings const &getSpawnSettings() const = 0;
    virtual void setSpawnSettings(::SpawnSettings const &) = 0;
    [[nodiscard]] virtual ActorSpawnRuleGroup const *getSpawnRules() const = 0;
    [[nodiscard]] virtual ActorSpawnRuleGroup *getSpawnRulesMutable() const = 0;
    [[nodiscard]] virtual SpawnGroupRegistry const *getSpawnGroupRegistry() const = 0;
    [[nodiscard]] virtual br::spawn::EntityTypeCache *getEntityTypeCache() const = 0;
    virtual Mob *spawnMob(BlockSource &region, ActorDefinitionIdentifier const &id, Actor *spawner, Vec3 const &pos,
                          bool natural_spawn, bool surface, bool from_spawner);
    virtual ItemActor *spawnItem(BlockSource &region, ItemStack const &inst, Actor *spawner, Vec3 const &pos,
                                 int throw_time);
    virtual Actor *spawnProjectile(BlockSource &region, ActorDefinitionIdentifier const &id, Actor *spawner,
                                   Vec3 const &position, Vec3 const &direction);
    virtual void postProcessSpawnMobs(BlockSource &, int, int, Random &) = 0;
    virtual void tick(BlockSource &, LevelChunkVolumeData const &,
                      ChunkPos const /*NOLINT(readability-avoid-const-params-in-decls)*/) = 0;
    virtual void tickMobCount() = 0;
    virtual void incrementSpawnableTickedMob() = 0;
    [[nodiscard]] virtual int getSpawnableTickedMobCountPrevious() const = 0;
    virtual std::unordered_set<::ActorUniqueID> spawnMobGroup(BlockSource &, std::string const &, Vec3 const &, bool,
                                                              bool, std::function<void(::Mob &)> &&) = 0;
    virtual br::spawn::SpawnPlacements &getSpawnPlacements() = 0;
    [[nodiscard]] virtual std::tuple<std::array<SpawnCategory::Type, 8>, uint64_t> filteredSpawningCategories(
        br::spawn::State const &, bool, bool, bool) const;
    virtual void spawnForChunk(BlockSource &, LevelChunkVolumeData const &, br::spawn::State &,
                               gsl::span<::SpawnCategory::Type>, IRandom &) const;
    virtual void spawnMobsForChunkGeneration(BlockSource &, Biome const &, ChunkPos, IRandom &) const;
};
