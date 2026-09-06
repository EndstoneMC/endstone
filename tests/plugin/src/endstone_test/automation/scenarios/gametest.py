from __future__ import annotations

from ..models import EventScenario


def build_gametest_scenarios(ctx, namespace) -> list[EventScenario]:
    run_gametest = namespace['run_gametest']
    verify_actor_explode = namespace['verify_actor_explode']
    verify_actor_knockback = namespace['verify_actor_knockback']
    verify_block_break = namespace['verify_block_break']
    verify_block_explode = namespace['verify_block_explode']
    verify_block_place = namespace['verify_block_place']
    verify_cauldron_level_change = namespace['verify_cauldron_level_change']
    verify_gametest_player_interact = namespace['verify_gametest_player_interact']
    verify_inventory_open_close = namespace['verify_inventory_open_close']
    verify_player_armor_stand_manipulate = namespace['verify_player_armor_stand_manipulate']
    verify_player_bucket = namespace['verify_player_bucket']
    verify_player_bucket_actor = namespace['verify_player_bucket_actor']
    verify_player_item_consume = namespace['verify_player_item_consume']
    verify_player_shear_actor = namespace['verify_player_shear_actor']
    return [
        EventScenario(
                    "gametest-actor-explode",
                    ("ActorExplodeEvent",),
                    lambda: run_gametest("actor_explode"),
                    timeout_ticks=160,
                    gametest_case="actor_explode",
                    verify=lambda snapshots: verify_actor_explode(snapshots, False),
                ),
        EventScenario(
                    "gametest-actor-explode-cancel",
                    ("ActorExplodeEvent",),
                    lambda: run_gametest("actor_explode_cancel"),
                    timeout_ticks=160,
                    gametest_case="actor_explode_cancel",
                    cancel_events=("ActorExplodeEvent",),
                    verify=lambda snapshots: verify_actor_explode(snapshots, True),
                ),
        EventScenario(
                    "gametest-actor-knockback",
                    ("ActorKnockbackEvent",),
                    lambda: run_gametest("actor_knockback"),
                    timeout_ticks=160,
                    gametest_case="actor_knockback",
                    verify=lambda snapshots: verify_actor_knockback(snapshots, False),
                ),
        EventScenario(
                    "gametest-actor-knockback-cancel",
                    ("ActorKnockbackEvent",),
                    lambda: run_gametest("actor_knockback_cancel"),
                    timeout_ticks=160,
                    gametest_case="actor_knockback_cancel",
                    cancel_events=("ActorKnockbackEvent",),
                    verify=lambda snapshots: verify_actor_knockback(snapshots, True),
                ),
        EventScenario(
                    "gametest-block-explode",
                    ("BlockExplodeEvent",),
                    lambda: run_gametest("block_explode"),
                    timeout_ticks=160,
                    gametest_case="block_explode",
                    verify=lambda snapshots: verify_block_explode(snapshots, False),
                ),
        EventScenario(
                    "gametest-block-explode-cancel",
                    ("BlockExplodeEvent",),
                    lambda: run_gametest("block_explode_cancel"),
                    timeout_ticks=160,
                    gametest_case="block_explode_cancel",
                    cancel_events=("BlockExplodeEvent",),
                    verify=lambda snapshots: verify_block_explode(snapshots, True),
                ),
        EventScenario(
                    "gametest-simulated-block-place",
                    ("BlockPlaceEvent",),
                    lambda: run_gametest("simulated_block_place"),
                    timeout_ticks=160,
                    gametest_case="simulated_block_place",
                    verify=lambda snapshots: verify_block_place(snapshots, False),
                ),
        EventScenario(
                    "gametest-simulated-block-place-cancel",
                    ("BlockPlaceEvent",),
                    lambda: run_gametest("simulated_block_place_cancel"),
                    timeout_ticks=160,
                    gametest_case="simulated_block_place_cancel",
                    cancel_events=("BlockPlaceEvent",),
                    verify=lambda snapshots: verify_block_place(
                        snapshots, True, "EndstoneSimPlaceCancel"
                    ),
                ),
        EventScenario(
                    "gametest-simulated-block-break",
                    ("BlockBreakEvent",),
                    lambda: run_gametest("simulated_block_break"),
                    timeout_ticks=160,
                    gametest_case="simulated_block_break",
                    verify=lambda snapshots: verify_block_break(snapshots, False),
                ),
        EventScenario(
                    "gametest-simulated-block-break-cancel",
                    ("BlockBreakEvent",),
                    lambda: run_gametest("simulated_block_break_cancel"),
                    timeout_ticks=160,
                    gametest_case="simulated_block_break_cancel",
                    cancel_events=("BlockBreakEvent",),
                    verify=lambda snapshots: verify_block_break(
                        snapshots, True, "EndstoneSimBreakCancel"
                    ),
                ),
        EventScenario(
                    "gametest-simulated-block-interact",
                    ("PlayerInteractEvent",),
                    lambda: run_gametest("simulated_block_interact"),
                    timeout_ticks=160,
                    gametest_case="simulated_block_interact",
                    verify=lambda snapshots: verify_gametest_player_interact(
                        snapshots, False
                    ),
                ),
        EventScenario(
                    "gametest-simulated-block-interact-cancel",
                    ("PlayerInteractEvent",),
                    lambda: run_gametest("simulated_block_interact_cancel"),
                    timeout_ticks=160,
                    gametest_case="simulated_block_interact_cancel",
                    cancel_events=("PlayerInteractEvent",),
                    verify=lambda snapshots: verify_gametest_player_interact(
                        snapshots, True, "EndstoneSimInteractCancel"
                    ),
                ),
        EventScenario(
                    "gametest-simulated-bucket-fill",
                    ("PlayerBucketFillEvent",),
                    lambda: run_gametest("simulated_bucket_fill"),
                    timeout_ticks=160,
                    gametest_case="simulated_bucket_fill",
                    verify=lambda snapshots: verify_player_bucket(
                        snapshots,
                        "PlayerBucketFillEvent",
                        False,
                        "EndstoneSimBucketFill",
                        "minecraft:bucket",
                        "minecraft:water",
                    ),
                ),
        EventScenario(
                    "gametest-simulated-bucket-fill-cancel",
                    ("PlayerBucketFillEvent",),
                    lambda: run_gametest("simulated_bucket_fill_cancel"),
                    timeout_ticks=160,
                    gametest_case="simulated_bucket_fill_cancel",
                    cancel_events=("PlayerBucketFillEvent",),
                    verify=lambda snapshots: verify_player_bucket(
                        snapshots,
                        "PlayerBucketFillEvent",
                        True,
                        "EndstoneSimBucketFillCancel",
                        "minecraft:bucket",
                        "minecraft:water",
                    ),
                ),
        EventScenario(
                    "gametest-simulated-bucket-empty",
                    ("PlayerBucketEmptyEvent",),
                    lambda: run_gametest("simulated_bucket_empty"),
                    timeout_ticks=160,
                    gametest_case="simulated_bucket_empty",
                    verify=lambda snapshots: verify_player_bucket(
                        snapshots,
                        "PlayerBucketEmptyEvent",
                        False,
                        "EndstoneSimBucketEmpty",
                        "minecraft:water_bucket",
                        "minecraft:stone",
                    ),
                ),
        EventScenario(
                    "gametest-simulated-bucket-empty-cancel",
                    ("PlayerBucketEmptyEvent",),
                    lambda: run_gametest("simulated_bucket_empty_cancel"),
                    timeout_ticks=160,
                    gametest_case="simulated_bucket_empty_cancel",
                    cancel_events=("PlayerBucketEmptyEvent",),
                    verify=lambda snapshots: verify_player_bucket(
                        snapshots,
                        "PlayerBucketEmptyEvent",
                        True,
                        "EndstoneSimBucketEmptyCancel",
                        "minecraft:water_bucket",
                        "minecraft:stone",
                    ),
                ),
        EventScenario(
                    "gametest-simulated-cauldron-bucket-empty",
                    ("PlayerBucketEmptyEvent", "CauldronLevelChangeEvent"),
                    lambda: run_gametest("simulated_cauldron_bucket_empty"),
                    timeout_ticks=160,
                    gametest_case="simulated_cauldron_bucket_empty",
                    verify=lambda snapshots: (
                        verify_player_bucket(
                            snapshots,
                            "PlayerBucketEmptyEvent",
                            False,
                            "EndstoneSimCauldron",
                            "minecraft:water_bucket",
                            "minecraft:cauldron",
                        ),
                        verify_cauldron_level_change(snapshots, False),
                    ),
                ),
        EventScenario(
                    "gametest-simulated-cauldron-bucket-empty-cancel",
                    ("PlayerBucketEmptyEvent", "CauldronLevelChangeEvent"),
                    lambda: run_gametest("simulated_cauldron_bucket_empty_cancel"),
                    timeout_ticks=160,
                    gametest_case="simulated_cauldron_bucket_empty_cancel",
                    cancel_events=("CauldronLevelChangeEvent",),
                    verify=lambda snapshots: (
                        verify_player_bucket(
                            snapshots,
                            "PlayerBucketEmptyEvent",
                            False,
                            "EndstoneSimCauldronCancel",
                            "minecraft:water_bucket",
                            "minecraft:cauldron",
                        ),
                        verify_cauldron_level_change(snapshots, True),
                    ),
                ),
        EventScenario(
                    "gametest-simulated-inventory-open",
                    ("InventoryOpenEvent",),
                    lambda: run_gametest("simulated_inventory_open"),
                    timeout_ticks=160,
                    gametest_case="simulated_inventory_open",
                    verify=lambda snapshots: verify_inventory_open_close(
                        snapshots, False, expect_close=False
                    ),
                ),
        EventScenario(
                    "gametest-simulated-inventory-open-cancel",
                    ("InventoryOpenEvent",),
                    lambda: run_gametest("simulated_inventory_open_cancel"),
                    timeout_ticks=160,
                    gametest_case="simulated_inventory_open_cancel",
                    cancel_events=("InventoryOpenEvent",),
                    verify=lambda snapshots: verify_inventory_open_close(
                        snapshots, True, "EndstoneSimContainerCancel"
                    ),
                ),
        EventScenario(
                    "gametest-simulated-bucket-actor",
                    ("PlayerBucketActorEvent",),
                    lambda: run_gametest("simulated_bucket_actor"),
                    timeout_ticks=160,
                    gametest_case="simulated_bucket_actor",
                    verify=lambda snapshots: verify_player_bucket_actor(snapshots, False),
                ),
        EventScenario(
                    "gametest-simulated-bucket-actor-cancel",
                    ("PlayerBucketActorEvent",),
                    lambda: run_gametest("simulated_bucket_actor_cancel"),
                    timeout_ticks=160,
                    gametest_case="simulated_bucket_actor_cancel",
                    cancel_events=("PlayerBucketActorEvent",),
                    verify=lambda snapshots: verify_player_bucket_actor(
                        snapshots, True, "EndstoneSimBucketCancel"
                    ),
                ),
        EventScenario(
                    "gametest-simulated-armor-stand-manipulate",
                    ("PlayerArmorStandManipulateEvent",),
                    lambda: run_gametest("simulated_armor_stand_manipulate"),
                    timeout_ticks=160,
                    gametest_case="simulated_armor_stand_manipulate",
                    verify=lambda snapshots: verify_player_armor_stand_manipulate(
                        snapshots, False
                    ),
                ),
        EventScenario(
                    "gametest-simulated-armor-stand-manipulate-cancel",
                    ("PlayerArmorStandManipulateEvent",),
                    lambda: run_gametest("simulated_armor_stand_manipulate_cancel"),
                    timeout_ticks=160,
                    gametest_case="simulated_armor_stand_manipulate_cancel",
                    cancel_events=("PlayerArmorStandManipulateEvent",),
                    verify=lambda snapshots: verify_player_armor_stand_manipulate(
                        snapshots, True, "EndstoneSimArmorCancel"
                    ),
                ),
        EventScenario(
                    "gametest-simulated-shear-actor",
                    ("PlayerShearActorEvent",),
                    lambda: run_gametest("simulated_shear_actor"),
                    timeout_ticks=160,
                    gametest_case="simulated_shear_actor",
                    verify=lambda snapshots: verify_player_shear_actor(snapshots, False),
                ),
        EventScenario(
                    "gametest-simulated-shear-actor-cancel",
                    ("PlayerShearActorEvent",),
                    lambda: run_gametest("simulated_shear_actor_cancel"),
                    timeout_ticks=160,
                    gametest_case="simulated_shear_actor_cancel",
                    cancel_events=("PlayerShearActorEvent",),
                    isolated=True,
                    verify=lambda snapshots: verify_player_shear_actor(
                        snapshots, True, "EndstoneSimShearCancel"
                    ),
                ),
        EventScenario(
                    "gametest-simulated-item-consume",
                    ("PlayerItemConsumeEvent",),
                    lambda: run_gametest("simulated_item_consume"),
                    timeout_ticks=160,
                    gametest_case="simulated_item_consume",
                    verify=lambda snapshots: verify_player_item_consume(
                        snapshots, False, 1, "EndstoneSimConsume"
                    ),
                ),
        EventScenario(
                    "gametest-simulated-item-consume-cancel",
                    ("PlayerItemConsumeEvent",),
                    lambda: run_gametest("simulated_item_consume_cancel"),
                    timeout_ticks=160,
                    gametest_case="simulated_item_consume_cancel",
                    cancel_events=("PlayerItemConsumeEvent",),
                    verify=lambda snapshots: verify_player_item_consume(
                        snapshots, True, 1, "EndstoneSimConsumeCancel"
                    ),
                ),
    ]
