import contextlib

import pytest
from endstone import Server
from endstone.actor import Actor, ActorType, Item
from endstone.attribute import Attribute
from endstone.boss import BarColor, BarFlag, BarStyle
from endstone.enchantments import Enchantment
from endstone.inventory import ItemStack
from endstone.level import Location
from endstone.potion import EffectType, PotionType
from endstone.scoreboard import Criteria, DisplaySlot, ObjectiveSortOrder, RenderType

OBJECTIVE = "api_actors_objective"


@contextlib.contextmanager
def spawned(server: Server, plot, kind: str, dx: float = 0.0, dy: float = 0.0, dz: float = 0.0):
    overworld = server.level.get_dimension("overworld")
    x, y, z = plot.pos(*plot.middle)
    actor = overworld.spawn_actor(Location(overworld, x + 0.5 + dx, y + dy, z + 0.5 + dz), kind)
    assert actor is not None
    try:
        yield actor
    finally:
        actor.remove()


@contextlib.contextmanager
def dropped(server: Server, plot, stack: ItemStack):
    overworld = server.level.get_dimension("overworld")
    x, y, z = plot.pos(*plot.middle)
    drop = overworld.drop_item(Location(overworld, x + 0.5, y + 1, z + 0.5), stack)
    try:
        yield drop
    finally:
        drop.remove()


def same_actor(server: Server, actor: Actor) -> Actor:
    found = [a for a in server.level.actors if a.id == actor.id]
    assert len(found) == 1, found
    return found[0]


def test_actor_level_is_the_server_level(server: Server, plot) -> None:
    with spawned(server, plot, "minecraft:pig") as pig:
        assert pig.level is server.level
        assert pig.dimension.level is server.level
        assert pig.dimension.id == "minecraft:overworld"
        assert pig.level.get_dimension("overworld").id == pig.dimension.id


def test_actor_rotation_is_written_to_the_location(server: Server, plot) -> None:
    with spawned(server, plot, "minecraft:pig") as pig:
        pig.set_rotation(90.0, 30.0)
        location = pig.location
        assert location.yaw == pytest.approx(90.0)
        assert location.pitch == pytest.approx(30.0)
        pig.set_rotation(-45.0, -20.0)
        location = pig.location
        assert location.yaw == pytest.approx(-45.0)
        assert location.pitch == pytest.approx(-20.0)


def test_actor_rotation_does_not_move_the_actor(server: Server, plot) -> None:
    with spawned(server, plot, "minecraft:pig") as pig:
        before = pig.location
        pig.set_rotation(180.0, 10.0)
        after = pig.location
        assert (after.x, after.y, after.z) == (before.x, before.y, before.z)


def test_actor_rotation_wraps_the_yaw(server: Server, plot) -> None:
    with spawned(server, plot, "minecraft:pig") as pig:
        pig.set_rotation(90.0, 0.0)
        reference = pig.location.yaw
        pig.set_rotation(90.0 + 360.0, 0.0)
        assert pig.location.yaw == pytest.approx(reference)


def test_actor_velocity_of_a_resting_actor_is_zero(server: Server, plot) -> None:
    with spawned(server, plot, "minecraft:pig") as pig:
        velocity = pig.velocity
        assert (velocity.x, velocity.y, velocity.z) == (0.0, 0.0, 0.0)
        assert velocity.length == 0.0


def test_actor_velocity_getter_returns_a_copy(server: Server, plot) -> None:
    with spawned(server, plot, "minecraft:pig") as pig:
        velocity = pig.velocity
        velocity.x = 5.0
        assert pig.velocity.x == 0.0


def test_actor_is_in_lava(server: Server, plot) -> None:
    overworld = server.level.get_dimension("overworld")
    x, y, z = plot.pos(2, 1, 2)
    block = overworld.get_block_at(x, y, z)
    block.set_type("minecraft:lava")
    try:
        with spawned(
            server, plot, "minecraft:pig", dx=2 - plot.middle[0], dy=1 - plot.middle[1], dz=2 - plot.middle[2]
        ) as pig:
            assert pig.is_in_lava
            assert not pig.is_in_water
    finally:
        overworld.get_block_at(x, y, z).set_type("minecraft:air")


def test_actor_is_not_in_lava_on_dry_ground(server: Server, plot) -> None:
    with spawned(server, plot, "minecraft:pig") as pig:
        assert not pig.is_in_lava


