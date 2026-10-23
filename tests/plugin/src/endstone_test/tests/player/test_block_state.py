import pytest
from endstone import Player, Rotation
from endstone.block import (
    Block,
    BlockState,
    Campfire,
    Container,
    CreatureSpawner,
    Furnace,
    ItemFrame,
    Lectern,
    Side,
    Sign,
    SignSide,
)
from endstone.inventory import Inventory, ItemStack

# =============================================================================
# Fixtures
# =============================================================================


@pytest.fixture
def place(player: Player):
    """Place a block on a scratch position above the player and clear it afterwards."""
    location = player.location
    x, y, z = location.block_x, location.block_y + 8, location.block_z
    dimension = player.dimension
    server = player.server

    def _place(block_type: str) -> Block:
        server.dispatch_command(
            server.command_sender, f"setblock {x} {y} {z} {block_type}"
        )
        block = dimension.get_block_at(x=x, y=y, z=z)
        if block.type != block_type:
            pytest.skip(f"{block_type} is not available on this server")
        return block

    yield _place

    server.dispatch_command(server.command_sender, f"setblock {x} {y} {z} air")
    assert dimension.get_block_at(x=x, y=y, z=z).type == "minecraft:air"


# =============================================================================
# Section 1: The base BlockState
# =============================================================================


def test_plain_block_captures_a_base_state(place) -> None:
    """Verify a block with no block entity captures a plain BlockState."""
    state = place("minecraft:stone").capture_state()
    assert isinstance(state, BlockState)
    assert type(state) is BlockState


def test_state_knows_where_it_is(place, player: Player) -> None:
    """Verify a captured state reports its own coordinates and dimension."""
    block = place("minecraft:stone")
    state = block.capture_state()
    assert (state.x, state.y, state.z) == (block.x, block.y, block.z)
    assert state.location.block_y == block.y
    assert state.dimension.id == player.dimension.id
    assert state.block.type == block.type
    assert str(state)


def test_state_type_and_data_are_writable(place) -> None:
    """Verify a captured state can be retyped and pushed back to the world."""
    block = place("minecraft:stone")
    state = block.capture_state()
    state.type = "minecraft:dirt"
    assert state.type == "minecraft:dirt"
    assert state.update(True) is True
    assert block.type == "minecraft:dirt"


# =============================================================================
# Section 2: Container and its subclasses
# =============================================================================


@pytest.mark.parametrize(
    "block_type", ["minecraft:chest", "minecraft:barrel", "minecraft:hopper"]
)
def test_container(place, block_type: str) -> None:
    """Verify a container block captures a Container exposing its inventory."""
    state = place(block_type).capture_state()
    assert isinstance(state, Container)
    assert isinstance(state.inventory, Inventory)
    assert state.inventory.size > 0
    assert state.inventory.is_empty is True


def test_container_inventory_round_trip(place) -> None:
    """Verify an item written into a container survives an update."""
    state = place("minecraft:chest").capture_state()
    state.inventory.set_item(0, ItemStack("minecraft:diamond", 3))
    assert state.update(True) is True

    stored = state.block.capture_state().inventory.get_item(0)
    assert stored is not None
    assert stored.type == "minecraft:diamond"
    assert stored.amount == 3


def test_container_inventory_is_not_a_player_inventory(place) -> None:
    """Verify a container's inventory lacks the PlayerInventory-only members."""
    inventory = place("minecraft:chest").capture_state().inventory
    assert not hasattr(inventory, "item_in_main_hand")
    assert not hasattr(inventory, "held_item_slot")


@pytest.mark.parametrize(
    "block_type",
    ["minecraft:furnace", "minecraft:blast_furnace", "minecraft:smoker"],
)
def test_furnace(place, block_type: str) -> None:
    """Verify a furnace captures a Furnace, which is a Container."""
    state = place(block_type).capture_state()
    assert isinstance(state, Furnace)
    assert isinstance(state, Container)
    assert state.inventory.size >= 3
    assert state.burn_time >= 0
    assert state.cook_time >= 0


