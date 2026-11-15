package dev.zernyth.stable;

import java.io.IOException;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicLong;

import dev.zernyth.api.command.Command;
import dev.zernyth.api.command.CommandSender;
import dev.zernyth.api.command.PlayerSender;
import dev.zernyth.api.event.EventHandler;
import dev.zernyth.api.event.EventPriority;
import dev.zernyth.api.event.Listener;
import dev.zernyth.api.event.PlayerMoveEvent;
import dev.zernyth.api.event.Registration;
import dev.zernyth.api.form.FormRequest;
import dev.zernyth.api.form.FormResponse;
import dev.zernyth.api.form.MessageForm;
import dev.zernyth.api.network.NetworkCapability;
import dev.zernyth.api.network.PacketFilter;
import dev.zernyth.api.plugin.JavaPlugin;
import dev.zernyth.api.scheduler.Task;
import dev.zernyth.api.service.ServicePriority;
import dev.zernyth.api.world.BlockData;
import dev.zernyth.api.world.BlockPosition;
import dev.zernyth.api.world.World;

/**
 * Operator-driven probes on the server thread. No world writes occur during activation.
 * Native resources belong to this plugin generation; callbacks never retain command senders.
 */
public final class StableTestPlugin extends JavaPlugin implements Listener {

    /** Creates an uninitialized instance for the native plugin loader. */
    public StableTestPlugin() {
    }

    private Command command;
    private Registration events;
    private Registration packets;
    private Task task;
    private FormRequest form;
    private long moves;
    private long movesAtStop;
    private long packetsAtStop;
    private final AtomicLong packetCount = new AtomicLong();
    private boolean loaded;

    /** Public service contract used only by this probe; calls require the server thread. */
    public interface Probe {

        /**
         * Returns a fixed marker while the native registration is live.
         * @return the marker
         */
        String marker();
    }

    @Override
    public void onLoad() {
        loaded = true;
        result("PASS", "lifecycle.load", "Neue Plugininstanz geladen");
    }

    @Override
    public void onEnable() {
        command = getServer().registerCommand("zernythtest", "Stable runtime probes",
                "zernyth.test", this::execute);
        result(loaded ? "PASS" : "FAIL", "lifecycle.enable", "Load vor Enable: " + loaded);
        result("NOT TESTED", "runtime", "Tests einzeln mit /zernythtest ausloesen");
    }

    private void result(String status, String test, String reason) {
        getLogger().info(status + " | " + test + " | " + reason);
    }

    private boolean execute(CommandSender sender, Command ignored, String[] args) {
        if (!sender.hasPermission("zernyth.test")) {
            result("NOT TESTED", "commands.permission", "Berechtigung fehlt; keine Tests ausgefuehrt");
            sender.sendMessage("Berechtigung zernyth.test erforderlich.");
            return true;
        }
        args = Safety.arguments(args);
        String test = args.length == 0 ? "help" : args[0];
        try {
            switch (test) {
                case "lifecycle" -> result("PASS", test, getDiagnostics().toString());
                case "commands" -> result("PASS", test, "Berechtigter nativer Dispatch: " + sender.getName());
                case "events" -> events(args);
                case "scheduler" -> scheduler();
                case "services" -> services();
                case "config" -> configuration();
                case "gameplay" -> gameplay(sender, args);
                case "forms" -> forms(sender);
                case "network" -> network(args);
                case "cleanup" -> {
                    if (args.length == 2 && args[1].equals("check")) {
                        cleanupCheck();
                    } else {
                        cleanup();
                    }
                }
                default -> sender.sendMessage("/zernythtest lifecycle|commands|events start/check/stop|"
                        + "scheduler|services|config|gameplay read/player/block x y z|forms|"
                        + "network start/check/stop|cleanup");
            }
            sender.sendMessage("Test " + test + ": Ergebnis und Begruendung im Serverlog.");
        } catch (Exception | AssertionError failure) {
            result("FAIL", test, failure.getClass().getSimpleName() + ": " + failure.getMessage());
            sender.sendMessage("FAIL " + test + ": siehe Serverlog");
        }
        return true;
    }

