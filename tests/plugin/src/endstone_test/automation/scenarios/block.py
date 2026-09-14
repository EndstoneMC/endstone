from __future__ import annotations

from ..models import EventScenario


def build_block_scenarios(ctx, namespace) -> list[EventScenario]:
    cleanup_block_grow = namespace['cleanup_block_grow']
    cleanup_leaves_decay = namespace['cleanup_leaves_decay']
    cleanup_piston = namespace['cleanup_piston']
    prepare_block_form = namespace['prepare_block_form']
    cleanup_block_from_to = namespace['cleanup_block_from_to']
    prepare_block_from_to = namespace['prepare_block_from_to']
    prepare_block_grow = namespace['prepare_block_grow']
    prepare_campfire = namespace['prepare_campfire']
    prepare_leaves_decay = namespace['prepare_leaves_decay']
    prepare_piston = namespace['prepare_piston']
    verify_block_cook = namespace['verify_block_cook']
    verify_block_form = namespace['verify_block_form']
    ready_block_from_to = namespace['ready_block_from_to']
    verify_block_from_to = namespace['verify_block_from_to']
    verify_block_grow = namespace['verify_block_grow']
    verify_leaves_decay = namespace['verify_leaves_decay']
    verify_piston = namespace['verify_piston']
    return [
        EventScenario(
                    "block-from-to",
                    ("BlockFromToEvent",),
                    prepare_block_from_to,
                    timeout_ticks=120,
                    settle_ticks=4,
                    requires_player=True,
                    cleanup=cleanup_block_from_to,
                    ready=ready_block_from_to,
                    verify=lambda snapshots: verify_block_from_to(snapshots, False),
                ),
        EventScenario(
                    "block-from-to-cancel",
                    ("BlockFromToEvent",),
                    prepare_block_from_to,
                    timeout_ticks=120,
                    settle_ticks=4,
                    requires_player=True,
                    cleanup=cleanup_block_from_to,
                    cancel_events=("BlockFromToEvent",),
                    ready=ready_block_from_to,
                    verify=lambda snapshots: verify_block_from_to(snapshots, True),
                ),
        EventScenario(
                    "block-piston-cycle",
                    ("BlockPistonExtendEvent", "BlockPistonRetractEvent"),
                    prepare_piston,
                    timeout_ticks=160,
                    settle_ticks=20,
                    requires_player=True,
                    cleanup=cleanup_piston,
                    verify=lambda snapshots: verify_piston(
                        snapshots,
                        {
                            "BlockPistonExtendEvent": ("east", False),
                            "BlockPistonRetractEvent": ("west", False),
                        },
                    ),
                ),
        EventScenario(
                    "block-piston-extend-cancel",
                    ("BlockPistonExtendEvent",),
                    prepare_piston,
                    timeout_ticks=160,
                    settle_ticks=20,
                    requires_player=True,
                    cleanup=cleanup_piston,
                    cancel_events=("BlockPistonExtendEvent",),
                    verify=lambda snapshots: verify_piston(
                        snapshots, {"BlockPistonExtendEvent": ("east", True)}
                    ),
                ),
        EventScenario(
                    "block-piston-retract-cancel",
                    ("BlockPistonExtendEvent", "BlockPistonRetractEvent"),
                    prepare_piston,
                    timeout_ticks=160,
                    settle_ticks=20,
                    requires_player=True,
                    cleanup=cleanup_piston,
                    cancel_events=("BlockPistonRetractEvent",),
                    verify=lambda snapshots: verify_piston(
                        snapshots,
                        {
                            "BlockPistonExtendEvent": ("east", False),
                            "BlockPistonRetractEvent": ("west", True),
                        },
                    ),
                ),
        EventScenario(
                    "block-form",
                    ("BlockFormEvent",),
                    prepare_block_form,
                    timeout_ticks=180,
                    requires_player=True,
                    verify=lambda snapshots: verify_block_form(snapshots, False),
                ),
        EventScenario(
                    "block-form-cancel",
                    ("BlockFormEvent",),
                    prepare_block_form,
                    timeout_ticks=180,
                    requires_player=True,
                    cancel_events=("BlockFormEvent",),
                    verify=lambda snapshots: verify_block_form(snapshots, True),
                ),
        EventScenario(
                    "block-grow",
                    ("BlockGrowEvent",),
                    prepare_block_grow,
                    timeout_ticks=180,
                    cleanup=cleanup_block_grow,
                    requires_player=True,
                    verify=lambda snapshots: verify_block_grow(snapshots, False),
                ),
        EventScenario(
                    "block-grow-cancel",
                    ("BlockGrowEvent",),
                    prepare_block_grow,
                    timeout_ticks=180,
                    cleanup=cleanup_block_grow,
                    requires_player=True,
                    cancel_events=("BlockGrowEvent",),
                    verify=lambda snapshots: verify_block_grow(snapshots, True),
                ),
        EventScenario(
                    "leaves-decay",
                    ("LeavesDecayEvent",),
                    prepare_leaves_decay,
                    timeout_ticks=180,
                    cleanup=cleanup_leaves_decay,
                    requires_player=True,
                    verify=lambda snapshots: verify_leaves_decay(snapshots, False),
                ),
        EventScenario(
                    "leaves-decay-cancel",
                    ("LeavesDecayEvent",),
                    prepare_leaves_decay,
                    timeout_ticks=180,
                    cleanup=cleanup_leaves_decay,
                    requires_player=True,
                    cancel_events=("LeavesDecayEvent",),
                    verify=lambda snapshots: verify_leaves_decay(snapshots, True),
                ),
        EventScenario(
                    "block-cook",
                    ("BlockCookEvent",),
                    prepare_campfire,
                    timeout_ticks=120,
                    requires_player=True,
                    verify=lambda snapshots: verify_block_cook(snapshots, False),
                ),
        EventScenario(
                    "block-cook-cancel",
                    ("BlockCookEvent",),
                    prepare_campfire,
                    timeout_ticks=120,
                    requires_player=True,
                    cancel_events=("BlockCookEvent",),
                    verify=lambda snapshots: verify_block_cook(snapshots, True),
                ),
    ]