def test_furnace_times_are_writable(place) -> None:
    """Verify the furnace burn and cook times round-trip."""
    state = place("minecraft:furnace").capture_state()
    state.burn_time = 120
    state.cook_time = 40
    assert state.burn_time == 120
    assert state.cook_time == 40


def test_furnace_has_no_cook_time_total(place) -> None:
    """Verify Bedrock derives the total cook time, so there is no such member."""
    assert not hasattr(place("minecraft:furnace").capture_state(), "cook_time_total")


def test_lectern(place) -> None:
    """Verify a lectern captures a Lectern, which is not a Container."""
    state = place("minecraft:lectern").capture_state()
    assert isinstance(state, Lectern)
    assert not isinstance(state, Container)
    assert state.page == 0


def test_lectern_page_clamps_to_an_empty_book(place) -> None:
    """Verify the page is clamped rather than raising when there is no book."""
    state = place("minecraft:lectern").capture_state()
    state.page = 99
    assert state.page >= 0


# =============================================================================
# Section 3: Campfire
# =============================================================================


def test_campfire(place) -> None:
    """Verify a campfire captures a Campfire with four cooking slots."""
    state = place("minecraft:campfire").capture_state()
    assert isinstance(state, Campfire)
    assert state.size == 4


def test_campfire_items(place) -> None:
    """Verify a campfire slot holds an item and reports it back."""
    state = place("minecraft:campfire").capture_state()
    assert state.get_item(0) is None

    state.set_item(0, ItemStack("minecraft:cod"))
    assert state.get_item(0).type == "minecraft:cod"

    state.set_item(0, None)
    assert state.get_item(0) is None


def test_campfire_cook_times(place) -> None:
    """Verify each campfire slot carries its own cook time."""
    state = place("minecraft:campfire").capture_state()
    for index in range(state.size):
        state.set_cook_time(index, index * 10)
    for index in range(state.size):
        assert state.get_cook_time(index) == index * 10


# =============================================================================
# Section 4: CreatureSpawner
# =============================================================================


def test_creature_spawner(place) -> None:
    """Verify a monster spawner captures a CreatureSpawner."""
    assert isinstance(place("minecraft:mob_spawner").capture_state(), CreatureSpawner)


def test_creature_spawner_spawned_type(place) -> None:
    """Verify the spawned type is written as an id and read back as an ActorType."""
    state = place("minecraft:mob_spawner").capture_state()
    state.spawned_type = "minecraft:zombie"
    assert state.spawned_type == "minecraft:zombie"
    assert str(state.spawned_type) == "minecraft:zombie"


@pytest.mark.parametrize(
    "attribute,value",
    [
        ("delay", 100),
        ("min_spawn_delay", 200),
        ("max_spawn_delay", 800),
        ("spawn_count", 4),
        ("max_nearby_entities", 6),
        ("required_player_range", 16),
        ("spawn_range", 4),
    ],
)
def test_creature_spawner_attributes(place, attribute: str, value: int) -> None:
    """Verify every spawner attribute round-trips."""
    state = place("minecraft:mob_spawner").capture_state()
    setattr(state, attribute, value)
    assert getattr(state, attribute) == value


# =============================================================================
# Section 5: ItemFrame
# =============================================================================


@pytest.mark.parametrize("block_type", ["minecraft:frame", "minecraft:glow_frame"])
def test_item_frame(place, block_type: str) -> None:
    """Verify an item frame captures an ItemFrame."""
    state = place(block_type).capture_state()
    assert isinstance(state, ItemFrame)
    assert state.item.type == "minecraft:air"
    assert 0.0 <= state.item_drop_chance <= 1.0


