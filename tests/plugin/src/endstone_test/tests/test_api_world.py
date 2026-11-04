import pytest
from endstone import Identifier, Server
from endstone.block import Biome, BlockFace
from endstone.command import CommandSenderWrapper
from endstone.inventory import (
    ComplexAliasIngredient,
    CrossbowMeta,
    ItemMeta,
    ItemStack,
    ItemType,
    ItemTypeIngredient,
    MapMeta,
    PotionMeta,
    RecipeIngredient,
    ShapedRecipe,
    SmithingRecipe,
    SmithingTransformRecipe,
    SmithingTrimRecipe,
)
from endstone.level import Dimension, Location
from endstone.map import MapRenderer, MapView
from endstone.nbt import CompoundTag, LongTag, StringTag


def run_command(server: Server, command_line: str) -> list[str]:
    output: list[str] = []
    sender = CommandSenderWrapper(
        server.command_sender,
        on_message=lambda message: output.append(str(message)),
        on_error=lambda message: output.append(str(message)),
    )
    assert server.dispatch_command(sender, command_line), f"{command_line!r} failed"
    return output


@pytest.fixture
def overworld(server: Server) -> Dimension:
    return server.level.get_dimension(Dimension.OVERWORLD)


def plot_chunk(overworld: Dimension, plot):
    x, _, z = plot.pos(0, 0, 0)
    return next(chunk for chunk in overworld.loaded_chunks if (chunk.x, chunk.z) == (x >> 4, z >> 4))


class RecordingRenderer(MapRenderer):
    def __init__(self):
        MapRenderer.__init__(self, is_contextual=False)
        self.initialized = []

    def initialize(self, view: MapView) -> None:
        self.initialized.append(view.id)


def test_biome_get_returns_the_registry_instance(server: Server) -> None:
    plains = Biome.get("minecraft:plains")
    assert plains is not None
    assert plains is server.get_registry(Biome).get_or_throw("minecraft:plains")
    assert Biome.get(Identifier("minecraft", "plains")) is plains
    assert Biome.get("minecraft:desert") != plains
    assert Biome.get("minecraft:not_a_biome") is None


def test_block_str_describes_position_type_and_data(overworld: Dimension, plot) -> None:
    x, y, z = plot.pos(2, 1, 3)
    block = overworld.get_block_at(x, y, z)
    try:
        block.set_type("minecraft:gold_block")
        text = str(overworld.get_block_at(x, y, z))
        assert text.startswith("Block(")
        assert f"x={x}, y={y}, z={z}" in text
        assert "type=minecraft:gold_block" in text
    finally:
        block.set_type("minecraft:air")
    assert "type=minecraft:air" in str(overworld.get_block_at(x, y, z))


def test_block_get_relative_by_offset(overworld: Dimension, plot) -> None:
    x, y, z = plot.pos(4, 2, 4)
    origin = overworld.get_block_at(x, y, z)
    target = overworld.get_block_at(x + 1, y + 2, z - 3)
    try:
        target.set_type("minecraft:emerald_block")
        relative = origin.get_relative(1, 2, -3)
        assert (relative.x, relative.y, relative.z) == (x + 1, y + 2, z - 3)
        assert relative.type == "minecraft:emerald_block"
        assert origin.get_relative(0, 0, 0).type == origin.type
        back = origin.get_relative(-4, -2, -4)
        assert (back.x, back.y, back.z) == (x - 4, y - 2, z - 4)
    finally:
        target.set_type("minecraft:air")