    private World testWorld() {
        var worlds = getServer().getWorlds();
        if (!Safety.isolated(worlds.stream().map(World::getName).toList())) {
            throw new IllegalStateException("Nur die separate Welt zernyth-stable-test darf geladen sein");
        }
        return worlds.getFirst();
    }

    private void gameplay(CommandSender sender, String[] args) {
        var world = testWorld();
        if (args.length == 2 && args[1].equals("read")) {
            var dimensions = world.getDimensions();
            if (dimensions.isEmpty()) {
                throw new AssertionError("Keine native Dimension verfuegbar");
            }
            result("PASS", "gameplay.read", "Testwelt=" + world.getName()
                    + ", Dimensionen=" + dimensions.size());
            var entity = world.getEntities().stream().findFirst();
            result(entity.isPresent() ? "PASS" : "NOT TESTED", "gameplay.entity",
                    entity.isPresent() ? "Vorhandener Entity-Typ=" + entity.get().getType()
                            : "Keine Entity vorhanden");
        } else if (args.length == 2 && args[1].equals("player")) {
            if (!(sender instanceof PlayerSender playerSender) || !playerSender.getPlayer().isOnline()) {
                result("NOT TESTED", "gameplay.player", "Verbundener Bedrock-Testclient erforderlich");
                return;
            }
            var player = playerSender.getPlayer();
            if (player.getUniqueId() == null) {
                throw new AssertionError("Spieler-UUID fehlt");
            }
            var inventory = player.getInventory();
            if (inventory.getContents().size() != inventory.getSize()) {
                throw new AssertionError("Inventar-Snapshot und Slotzahl stimmen nicht ueberein");
            }
            result("PASS", "gameplay.player", "Online, UUID, Position, Spielmodus und Inventar gelesen; "
                    + "Slots=" + inventory.getSize() + ", Position=" + player.getLocation()
                    + ", Modus=" + player.getGameMode()
                    + ", UUID vorhanden=" + (player.getUniqueId() != null));
            result("PASS", "gameplay.entity", "Spieler-Entity-Typ=" + player.asEntity().getType());
        } else if (args.length == 5 && args[1].equals("block")) {
            var position = new BlockPosition(Integer.parseInt(args[2]), Integer.parseInt(args[3]),
                    Integer.parseInt(args[4]));
            var dimension = world.getDimensions().stream()
                    .filter(candidate -> candidate.getIdentifier().equalsIgnoreCase("overworld"))
                    .findFirst().orElseThrow();
            var block = dimension.getBlock(position).orElseThrow();
            BlockData original = block.getData();
            if (!original.type().equals("minecraft:stone") && !original.type().equals("minecraft:dirt")) {
                result("NOT TESTED", "gameplay.block", "Nur vorbereitete Stein-/Erdebloecke erlaubt");
                return;
            }
            String replacement = original.type().equals("minecraft:stone")
                    ? "minecraft:dirt" : "minecraft:stone";
            try {
                block.setType(replacement);
                if (!block.getData().type().equals(replacement)) {
                    throw new AssertionError("Angeforderter Blocktyp nicht gesetzt");
                }
            } finally {
                block.setData(original, false);
            }
            if (!block.getData().equals(original)) {
                throw new AssertionError("Blockdaten nicht wiederhergestellt");
            }
            result("PASS", "gameplay.block", "Einzelner Block geaendert und Daten restauriert; "
                    + "Nachbarupdates/Blockentity-Inhalte manuell pruefen");
        } else {
            result("NOT TESTED", "gameplay", "read, player oder block x y z einzeln erforderlich");
        }
    }

    private void events(String[] args) {
        testWorld();
        String action = args.length == 2 ? args[1] : "";
        switch (action) {
            case "start" -> {
                if (events != null) {
                    events.close();
                }
                moves = 0;
                events = getServer().getPluginManager().registerEvents(this, this);
                result("NOT TESTED", "events", "Beobachter aktiv; Bedrock-Testclient bewegen, dann check");
            }
            case "check" -> {
                if (events == null) {
                    result(moves == movesAtStop ? "NOT TESTED" : "FAIL", "events.closed",
                            "Callbacks nach Stop=" + (moves - movesAtStop)
                                    + "; Clientbewegung und erneuten Check manuell bestaetigen");
                } else {
                    result(moves > 0 ? "PASS" : "NOT TESTED", "events", "Move-Aufrufe=" + moves);
                }
            }
            case "stop" -> {
                if (events != null) {
                    events.close();
                    events = null;
                }
                movesAtStop = moves;
                result("PASS", "events.close", "Registrierung geschlossen; danach Client bewegen");
            }
            default -> result("NOT TESTED", "events", "start/check/stop erforderlich");
        }
    }