def test_actor_name_tag_visible(server: Server, plot) -> None:
    with spawned(server, plot, "minecraft:pig") as pig:
        pig.name_tag = "Bacon"
        pig.is_name_tag_visible = True
        assert same_actor(server, pig).is_name_tag_visible
        pig.is_name_tag_visible = False
        assert not same_actor(server, pig).is_name_tag_visible
        assert same_actor(server, pig).name_tag == "Bacon"


def test_actor_name_tag_always_visible_is_independent(server: Server, plot) -> None:
    with spawned(server, plot, "minecraft:pig") as pig:
        pig.is_name_tag_visible = False
        pig.is_name_tag_always_visible = True
        found = same_actor(server, pig)
        assert found.is_name_tag_always_visible
        assert not found.is_name_tag_visible
        pig.is_name_tag_always_visible = False
        found = same_actor(server, pig)
        assert not found.is_name_tag_always_visible
        assert not found.is_name_tag_visible


def test_actor_name_tag_always_visible_defaults_to_false(server: Server, plot) -> None:
    with spawned(server, plot, "minecraft:pig") as pig:
        assert not pig.is_name_tag_always_visible


def test_actor_score_tag_is_ignored_on_a_mob(server: Server, plot) -> None:
    with spawned(server, plot, "minecraft:pig") as pig:
        assert pig.score_tag == ""
        pig.name_tag = "Bacon"
        pig.score_tag = "12 points"
        found = same_actor(server, pig)
        assert found.score_tag == ""
        assert found.name_tag == "Bacon"


def test_actor_type_hash_and_inequality(server: Server) -> None:
    pig = ActorType.get(ActorType.PIG)
    cow = ActorType.get(ActorType.COW)
    assert pig is not None and cow is not None
    assert not (pig != ActorType.get("minecraft:pig"))
    assert pig != cow
    assert hash(pig) == hash(ActorType.get("minecraft:pig"))
    assert hash(pig) != hash(cow)
    assert len({pig, cow, ActorType.get("minecraft:pig")}) == 2
    assert {pig: "pig"}[ActorType.get(ActorType.PIG)] == "pig"


def test_item_item_stack_setter_replaces_the_dropped_stack(server: Server, plot) -> None:
    with dropped(server, plot, ItemStack("minecraft:gold_ingot", 3)) as drop:
        assert drop.item_stack.type.id == "minecraft:gold_ingot"
        drop.item_stack = ItemStack("minecraft:diamond", 5)
        found = same_actor(server, drop)
        assert isinstance(found, Item)
        assert found.item_stack.type.id == "minecraft:diamond"
        assert found.item_stack.amount == 5


def test_item_item_stack_setter_copies_the_stack(server: Server, plot) -> None:
    with dropped(server, plot, ItemStack("minecraft:gold_ingot", 3)) as drop:
        stack = ItemStack("minecraft:diamond", 5)
        drop.item_stack = stack
        stack.amount = 9
        assert drop.item_stack.amount == 5


def test_item_pickup_delay(server: Server, plot) -> None:
    with dropped(server, plot, ItemStack("minecraft:gold_ingot", 3)) as drop:
        drop.pickup_delay = 123
        assert same_actor(server, drop).pickup_delay == 123
        drop.pickup_delay = 0
        assert same_actor(server, drop).pickup_delay == 0


def test_item_pickup_delay_is_per_item(server: Server, plot) -> None:
    with dropped(server, plot, ItemStack("minecraft:gold_ingot", 3)) as first:
        with dropped(server, plot, ItemStack("minecraft:diamond", 3)) as second:
            first.pickup_delay = 200
            second.pickup_delay = 7
            assert same_actor(server, first).pickup_delay == 200
            assert same_actor(server, second).pickup_delay == 7


def test_item_unlimited_lifetime(server: Server, plot) -> None:
    with dropped(server, plot, ItemStack("minecraft:gold_ingot", 3)) as drop:
        assert not drop.is_unlimited_lifetime
        drop.is_unlimited_lifetime = True
        found = same_actor(server, drop)
        assert found.is_unlimited_lifetime
        assert found.item_stack.type.id == "minecraft:gold_ingot"
        drop.is_unlimited_lifetime = False
        assert not same_actor(server, drop).is_unlimited_lifetime