@pytest.mark.parametrize(
    "face,delta",
    [
        (BlockFace.UP, (0, 1, 0)),
        (BlockFace.DOWN, (0, -1, 0)),
        (BlockFace.NORTH, (0, 0, -1)),
        (BlockFace.SOUTH, (0, 0, 1)),
        (BlockFace.EAST, (1, 0, 0)),
        (BlockFace.WEST, (-1, 0, 0)),
        (BlockFace.SELF, (0, 0, 0)),
    ],
)
def test_block_get_relative_by_face(overworld: Dimension, plot, face: BlockFace, delta) -> None:
    x, y, z = plot.pos(6, 3, 6)
    origin = overworld.get_block_at(x, y, z)
    one = origin.get_relative(face)
    assert (one.x - x, one.y - y, one.z - z) == delta
    three = origin.get_relative(face, 3)
    assert (three.x - x, three.y - y, three.z - z) == tuple(3 * d for d in delta)


def test_block_set_data_applies_block_states(server: Server, overworld: Dimension, plot) -> None:
    x, y, z = plot.pos(8, 1, 8)
    block = overworld.get_block_at(x, y, z)
    try:
        axis_x = server.create_block_data("minecraft:oak_log", {"pillar_axis": "x"})
        block.set_data(axis_x)
        placed = overworld.get_block_at(x, y, z)
        assert placed.type == "minecraft:oak_log"
        assert placed.data.block_states["pillar_axis"] == "x"
        assert placed.data.runtime_id == axis_x.runtime_id

        block.set_data(server.create_block_data("minecraft:oak_log", {"pillar_axis": "z"}), apply_physics=False)
        assert overworld.get_block_at(x, y, z).data.block_states["pillar_axis"] == "z"

        block.set_data(server.create_block_data("minecraft:stone"))
        assert overworld.get_block_at(x, y, z).type == "minecraft:stone"
    finally:
        block.set_type("minecraft:air")


def test_chunk_load_and_unload_on_a_ticking_chunk(overworld: Dimension, plot) -> None:
    chunk = plot_chunk(overworld, plot)
    assert chunk.is_loaded
    assert chunk.load() is True
    assert chunk.load(generate=False) is True
    assert chunk.unload() is False
    assert chunk.is_loaded
    assert overworld.is_chunk_loaded(chunk.x, chunk.z)
    assert chunk.unload() is False


def test_chunk_unload_matches_dimension_unload(overworld: Dimension, plot) -> None:
    chunk = plot_chunk(overworld, plot)
    assert chunk.unload() == overworld.unload_chunk(chunk.x, chunk.z)
    assert chunk.load() == overworld.load_chunk(chunk.x, chunk.z)
    assert chunk.unload() == overworld.unload_chunk(chunk.x, chunk.z)


def test_dimension_players_is_empty_without_a_player(server: Server) -> None:
    assert not server.online_players
    for dimension in server.level.dimensions:
        assert dimension.players == []


def test_dimension_players_partition_the_online_players(server: Server) -> None:
    listed = [player for dimension in server.level.dimensions for player in dimension.players]
    assert sorted(p.name for p in listed) == sorted(p.name for p in server.online_players)
    for dimension in server.level.dimensions:
        assert all(p.dimension.id == dimension.id for p in dimension.players)


def test_level_seed_matches_the_seed_command(server: Server) -> None:
    seed = server.level.seed
    assert isinstance(seed, int)
    assert any(str(seed) in line for line in run_command(server, "seed"))
    assert server.level.seed == seed


def test_level_time_set_is_what_the_time_command_adds_to(server: Server) -> None:
    level = server.level
    original = level.time
    try:
        for value in (0, 1234, 6000, 18000):
            level.time = value
            assert level.time == value
            run_command(server, "time add 100")
            assert level.time == value + 100
        level.time = 5000
        run_command(server, "time set 0")
        assert level.time == 0
    finally:
        level.time = original


def test_level_time_follows_the_time_command(server: Server) -> None:
    level = server.level
    original = level.time
    try:
        run_command(server, "time set 4321")
        assert level.time % 24000 == 4321
        run_command(server, "time add 100")
        assert level.time % 24000 == 4421
    finally:
        level.time = original


def test_level_recipes_match_the_server_recipes(server: Server) -> None:
    recipes = server.level.recipes
    assert recipes
    assert [(r.tag, r.id) for r in recipes] == [(r.tag, r.id) for r in server.recipes]


