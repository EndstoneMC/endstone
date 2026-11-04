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
    Divider,
    Dropdown,
    Header,
    Label,
    MessageForm,
    ModalForm,
    Slider,
    StepSlider,
    TextInput,
    Toggle,
)
from endstone.inventory import ItemStack
from endstone.lang import Translatable as tr
from endstone.plugin import Plugin

from endstone_test.image_renderer import ImageRenderer

ENDSTONE_ICON = "https://avatars.githubusercontent.com/u/142812342"


class TestCommandExecutor(CommandExecutor):
    __test__ = False

    def __init__(self, plugin: Plugin):
        CommandExecutor.__init__(self)
        self.plugin = plugin

    def on_command(
        self, sender: CommandSender, command: Command, args: list[str]
    ) -> bool:
        match args:
            case []:
                return self._run_tests(sender)

            case ["sender"]:
                return self._sender(sender)

            case ["block", *rest]:
                sender.send_message(str(rest))

            case ["form", ("message" | "action" | "modal") as form_type]:
                player = self._as_player(sender)
                return False if player is None else self._form(player, form_type)

            case ["player", test_type]:
                player = self._as_player(sender)
                return False if player is None else self._player(player, test_type)

            case ["inv", inv_type]:
                player = self._as_player(sender)
                return False if player is None else self._inventory(player, inv_type)

            case ["spawn", actor_type]:
                player = self._as_player(sender)
                if player is None:
                    return False
                player.dimension.spawn_actor(player.location, actor_type)
                player.send_message(f"Spawned {actor_type} at {player.location}")

            case ["map"]:
                player = self._as_player(sender)
                return False if player is None else self._map(player)

            case _:
                return False

        return True

    def _as_player(self, sender: CommandSender) -> Player | None:
        if isinstance(sender, Player):
            return sender
        sender.send_error_message("You must execute this command as a player")
        return None

    def _run_tests(self, sender: CommandSender) -> bool:
        self.plugin.run_tests(sender if isinstance(sender, Player) else None)
        return True

    def _sender(self, sender: CommandSender) -> bool:
        if isinstance(sender, Player):
            sender.send_message("You are a player!")
        elif isinstance(sender, ConsoleCommandSender):
            sender.send_message("You are the console!")
        else:
            sender.send_error_message(f"Unknown sender: {sender.__class__}")
            return False
        return True

    def _form(self, player: Player, form_type: str) -> bool:
        if form_type == "message":
            player.send_form(
                MessageForm(
                    title=tr("permissions.removeplayer"),
                    content=tr("accessibility.list.or.two", ["Player 1", "Player 2"]),
                    button1="Yes",
                    button2="No",
                    on_submit=lambda p, selection: p.send_message(
                        f"You've selected #{selection}"
                    ),
                    on_close=lambda p: p.send_message(
                        f"You just closed a {ColorFormat.GREEN}message form"
                    ),
                )
            )
        elif form_type == "action":
            player.send_form(
                ActionForm(
                    title=tr("permissions.removeplayer"),
                    content=tr("accessibility.list.or.two", ["Player 1", "Player 2"]),
                    buttons=[
                        Header("Links"),
                        Button(
                            "Endstone",
                            icon=ENDSTONE_ICON,
                            on_click=lambda p: p.send_message("on_click fired"),
                        ),
                        Divider(),
                        Button("Instagram"),
                        Button("Twitter"),
                        Label("This is a label"),
                    ],
                    on_submit=lambda p, selection: p.send_message(
                        f"You've selected #{selection}"
                    ),
                    on_close=lambda p: p.send_message(
                        f"You just closed an {ColorFormat.GREEN}action form"
                    ),
                )
            )
        else:
            player.send_form(
                ModalForm(
                    title=tr("permissions.removeplayer"),
                    controls=[
                        Header("This is a header"),
                        Dropdown(
                            label="This is a dropdown",
                            options=["Apple", "Orange", "Banana"],
                        ),
                        Divider(),
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
                            placeholder="This is the placeholder",
                            default_value="Delete me",
                        ),
                        Toggle(label="This is a toggle", default_value=True),
                    ],
                    submit_button="Let's GO",
                    icon=ENDSTONE_ICON,
                    on_submit=lambda p, data: p.send_message(f"Response {data}"),
                    on_close=lambda p: p.send_message(
                        f"You just closed a {ColorFormat.GREEN}modal form"
                    ),
                )
            )
        return True

    def _player(self, player: Player, test_type: str) -> bool:
        match test_type:
            case "toast":
                player.send_toast("This is the title", "This is the content")
            case "title":
                player.send_title("Welcome!", player.name)
            case "actionbar":
                player.send_action_bar(f"{ColorFormat.GREEN}This is the action bar")
            case "kick":
                player.kick("kick is working!")
            case "particle":
                origin = player.location
                angles = np.linspace(0, 2 * np.pi, 20, endpoint=False)
                for x, z in zip(np.cos(angles), np.sin(angles)):
                    player.spawn_particle(
                        "minecraft:basic_flame_particle",
                        origin.x + x,
                        origin.y + 1,
                        origin.z + z,
                    )
            case "sound":
                player.play_sound(player.location, "bucket.empty_water", 1, 1)
                player.send_message("Sound played")
            case _:
                player.send_error_message(f"Unknown player test: {test_type}")
                return False
        return True

    def _inventory(self, player: Player, inv_type: str) -> bool:
        match inv_type:
            case "mainhand":
                player.send_message(
                    f"Main hand item is: {player.inventory.item_in_main_hand}"
                )
            case "offhand":
                player.send_message(
                    f"Off hand item is: {player.inventory.item_in_off_hand}"
                )
            case "meta":
                slot = player.inventory.held_item_slot
                item = player.inventory.get_item(slot)
                if item is None:
                    player.send_error_message("Please hold the item.")
                    return True
                meta = item.item_meta
                meta.add_enchant("sharpness", 10, True)
                meta.display_name = "name"
                meta.lore = ["line 1", "line 2"]
                item.set_item_meta(meta)
                player.inventory.set_item(slot, item)
                player.send_message("The item has been edited.")
            case _:
                player.send_error_message(f"Unknown inventory test: {inv_type}")
                return False
        return True

    def _map(self, player: Player) -> bool:
        view = player.server.create_map(player.dimension)
        for renderer in view.renderers:
            view.remove_renderer(renderer)
        view.add_renderer(ImageRenderer())

        item = ItemStack("minecraft:filled_map")
        meta = item.item_meta
        meta.map_view = view
        item.set_item_meta(meta)
        player.inventory.add_item(item)
        player.send_message(f"Map #{view.id} added to your inventory.")
        return True