def test_item_thrower(server: Server, plot) -> None:
    with dropped(server, plot, ItemStack("minecraft:gold_ingot", 3)) as drop:
        with spawned(server, plot, "minecraft:pig", dx=3) as pig:
            drop.thrower = pig.id
            assert same_actor(server, drop).thrower == pig.id
            drop.thrower = None


def test_item_thrower_can_be_cleared(server: Server, plot) -> None:
    with dropped(server, plot, ItemStack("minecraft:gold_ingot", 3)) as drop:
        with spawned(server, plot, "minecraft:pig", dx=3) as pig:
            drop.thrower = pig.id
            drop.thrower = None
            assert same_actor(server, drop).thrower is None


@pytest.mark.parametrize(
    "registry_type, constant",
    [
        (ActorType, ActorType.PIG),
        (Attribute, Attribute.HEALTH),
        (Enchantment, Enchantment.SHARPNESS),
        (EffectType, EffectType.SPEED),
        (PotionType, PotionType.SWIFTNESS),
    ],
    ids=lambda v: getattr(v, "__name__", None),
)
def test_registry_entry_equals_its_identifier_constant(server: Server, registry_type, constant) -> None:
    entry = registry_type.get(constant)
    assert entry is not None
    assert entry == constant
    assert not (entry != constant)


def test_attribute_get(server: Server) -> None:
    health = Attribute.get("minecraft:health")
    assert health is not None
    assert health.id == "minecraft:health"
    assert Attribute.get(Attribute.HEALTH) is health
    assert Attribute.get("health") is health
    assert Attribute.get("minecraft:no_such_attribute") is None
    assert Attribute.get("minecraft:movement") is not health


def test_attribute_get_matches_the_registry(server: Server) -> None:
    registry = server.get_registry(Attribute)
    for attribute in registry:
        assert Attribute.get(attribute.id) is attribute


def test_attribute_equality_hash_and_text(server: Server) -> None:
    health = Attribute.get(Attribute.HEALTH)
    movement = Attribute.get(Attribute.MOVEMENT_SPEED)
    assert health is not None and movement is not None
    assert health == Attribute.get("minecraft:health")
    assert not (health != Attribute.get("minecraft:health"))
    assert health != movement
    assert hash(health) == hash(Attribute.get("minecraft:health"))
    assert hash(health) == hash("minecraft:health")
    assert len({health, movement, Attribute.get("minecraft:health")}) == 2
    assert str(health) == "minecraft:health"
    assert repr(health) == "Attribute(minecraft:health)"
    assert repr(movement) == "Attribute(minecraft:movement)"
    assert repr(health) != repr(movement)


def test_attribute_text_follows_each_registry_entry(server: Server) -> None:
    for attribute in server.get_registry(Attribute):
        assert str(attribute) == str(attribute.id)
        assert repr(attribute) == f"Attribute({attribute.id})"


def test_boss_bar_flags(server: Server) -> None:
    bar = server.create_boss_bar("flags", BarColor.RED, BarStyle.SOLID)
    assert not bar.has_flag(BarFlag.DARKEN_SKY)
    assert not bar.has_flag(BarFlag.CREATE_FOG)
    bar.add_flag(BarFlag.DARKEN_SKY)
    bar.add_flag(BarFlag.CREATE_FOG)
    assert bar.has_flag(BarFlag.DARKEN_SKY)
    assert bar.has_flag(BarFlag.CREATE_FOG)
    bar.remove_flag(BarFlag.DARKEN_SKY)
    assert not bar.has_flag(BarFlag.DARKEN_SKY)
    assert bar.has_flag(BarFlag.CREATE_FOG)
    bar.remove_flag(BarFlag.CREATE_FOG)
    assert not bar.has_flag(BarFlag.CREATE_FOG)


def test_boss_bar_remove_flag_that_is_not_set(server: Server) -> None:
    bar = server.create_boss_bar("flags", BarColor.RED, BarStyle.SOLID)
    bar.remove_flag(BarFlag.CREATE_FOG)
    assert not bar.has_flag(BarFlag.CREATE_FOG)
    bar.add_flag(BarFlag.DARKEN_SKY)
    bar.remove_flag(BarFlag.CREATE_FOG)
    assert bar.has_flag(BarFlag.DARKEN_SKY)