def find_recipes(server: Server, cls):
    return [recipe for recipe in server.level.recipes if isinstance(recipe, cls)]


def test_every_ingredient_consumes_a_positive_count(server: Server) -> None:
    seen = 0
    for recipe in server.level.recipes:
        for ingredient in recipe.ingredients:
            if ingredient is not None:
                assert isinstance(ingredient, RecipeIngredient)
                assert ingredient.count >= 1
                seen += 1
    assert seen > 1000


def test_ingredient_count_adds_up_to_the_crafted_amount(server: Server) -> None:
    ingots = [
        r
        for r in server.level.recipes
        if r.tag == "crafting_table" and r.result.type == "minecraft:iron_ingot" and r.result.amount == 9
    ]
    assert ingots
    for recipe in ingots:
        assert sum(i.count for i in recipe.ingredients if i is not None) == 1

    sticks = [
        r
        for r in find_recipes(server, ShapedRecipe)
        if r.result.type == "minecraft:stick" and r.result.amount == 4
    ]
    assert sticks
    for recipe in sticks:
        assert sum(i.count for i in recipe.ingredients if i is not None) == 2


def test_complex_alias_ingredient_reports_its_alias(server: Server) -> None:
    aliases = [
        ingredient
        for recipe in server.level.recipes
        for ingredient in recipe.ingredients
        if isinstance(ingredient, ComplexAliasIngredient)
    ]
    assert aliases
    assert all(isinstance(i.alias, str) and i.alias.startswith("minecraft:") for i in aliases)
    assert "minecraft:log" in {i.alias for i in aliases}


def test_shaped_recipe_size_matches_its_grid(server: Server) -> None:
    shaped = find_recipes(server, ShapedRecipe)
    assert shaped
    for recipe in shaped:
        assert 1 <= recipe.width <= 3
        assert 1 <= recipe.height <= 3
        assert len(recipe.ingredients) == recipe.width * recipe.height


@pytest.mark.parametrize(
    "result,size",
    [
        ("minecraft:wooden_sword", (1, 3)),
        ("minecraft:crafting_table", (2, 2)),
        ("minecraft:bucket", (3, 2)),
        ("minecraft:chest", (3, 3)),
    ],
)
def test_shaped_recipe_known_sizes(server: Server, result: str, size) -> None:
    recipes = [r for r in find_recipes(server, ShapedRecipe) if r.result.type == result and r.tag == "crafting_table"]
    assert recipes
    assert (recipes[0].width, recipes[0].height) == size


def test_smithing_transform_recipe_parts(server: Server) -> None:
    recipes = find_recipes(server, SmithingTransformRecipe)
    assert recipes
    axe = next(r for r in recipes if r.result.type == "minecraft:netherite_axe")
    assert isinstance(axe, SmithingRecipe)
    assert isinstance(axe.template, ItemTypeIngredient)
    assert axe.template.item_type == "minecraft:netherite_upgrade_smithing_template"
    assert axe.base.item_type == "minecraft:diamond_axe"
    assert axe.addition.item_type == "minecraft:netherite_ingot"
    assert axe.base.test(ItemStack("minecraft:diamond_axe"))
    assert not axe.base.test(ItemStack("minecraft:iron_axe"))
    assert axe.addition.test(ItemStack("minecraft:netherite_ingot"))
    assert axe.template.test(ItemStack("minecraft:netherite_upgrade_smithing_template"))
    for recipe in recipes:
        assert recipe.template is not None
        assert recipe.base is not None
        assert recipe.addition is not None
        assert recipe.base.test(ItemStack(recipe.base.item_type)) is True


