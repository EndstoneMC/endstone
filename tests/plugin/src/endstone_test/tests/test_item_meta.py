import pytest
from endstone.inventory import (
    BookMeta,
    BookMetaGeneration,
    CrossbowMeta,
    ItemMeta,
    ItemStack,
    WritableBookMeta,
)

# =============================================================================
# Section 1: Meta types by item
# =============================================================================


@pytest.mark.parametrize(
    "item_type,meta_type",
    [
        ("minecraft:writable_book", WritableBookMeta),
        ("minecraft:written_book", BookMeta),
        ("minecraft:crossbow", CrossbowMeta),
        ("minecraft:diamond_sword", ItemMeta),
    ],
)
def test_meta_type_for_item(item_type: str, meta_type: type) -> None:
    """Verify each item hands back the meta type that matches it."""
    assert isinstance(ItemStack(item_type).item_meta, meta_type)


def test_book_meta_is_a_writable_book_meta() -> None:
    """Verify BookMeta extends WritableBookMeta, as the page API is shared."""
    assert issubclass(BookMeta, WritableBookMeta)
    assert isinstance(ItemStack("minecraft:written_book").item_meta, WritableBookMeta)


# =============================================================================
# Section 2: WritableBookMeta
# =============================================================================


def test_pages_round_trip() -> None:
    """Verify pages can be written and read back through the item."""
    item = ItemStack("minecraft:writable_book")
    meta = item.item_meta
    assert meta.has_pages is False

    meta.pages = ["one", "two"]
    item.set_item_meta(meta)

    assert item.item_meta.pages == ["one", "two"]
    assert item.item_meta.page_count == 2
    assert item.item_meta.has_pages is True


def test_get_and_set_page_are_one_indexed() -> None:
    """Verify get_page and set_page count from one, unlike the pages list."""
    item = ItemStack("minecraft:writable_book")
    meta = item.item_meta
    meta.pages = ["first", "second"]

    assert meta.get_page(1) == "first"
    assert meta.get_page(2) == "second"

    meta.set_page(1, "rewritten")
    assert meta.get_page(1) == "rewritten"
    assert meta.pages[0] == "rewritten"


def test_add_page_takes_varargs() -> None:
    """Verify add_page appends any number of pages at once."""
    meta = ItemStack("minecraft:writable_book").item_meta
    meta.add_page("one")
    meta.add_page("two", "three")
    assert meta.pages == ["one", "two", "three"]
    assert meta.page_count == 3


def test_set_page_truncates_a_long_page() -> None:
    """Verify a page longer than the limit is truncated rather than rejected."""
    meta = ItemStack("minecraft:writable_book").item_meta
    meta.pages = [""]
    meta.set_page(1, "x" * 2000)
    assert len(meta.get_page(1)) <= 1024


def test_page_count_is_capped() -> None:
    """Verify a book cannot hold more than fifty pages."""
    meta = ItemStack("minecraft:writable_book").item_meta
    meta.add_page(*[f"page {i}" for i in range(60)])
    assert meta.page_count <= 50


# =============================================================================
# Section 3: BookMeta
# =============================================================================


def test_title_and_author() -> None:
    """Verify a written book keeps its title and author."""
    item = ItemStack("minecraft:written_book")
    meta = item.item_meta
    assert meta.has_title is False
    assert meta.has_author is False

    meta.title = "The Book"
    meta.author = "Vincent"
    item.set_item_meta(meta)

    assert item.item_meta.title == "The Book"
    assert item.item_meta.author == "Vincent"
    assert item.item_meta.has_title is True
    assert item.item_meta.has_author is True


@pytest.mark.parametrize("generation", list(BookMetaGeneration))
def test_generation_round_trip(generation: BookMetaGeneration) -> None:
    """Verify every book generation round-trips through the item."""
    item = ItemStack("minecraft:written_book")
    meta = item.item_meta
    meta.generation = generation
    item.set_item_meta(meta)

    assert item.item_meta.generation == generation
    assert item.item_meta.has_generation is True


def test_generation_members() -> None:
    """Verify the book generations match Bukkit's."""
    assert [member.name for member in BookMetaGeneration] == [
        "ORIGINAL",
        "COPY_OF_ORIGINAL",
        "COPY_OF_COPY",
    ]


def test_clearing_the_title() -> None:
    """Verify assigning None clears the title again."""
    meta = ItemStack("minecraft:written_book").item_meta
    meta.title = "The Book"
    meta.title = None
    assert meta.has_title is False


def test_title_of_sixteen_multibyte_characters_is_accepted() -> None:
    """Verify the title limit counts characters, not bytes."""
    meta = ItemStack("minecraft:written_book").item_meta
    assert meta.set_title("€" * 16) is True
    assert meta.title == "€" * 16


def test_set_title_rejects_a_title_over_sixteen_characters() -> None:
    """Verify set_title refuses a long title and keeps the current one."""
    meta = ItemStack("minecraft:written_book").item_meta
    meta.title = "Kept"
    assert meta.set_title("x" * 17) is False
    assert meta.title == "Kept"


def test_assigning_a_title_over_sixteen_characters_raises() -> None:
    """Verify assigning a long title raises instead of cutting it."""
    meta = ItemStack("minecraft:written_book").item_meta
    with pytest.raises(ValueError):
        meta.title = "x" * 17