def test_boss_bar_flags_are_per_bar(server: Server) -> None:
    first = server.create_boss_bar("first", BarColor.RED, BarStyle.SOLID)
    second = server.create_boss_bar("second", BarColor.RED, BarStyle.SOLID)
    first.add_flag(BarFlag.CREATE_FOG)
    assert first.has_flag(BarFlag.CREATE_FOG)
    assert not second.has_flag(BarFlag.CREATE_FOG)


def test_boss_bar_flags_survive_other_changes(server: Server) -> None:
    bar = server.create_boss_bar("flags", BarColor.RED, BarStyle.SOLID)
    bar.add_flag(BarFlag.DARKEN_SKY)
    bar.color = BarColor.BLUE
    bar.style = BarStyle.SEGMENTED_10
    bar.progress = 0.5
    bar.title = "renamed"
    assert bar.has_flag(BarFlag.DARKEN_SKY)
    assert not bar.has_flag(BarFlag.CREATE_FOG)


def sword() -> ItemStack:
    return ItemStack("minecraft:diamond_sword")


def test_enchantment_get(server: Server) -> None:
    sharpness = Enchantment.get("minecraft:sharpness")
    assert sharpness is not None
    assert sharpness.id == "minecraft:sharpness"
    assert Enchantment.get(Enchantment.SHARPNESS) is sharpness
    assert Enchantment.get("sharpness") is sharpness
    assert Enchantment.get("minecraft:no_such_enchantment") is None
    for enchantment in server.get_registry(Enchantment):
        assert Enchantment.get(enchantment.id) is enchantment


def test_enchantment_inequality(server: Server) -> None:
    sharpness = Enchantment.get(Enchantment.SHARPNESS)
    smite = Enchantment.get(Enchantment.SMITE)
    assert sharpness is not None and smite is not None
    assert sharpness != smite
    assert not (sharpness != Enchantment.get("minecraft:sharpness"))


@pytest.mark.parametrize(
    "first, second",
    [
        (Enchantment.SHARPNESS, Enchantment.SMITE),
        (Enchantment.SHARPNESS, Enchantment.BANE_OF_ARTHROPODS),
        (Enchantment.SMITE, Enchantment.BANE_OF_ARTHROPODS),
        (Enchantment.PROTECTION, Enchantment.FIRE_PROTECTION),
        (Enchantment.PROTECTION, Enchantment.BLAST_PROTECTION),
        (Enchantment.FIRE_PROTECTION, Enchantment.PROJECTILE_PROTECTION),
        (Enchantment.SILK_TOUCH, Enchantment.FORTUNE),
        (Enchantment.MULTISHOT, Enchantment.PIERCING),
        (Enchantment.DEPTH_STRIDER, Enchantment.FROST_WALKER),
    ],
)
def test_enchantment_conflicts_are_symmetric(server: Server, first, second) -> None:
    a = Enchantment.get(first)
    b = Enchantment.get(second)
    assert a is not None and b is not None
    assert a.conflicts_with(b)
    assert b.conflicts_with(a)


@pytest.mark.parametrize(
    "first, second",
    [
        (Enchantment.SHARPNESS, Enchantment.LOOTING),
        (Enchantment.SHARPNESS, Enchantment.UNBREAKING),
        (Enchantment.PROTECTION, Enchantment.THORNS),
        (Enchantment.EFFICIENCY, Enchantment.FORTUNE),
        (Enchantment.POWER, Enchantment.PUNCH),
        (Enchantment.MENDING, Enchantment.UNBREAKING),
    ],
)
def test_enchantment_compatible_pairs_do_not_conflict(server: Server, first, second) -> None:
    a = Enchantment.get(first)
    b = Enchantment.get(second)
    assert a is not None and b is not None
    assert not a.conflicts_with(b)
    assert not b.conflicts_with(a)