def test_item_frame_item(place) -> None:
    """Verify an item frame holds an item and hands back a copy of it."""
    state = place("minecraft:frame").capture_state()
    state.item = ItemStack("minecraft:diamond")
    assert state.item.type == "minecraft:diamond"
    assert state.item is not state.item

    state.item = None
    assert state.item.type == "minecraft:air"


def test_item_frame_rotation(place) -> None:
    """Verify the rotation round-trips through all eight steps."""
    state = place("minecraft:frame").capture_state()
    state.item = ItemStack("minecraft:diamond")
    for rotation in Rotation:
        state.rotation = rotation
        assert state.rotation == rotation


def test_item_frame_assigning_an_item_resets_the_rotation(place) -> None:
    """Verify putting a new item into a frame resets its rotation."""
    state = place("minecraft:frame").capture_state()
    state.item = ItemStack("minecraft:diamond")
    state.rotation = Rotation.CLOCKWISE
    state.item = ItemStack("minecraft:emerald")
    assert state.rotation == Rotation.NONE


def test_item_frame_drop_chance(place) -> None:
    """Verify the drop chance round-trips."""
    state = place("minecraft:frame").capture_state()
    state.item_drop_chance = 0.25
    assert abs(state.item_drop_chance - 0.25) <= 0.00001


# =============================================================================
# Section 6: Sign
# =============================================================================


def test_sign(place) -> None:
    """Verify a sign captures a Sign with a front and a back side."""
    state = place("minecraft:standing_sign").capture_state()
    assert isinstance(state, Sign)
    assert isinstance(state.get_side(Side.FRONT), SignSide)
    assert isinstance(state.get_side(Side.BACK), SignSide)


def test_sign_side_enum_is_not_exported_onto_the_class() -> None:
    """Verify Side has no export_values, unlike Operation and RespawnReason."""
    assert [member.name for member in Side] == ["FRONT", "BACK"]
    assert not hasattr(Sign, "FRONT")
    assert not hasattr(Sign, "BACK")


def test_sign_lines(place) -> None:
    """Verify each of the four lines on a side round-trips."""
    side = place("minecraft:standing_sign").capture_state().get_side(Side.FRONT)
    for index in range(4):
        side.set_line(index, f"line {index}")
    assert side.lines == [f"line {index}" for index in range(4)]
    assert side.get_line(0) == "line 0"


def test_sign_sides_are_independent(place) -> None:
    """Verify writing the front of a sign leaves the back alone."""
    state = place("minecraft:standing_sign").capture_state()
    state.get_side(Side.FRONT).set_line(0, "front")
    assert state.get_side(Side.BACK).get_line(0) == ""


def test_sign_glowing_text(place) -> None:
    """Verify the glowing text flag round-trips."""
    side = place("minecraft:standing_sign").capture_state().get_side(Side.FRONT)
    side.glowing_text = True
    assert side.glowing_text is True


def test_sign_color_reads_back_as_rgba(place) -> None:
    """Verify a sign colour is written as RGB or RGBA and always read back as RGBA."""
    side = place("minecraft:standing_sign").capture_state().get_side(Side.FRONT)
    side.color = (255, 0, 0)
    assert side.color == (255, 0, 0, 255)

    side.color = (0, 255, 0, 128)
    assert side.color == (0, 255, 0, 128)


@pytest.mark.parametrize("color", [(1, 2), (1, 2, 3, 4, 5)])
def test_sign_color_rejects_a_bad_length(place, color: tuple) -> None:
    """Verify a colour must have three or four components."""
    side = place("minecraft:standing_sign").capture_state().get_side(Side.FRONT)
    with pytest.raises(ValueError) as err_info:
        side.color = color
    assert str(err_info.value) == "Color tuple must have length 3 or 4"


def test_sign_waxed(place) -> None:
    """Verify the waxed flag round-trips."""
    state = place("minecraft:standing_sign").capture_state()
    assert state.waxed is False
    state.waxed = True
    assert state.waxed is True