def test_smithing_trim_recipe_parts(server: Server) -> None:
    recipes = find_recipes(server, SmithingTrimRecipe)
    assert recipes
    for recipe in recipes:
        assert isinstance(recipe, SmithingRecipe)
        assert not isinstance(recipe, SmithingTransformRecipe)
        assert recipe.template is not None
        assert recipe.base is not None
        assert recipe.addition is not None
        assert recipe.template.item_tag
        assert recipe.base.item_tag
        assert recipe.addition.item_tag
        assert recipe.template.item_tag != recipe.base.item_tag


def test_item_stack_amount_set_survives_a_drop(overworld: Dimension, plot) -> None:
    x, y, z = plot.pos(3, 0, 3)
    stack = ItemStack("minecraft:cobblestone", 1)
    stack.amount = 17
    assert str(stack) == "ItemStack(minecraft:cobblestone x 17)"
    dropped = overworld.drop_item(Location(overworld, x + 0.5, y + 1, z + 0.5), stack)
    try:
        assert dropped.item_stack.amount == 17
        assert dropped.item_stack == stack
        stack.amount = 3
        assert dropped.item_stack.amount == 17
    finally:
        dropped.remove()


@pytest.mark.parametrize("amount", [0, -1, 256])
def test_item_stack_amount_set_rejects_out_of_range(amount: int) -> None:
    stack = ItemStack("minecraft:apple", 5)
    with pytest.raises(ValueError, match="between 1 to 255"):
        stack.amount = amount
    assert stack.amount == 5


def test_item_stack_data_set_changes_the_item(server: Server, overworld: Dimension, plot) -> None:
    x, y, z = plot.pos(5, 0, 5)
    stack = ItemStack("minecraft:potion")
    before = stack.translation_key
    stack.data = 7
    assert stack.data == 7
    assert stack.translation_key != before
    assert stack.translation_key == ItemStack("minecraft:potion", data=7).translation_key
    assert server.language.translate(stack.translation_key, locale="zh_CN") == "隐身药水"
    dropped = overworld.drop_item(Location(overworld, x + 0.5, y + 1, z + 0.5), stack)
    try:
        assert dropped.item_stack.data == 7
        assert dropped.item_stack.translation_key == stack.translation_key
    finally:
        dropped.remove()


def test_item_stack_equality() -> None:
    apple = ItemStack("minecraft:apple", 5)
    assert apple == ItemStack("minecraft:apple", 5)
    assert not apple != ItemStack("minecraft:apple", 5)
    assert apple != ItemStack("minecraft:apple", 4)
    assert apple != ItemStack("minecraft:golden_apple", 5)
    assert apple != None  # noqa: E711
    assert apple != "minecraft:apple"
    assert not apple == 5
    assert apple == apple

    named = ItemStack("minecraft:apple", 5)
    meta = named.item_meta
    meta.display_name = "Shiny"
    assert named.set_item_meta(meta)
    assert named != apple
    assert not named == apple
    plain = ItemStack("minecraft:apple", 5)
    plain.set_item_meta(meta)
    assert named == plain


def test_item_stack_str() -> None:
    assert str(ItemStack("minecraft:apple", 5)) == "ItemStack(minecraft:apple x 5)"
    assert str(ItemStack("apple")) == "ItemStack(minecraft:apple x 1)"
    assert str(ItemStack("minecraft:diamond_sword", 1)) == "ItemStack(minecraft:diamond_sword x 1)"


def test_item_stack_is_similar_ignores_only_the_amount() -> None:
    apple = ItemStack("minecraft:apple", 5)
    assert apple.is_similar(ItemStack("minecraft:apple", 1))
    assert ItemStack("minecraft:apple", 1).is_similar(apple)
    assert apple.is_similar(apple)
    assert not apple.is_similar(ItemStack("minecraft:golden_apple", 5))

    lored = ItemStack("minecraft:apple", 5)
    meta = lored.item_meta
    meta.lore = ["tasty"]
    lored.set_item_meta(meta)
    assert not apple.is_similar(lored)
    assert not lored.is_similar(apple)
    twin = ItemStack("minecraft:apple", 9)
    twin.set_item_meta(meta)
    assert lored.is_similar(twin)