@pytest.mark.parametrize(
    "enchantment, item, expected",
    [
        (Enchantment.SHARPNESS, "minecraft:diamond_sword", True),
        (Enchantment.SHARPNESS, "minecraft:diamond_axe", True),
        (Enchantment.SHARPNESS, "minecraft:diamond_pickaxe", False),
        (Enchantment.SHARPNESS, "minecraft:apple", False),
        (Enchantment.EFFICIENCY, "minecraft:diamond_pickaxe", True),
        (Enchantment.EFFICIENCY, "minecraft:bow", False),
        (Enchantment.PROTECTION, "minecraft:diamond_chestplate", True),
        (Enchantment.PROTECTION, "minecraft:diamond_sword", False),
        (Enchantment.POWER, "minecraft:bow", True),
        (Enchantment.POWER, "minecraft:diamond_sword", False),
        (Enchantment.MENDING, "minecraft:diamond_sword", True),
        (Enchantment.UNBREAKING, "minecraft:stick", False),
        (Enchantment.RIPTIDE, "minecraft:trident", True),
        (Enchantment.RIPTIDE, "minecraft:bow", False),
        (Enchantment.FEATHER_FALLING, "minecraft:diamond_boots", True),
        (Enchantment.FEATHER_FALLING, "minecraft:diamond_helmet", False),
    ],
)
def test_enchantment_can_enchant_item(server: Server, enchantment, item, expected) -> None:
    found = Enchantment.get(enchantment)
    assert found is not None
    assert found.can_enchant_item(ItemStack(item)) is expected


def test_enchantment_can_enchant_item_ignores_existing_enchantments(server: Server) -> None:
    smite = Enchantment.get(Enchantment.SMITE)
    sharpness = Enchantment.get(Enchantment.SHARPNESS)
    assert smite is not None and sharpness is not None
    stack = sword()
    meta = stack.item_meta
    meta.add_enchant(Enchantment.SHARPNESS, 3, True)
    stack.set_item_meta(meta)
    assert smite.can_enchant_item(stack)


def test_effect_type_equality_hash_and_text(server: Server) -> None:
    speed = EffectType.get(EffectType.SPEED)
    slowness = EffectType.get(EffectType.SLOWNESS)
    assert speed is not None and slowness is not None
    assert speed == EffectType.get("minecraft:speed")
    assert speed == "minecraft:speed"
    assert not (speed == slowness)
    assert speed != slowness
    assert not (speed != EffectType.get("minecraft:speed"))
    assert hash(speed) == hash(EffectType.get(EffectType.SPEED))
    assert hash(speed) == hash("minecraft:speed")
    assert hash(speed) != hash(slowness)
    assert len({speed, slowness, EffectType.get("minecraft:speed")}) == 2
    assert str(speed) == "minecraft:speed"
    assert repr(speed) == "EffectType(minecraft:speed)"
    assert repr(speed) != repr(slowness)


def test_effect_type_text_follows_each_registry_entry(server: Server) -> None:
    for effect_type in server.get_registry(EffectType):
        assert str(effect_type) == str(effect_type.id)
        assert repr(effect_type) == f"EffectType({effect_type.id})"
        assert effect_type == str(effect_type.id)
        assert hash(effect_type) == hash(str(effect_type.id))


def test_effect_type_equality_with_a_foreign_type(server: Server) -> None:
    speed = EffectType.get(EffectType.SPEED)
    assert speed is not None
    assert not (speed == 5)
    assert speed != 5
    assert speed != None  # noqa: E711
    assert speed != Attribute.get(Attribute.HEALTH)


def test_potion_type_get(server: Server) -> None:
    water = PotionType.get("minecraft:potion_type:water")
    assert water is not None
    assert water.id == "minecraft:potion_type:water"
    assert PotionType.get(PotionType.WATER) is water
    assert PotionType.get("minecraft:potion_type:no_such_potion") is None
    for potion_type in server.get_registry(PotionType):
        assert PotionType.get(potion_type.id) is potion_type


def test_potion_type_equality_hash_and_text(server: Server) -> None:
    swiftness = PotionType.get(PotionType.SWIFTNESS)
    strong = PotionType.get(PotionType.STRONG_SWIFTNESS)
    assert swiftness is not None and strong is not None
    assert swiftness == PotionType.get("minecraft:potion_type:swiftness")
    assert not (swiftness == strong)
    assert swiftness != strong
    assert not (swiftness != PotionType.get(PotionType.SWIFTNESS))
    assert hash(swiftness) == hash(PotionType.get(PotionType.SWIFTNESS))
    assert hash(swiftness) == hash("minecraft:potion_type:swiftness")
    assert hash(swiftness) != hash(strong)
    assert len({swiftness, strong, PotionType.get(PotionType.SWIFTNESS)}) == 2
    assert str(swiftness) == "minecraft:potion_type:swiftness"
    assert repr(swiftness) == "PotionType(minecraft:potion_type:swiftness)"
    assert repr(swiftness) != repr(strong)


