import lazy_loader as lazy

from endstone._python.event import EventPriority


def event_handler(func=None, *, priority: EventPriority = EventPriority.NORMAL, ignore_cancelled: bool = False):
    def decorator(f):
        setattr(f, "_is_event_handler", True)
        setattr(f, "_priority", priority)
        setattr(f, "_ignore_cancelled", ignore_cancelled)
        return f

    if func:
        return decorator(func)

    return decorator


__getattr__, __dir__, __all__ = lazy.attach(
    "endstone._python",
    submod_attrs={
        "event": [
            "ActorDamageEvent",
            "ActorDeathEvent",
            "ActorEvent",
            "ActorExplodeEvent",
            "ActorKnockbackEvent",
            "ActorRemoveEvent",
            "ActorSpawnEvent",
            "ActorTeleportEvent",
            "BlockBreakEvent",
            "BlockCookEvent",
            "BlockEvent",
            "BlockExplodeEvent",
            "BlockFormEvent",
            "BlockFromToEvent",
            "BlockGrowEvent",
            "BlockPistonEvent",
            "BlockPistonExtendEvent",
            "BlockPistonRetractEvent",
            "BlockPlaceEvent",
            "BroadcastMessageEvent",
            "Cancellable",
            "ChunkEvent",
            "ChunkLoadEvent",
            "ChunkUnloadEvent",
            "DimensionEvent",
            "Event",
            "EventResult",
            "LeavesDecayEvent",
            "LevelEvent",
            "MapInitializeEvent",
            "MobEvent",
            "PacketReceiveEvent",
            "PacketSendEvent",
            "PlayerBedEnterEvent",
            "PlayerBedLeaveEvent",
            "PlayerChatEvent",
            "PlayerCommandEvent",
            "PlayerDeathEvent",
            "PlayerDimensionChangeEvent",
            "PlayerDropItemEvent",
            "PlayerEmoteEvent",
            "PlayerEvent",
            "PlayerGameModeChangeEvent",
            "PlayerInteractActorEvent",
            "PlayerInteractEvent",
            "PlayerItemConsumeEvent",
            "PlayerItemHeldEvent",
            "PlayerJoinEvent",
            "PlayerJumpEvent",
            "PlayerKickEvent",
            "PlayerLoginEvent",
            "PlayerMoveEvent",
            "PlayerPickupItemEvent",
            "PlayerPortalEvent",
            "PlayerQuitEvent",
            "PlayerRespawnEvent",
            "PlayerSkinChangeEvent",
            "PlayerTeleportEvent",
            "PluginDisableEvent",
            "PluginEnableEvent",
            "ScriptMessageEvent",
            "ServerCommandEvent",
            "ServerEvent",
            "ServerListPingEvent",
            "ServerLoadEvent",
            "ThunderChangeEvent",
            "WeatherChangeEvent",
            "WeatherEvent",
        ],
    },
)

__all__.extend(["EventPriority", "event_handler"])
