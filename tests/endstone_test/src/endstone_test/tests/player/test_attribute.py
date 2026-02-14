from endstone import Player
from endstone import attribute
def test_get_attributes(player: Player)->None:
    for attr in player.attributes:
        player.send_message(f"{attr.type}:{attr.base_value},{attr.value},{attr.modifiers}")
        attr.add_modifier(attribute.AttributeModifier("test:test",100,attribute.AttributeModifier.ADD))
        player.send_message(f"{attr.type}:{attr.base_value},{attr.value},{attr.modifiers}")
