import collections.abc

import lazy_loader as lazy

from endstone._python.event import EventPriority


def event_handler(
    func=None,
    *,
    priority: EventPriority = EventPriority.NORMAL,
    ignore_cancelled: bool = False,
    packet_ids: collections.abc.Iterable[int] | None = None,
):
    if packet_ids is not None:
        packet_ids = tuple(packet_ids)
        if any(type(packet_id) is not int or packet_id < 0 for packet_id in packet_ids):
            raise ValueError("packet_ids must contain non-negative integers")

    def decorator(f):
        f._is_event_handler = True
        f._priority = priority
        f._ignore_cancelled = ignore_cancelled
        f._packet_ids = packet_ids
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