def test_item_stack_nbt_set_drives_the_map_id() -> None:
    stack = ItemStack("minecraft:filled_map")
    assert stack.item_meta.has_map_id is False
    stack.nbt = CompoundTag({"map_uuid": LongTag(4242)})
    assert stack.nbt["map_uuid"].value == 4242
    meta = stack.item_meta
    assert isinstance(meta, MapMeta)
    assert meta.has_map_id is True
    assert meta.map_id == 4242


def test_item_stack_nbt_set_drives_the_display_name() -> None:
    stack = ItemStack("minecraft:apple")
    assert stack.item_meta.has_display_name is False
    stack.nbt = CompoundTag({"display": CompoundTag({"Name": StringTag("Renamed")})})
    meta = stack.item_meta
    assert meta.has_display_name is True
    assert meta.display_name == "Renamed"
    stack.nbt = CompoundTag()
    assert stack.item_meta.has_display_name is False


def test_item_stack_nbt_set_is_dropped_with_the_item(overworld: Dimension, plot) -> None:
    x, y, z = plot.pos(7, 0, 7)
    stack = ItemStack("minecraft:apple")
    stack.nbt = CompoundTag({"display": CompoundTag({"Name": StringTag("Dropped")})})
    dropped = overworld.drop_item(Location(overworld, x + 0.5, y + 1, z + 0.5), stack)
    try:
        assert dropped.item_stack.item_meta.display_name == "Dropped"
        assert dropped.item_stack == stack
    finally:
        dropped.remove()


def test_item_type_inequality() -> None:
    apple = ItemType.get("minecraft:apple")
    stone = ItemType.get("minecraft:stone")
    assert apple != stone
    assert not apple != ItemType.get("minecraft:apple")
    assert apple != "minecraft:stone"
    assert not apple != "minecraft:apple"
    assert apple != None  # noqa: E711
    assert apple != 5
    assert hash(apple) != hash(stone)


def test_item_type_create_item_stack() -> None:
    sword = ItemType.get("minecraft:diamond_sword")
    stack = sword.create_item_stack()
    assert isinstance(stack, ItemStack)
    assert stack.type == sword
    assert stack.amount == 1
    assert stack.data == 0
    assert stack == ItemStack("minecraft:diamond_sword")

    apples = ItemType.get("minecraft:apple").create_item_stack(12)
    assert apples.amount == 12
    assert apples.type == "minecraft:apple"
    assert apples.max_stack_size == ItemType.get("minecraft:apple").max_stack_size

    assert ItemType.get("minecraft:stone").create_item_stack(64).amount == 64


def test_item_type_get_translation_key_by_data(server: Server) -> None:
    potion = ItemType.get("minecraft:potion")
    assert potion.get_translation_key() == potion.translation_key
    assert potion.get_translation_key(0) == potion.translation_key
    invisibility = potion.get_translation_key(7)
    assert invisibility != potion.get_translation_key(0)
    assert invisibility == ItemStack("minecraft:potion", data=7).translation_key
    assert server.language.translate(invisibility, locale="zh_CN") == "隐身药水"
    assert server.language.translate(potion.get_translation_key(0), locale="zh_CN") == "水瓶"
    apple = ItemType.get("minecraft:apple")
    assert apple.get_translation_key(0) == apple.get_translation_key()


def test_item_meta_has_damage_follows_damage(server: Server) -> None:
    meta = ItemStack("minecraft:diamond_sword").item_meta
    assert meta.has_damage is False
    assert meta.damage == 0
    meta.damage = 12
    assert meta.has_damage is True
    assert meta.damage == 12
    meta.damage = 0
    assert meta.has_damage is False

    stack = ItemStack("minecraft:diamond_sword")
    meta.damage = 30
    assert stack.set_item_meta(meta)
    assert stack.item_meta.has_damage is True
    assert stack.item_meta.damage == 30
    assert ItemStack("minecraft:diamond_sword").item_meta.has_damage is False
    assert server.item_factory.equals(stack.item_meta, ItemStack("minecraft:diamond_sword").item_meta) is False


