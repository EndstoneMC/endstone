from __future__ import annotations


EVENT_MANIFEST = {
    "version": 2,
    "server_automatable": (
        "ActorCollideWithActorEvent",
        "ActorDamageEvent",
        "ActorDeathEvent",
        "ActorDismountEvent",
        "ActorEffectEvent",
        "ActorRemoveEvent",
        "ActorSpawnEvent",
        "ActorTeleportEvent",
        "ActorPickupItemEvent",
        "BlockCookEvent",
        "BlockFormEvent",
        "BlockFromToEvent",
        "BlockPistonExtendEvent",
        "BlockPistonRetractEvent",
        "ChunkLoadEvent",
        "ChunkUnloadEvent",
        "LeavesDecayEvent",
        "PlayerDeathEvent",
        "PlayerDimensionChangeEvent",
        "PlayerExpChangeEvent",
        "PlayerGameModeChangeEvent",
        "PlayerLevelChangeEvent",
        "PlayerPickupExperienceEvent",
        "PlayerPickupItemEvent",
        "PlayerPickupArrowEvent",
        "PlayerSetSpawnEvent",
        "PlayerTeleportEvent",
        "PlayerOpenSignEvent",
        "PlayerKickEvent",
        "PlayerQuitEvent",
        "PlayerHideActorEvent",
        "PlayerShowActorEvent",
        "ServerCommandEvent",
        "UnknownCommandEvent",
        "ScriptMessageEvent",
        "BroadcastMessageEvent",
        "ThunderChangeEvent",
        "WeatherChangeEvent",
        "DimensionLoadEvent",
        "MapInitializeEvent",
        "PluginEnableEvent",
        "PluginDisableEvent",
        "ServerLoadEvent",
    ),
    # These actions are emitted by the server's world simulation or by a
    # native simulated player. They remain a separate category so a
    # protocol-only run cannot claim them as covered.
    "gametest_required": (
        "ActorExplodeEvent",
        "ActorKnockbackEvent",
        "BlockExplodeEvent",
        "BlockBreakEvent",
        "BlockPlaceEvent",
        "InventoryOpenEvent",
        "PlayerBucketActorEvent",
        "PlayerBucketFillEvent",
        "PlayerBucketEmptyEvent",
        "PlayerShearActorEvent",
        "PlayerItemConsumeEvent",
        "CauldronLevelChangeEvent",
        "PlayerInteractEvent",
        "PlayerInteractActorEvent",
        "PlayerArmorStandManipulateEvent",
    ),
    "protocol_driver_required": (
        "PlayerLoginEvent",
        "PlayerJoinEvent",
        "PlayerSkinChangeEvent",
        "PlayerArmSwingEvent",
        "PlayerBedEnterEvent",
        "PlayerBedLeaveEvent",
        "PlayerChatEvent",
        "PlayerCommandEvent",
        "PlayerCraftItemEvent",
        "PlayerDropItemEvent",
        "PlayerEditBookEvent",
        "PlayerEmoteEvent",
        "PlayerInputEvent",
        "ActorToggleGlideEvent",
        "ActorToggleSwimEvent",
        "PlayerItemHeldEvent",
        "PlayerJumpEvent",
        "PlayerMoveEvent",
        "PlayerRecipeBookSettingsChangeEvent",
        "PlayerRespawnEvent",
        "PlayerToggleCrawlEvent",
        "PlayerToggleFlightEvent",
        "PlayerToggleSneakEvent",
        "PlayerToggleSprintEvent",
        "ServerListPingEvent",
        "PacketReceiveEvent",
        "PacketSendEvent",
    ),
    "no_fire_site_or_unsupported": {
        "ActorChangeBlockEvent": "requires nondeterministic mob griefing AI",
        "BlockGrowEvent": (
            "the current runtime has no verified ABI hook or native dispatch for sapling/crop growth"
        ),
        "InventoryInteractEvent": "no C++ dispatch site in the current runtime",
        "InventoryCloseEvent": (
            "GameTest simulated players do not publish PlayerClosedContainerEvent when their "
            "interaction ends, and the current protocol bot cannot open a server-authoritative "
            "container without a real client"
        ),
        "EnchantItemEvent": (
            "requires a real Bedrock container UI selection; the exact protocol driver cannot open "
            "the server-authoritative enchanting table and GameTest has no matching API"
        ),
        "PlayerPortalEvent": (
            "the actual portal contact path uses Level::requestPlayerChangeDimension; "
            "the current hook does not dispatch PlayerPortalEvent there"
        ),
        "PlayerRiptideEvent": (
            "requires a real Bedrock client: the protocol driver cannot reproduce the native "
            "Riptide spin-attack lifecycle without a test-only runtime workaround"
        ),
    },
}


# Keep scenarios that need unavailable client behavior out of automated runs.
EXCLUDED_AUTOMATED_SCENARIOS = frozenset({
    "block-grow",
    "block-grow-cancel",
    "player-riptide",
})

