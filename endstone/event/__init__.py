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


class Cancellable:
    """
    Represents an event that may be cancelled by a plugin or the server.
    """

    @property
    def is_cancelled(self) -> bool:
        """
        Gets or sets the cancellation state of this event.

        A cancelled event will not be executed in the server, but will still pass to other plugins.
        """
        return getattr(self, "_cancelled", False)

    @is_cancelled.setter
    def is_cancelled(self, arg1: bool) -> None:  # noqa
        setattr(self, "_cancelled", arg1)

    def cancel(self) -> None:
        """
        Cancel this event.

        A cancelled event will not be executed in the server, but will still pass to other plugins.
        """
        self.is_cancelled = True


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

__all__.extend(["Cancellable", "EventPriority", "event_handler"])