def test_potion_meta_has_base_potion_type() -> None:
    meta = ItemStack("minecraft:potion").item_meta
    assert isinstance(meta, PotionMeta)
    assert meta.has_base_potion_type is True
    assert meta.base_potion_type == "minecraft:potion_type:water"

    stack = ItemStack("minecraft:potion", data=7)
    assert stack.item_meta.has_base_potion_type is True
    assert stack.item_meta.base_potion_type != meta.base_potion_type

    meta.base_potion_type = None
    assert meta.has_base_potion_type is False
    assert meta.base_potion_type is None
    meta.base_potion_type = stack.item_meta.base_potion_type
    assert meta.has_base_potion_type is True
    assert meta.base_potion_type == stack.item_meta.base_potion_type


def test_crossbow_meta_add_charged_projectile() -> None:
    meta = ItemStack("minecraft:crossbow").item_meta
    assert isinstance(meta, CrossbowMeta)
    assert meta.has_charged_projectiles is False
    meta.add_charged_projectile(ItemStack("minecraft:arrow", 1))
    assert meta.has_charged_projectiles is True
    assert [p.type.id for p in meta.charged_projectiles] == ["minecraft:arrow"]

    with pytest.raises(ValueError):
        meta.add_charged_projectile(ItemStack("minecraft:arrow", 1))
    assert len(meta.charged_projectiles) == 1

    crossbow = ItemStack("minecraft:crossbow")
    assert crossbow.set_item_meta(meta)
    assert crossbow.item_meta.has_charged_projectiles is True
    assert crossbow.item_meta.charged_projectiles[0].type == "minecraft:arrow"
    assert crossbow != ItemStack("minecraft:crossbow")

    fresh = ItemStack("minecraft:crossbow").item_meta
    fresh.add_charged_projectile(ItemStack("minecraft:firework_rocket", 1))
    assert fresh.charged_projectiles[0].type == "minecraft:firework_rocket"


def test_item_factory_get_item_meta(server: Server) -> None:
    factory = server.item_factory
    assert factory.get_item_meta("minecraft:air") is None
    assert type(factory.get_item_meta("minecraft:apple")) is ItemMeta
    assert isinstance(factory.get_item_meta("minecraft:crossbow"), CrossbowMeta)
    assert isinstance(factory.get_item_meta("minecraft:filled_map"), MapMeta)
    assert isinstance(factory.get_item_meta("minecraft:potion"), PotionMeta)
    assert isinstance(factory.get_item_meta(Identifier("minecraft", "crossbow")), CrossbowMeta)
    first = factory.get_item_meta("minecraft:apple")
    first.display_name = "Mine"
    assert factory.get_item_meta("minecraft:apple").has_display_name is False


def test_item_factory_is_applicable(server: Server) -> None:
    factory = server.item_factory
    named = factory.get_item_meta("minecraft:apple")
    named.display_name = "Named"
    assert factory.is_applicable(named, "minecraft:apple")
    assert factory.is_applicable(named, "minecraft:diamond_sword")
    assert not factory.is_applicable(named, "minecraft:air")

    crossbow = factory.get_item_meta("minecraft:crossbow")
    crossbow.add_charged_projectile(ItemStack("minecraft:arrow"))
    assert factory.is_applicable(crossbow, "minecraft:crossbow")
    assert not factory.is_applicable(crossbow, "minecraft:diamond_sword")
    assert not factory.is_applicable(crossbow, "minecraft:apple")
    assert not factory.is_applicable(crossbow, Identifier("minecraft", "air"))