    /**
     * Counts movement without modifying the event or retaining its callback-scoped view.
     * @param event current native event, valid only during this callback
     */
    @EventHandler(priority = EventPriority.MONITOR)
    public void move(PlayerMoveEvent event) {
        moves++;
    }

    private void scheduler() {
        if (task != null) {
            result("NOT TESTED", "scheduler", "Vorherige Probe laeuft; auf Abschluss warten oder cleanup");
            return;
        }
        var cancelledCallbackRan = new AtomicBoolean();
        task = getServer().getScheduler().runTaskLater(() -> {
            cancelledCallbackRan.set(true);
            result("FAIL", "scheduler.cancel", "Abgebrochener Callback wurde ausgefuehrt");
        }, 20);
        task.cancel();
        if (!task.isCancelled()) {
            throw new AssertionError("Cancellation nicht markiert");
        }
        task = getServer().getScheduler().runTaskLater(() -> {
            result(cancelledCallbackRan.get() ? "FAIL" : "PASS", "scheduler.delay",
                    "40-Tick-Callback erreicht; abgebrochener Callback lief: " + cancelledCallbackRan.get());
            task = null;
        }, 40);
        result("NOT TESTED", "scheduler.delay", "Auf 40 Ticks warten; Cancellation ist kooperativ");
    }

    private void services() {
        var manager = getServer().getServicesManager();
        Probe proxy;
        var registration = manager.register(Probe.class, () -> "stable-probe", ServicePriority.NORMAL);
        try {
            proxy = manager.find(Probe.class).orElseThrow();
            if (!proxy.marker().equals("stable-probe")) {
                throw new AssertionError("Service-Marker falsch");
            }
        } finally {
            registration.close();
        }
        try {
            proxy.marker();
            throw new AssertionError("Widerrufener Proxy weiter nutzbar");
        } catch (IllegalStateException expected) {
            result("PASS", "services", "Native Registrierung, Aufruf und Proxy-Widerruf geprueft");
        }
    }

    private void configuration() throws IOException {
        var storage = getStorage();
        var config = storage.load("stable-probe.yml");
        config.set("probe.value", "stable-roundtrip");
        storage.save("stable-probe.yml");
        storage.reload("stable-probe.yml");
        if (!storage.load("stable-probe.yml").getString("probe.value", "").equals("stable-roundtrip")) {
            throw new AssertionError("Expliziter Save/Reload fehlgeschlagen");
        }
        result("PASS", "config", "Expliziter Roundtrip in dedizierter stable-probe.yml");
    }

    private void forms(CommandSender sender) {
        testWorld();
        if (!(sender instanceof PlayerSender playerSender) || !playerSender.getPlayer().isOnline()) {
            result("NOT TESTED", "forms", "Verbundener Bedrock-Testclient als Sender erforderlich");
            return;
        }
        if (form != null) {
            form.cancel();
        }
        form = playerSender.getPlayer().showForm(
                new MessageForm("Zernyth Stable Test", "Test bestaetigen", "Bestaetigen", "Abbrechen"));
        form.response().whenComplete((response, failure) -> {
            if (failure != null) {
                result("FAIL", "forms.response", failure.getClass().getSimpleName());
                return;
            }
            if (response.status() == FormResponse.Status.SUBMITTED) {
                boolean valid = response.values().size() == 1
                        && response.values().getFirst() instanceof Integer selection
                        && (selection == 0 || selection == 1);
                result(valid ? "PASS" : "FAIL", "forms.response", "MessageForm-Auswahl validiert=" + valid);
            } else {
                result(response.status() == FormResponse.Status.INVALID_RESPONSE ? "FAIL" : "NOT TESTED",
                        "forms.response", "Clientantwort: " + response.status());
            }
        });
        result("NOT TESTED", "forms", "Clientantwort ausstehend; keine blockierende Warteoperation");
    }