def test_potion_type_text_follows_each_registry_entry(server: Server) -> None:
    for potion_type in server.get_registry(PotionType):
        assert str(potion_type) == str(potion_type.id)
        assert repr(potion_type) == f"PotionType({potion_type.id})"
        assert potion_type == str(potion_type.id)
        assert hash(potion_type) == hash(str(potion_type.id))


def run(server: Server, command: str) -> None:
    assert server.dispatch_command(server.command_sender, command)


@pytest.fixture
def board(server: Server):
    scoreboard = server.scoreboard
    if scoreboard.get_objective(OBJECTIVE) is not None:
        server.dispatch_command(server.command_sender, f"scoreboard objectives remove {OBJECTIVE}")
    yield scoreboard
    if scoreboard.get_objective(OBJECTIVE) is not None:
        server.dispatch_command(server.command_sender, f"scoreboard objectives remove {OBJECTIVE}")
    scoreboard.clear_slot(DisplaySlot.SIDE_BAR)


def test_criteria_dummy_properties(server: Server, board) -> None:
    objective = board.add_objective(OBJECTIVE, Criteria.DUMMY)
    criteria = objective.criteria
    assert criteria.name == "dummy"
    assert criteria.is_read_only is False
    assert criteria.default_render_type is RenderType.INTEGER
    assert objective.is_modifiable is True
    assert objective.render_type is criteria.default_render_type


def test_objective_inequality(server: Server, board) -> None:
    objective = board.add_objective(OBJECTIVE, Criteria.DUMMY)
    other = board.add_objective(OBJECTIVE + "_2", Criteria.DUMMY)
    try:
        assert not (objective != board.get_objective(OBJECTIVE))
        assert objective == board.get_objective(OBJECTIVE)
        assert objective != other
        assert not (objective == other)
        assert objective != "x"
    finally:
        other.unregister()


def test_objective_display_name(server: Server, board) -> None:
    objective = board.add_objective(OBJECTIVE, Criteria.DUMMY)
    assert objective.display_name == OBJECTIVE
    objective.display_name = "Shown Name"
    fetched = board.get_objective(OBJECTIVE)
    assert fetched is not None
    assert fetched.display_name == "Shown Name"
    assert fetched.name == OBJECTIVE
    objective.display_name = OBJECTIVE
    assert board.get_objective(OBJECTIVE).display_name == OBJECTIVE


def test_objective_display_slot(server: Server, board) -> None:
    objective = board.add_objective(OBJECTIVE, Criteria.DUMMY)
    assert not objective.is_displayed
    assert objective.display_slot is None
    assert board.get_objective(DisplaySlot.SIDE_BAR) is None

    objective.display_slot = DisplaySlot.SIDE_BAR
    assert objective.is_displayed
    assert objective.display_slot is DisplaySlot.SIDE_BAR
    assert board.get_objective(DisplaySlot.SIDE_BAR) == objective
    assert board.get_objective(DisplaySlot.PLAYER_LIST) is None

    objective.display_slot = None
    assert not objective.is_displayed
    assert objective.display_slot is None
    assert board.get_objective(DisplaySlot.SIDE_BAR) is None


def test_objective_display_slot_moves_between_slots(server: Server, board) -> None:
    objective = board.add_objective(OBJECTIVE, Criteria.DUMMY)
    objective.display_slot = DisplaySlot.SIDE_BAR
    objective.display_slot = DisplaySlot.PLAYER_LIST
    try:
        assert board.get_objective(DisplaySlot.SIDE_BAR) is None
        assert board.get_objective(DisplaySlot.PLAYER_LIST) == objective
        assert objective.display_slot is DisplaySlot.PLAYER_LIST
    finally:
        board.clear_slot(DisplaySlot.PLAYER_LIST)


def test_objective_sort_order(server: Server, board) -> None:
    objective = board.add_objective(OBJECTIVE, Criteria.DUMMY)
    assert objective.sort_order is None
    objective.set_display(DisplaySlot.SIDE_BAR, ObjectiveSortOrder.ASCENDING)
    assert objective.sort_order is ObjectiveSortOrder.ASCENDING
    objective.sort_order = ObjectiveSortOrder.DESCENDING
    assert board.get_objective(OBJECTIVE).sort_order is ObjectiveSortOrder.DESCENDING
    assert objective.display_slot is DisplaySlot.SIDE_BAR
    objective.sort_order = ObjectiveSortOrder.ASCENDING
    assert board.get_objective(OBJECTIVE).sort_order is ObjectiveSortOrder.ASCENDING
    objective.display_slot = None
    assert objective.sort_order is None


