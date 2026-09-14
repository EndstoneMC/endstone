from __future__ import annotations

from ..models import EventScenario


def build_player_scenarios(ctx, namespace) -> list[EventScenario]:
    allow_player_flight = namespace['allow_player_flight']
    armor_stand_driver_data = namespace['armor_stand_driver_data']
    book_driver_data = namespace['book_driver_data']
    change_player_dimension = namespace['change_player_dimension']
    change_player_game_mode = namespace['change_player_game_mode']
    craft_driver_data = namespace['craft_driver_data']
    drop_item_for_player = namespace['drop_item_for_player']
    give_player_exp = namespace['give_player_exp']
    give_player_exp_level = namespace['give_player_exp_level']
    kick_player = namespace['kick_player']
    kill_player = namespace['kill_player']
    prepare_arm_swing = namespace['prepare_arm_swing']
    prepare_armor_stand_interaction = namespace['prepare_armor_stand_interaction']
    prepare_book = namespace['prepare_book']
    prepare_craft_item = namespace['prepare_craft_item']
    prepare_drop_item = namespace['prepare_drop_item']
    prepare_hotbar_item = namespace['prepare_hotbar_item']
    prepare_jump = namespace['prepare_jump']
    prepare_pickup_arrow = namespace['prepare_pickup_arrow']
    prepare_player_flight_stop = namespace['prepare_player_flight_stop']
    prepare_player_move = namespace['prepare_player_move']
    prepare_player_sneak_stop = namespace['prepare_player_sneak_stop']
    prepare_player_sprint_stop = namespace['prepare_player_sprint_stop']
    prepare_riptide = namespace['prepare_riptide']
    prepare_sign = namespace['prepare_sign']
    restore_item_held_logger = namespace['restore_item_held_logger']
    run_gametest = namespace['run_gametest']
    set_player_spawn = namespace['set_player_spawn']
    spawn_experience_orb = namespace['spawn_experience_orb']
    teleport_player = namespace['teleport_player']
    verify_player_arm_swing = namespace['verify_player_arm_swing']
    verify_player_bed = namespace['verify_player_bed']
    verify_player_chat = namespace['verify_player_chat']
    verify_player_command = namespace['verify_player_command']
    verify_player_craft_item = namespace['verify_player_craft_item']
    verify_player_death_respawn = namespace['verify_player_death_respawn']
    verify_player_dimension_change = namespace['verify_player_dimension_change']
    verify_player_drop_item = namespace['verify_player_drop_item']
    verify_player_edit_book = namespace['verify_player_edit_book']
    verify_player_emote = namespace['verify_player_emote']
    verify_player_exp_change = namespace['verify_player_exp_change']
    verify_player_game_mode_change = namespace['verify_player_game_mode_change']
    verify_player_input = namespace['verify_player_input']
    verify_player_interact_actor = namespace['verify_player_interact_actor']
    verify_player_item_held = namespace['verify_player_item_held']
    verify_player_jump = namespace['verify_player_jump']
    verify_player_kick_cancel = namespace['verify_player_kick_cancel']
    verify_player_kick_quit = namespace['verify_player_kick_quit']
    verify_player_level_change = namespace['verify_player_level_change']
    verify_player_move = namespace['verify_player_move']
    verify_player_open_sign = namespace['verify_player_open_sign']
    verify_player_pickup_arrow = namespace['verify_player_pickup_arrow']
    verify_player_pickup_experience = namespace['verify_player_pickup_experience']
    verify_player_pickup_item = namespace['verify_player_pickup_item']
    verify_player_recipe_book = namespace['verify_player_recipe_book']
    verify_player_riptide = namespace['verify_player_riptide']
    verify_player_set_spawn = namespace['verify_player_set_spawn']
    verify_player_teleport = namespace['verify_player_teleport']
    verify_player_toggle_flight = namespace['verify_player_toggle_flight']
    verify_toggle_event = namespace['verify_toggle_event']
    return [
        EventScenario(
                    "player-move",
                    ("PlayerMoveEvent",),
                    prepare_player_move,
                    driver_action="move",
                    requires_player=True,
                    verify=lambda snapshots: verify_player_move(snapshots, False),
                ),
        EventScenario(
                    "player-move-cancel",
                    ("PlayerMoveEvent",),
                    prepare_player_move,
                    driver_action="move",
                    requires_player=True,
                    cancel_events=("PlayerMoveEvent",),
                    verify=lambda snapshots: verify_player_move(snapshots, True),
                ),
        EventScenario(
                    "player-pickup-arrow",
                    ("PlayerPickupArrowEvent",),
                    prepare_pickup_arrow,
                    timeout_ticks=120,
                    requires_player=True,
                    verify=lambda snapshots: verify_player_pickup_arrow(snapshots, False),
                ),
        EventScenario(
                    "player-pickup-arrow-cancel",
                    ("PlayerPickupArrowEvent",),
                    prepare_pickup_arrow,
                    timeout_ticks=120,
                    requires_player=True,
                    cancel_events=("PlayerPickupArrowEvent",),
                    verify=lambda snapshots: verify_player_pickup_arrow(snapshots, True),
                ),
        EventScenario(
                    "player-set-spawn",
                    ("PlayerSetSpawnEvent",),
                    set_player_spawn,
                    timeout_ticks=120,
                    requires_player=True,
                    verify=lambda snapshots: verify_player_set_spawn(snapshots, False),
                ),
        EventScenario(
                    "player-set-spawn-cancel",
                    ("PlayerSetSpawnEvent",),
                    set_player_spawn,
                    timeout_ticks=120,
                    requires_player=True,
                    cancel_events=("PlayerSetSpawnEvent",),
                    verify=lambda snapshots: verify_player_set_spawn(snapshots, True),
                ),
        EventScenario(
                    "player-bed-enter-leave",
                    ("PlayerBedEnterEvent", "PlayerBedLeaveEvent"),
                    lambda: run_gametest("simulated_bed_enter"),
                    timeout_ticks=160,
                    gametest_case="simulated_bed_enter",
                    verify=lambda snapshots: verify_player_bed(
                        snapshots, False, "EndstoneSimBed"
                    ),
                ),
        EventScenario(
                    "player-bed-enter-cancel",
                    ("PlayerBedEnterEvent",),
                    lambda: run_gametest("simulated_bed_enter_cancel"),
                    timeout_ticks=160,
                    gametest_case="simulated_bed_enter_cancel",
                    cancel_events=("PlayerBedEnterEvent",),
                    verify=lambda snapshots: verify_player_bed(
                        snapshots, True, "EndstoneSimBedCancel"
                    ),
                ),
        EventScenario(
                    "player-chat",
                    ("PlayerChatEvent",),
                    lambda: None,
                    driver_action="chat",
                    requires_player=True,
                    verify=lambda snapshots: verify_player_chat(snapshots, False),
                ),
        EventScenario(
                    "player-chat-cancel",
                    ("PlayerChatEvent",),
                    lambda: None,
                    driver_action="chat",
                    requires_player=True,
                    cancel_events=("PlayerChatEvent",),
                    verify=lambda snapshots: verify_player_chat(snapshots, True),
                ),
        EventScenario(
                    "player-command",
                    ("PlayerCommandEvent",),
                    lambda: None,
                    driver_action="command",
                    requires_player=True,
                    verify=lambda snapshots: verify_player_command(snapshots, False),
                ),
        EventScenario(
                    "player-command-cancel",
                    ("PlayerCommandEvent",),
                    lambda: None,
                    driver_action="command",
                    requires_player=True,
                    cancel_events=("PlayerCommandEvent",),
                    verify=lambda snapshots: verify_player_command(snapshots, True),
                ),
        EventScenario(
                    "player-arm-swing",
                    ("PlayerArmSwingEvent",),
                    prepare_arm_swing,
                    driver_action="arm-swing",
                    requires_player=True,
                    verify=lambda snapshots: verify_player_arm_swing(snapshots, False),
                ),
        EventScenario(
                    "player-arm-swing-cancel",
                    ("PlayerArmSwingEvent",),
                    prepare_arm_swing,
                    driver_action="arm-swing",
                    requires_player=True,
                    cancel_events=("PlayerArmSwingEvent",),
                    verify=lambda snapshots: verify_player_arm_swing(snapshots, True),
                ),
        EventScenario(
                    "player-input",
                    ("PlayerInputEvent",),
                    lambda: None,
                    driver_action="input",
                    requires_player=True,
                    verify=verify_player_input,
                ),
        EventScenario(
                    "player-jump",
                    ("PlayerJumpEvent",),
                    prepare_jump,
                    driver_action="jump",
                    timeout_ticks=120,
                    requires_player=True,
                    verify=lambda snapshots: verify_player_jump(snapshots, False),
                ),
        EventScenario(
                    "player-jump-cancel",
                    ("PlayerJumpEvent",),
                    prepare_jump,
                    driver_action="jump",
                    timeout_ticks=120,
                    requires_player=True,
                    cancel_events=("PlayerJumpEvent",),
                    verify=lambda snapshots: verify_player_jump(snapshots, True),
                ),
        EventScenario(
                    "player-open-sign",
                    ("PlayerOpenSignEvent",),
                    prepare_sign,
                    timeout_ticks=180,
                    requires_player=True,
                    verify=lambda snapshots: verify_player_open_sign(snapshots, False),
                ),
        EventScenario(
                    "player-open-sign-cancel",
                    ("PlayerOpenSignEvent",),
                    prepare_sign,
                    timeout_ticks=180,
                    requires_player=True,
                    cancel_events=("PlayerOpenSignEvent",),
                    verify=lambda snapshots: verify_player_open_sign(snapshots, True),
                ),
        EventScenario(
                    "player-interact-actor",
                    ("PlayerInteractActorEvent",),
                    prepare_armor_stand_interaction,
                    timeout_ticks=200,
                    driver_action="interact-actor",
                    driver_data=armor_stand_driver_data,
                    requires_player=True,
                    verify=lambda snapshots: verify_player_interact_actor(
                        snapshots, False, expected_actor="minecraft:armor_stand"
                    ),
                ),
        EventScenario(
                    "player-interact-actor-cancel",
                    ("PlayerInteractActorEvent",),
                    prepare_armor_stand_interaction,
                    timeout_ticks=200,
                    driver_action="interact-actor",
                    driver_data=armor_stand_driver_data,
                    requires_player=True,
                    cancel_events=("PlayerInteractActorEvent",),
                    verify=lambda snapshots: verify_player_interact_actor(
                        snapshots, True, expected_actor="minecraft:armor_stand"
                    ),
                ),
        EventScenario(
                    "player-toggle-sneak-start",
                    ("PlayerToggleSneakEvent",),
                    lambda: None,
                    driver_action="toggle-sneak-start",
                    requires_player=True,
                    verify=lambda snapshots: verify_toggle_event(
                        snapshots, "PlayerToggleSneakEvent", "is_sneaking", True
                    ),
                ),
        EventScenario(
                    "player-toggle-sneak-stop",
                    ("PlayerToggleSneakEvent",),
                    prepare_player_sneak_stop,
                    driver_action="toggle-sneak-stop",
                    requires_player=True,
                    verify=lambda snapshots: verify_toggle_event(
                        snapshots, "PlayerToggleSneakEvent", "is_sneaking", False
                    ),
                ),
        EventScenario(
                    "player-toggle-sprint-start",
                    ("PlayerToggleSprintEvent",),
                    lambda: None,
                    driver_action="toggle-sprint-start",
                    requires_player=True,
                    verify=lambda snapshots: verify_toggle_event(
                        snapshots, "PlayerToggleSprintEvent", "is_sprinting", True
                    ),
                ),
        EventScenario(
                    "player-toggle-sprint-stop",
                    ("PlayerToggleSprintEvent",),
                    prepare_player_sprint_stop,
                    driver_action="toggle-sprint-stop",
                    requires_player=True,
                    verify=lambda snapshots: verify_toggle_event(
                        snapshots, "PlayerToggleSprintEvent", "is_sprinting", False
                    ),
                ),
        EventScenario(
                    "player-toggle-crawl",
                    ("PlayerToggleCrawlEvent",),
                    lambda: None,
                    driver_action="toggle-crawl",
                    timeout_ticks=140,
                    requires_player=True,
                    verify=lambda snapshots: verify_toggle_event(
                        snapshots, "PlayerToggleCrawlEvent", "is_crawling", True
                    ),
                ),
        EventScenario(
                    "player-recipe-book",
                    ("PlayerRecipeBookSettingsChangeEvent",),
                    lambda: None,
                    driver_action="recipe-book",
                    requires_player=True,
                    verify=verify_player_recipe_book,
                ),
        EventScenario(
                    "player-craft-item",
                    ("PlayerCraftItemEvent",),
                    prepare_craft_item,
                    timeout_ticks=180,
                    driver_action="craft",
                    driver_data=craft_driver_data,
                    requires_player=True,
                    verify=lambda snapshots: verify_player_craft_item(snapshots, False),
                ),
        EventScenario(
                    "player-craft-item-cancel",
                    ("PlayerCraftItemEvent",),
                    prepare_craft_item,
                    timeout_ticks=180,
                    driver_action="craft",
                    driver_data=craft_driver_data,
                    requires_player=True,
                    cancel_events=("PlayerCraftItemEvent",),
                    verify=lambda snapshots: verify_player_craft_item(snapshots, True),
                ),
        EventScenario(
                    "player-edit-book",
                    ("PlayerEditBookEvent",),
                    prepare_book,
                    timeout_ticks=180,
                    driver_action="edit-book",
                    driver_data=book_driver_data,
                    requires_player=True,
                    verify=lambda snapshots: verify_player_edit_book(snapshots, False),
                ),
        EventScenario(
                    "player-edit-book-cancel",
                    ("PlayerEditBookEvent",),
                    prepare_book,
                    timeout_ticks=180,
                    driver_action="edit-book",
                    driver_data=book_driver_data,
                    requires_player=True,
                    cancel_events=("PlayerEditBookEvent",),
                    verify=lambda snapshots: verify_player_edit_book(snapshots, True),
                ),
        EventScenario(
                    "player-emote",
                    ("PlayerEmoteEvent",),
                    lambda: None,
                    driver_action="emote",
                    requires_player=True,
                    verify=lambda snapshots: verify_player_emote(snapshots, False),
                ),
        EventScenario(
                    "player-emote-cancel",
                    ("PlayerEmoteEvent",),
                    lambda: None,
                    driver_action="emote",
                    requires_player=True,
                    cancel_events=("PlayerEmoteEvent",),
                    verify=lambda snapshots: verify_player_emote(snapshots, True),
                ),
        EventScenario(
                    "player-item-held",
                    ("PlayerItemHeldEvent",),
                    prepare_hotbar_item,
                    driver_action="item-held",
                    driver_data={"slot": 1, "expected_network_id": 306, "requires_item": True},
                    requires_player=True,
                    cleanup=restore_item_held_logger,
                    verify=lambda snapshots: verify_player_item_held(snapshots, 1),
                ),
        EventScenario(
                    "player-item-held-cancel",
                    ("PlayerItemHeldEvent",),
                    prepare_hotbar_item,
                    driver_action="item-held",
                    driver_data={"slot": 1, "expected_network_id": 306, "requires_item": True},
                    requires_player=True,
                    cancel_events=("PlayerItemHeldEvent",),
                    isolated=True,
                    cleanup=restore_item_held_logger,
                    verify=lambda snapshots: verify_player_item_held(snapshots, 1, True),
                ),
        EventScenario(
                    "player-drop-item",
                    ("PlayerDropItemEvent",),
                    prepare_drop_item,
                    timeout_ticks=180,
                    driver_action="drop-item",
                    driver_data={"slot": 0, "expected_network_id": 323, "expected_count": 2},
                    requires_player=True,
                    verify=lambda snapshots: verify_player_drop_item(snapshots, False),
                ),
        EventScenario(
                    "player-drop-item-cancel",
                    ("PlayerDropItemEvent",),
                    prepare_drop_item,
                    timeout_ticks=180,
                    driver_action="drop-item",
                    driver_data={"slot": 0, "expected_network_id": 323, "expected_count": 2},
                    requires_player=True,
                    cancel_events=("PlayerDropItemEvent",),
                    verify=lambda snapshots: verify_player_drop_item(snapshots, True),
                ),
        EventScenario(
                    "player-exp-change",
                    ("PlayerExpChangeEvent",),
                    give_player_exp,
                    requires_player=True,
                    verify=verify_player_exp_change,
                ),
        EventScenario(
                    "player-level-change",
                    ("PlayerLevelChangeEvent",),
                    give_player_exp_level,
                    requires_player=True,
                    verify=verify_player_level_change,
                ),
        EventScenario(
                    "player-pickup-item",
                    ("PlayerPickupItemEvent",),
                    drop_item_for_player,
                    timeout_ticks=120,
                    requires_player=True,
                    verify=lambda snapshots: verify_player_pickup_item(snapshots, False),
                ),
        EventScenario(
                    "player-pickup-item-cancel",
                    ("PlayerPickupItemEvent",),
                    drop_item_for_player,
                    timeout_ticks=120,
                    requires_player=True,
                    cancel_events=("PlayerPickupItemEvent",),
                    verify=lambda snapshots: verify_player_pickup_item(snapshots, True),
                ),
        EventScenario(
                    "player-pickup-experience",
                    ("PlayerPickupExperienceEvent",),
                    spawn_experience_orb,
                    timeout_ticks=120,
                    requires_player=True,
                    verify=lambda snapshots: verify_player_pickup_experience(
                        snapshots, False
                    ),
                ),
        EventScenario(
                    "player-pickup-experience-cancel",
                    ("PlayerPickupExperienceEvent",),
                    spawn_experience_orb,
                    timeout_ticks=120,
                    requires_player=True,
                    cancel_events=("PlayerPickupExperienceEvent",),
                    verify=lambda snapshots: verify_player_pickup_experience(
                        snapshots, True
                    ),
                ),
        EventScenario(
                    "player-riptide",
                    ("PlayerRiptideEvent",),
                    prepare_riptide,
                    timeout_ticks=180,
                    driver_action="riptide",
                    driver_data={"slot": 0},
                    requires_player=True,
                    verify=verify_player_riptide,
                ),
        EventScenario(
                    "player-death-respawn",
                    ("PlayerDeathEvent", "PlayerRespawnEvent"),
                    kill_player,
                    timeout_ticks=160,
                    driver_action="respawn",
                    requires_player=True,
                    verify=verify_player_death_respawn,
                ),
        EventScenario(
                    "player-teleport",
                    ("PlayerTeleportEvent",),
                    teleport_player,
                    requires_player=True,
                    verify=lambda snapshots: verify_player_teleport(snapshots, False),
                ),
        EventScenario(
                    "player-teleport-cancel",
                    ("PlayerTeleportEvent",),
                    teleport_player,
                    requires_player=True,
                    cancel_events=("PlayerTeleportEvent",),
                    verify=lambda snapshots: verify_player_teleport(snapshots, True),
                ),
        EventScenario(
                    "player-dimension-change",
                    ("PlayerDimensionChangeEvent",),
                    change_player_dimension,
                    timeout_ticks=160,
                    requires_player=True,
                    verify=verify_player_dimension_change,
                ),
        EventScenario(
                    "player-game-mode-change",
                    ("PlayerGameModeChangeEvent",),
                    change_player_game_mode,
                    requires_player=True,
                    verify=lambda snapshots: verify_player_game_mode_change(
                        snapshots, False
                    ),
                ),
        EventScenario(
                    "player-game-mode-change-cancel",
                    ("PlayerGameModeChangeEvent",),
                    change_player_game_mode,
                    requires_player=True,
                    cancel_events=("PlayerGameModeChangeEvent",),
                    verify=lambda snapshots: verify_player_game_mode_change(
                        snapshots, True
                    ),
                ),
        EventScenario(
                    "player-toggle-flight-start",
                    ("PlayerToggleFlightEvent",),
                    allow_player_flight,
                    driver_action="toggle-flight-start",
                    requires_player=True,
                    verify=lambda snapshots: verify_player_toggle_flight(snapshots, True),
                ),
        EventScenario(
                    "player-toggle-flight-stop",
                    ("PlayerToggleFlightEvent",),
                    prepare_player_flight_stop,
                    driver_action="toggle-flight-stop",
                    requires_player=True,
                    verify=lambda snapshots: verify_player_toggle_flight(snapshots, False),
                ),
        EventScenario(
                    "player-kick-cancel",
                    ("PlayerKickEvent",),
                    kick_player,
                    timeout_ticks=160,
                    requires_player=True,
                    cancel_events=("PlayerKickEvent",),
                    isolated=True,
                    verify=verify_player_kick_cancel,
                ),
        EventScenario(
                    "player-kick-quit",
                    ("PlayerKickEvent", "PlayerQuitEvent"),
                    kick_player,
                    timeout_ticks=160,
                    requires_player=True,
                    verify=verify_player_kick_quit,
                ),
    ]