    private void network(String[] args) {
        testWorld();
        var network = getServer().getNetworkManager();
        String action = args.length == 2 ? args[1] : "check";
        switch (action) {
            case "start" -> {
                if (!network.getCapabilities().contains(NetworkCapability.PACKET_METADATA)) {
                    result("NOT TESTED", "network.observe", "Native Metadaten-Capability fehlt");
                    return;
                }
                if (packets != null) {
                    packets.close();
                }
                packetCount.set(0);
                packets = network.listen(PacketFilter.all(), EventPriority.MONITOR,
                        context -> packetCount.incrementAndGet());
                result("NOT TESTED", "network.observe",
                        "Read-only Metadatenbeobachter aktiv; Client nutzen");
            }
            case "stop" -> {
                if (packets != null) {
                    packets.close();
                    packets = null;
                }
                packetsAtStop = packetCount.get();
                result("PASS", "network.close", "Beobachter geschlossen");
            }
            case "check" -> {
                result("PASS", "network.metadata", "Protokoll=" + network.getProtocol()
                        + ", Capabilities=" + network.getCapabilities()
                        + ", Metriken=" + network.getMetrics());
                long count = packetCount.get();
                if (packets == null) {
                    result(count == packetsAtStop ? "NOT TESTED" : "FAIL", "network.closed",
                            "Callbacks nach Stop=" + (count - packetsAtStop)
                                    + "; Clientverkehr und erneuten Check manuell bestaetigen");
                } else {
                    result(count > 0 ? "PASS" : "NOT TESTED", "network.observe",
                            "Metadaten-Callbacks=" + count);
                }
            }
            default -> result("NOT TESTED", "network", "start/check/stop erforderlich");
        }
    }

    private void cleanupCheck() {
        var diagnostics = getDiagnostics();
        boolean empty = diagnostics.eventListeners() == 0 && diagnostics.packetListeners() == 0
                && diagnostics.tasks() == 0 && diagnostics.services() == 0;
        result(empty ? "PASS" : "FAIL", "cleanup.native", "Eigene native Ressourcenzaehler=" + diagnostics);
        result("NOT TESTED", "cleanup.forms", "Form-Schliessen und Disconnect am Testclient pruefen");
    }

    private boolean release(String resource, Runnable close) {
        try {
            close.run();
            return true;
        } catch (Exception | AssertionError failure) {
            result("FAIL", "cleanup." + resource, failure.getClass().getSimpleName());
            return false;
        }
    }

    private boolean cleanup() {
        boolean complete = true;
        if (form != null) {
            complete &= release("form", () -> {
                form.cancel();
                form = null;
            });
        }
        if (events != null) {
            complete &= release("events", () -> {
                events.close();
                events = null;
                movesAtStop = moves;
            });
        }
        if (packets != null) {
            complete &= release("packets", () -> {
                packets.close();
                packets = null;
                packetsAtStop = packetCount.get();
            });
        }
        if (task != null) {
            complete &= release("task", () -> {
                task.cancel();
                if (!task.isCancelled()) {
                    throw new AssertionError("Task-Cancellation fehlgeschlagen");
                }
                task = null;
            });
        }
        complete &= release("scheduler", () -> getServer().getScheduler().cancelTasks());
        result(complete ? "PASS" : "FAIL", "cleanup.requests",
                "Bereinigung aller eigenen Ressourcen versucht; keine Worker-Warteoperation");
        result("NOT TESTED", "cleanup.native", "Native Restzaehler nach Callback extern pruefen");
        return complete;
    }

    @Override
    public void onDisable() {
        boolean complete = cleanup();
        if (command != null) {
            complete &= release("command", () -> {
                command.close();
                command = null;
            });
        }
        result(complete ? "PASS" : "FAIL", "lifecycle.disable",
                "Disable-Hook beendet, kein automatisches Konfigurationsspeichern");
        result("NOT TESTED", "lifecycle.post-disable", "Native Entfernung und Re-Enable extern pruefen");
    }
}