def test_item_factory_equals(server: Server) -> None:
    factory = server.item_factory
    a = factory.get_item_meta("minecraft:diamond_sword")
    b = factory.get_item_meta("minecraft:diamond_sword")
    assert factory.equals(a, b)
    assert factory.equals(a, a.clone())

    a.display_name = "Blade"
    assert not factory.equals(a, b)
    assert not factory.equals(b, a)
    b.display_name = "Blade"
    assert factory.equals(a, b)

    a.lore = ["sharp"]
    assert not factory.equals(a, b)
    a.lore = None
    assert factory.equals(a, b)

    assert factory.equals(None, None)
    assert not factory.equals(a, None)
    assert not factory.equals(None, a)


def test_item_factory_as_meta_for(server: Server) -> None:
    factory = server.item_factory
    generic = factory.get_item_meta("minecraft:apple")
    generic.display_name = "Carried"
    generic.lore = ["one", "two"]

    crossbow = factory.as_meta_for(generic, "minecraft:crossbow")
    assert isinstance(crossbow, CrossbowMeta)
    assert crossbow.display_name == "Carried"
    assert crossbow.lore == ["one", "two"]
    assert factory.is_applicable(crossbow, "minecraft:crossbow")

    back = factory.as_meta_for(crossbow, "minecraft:apple")
    assert type(back) is ItemMeta
    assert back.display_name == "Carried"

    sword = factory.as_meta_for(generic, "minecraft:diamond_sword")
    assert factory.is_applicable(sword, "minecraft:diamond_sword")
    assert sword.display_name == "Carried"

    assert factory.as_meta_for(generic, "minecraft:air") is None
    assert generic.display_name == "Carried"


def test_map_meta_map_id_round_trip(server: Server) -> None:
    stack = ItemStack("minecraft:filled_map")
    meta = stack.item_meta
    assert meta.has_map_id is False
    assert meta.has_map_view is False
    assert meta.map_id == -1

    view = server.create_map(server.level.get_dimension(Dimension.OVERWORLD))
    meta.map_id = view.id
    assert meta.has_map_id is True
    assert meta.map_id == view.id
    assert meta.has_map_view is True
    assert meta.map_view.id == view.id

    assert stack.set_item_meta(meta)
    assert stack.nbt["map_uuid"].value == view.id
    assert stack.item_meta.map_id == view.id
    assert stack.item_meta.map_view.id == view.id

    other = ItemStack("minecraft:filled_map")
    other.nbt = CompoundTag({"map_uuid": LongTag(view.id)})
    assert other.item_meta.map_id == view.id
    assert other == stack


def test_map_meta_map_id_set_to_an_unknown_map(server: Server) -> None:
    meta = ItemStack("minecraft:filled_map").item_meta
    unknown = 123456789
    assert server.get_map(unknown) is None
    meta.map_id = unknown
    assert meta.has_map_id is True
    assert meta.map_id == unknown


def test_map_meta_has_map_view_follows_the_map_id_for_an_unknown_map(server: Server) -> None:
    meta = ItemStack("minecraft:filled_map").item_meta
    meta.map_id = 123456789
    assert server.get_map(123456789) is None
    assert meta.has_map_view is True
    assert meta.map_view is None


def test_item_stack_equality_and_similarity_consider_data() -> None:
    water = ItemStack("minecraft:potion", 1, 0)
    invisibility = ItemStack("minecraft:potion", 1, 7)
    assert water.item_meta.base_potion_type != invisibility.item_meta.base_potion_type
    assert water != invisibility
    assert not water == invisibility
    assert not water.is_similar(invisibility)


def test_map_meta_map_view_follows_the_view(server: Server) -> None:
    overworld = server.level.get_dimension(Dimension.OVERWORLD)
    first = server.create_map(overworld)
    second = server.create_map(overworld)
    assert first.id != second.id

    meta = ItemStack("minecraft:filled_map").item_meta
    assert meta.map_view is None
    meta.map_view = first
    assert meta.map_id == first.id
    assert meta.map_view.id == first.id
    meta.map_view = second
    assert meta.map_id == second.id
    assert meta.map_view.id == second.id
    assert meta.has_map_view is True


