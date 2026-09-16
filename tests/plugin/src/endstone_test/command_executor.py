import json

import numpy as np
from endstone import ColorFormat, Player
from endstone.command import (
    Command,
    CommandExecutor,
    CommandSender,
    ConsoleCommandSender,
)
from endstone.form import (
    ActionForm,
    Button,
    Dropdown,
    Label,
    MessageForm,
    ModalForm,
    Slider,
    StepSlider,
    TextInput,
    Toggle,
)
from endstone.lang import Translatable as tr
from endstone.plugin import Plugin
from endstone.util import Vector


class TestCommandExecutor(CommandExecutor):
    __test__ = False

    def __init__(self, plugin: Plugin):
        CommandExecutor.__init__(self)
        self.plugin = plugin

    def on_command(
        self, sender: CommandSender, command: Command, args: list[str]
    ) -> bool:
        match args:
            case ["form", ("message" | "action" | "modal") as form_type]:
                if not isinstance(sender, Player):
                    sender.send_error_message(
                        "You must execute this command as a player"
                    )
                    return False

                if form_type == "message":
                    sender.send_form(
                        MessageForm(
                            title=tr("permissions.removeplayer"),
                            content=tr(
                                "accessibility.list.or.two", ["Player 1", "Player 2"]
                            ),
                            button1="Yes",
                            button2="No",
                            on_submit=lambda player, selection: player.send_message(
                                f"You've selected #{selection}"
                            ),
                            on_close=lambda player: player.send_message(
                                f"You just closed a {ColorFormat.GREEN}message form"
                            ),
                        )
                    )
                elif form_type == "action":
                    sender.send_form(
                        ActionForm(
                            title=tr("permissions.removeplayer"),
                            content=tr(
                                "accessibility.list.or.two", ["Player 1", "Player 2"]
                            ),
                            buttons=[
                                Button(
                                    "Endstone",
                                    icon="https://avatars.githubusercontent.com/u/142812342",
                                ),
                                Button("Instagram"),
                                Button("Twitter"),
                            ],
                            on_submit=lambda player, selection: player.send_message(
                                f"You've selected #{selection}"
                            ),
                            on_close=lambda player: player.send_message(
                                f"You just closed an {ColorFormat.GREEN}action form"
                            ),
                        )
                    )
                elif form_type == "modal":
                    sender.send_form(
                        ModalForm(
                            title=tr("permissions.removeplayer"),
                            controls=[
                                Dropdown(
                                    label="This is a dropdown",
                                    options=["Apple", "Orange", "Banana"],
                                ),
                                Label(text="This is a label"),
                                Slider(
                                    label="This is a slider",
                                    min=0,
                                    max=5,
                                    step=1,
                                    default_value=2,
                                ),
                                StepSlider(
                                    label="This is a step slider",
                                    options=["Mild", "Hot", "Extra hot"],
                                ),
                                TextInput(
                                    label="This is a text input",
                                    placeholder="This is the placehoder",
                                    default_value="Delete me",
                                ),
                                Toggle(label="This is a toggle", default_value=True),
                            ],
                            submit_button="Let's GO",
                            icon="https://avatars.githubusercontent.com/u/142812342",
                            on_submit=lambda player, data: player.send_message(
                                f"Response {json.loads(data)}"
                            ),
                            on_close=lambda player: player.send_message(
                                f"You just closed a {ColorFormat.GREEN}modal form"
                            ),
                        )
                    )
                else:
                    sender.send_error_message(f"Unknown form type: {form_type}")

            case ["sender"]:
                if isinstance(sender, Player):
                    sender.send_message("You are a player!")
                elif isinstance(sender, ConsoleCommandSender):
                    sender.send_message("You are the console!")
                else:
                    sender.send_error_message(f"Unknown sender: {sender.__class__}")
                    return False

            case ["player", test_type]:
                if not isinstance(sender, Player):
                    sender.send_error_message(
                        "You must execute this command as a player"
                    )
                    return False

                if test_type == "toast":
                    sender.send_toast("This is the title", "This is the content")
                elif test_type == "title":
                    sender.send_title("Welcome!", sender.name)
                elif test_type == "kick":
                    sender.kick("kick is working!")
                elif test_type == "particle":
                    radius = 1.0
                    points = 20
                    angles = np.linspace(0, 2 * np.pi, points, endpoint=False)
                    x_values = radius * np.cos(angles)
                    z_values = radius * np.sin(angles)
                    for x, z in zip(x_values, z_values):
                        location = sender.location + Vector(x, 1, z)
                        sender.spawn_particle(
                            "minecraft:basic_flame_particle",
                            location.x,
                            location.y,
                            location.z,
                        )
                elif test_type == "sound":
                    sender.play_sound(sender.location, "bucket.empty_water", 1, 1)
                    sender.send_message("Sound played")

            case ["block", *rest]:
                sender.send_message(str(rest))

            case ["broadcast"]:
                sender.server.broadcast_message(f"Hello from {sender.name}!")

            case ["inv", type]:
                if not isinstance(sender, Player):
                    sender.send_error_message(
                        "You must execute this command as a player"
                    )
                    return False
                player: Player = sender
                if type == "mainhand":
                    sender.send_message(
                        f"Main hand item is: {player.inventory.item_in_main_hand}"
                    )
                elif type == "offhand":
                    sender.send_message(
                        f"Off hand item is: {player.inventory.item_in_off_hand}"
                    )
                elif type == "meta":
                    slot = player.inventory.held_item_slot
                    item = player.inventory.get_item(slot)
                    if item is None:
                        sender.send_error_message("Please hold the item.")
                    else:
                        meta = item.item_meta
                        meta.add_enchant("sharpness", 10, True)
                        meta.display_name = "name"
                        meta.lore = ["line 1", "line 2"]
                        item.set_item_meta(meta)
                        player.inventory.set_item(slot, item)
                        sender.send_message("The item has been edited.")
                else:
                    sender.send_error_message(f"Unknown inventory test: {type}")

            case ["spawn", type]:
                if not isinstance(sender, Player):
                    sender.send_error_message(
                        "You must execute this command as a player"
                    )
                    return False

                sender.dimension.spawn_actor(sender.location, type)
                sender.send_message(f"Spawned {type} at {sender.location}")
                return True

        return True
