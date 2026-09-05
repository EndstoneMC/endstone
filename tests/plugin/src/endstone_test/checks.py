from typing import NamedTuple

CANCEL = "cancel"
MUTATE = "mutate"

WAND_PREFIX = "endstone-test:"

WAND_ITEMS = {
    "effect": "minecraft:blaze_rod",
    "kick": "minecraft:stick",
}


class Check(NamedTuple):
    key: str
    hint: str
    items: tuple[tuple[str, int], ...] = ()
    wand: str | None = None
    detail: str | None = None


CHECKS = (
    Check(
        "PrepareItemEnchantEvent/cancel",
        "put the pickaxe in the enchanting table",
        (
            ("minecraft:enchanting_table", 1),
            ("minecraft:diamond_pickaxe", 1),
            ("minecraft:lapis_lazuli", 3),
        ),
        detail="Place the enchanting table, open it, and put the diamond pickaxe in "
        "the left slot with the lapis in the right. You need enough experience "
        "levels for offers to appear. Every offer is removed, so the table should "
        "show none at all.",
    ),
    Check(
        "PrepareItemEnchantEvent/mutate",
        "put the pickaxe in the table again",
        detail="Take the pickaxe out and put it back. All three offers are replaced "
        "with a cost-1 Unbreaking I.",
    ),
    Check(
        "EnchantItemEvent/cancel",
        "take an offer at the enchanting table",
        detail="Click one of the offers. The enchant is cancelled, so the pickaxe, "
        "your levels and the lapis should all be untouched.",
    ),
    Check(
        "EnchantItemEvent/mutate",
        "take an offer at the table again",
        detail="Click an offer once more. The cost drops to 1 and the enchantment "
        "becomes Unbreaking III.",
    ),
    Check(
        "InventoryOpenEvent/cancel",
        "place and open the barrel",
        (("minecraft:barrel", 1),),
        detail="Cancelling stops the container screen from appearing at all, so the "
        "barrel should refuse to open.",
    ),
    Check(
        "PlayerCraftItemEvent/cancel",
        "craft sticks from the planks",
        (("minecraft:oak_planks", 8),),
        detail="Open your inventory and craft sticks from the planks in the 2x2 "
        "grid. The craft is blocked and the planks stay where they are.",
    ),
    Check(
        "PlayerCraftItemEvent/mutate",
        "craft sticks in the 2x2 grid again",
        detail="Craft once more. The result is replaced with a diamond.",
    ),
    Check(
        "PlayerEditBookEvent/cancel",
        "write a page in the book and quill",
        (("minecraft:writable_book", 1),),
        detail="Write something on a page and close the book. The edit is dropped "
        "and the slot is sent back to your client, so the page should be blank.",
    ),
    Check(
        "PlayerEditBookEvent/mutate",
        "write a page in the book again",
        detail="Write another page. The first page is rewritten by the test.",
    ),
    Check(
        "PlayerBucketEmptyEvent/cancel",
        "empty the water bucket onto the ground",
        (("minecraft:water_bucket", 1),),
        detail="Cancelling leaves the world dry and the bucket still full.",
    ),
    Check(
        "PlayerBucketEmptyEvent/mutate",
        "empty a water bucket again",
        detail="The item you are left holding is replaced with a water bucket "
        "rather than an empty one.",
    ),
    Check(
        "PlayerBucketFillEvent/cancel",
        "fill the bucket from water",
        (("minecraft:bucket", 1),),
        detail="Place some water first if there is none nearby. Cancelling leaves "
        "the water in place and the bucket empty.",
    ),
    Check(
        "PlayerBucketFillEvent/mutate",
        "fill the bucket from water again",
        detail="The item you are left holding is replaced with an empty bucket.",
    ),
    Check(
        "CauldronLevelChangeEvent/cancel",
        "empty a water bucket into the cauldron",
        (("minecraft:cauldron", 1), ("minecraft:water_bucket", 1)),
        detail="Place the cauldron and empty the bucket into it. Cancelling leaves "
        "the cauldron as it is.",
    ),
    Check(
        "CauldronLevelChangeEvent/mutate",
        "fill the cauldron again",
        detail="The new state is edited in place, so the cauldron ends up empty "
        "rather than filled.",
    ),
    Check(
        "FoodLevelChangeEvent/cancel",
        "get hungry, then eat an apple",
        (("minecraft:apple", 4),),
        detail="Sprint and jump until at least two food points are missing, then "
        "eat an apple. The apple is consumed, but cancelling keeps the food bar "
        "at the same level.",
    ),
    Check(
        "FoodLevelChangeEvent/mutate",
        "eat another apple",
        detail="Eat another apple while still hungry. The final level is changed "
        "before it is applied, so the bar gains exactly one food point instead "
        "of the apple's usual four.",
    ),
    Check(
        "FoodLevelChangeEvent/item",
        "eat a food item",
        detail="A food item should be included in the event when eating from a stack.",
    ),
    Check(
        "FoodLevelChangeEvent/itemless",
        "sprint and jump until hunger decreases",
        detail="An exhaustion-driven food level change should report no food item.",
    ),
    Check(
        "FoodLevelChangeEvent/remove_effect",
        "apply Saturation while hungry",
        detail="Run /effect @s saturation 3 0 while hungry. The listener removes "
        "Saturation during the food change; the server should apply the food "
        "level and remain stable. Run it again if the effect test cancels the "
        "first attempt.",
    ),
    Check(
        "BlockFormEvent/cancel",
        "pour the lava onto water",
        (("minecraft:lava_bucket", 1), ("minecraft:water_bucket", 1)),
        detail="Empty the water bucket, then pour the lava into it. Cancelling "
        "means the lava stays lava: no obsidian or cobblestone, and no "
        "extinguishing sound.",
    ),
    Check(
        "PlayerOpenSignEvent/cancel",
        "place the sign",
        (("minecraft:oak_sign", 1),),
        detail="Cancelling stops the sign editor from opening, so the sign is "
        "placed blank.",
    ),
    Check(
        "PlayerSetSpawnEvent/cancel",
        "place the bed and sleep in it",
        (("minecraft:white_bed", 1),),
        detail="Sleep in the bed, or just run /spawnpoint. The respawn point is "
        "left unchanged, though /spawnpoint still reports success.",
    ),
    Check(
        "PlayerSetSpawnEvent/mutate",
        "set your spawn again",
        detail="The spawn location is raised by one block before it is stored.",
    ),
    Check(
        "PlayerPickupExperienceEvent/cancel",
        "throw a bottle o' enchanting and walk into the orbs",
        (("minecraft:experience_bottle", 8),),
        detail="Cancelling means the orb is not collected, so it should stay on "
        "the ground.",
    ),
    Check(
        "PlayerExpChangeEvent/mutate",
        "throw another bottle o' enchanting",
        detail="The experience gained is doubled.",
    ),
    Check(
        "PlayerPickupArrowEvent/cancel",
        "shoot the arrow and walk over it",
        (("minecraft:bow", 1), ("minecraft:arrow", 8)),
        detail="Shoot into a wall or the ground nearby so the arrow lands, then "
        "walk over it. The pickup is cancelled, so it should stay put.",
    ),
    Check(
        "PlayerShearActorEvent/cancel",
        "spawn a sheep and shear it",
        (("minecraft:shears", 1), ("minecraft:sheep_spawn_egg", 2)),
        detail="Cancelling means the sheep keeps its wool and none drops.",
    ),
    Check(
        "ActorPickupItemEvent/cancel",
        "spawn a zombie and drop the apple next to it",
        (("minecraft:zombie_spawn_egg", 2), ("minecraft:apple", 4)),
        detail="Zombies pick up items they walk over. Cancelling leaves the apple "
        "on the ground.",
    ),
    Check(
        "ActorDismountEvent/cancel",
        "place the boat, ride it, then dismount",
        (("minecraft:oak_boat", 1),),
        detail="Cancelling keeps you in the boat, so the dismount should not take.",
    ),
    Check(
        "ActorChangeBlockEvent/cancel",
        "spawn a creeper and let it explode",
        (("minecraft:creeper_spawn_egg", 2),),
        detail="Stand near the creeper until it detonates, next to some blocks. "
        "Cancelling leaves the blocks it would have destroyed in place.",
    ),
    Check(
        "PlayerTeleportEvent/cancel",
        "throw the ender pearl",
        (("minecraft:ender_pearl", 4),),
        detail="The teleport is cancelled, so you stay where you are when the "
        "pearl lands.",
    ),
    Check(
        "ActorEffectEvent/cancel",
        "right-click the effect wand",
        wand="effect",
        detail="The wand gives you Speed. Cancelling means the effect is never "
        "applied, so no icon should appear.",
    ),
    Check(
        "ActorEffectEvent/mutate",
        "right-click the effect wand again",
        wand="effect",
        detail="The effect is replaced with a longer, stronger one before it lands.",
    ),
    Check(
        "UnknownCommandEvent/mutate",
        "run /thiscommanddoesnotexist",
        detail="The unknown-command response is replaced with the test's own message.",
    ),
    Check(
        "PlayerKickEvent/cancel",
        "right-click the kick wand",
        wand="kick",
        detail="The kick is cancelled, so you stay connected. Right-click the wand "
        "once more and the kick goes through, which is also how PlayerQuitEvent "
        "gets exercised.",
    ),
)

HINTS = {check.key: check.hint for check in CHECKS}
ORDER = [check.key for check in CHECKS]
