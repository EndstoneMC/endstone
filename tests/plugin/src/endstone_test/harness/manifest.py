"""Coverage gate: every event path the bindings expose needs a scenario or a stated reason."""

from endstone_test.harness.capture import event_classes, writable
from endstone_test.harness.engine import Scenario

ABSTRACT = {
    "ActorEvent",
    "BlockEvent",
    "BlockPistonEvent",
    "ChunkEvent",
    "DimensionEvent",
    "InventoryEvent",
    "LevelEvent",
    "MobEvent",
    "PlayerBucketEvent",
    "PlayerEvent",
    "ServerEvent",
    "WeatherEvent",
}

NOT_AUTOMATABLE = {
    (
        "BlockGrowEvent",
        "*",
    ): "nothing dispatches it; only its subclass BlockFormEvent fires, and dispatch "
    "matches the exact event name",
    ("InventoryInteractEvent", "*"): "nothing dispatches it on 0.12",
}


NEEDS_BOT = {
    "ActorToggleGlideEvent": "fired only from a player's PlayerAuthInputPacket start/stop gliding flags",
    "ActorToggleSwimEvent": "fired only from a player's PlayerAuthInputPacket start/stop swimming flags",
    "BlockBreakEvent": "fired only for a player breaking a block",
    "BlockPlaceEvent": "fired only for a player placing a block",
    "CraftItemEvent": "fired only from a player's crafting request",
    "EnchantItemEvent": "fired only from a player's enchanting table screen",
    "PrepareItemEnchantEvent": "fired only from a player's enchanting table screen",
    "InventoryOpenEvent": "fired only for a player's container screen",
    "InventoryCloseEvent": "fired only for a player's container screen",
    "PacketSendEvent": "fired only for packets to a connected client",
    "PacketReceiveEvent": "fired only for packets from a connected client",
}


def required_paths(cls: type) -> list[str]:
    paths = ["normal"]
    if hasattr(cls, "cancel"):
        paths.append("cancel")
    if writable(cls):
        paths.append("mutate")
    return paths


def coverage(scenarios: list[Scenario]) -> dict:
    classes = {cls.__name__: cls for cls in event_classes()}
    covered: dict[str, set[str]] = {}
    for scenario in scenarios:
        for name, paths in scenario.covers.items():
            covered.setdefault(name, set()).update(paths)

    events: dict[str, dict] = {}
    missing: list[str] = []
    for name, cls in sorted(classes.items()):
        if name in ABSTRACT:
            continue
        entry = {
            "required": required_paths(cls),
            "covered": sorted(covered.get(name, ())),
            "waived": {},
        }
        for path in entry["required"]:
            reason = NOT_AUTOMATABLE.get((name, path)) or NOT_AUTOMATABLE.get(
                (name, "*")
            )
            if reason:
                entry["waived"][path] = reason
            elif path not in covered.get(name, ()):
                why = NEEDS_BOT.get(name) or (
                    "needs a bot" if name.startswith("Player") else ""
                )
                missing.append(
                    f"{name}.{path} has no scenario" + (f" ({why})" if why else "")
                )
        events[name] = entry

    for name in sorted(set(covered) - set(events)):
        missing.append(f"a scenario covers {name}, which is not a concrete event class")
    for name, _ in NOT_AUTOMATABLE:
        if name not in events:
            missing.append(
                f"NOT_AUTOMATABLE names {name}, which is not a concrete event class"
            )
    for name in ABSTRACT:
        if name not in classes:
            missing.append(f"ABSTRACT names {name}, which the bindings do not export")

    required = sum(len(e["required"]) for e in events.values())
    waived = sum(len(e["waived"]) for e in events.values())
    done = sum(len(set(e["covered"]) & set(e["required"])) for e in events.values())
    return {
        "events": events,
        "missing": missing,
        "summary": f"{done}/{required} event paths covered, {waived} waived, {len(missing)} missing "
        f"across {len(events)} events",
    }
