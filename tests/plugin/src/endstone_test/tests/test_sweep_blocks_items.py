import pytest
from endstone import Server
from endstone.command import Command, CommandSender, CommandSenderWrapper
from endstone.inventory import CrossbowMeta, ItemStack, PotionMeta


def test_written_book_title_is_not_cut_inside_a_character() -> None:
    item = ItemStack("minecraft:written_book")
    meta = item.item_meta
    meta.title = "€" * 5
    assert meta.set_title("€" * 6) is False
    item.set_item_meta(meta)
    title = item.item_meta.title
    assert "�" not in title
    assert title == "€" * 5


def test_crossbow_charged_projectiles_round_trip() -> None:
    item = ItemStack("minecraft:crossbow")
    meta = item.item_meta
    assert isinstance(meta, CrossbowMeta)
    meta.charged_projectiles = [ItemStack("minecraft:arrow", 1)]
    item.set_item_meta(meta)
    charged = item.item_meta.charged_projectiles[0]
    assert charged.type == "minecraft:arrow"
    assert charged.amount == 1


def test_base_potion_type_alone_is_applied() -> None:
    item = ItemStack("minecraft:potion")
    before = item.data
    meta = item.item_meta
    assert isinstance(meta, PotionMeta)
    meta.base_potion_type = "minecraft:potion_type:swiftness"
    item.set_item_meta(meta)
    assert item.item_meta.base_potion_type == "minecraft:potion_type:swiftness"
    assert item.data != before


def test_item_meta_of_another_family_is_rejected() -> None:
    crossbow_meta = ItemStack("minecraft:crossbow").item_meta
    crossbow_meta.charged_projectiles = [ItemStack("minecraft:arrow", 1)]
    sword = ItemStack("minecraft:diamond_sword")
    with pytest.raises(RuntimeError):
        sword.item_meta = crossbow_meta


class SenderProbe(Command):
    def __init__(self, name: str, **kwargs):
        super().__init__(name, **kwargs)
        self.senders: list[CommandSender] = []

    def execute(self, sender: CommandSender, args: list[str]) -> bool:
        self.senders.append(sender)
        return True


def test_sender_from_execute_can_be_forwarded_to_dispatch(server: Server) -> None:
    probe = SenderProbe("endstone_test_sweep_fwd")
    assert server.command_map.register_command(probe) is True
    server.dispatch_command(server.command_sender, "execute positioned 0 100 0 run endstone_test_sweep_fwd")
    assert len(probe.senders) == 1
    errors = []
    wrapper = CommandSenderWrapper(probe.senders[0], on_error=errors.append)
    server.dispatch_command(wrapper, "list")
    assert [str(e) for e in errors] == []
