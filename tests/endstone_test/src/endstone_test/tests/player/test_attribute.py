from endstone import Player
from endstone import attribute


def test_get_attributes(player: Player) -> None:
    attr=player.get_attribute(attribute.Attribute.HEALTH)
    attr.add_modifier(attribute.AttributeModifier("test:test", 100, attribute.AttributeModifier.ADD))
    assert len(attr.modifiers)==1
    attr.remove_modifier(attribute.AttributeModifier("test:test", 100, attribute.AttributeModifier.ADD))
    assert len(attr.modifiers)==0