def test_scoreboard_clear_slot(server: Server, board) -> None:
    objective = board.add_objective(OBJECTIVE, Criteria.DUMMY)
    objective.display_slot = DisplaySlot.SIDE_BAR
    assert objective.is_displayed
    board.clear_slot(DisplaySlot.SIDE_BAR)
    assert not objective.is_displayed
    assert objective.display_slot is None
    assert board.get_objective(DisplaySlot.SIDE_BAR) is None
    assert board.get_objective(OBJECTIVE) == objective


def test_scoreboard_clear_slot_leaves_other_slots(server: Server, board) -> None:
    objective = board.add_objective(OBJECTIVE, Criteria.DUMMY)
    objective.display_slot = DisplaySlot.PLAYER_LIST
    try:
        board.clear_slot(DisplaySlot.SIDE_BAR)
        assert objective.display_slot is DisplaySlot.PLAYER_LIST
    finally:
        board.clear_slot(DisplaySlot.PLAYER_LIST)


def test_scoreboard_objectives_by_criteria(server: Server, board) -> None:
    before = len(board.get_objectives_by_criteria(Criteria.DUMMY))
    objective = board.add_objective(OBJECTIVE, Criteria.DUMMY)
    found = board.get_objectives_by_criteria(Criteria.DUMMY)
    assert objective in found
    assert len(found) == before + 1
    objective.unregister()
    assert len(board.get_objectives_by_criteria(Criteria.DUMMY)) == before


def test_scoreboard_entries_and_scores(server: Server, board) -> None:
    board.add_objective(OBJECTIVE, Criteria.DUMMY)
    other = board.add_objective(OBJECTIVE + "_2", Criteria.DUMMY)
    try:
        assert "api_actors_entry" not in board.entries
        assert not any(score.is_score_set for score in board.get_scores("api_actors_entry"))

        run(server, f"scoreboard players set api_actors_entry {OBJECTIVE} 4")
        run(server, f"scoreboard players set api_actors_entry {OBJECTIVE}_2 9")
        assert "api_actors_entry" in board.entries
        assert board.entries.count("api_actors_entry") == 1

        scores = board.get_scores("api_actors_entry")
        assert len(scores) == 2
        by_objective = {score.objective.name: score for score in scores}
        assert set(by_objective) == {OBJECTIVE, OBJECTIVE + "_2"}
        assert by_objective[OBJECTIVE].value == 4
        assert by_objective[OBJECTIVE + "_2"].value == 9
        for score in scores:
            assert score.entry == "api_actors_entry"
            assert score.is_score_set

        board.reset_scores("api_actors_entry")
        assert "api_actors_entry" not in board.entries
        assert not any(score.is_score_set for score in board.get_scores("api_actors_entry"))
    finally:
        other.unregister()


def test_score_entry_and_scoreboard(server: Server, board) -> None:
    objective = board.add_objective(OBJECTIVE, Criteria.DUMMY)
    score = objective.get_score("api_actors_name")
    assert score.entry == "api_actors_name"
    assert score.scoreboard is board
    assert score.objective == objective
    assert not score.is_score_set
    score.value = 11
    assert score.is_score_set
    assert board.get_scores("api_actors_name")[0].value == 11
    assert score.scoreboard is board


def test_score_entry_of_an_actor(server: Server, board, plot) -> None:
    objective = board.add_objective(OBJECTIVE, Criteria.DUMMY)
    with spawned(server, plot, "minecraft:pig") as pig:
        score = objective.get_score(pig)
        assert isinstance(score.entry, Actor)
        assert score.entry.id == pig.id
        score.value = 5
        entries = [entry for entry in board.entries if isinstance(entry, Actor)]
        assert [entry.id for entry in entries] == [pig.id]
        assert board.get_scores(pig)[0].value == 5
        board.reset_scores(pig)
        assert not [entry for entry in board.entries if isinstance(entry, Actor)]


def test_actor_score_tag_defaults_to_empty(server: Server, plot) -> None:
    with spawned(server, plot, "minecraft:pig") as pig:
        assert pig.score_tag == ""
        assert same_actor(server, pig).score_tag == ""