def test_map_view_dimension_set_and_get(server: Server) -> None:
    overworld = server.level.get_dimension(Dimension.OVERWORLD)
    nether = server.level.get_dimension(Dimension.NETHER)
    view = server.create_map(overworld)
    assert view.dimension.id == "minecraft:overworld"

    view.dimension = nether
    assert view.dimension.id == "minecraft:nether"
    assert server.get_map(view.id).dimension.id == "minecraft:nether"

    view.dimension = overworld
    assert server.get_map(view.id).dimension.id == "minecraft:overworld"

    elsewhere = server.create_map(nether)
    assert elsewhere.dimension.id == "minecraft:nether"
    assert view.dimension.id == "minecraft:overworld"


def test_map_view_center_set(server: Server) -> None:
    overworld = server.level.get_dimension(Dimension.OVERWORLD)
    view = server.create_map(overworld)
    view.center_x = 1000
    view.center_z = -2000
    again = server.get_map(view.id)
    assert (again.center_x, again.center_z) == (1000, -2000)
    view.center_x = -64
    assert (again.center_x, again.center_z) == (-64, -2000)
    other = server.create_map(overworld)
    assert (other.center_x, other.center_z) != (-64, -2000)


def test_map_view_scale_set(server: Server) -> None:
    view = server.create_map(server.level.get_dimension(Dimension.OVERWORLD))
    for scale in (MapView.Scale.CLOSEST, MapView.Scale.FARTHEST, MapView.Scale.NORMAL, MapView.Scale.CLOSE):
        view.scale = scale
        assert view.scale == scale
        assert server.get_map(view.id).scale == scale
    assert MapView.CLOSEST == MapView.Scale.CLOSEST
    assert MapView.FARTHEST == 4


def test_map_view_locked_set(server: Server) -> None:
    overworld = server.level.get_dimension(Dimension.OVERWORLD)
    view = server.create_map(overworld)
    assert view.locked is False
    view.locked = True
    assert view.locked is True
    assert server.get_map(view.id).locked is True
    assert server.create_map(overworld).locked is False
    view.locked = False
    assert server.get_map(view.id).locked is False


def test_map_view_unlimited_tracking_set(server: Server) -> None:
    overworld = server.level.get_dimension(Dimension.OVERWORLD)
    view = server.create_map(overworld)
    assert view.is_unlimited_tracking is False
    view.is_unlimited_tracking = True
    assert view.is_unlimited_tracking is True
    assert server.get_map(view.id).is_unlimited_tracking is True
    assert server.create_map(overworld).is_unlimited_tracking is False
    view.is_unlimited_tracking = False
    assert server.get_map(view.id).is_unlimited_tracking is False


def test_map_view_is_virtual_follows_the_lowest_renderer(server: Server) -> None:
    view = server.create_map(server.level.get_dimension(Dimension.OVERWORLD))
    assert view.is_virtual is False
    for renderer in list(view.renderers):
        view.remove_renderer(renderer)
    plugin_renderer = RecordingRenderer()
    view.add_renderer(plugin_renderer)
    assert view.is_virtual is True
    view.remove_renderer(plugin_renderer)
    assert view.is_virtual is False


def test_map_renderer_initialize_is_called_when_added(server: Server) -> None:
    overworld = server.level.get_dimension(Dimension.OVERWORLD)
    view = server.create_map(overworld)
    other = server.create_map(overworld)
    renderer = RecordingRenderer()
    assert renderer.initialized == []
    view.add_renderer(renderer)
    assert renderer.initialized == [view.id]
    assert renderer in view.renderers
    other.add_renderer(renderer)
    assert renderer.initialized == [view.id, other.id]
    view.remove_renderer(renderer)
    other.remove_renderer(renderer)
    assert renderer.initialized == [view.id, other.id]