def test_set_title_none_clears_the_title() -> None:
    """Verify set_title(None) removes the title and reports success."""
    meta = ItemStack("minecraft:written_book").item_meta
    meta.title = "The Book"
    assert meta.set_title(None) is True
    assert meta.has_title is False


# =============================================================================
# Section 4: CrossbowMeta
# =============================================================================


def test_charged_projectile_round_trip() -> None:
    """Verify a crossbow keeps the single projectile charged into it."""
    item = ItemStack("minecraft:crossbow")
    meta = item.item_meta
    assert meta.has_charged_projectile is False
    assert meta.charged_projectile is None

    meta.charged_projectile = ItemStack("minecraft:arrow")
    item.set_item_meta(meta)

    assert item.item_meta.has_charged_projectile is True
    assert item.item_meta.charged_projectile.type == "minecraft:arrow"


def test_clearing_the_charged_projectile() -> None:
    """Verify assigning None discharges the crossbow."""
    meta = ItemStack("minecraft:crossbow").item_meta
    meta.charged_projectile = ItemStack("minecraft:arrow")
    meta.charged_projectile = None
    assert meta.has_charged_projectile is False


def test_crossbow_has_no_plural_accessor() -> None:
    """Verify Bedrock's single-projectile crossbow has no Bukkit-style list."""
    assert not hasattr(ItemStack("minecraft:crossbow").item_meta, "charged_projectiles")


# =============================================================================
# Section 5: Shared ItemMeta surface
# =============================================================================


def test_repair_cost() -> None:
    """Verify the repair cost is read and written."""
    meta = ItemStack("minecraft:diamond_sword").item_meta
    assert meta.has_repair_cost is False
    meta.repair_cost = 7
    assert meta.repair_cost == 7
    assert meta.has_repair_cost is True


def test_unbreakable() -> None:
    """Verify the unbreakable flag is read and written."""
    meta = ItemStack("minecraft:diamond_sword").item_meta
    assert meta.is_unbreakable is False
    meta.is_unbreakable = True
    assert meta.is_unbreakable is True


def test_clone_is_independent() -> None:
    """Verify a cloned meta does not share state with its source."""
    meta = ItemStack("minecraft:diamond_sword").item_meta
    meta.display_name = "Original"

    clone = meta.clone()
    assert clone.display_name == "Original"

    clone.display_name = "Copy"
    assert meta.display_name == "Original"


def test_remove_enchants() -> None:
    """Verify remove_enchants strips every enchantment at once."""
    meta = ItemStack("minecraft:diamond_sword").item_meta
    meta.add_enchant("sharpness", 3, True)
    meta.add_enchant("knockback", 1, True)
    assert meta.has_enchants is True

    meta.remove_enchants()
    assert meta.has_enchants is False
    assert meta.enchants == {}


# =============================================================================
# Section 6: Applying meta to another item type
# =============================================================================


def test_meta_of_another_item_type_is_not_applicable() -> None:
    """Verify a CrossbowMeta carrying data is rejected by a sword."""
    meta = ItemStack("minecraft:crossbow").item_meta
    meta.charged_projectile = ItemStack("minecraft:arrow", 1)
    sword = ItemStack("minecraft:diamond_sword")
    assert sword.set_item_meta(meta) is False


def test_empty_meta_of_another_item_type_clears_the_item() -> None:
    """Verify an empty CrossbowMeta on a sword clears the sword's data, as an empty meta of any type does."""
    sword = ItemStack("minecraft:diamond_sword")
    named = sword.item_meta
    named.display_name = "Blade"
    assert sword.set_item_meta(named) is True
    assert sword.set_item_meta(ItemStack("minecraft:crossbow").item_meta) is True
    assert sword.item_meta.has_display_name is False


def test_book_meta_is_not_applicable_to_a_writable_book() -> None:
    """Verify a signed book's meta is rejected by a book and quill."""
    meta = ItemStack("minecraft:written_book").item_meta
    meta.title = "Title"
    assert ItemStack("minecraft:writable_book").set_item_meta(meta) is False


def test_writable_book_meta_is_applicable_to_a_writable_book() -> None:
    """Verify a book and quill takes its own meta."""
    meta = ItemStack("minecraft:writable_book").item_meta
    meta.add_page("page")
    book = ItemStack("minecraft:writable_book")
    assert book.set_item_meta(meta) is True
    assert book.item_meta.pages == ["page"]


def test_plain_item_meta_is_applicable_to_a_crossbow() -> None:
    """Verify the plain ItemMeta applies to an item with a specialised meta type."""
    meta = ItemStack("minecraft:diamond_sword").item_meta
    meta.display_name = "Bow"
    crossbow = ItemStack("minecraft:crossbow")
    assert crossbow.set_item_meta(meta) is True
    assert crossbow.item_meta.display_name == "Bow"


def test_base_potion_type_alone_is_applied_to_a_potion() -> None:
    """Verify a PotionMeta that only sets the base potion type changes the potion."""
    potion = ItemStack("minecraft:potion")
    meta = potion.item_meta
    meta.base_potion_type = "minecraft:potion_type:swiftness"
    assert potion.set_item_meta(meta) is True
    assert potion.item_meta.base_potion_type == "minecraft:potion_type:swiftness"
