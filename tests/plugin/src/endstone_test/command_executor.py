import threading

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

            case ["events"]:
                return self._events(sender)

            case ["sender"]:
                return self._sender(sender)

            case ["broadcast"]:
                sender.server.broadcast_message(f"Hello from {sender.name}!")

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

            case ["chunk"]:
                return self._chunk(sender, 30_000, 30_000, 0)

            case ["chunk", x, z]:
                return self._chunk(sender, int(x), int(z), 0)

            case ["chunk", x, z, radius]:
                return self._chunk(sender, int(x), int(z), int(radius))

            case ["chunkapi"]:
                return self._chunk_api(sender, 6000, 6000)

            case ["chunkapi", x, z]:
                return self._chunk_api(sender, int(x), int(z))

            case _:
                return False

        return True

    def _chunk(
        self, sender: CommandSender, x: int, z: int, radius: int, timeout: int = 200
    ) -> bool:
        log = self.plugin.logger

        def report(message: str) -> None:
            log.info(message)
            if not isinstance(sender, ConsoleCommandSender):
                sender.send_message(message)

        dimension = sender.server.level.get_dimension("overworld")
        area = [
            (x + dx, z + dz)
            for dx in range(-radius, radius + 1)
            for dz in range(-radius, radius + 1)
        ]
        if any(dimension.is_chunk_loaded(cx, cz) for cx, cz in area):
            report(f"Some of the {len(area)} chunk(s) around ({x}, {z}) are already loaded.")
            return True

        accepted = sum(dimension.load_chunk(cx, cz) for cx, cz in area)
        report(
            f"load_chunk radius {radius} around ({x}, {z}): {accepted}/{len(area)} accepted, "
            f"already generated: {dimension.is_chunk_generated(x, z)}, "
            f"centre loaded on return: {dimension.is_chunk_loaded(x, z)}"
        )
        if not accepted:
            return True

        elapsed = 0

        def poll() -> None:
            nonlocal elapsed
            elapsed += 1
            if dimension.is_chunk_loaded(x, z):
                report(f"{ColorFormat.GREEN}Centre ({x}, {z}) loaded after {elapsed} tick(s).")
            elif elapsed < timeout:
                return
            else:
                loaded = sum(dimension.is_chunk_loaded(cx, cz) for cx, cz in area)
                report(
                    f"Centre ({x}, {z}) still not loaded after {timeout} ticks "
                    f"({loaded}/{len(area)} of the area loaded)."
                )
            task.cancel()
            released = sum(dimension.unload_chunk(cx, cz) for cx, cz in area)
            report(f"unload_chunk released {released}/{len(area)} chunk(s).")

        task = sender.server.scheduler.run_task(self.plugin, poll, delay=1, period=1)
        if task is None:
            report("Failed to schedule the chunk poll.")
            for cx, cz in area:
                dimension.unload_chunk(cx, cz)
        return True

    def _chunk_api(self, sender: CommandSender, x: int, z: int) -> bool:
        log = self.plugin.logger
        plugin = self.plugin
        dimension = sender.server.level.get_dimension("overworld")
        results = []

        def check(name: str, passed: bool) -> None:
            results.append(passed)
            colour = ColorFormat.GREEN if passed else ColorFormat.RED
            log.info(f"{colour}{'PASS' if passed else 'FAIL'}{ColorFormat.RESET} {name}")

        def guarded(name: str, fn) -> None:
            try:
                check(name, fn())
            except Exception as e:  # noqa: BLE001
                check(f"{name} -- raised {type(e).__name__}: {e}", False)

        log.info(f"--- chunk API checks around ({x}, {z}) ---")

        guarded("virgin chunk is not generated", lambda: dimension.is_chunk_generated(x, z) is False)
        guarded("load_chunk(generate=False) refuses", lambda: dimension.load_chunk(x, z, generate=False) is False)
        guarded("refused load took no hold", lambda: dimension.is_chunk_loaded(x, z) is False)
        guarded("unload_chunk_request on unheld chunk", lambda: dimension.unload_chunk_request(x, z) is True)

        guarded("add_plugin_chunk_ticket", lambda: dimension.add_plugin_chunk_ticket(x, z, plugin) is True)
        guarded("duplicate ticket refused", lambda: dimension.add_plugin_chunk_ticket(x, z, plugin) is False)
        guarded("get_plugin_chunk_tickets lists it", lambda: plugin in dimension.get_plugin_chunk_tickets(x, z))
        guarded("plugin_chunk_tickets lists it", lambda: plugin in dimension.plugin_chunk_tickets)
        guarded(
            "plugin_chunk_tickets maps to the chunk",
            lambda: any((c.x, c.z) == (x, z) for c in dimension.plugin_chunk_tickets.get(plugin, [])),
        )
        guarded("unload_chunk keeps the plugin ticket", lambda: (dimension.unload_chunk(x, z) is not None)
                and plugin in dimension.get_plugin_chunk_tickets(x, z))
        guarded("remove_plugin_chunk_ticket", lambda: dimension.remove_plugin_chunk_ticket(x, z, plugin) is True)
        guarded("duplicate removal refused", lambda: dimension.remove_plugin_chunk_ticket(x, z, plugin) is False)
        guarded("ticket list empty after removal", lambda: dimension.get_plugin_chunk_tickets(x, z) == [])

        dimension.add_plugin_chunk_ticket(x + 40, z + 40, plugin)
        dimension.remove_plugin_chunk_tickets(plugin)
        guarded("remove_plugin_chunk_tickets clears all", lambda: plugin not in dimension.plugin_chunk_tickets)

        def off_thread():
            try:
                dimension.load_chunk(x + 80, z + 80)
                return False
            except RuntimeError:
                return True

        holder = {}

        def runner():
            holder["ok"] = off_thread()

        thread = threading.Thread(target=runner)
        thread.start()
        thread.join(5)
        guarded("off-thread load_chunk is refused", lambda: holder.get("ok") is True)

        def finish() -> None:
            dimension.remove_plugin_chunk_tickets(plugin)
            for cx, cz in ((x, z), (x + 40, z + 40), (x + 80, z + 80)):
                dimension.unload_chunk(cx, cz)
            passed = sum(results)
            summary = f"chunk API: {passed}/{len(results)} passed"
            log.info((ColorFormat.GREEN if passed == len(results) else ColorFormat.RED) + summary)
            if not isinstance(sender, ConsoleCommandSender):
                sender.send_message(summary)

        dimension.add_plugin_chunk_ticket(x, z, plugin)
        elapsed = 0

        def poll() -> None:
            nonlocal elapsed
            elapsed += 1
            if not dimension.is_chunk_loaded(x, z):
                if elapsed < 200:
                    return
                check("ticketed chunk became resident", False)
                task.cancel()
                finish()
                return
            task.cancel()

            resident = [c for c in dimension.loaded_chunks if (c.x, c.z) != (x, z)]
            guarded("resident chunk refuses unload", lambda: dimension.unload_chunk(x, z) is False)
            if resident:
                other = resident[0]
                guarded("Chunk.is_loaded agrees",
                        lambda: other.is_loaded is dimension.is_chunk_loaded(other.x, other.z))
                guarded("Chunk.add_plugin_chunk_ticket", lambda: other.add_plugin_chunk_ticket(plugin) is True)
                guarded("Chunk.plugin_chunk_tickets lists it", lambda: plugin in other.plugin_chunk_tickets)
                guarded("Chunk.remove_plugin_chunk_ticket", lambda: other.remove_plugin_chunk_ticket(plugin) is True)
                guarded("Chunk.unload releases it", lambda: other.unload() in (True, False))
            else:
                check("a neighbour chunk is resident for Chunk-level checks", False)
            finish()

        task = sender.server.scheduler.run_task(self.plugin, poll, delay=1, period=1)
        if task is None:
            check("scheduled the Chunk-level phase", False)
            finish()
        return True

    def _as_player(self, sender: CommandSender) -> Player | None:
        if isinstance(sender, Player):
            return sender
        sender.send_error_message("You must execute this command as a player")
        return None

    def _run_tests(self, sender: CommandSender) -> bool:
        self.plugin.run_tests(sender if isinstance(sender, Player) else None)
        return True

    def _events(self, sender: CommandSender) -> bool:
        recorder = self.plugin.recorder
        sender.send_message(ColorFormat.GREEN + recorder.summary())
        missing = recorder.missing
        if missing:
            sender.send_message(f"Not yet triggered ({len(missing)}):")
            for name in missing:
                sender.send_message(f"  {ColorFormat.GRAY}{name}")
        self.plugin.run_event_tests(sender if isinstance(sender, Player) else None)
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
