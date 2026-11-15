package dev.zernyth.stable;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Proxy;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.Set;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;
import java.util.logging.Handler;
import java.util.logging.LogRecord;
import java.util.logging.Logger;

import dev.zernyth.api.Server;
import dev.zernyth.api.command.Command;
import dev.zernyth.api.command.CommandExecutor;
import dev.zernyth.api.command.CommandSender;
import dev.zernyth.api.command.PlayerSender;
import dev.zernyth.api.event.Registration;
import dev.zernyth.api.form.FormRequest;
import dev.zernyth.api.form.FormResponse;
import dev.zernyth.api.network.NetworkCapability;
import dev.zernyth.api.network.NetworkManager;
import dev.zernyth.api.player.Player;
import dev.zernyth.api.plugin.PluginDescription;
import dev.zernyth.api.plugin.PluginManager;
import dev.zernyth.api.scheduler.Scheduler;
import dev.zernyth.api.scheduler.Task;
import dev.zernyth.api.world.Block;
import dev.zernyth.api.world.BlockData;
import dev.zernyth.api.world.Dimension;
import dev.zernyth.api.world.World;

/** Dependency-free safety regression; failures cause a nonzero JavaExec exit. */
public final class SafetyTest {

    private SafetyTest() {
    }

    /**
     * Checks fail-closed world selection using synthetic names only.
     * @param args unused
     */
    public static void main(String[] args) {
        if (!Safety.isolated(List.of("zernyth-stable-test"))) {
            throw new AssertionError("Dedicated test world rejected");
        }
        for (var names : List.of(List.<String>of(), List.of("world"),
                List.of("zernyth-stable-test", "world"), List.of("Zernyth-stable-test"))) {
            if (Safety.isolated(names)) {
                throw new AssertionError("Unsafe selection accepted: " + names);
            }
        }
        System.out.println("PASS | safety.world | Exact isolated test-world policy");
        if (!List.of(Safety.arguments(new String[]{"gameplay block 1 64 -2"}))
                .equals(List.of("gameplay", "block", "1", "64", "-2"))) {
            throw new AssertionError("BDS message argument was not expanded");
        }
        if (Safety.arguments(new String[]{"  "}).length != 0
                || Safety.arguments(new String[0]).length != 0) {
            throw new AssertionError("Blank command arguments were not handled");
        }
        System.out.println("PASS | arguments | Packed BDS message and blank argument handling");
        commandSafety();
        schedulerCancellation();
        cleanupFailure();
        networkCapabilities();
        formValidation();
        blockRestoration();
        System.out.println("PASS | suite | 8 bounded mock test groups completed; BDS NOT TESTED");
    }

    private static void commandSafety() {
        var executor = new AtomicReference<CommandExecutor>();
        var accesses = new AtomicInteger();
        Command command = (Command) Proxy.newProxyInstance(Command.class.getClassLoader(),
                new Class<?>[]{Command.class}, (proxy, method, values) -> null);
        Server server = (Server) Proxy.newProxyInstance(Server.class.getClassLoader(),
                new Class<?>[]{Server.class}, (proxy, method, values) -> {
                    if (method.getName().equals("registerCommand")) {
                        if (!values[2].equals("zernyth.test")) {
                            throw new AssertionError("Missing native permission");
                        }
                        executor.set((CommandExecutor) values[3]);
                        return command;
                    }
                    accesses.incrementAndGet();
                    if (method.getName().equals("getWorlds")) {
                        return List.of();
                    }
                    throw new AssertionError("Unexpected API access: " + method.getName());
                });
        var plugin = new StableTestPlugin();
        plugin.initialize(new PluginDescription("test", "test", "test", List.of(), List.of(), List.of()),
                Logger.getLogger("stable-safety"));
        plugin.initializeServer(server);
        plugin.onLoad();
        plugin.onEnable();
        if (accesses.get() != 0) {
            throw new AssertionError("Activation accessed gameplay");
        }
        for (boolean allowed : List.of(false, true)) {
            CommandSender sender = (CommandSender) Proxy.newProxyInstance(
                    CommandSender.class.getClassLoader(),
                    new Class<?>[]{CommandSender.class}, (proxy, method, values) -> {
                        if (method.getName().equals("hasPermission")) {
                            return allowed;
                        }
                        return null;
                    });
            executor.get().onCommand(sender, command, new String[]{"gameplay", "block", "0", "64", "0"});
            if (accesses.get() != (allowed ? 1 : 0)) {
                throw new AssertionError("Permission or world guard failed");
            }
        }
        System.out.println("PASS | safety.command | Permission gate and world guard prevent mutations");
    }

    private static void schedulerCancellation() {
        var fixture = new Fixture();
        var queued = new ArrayList<Scheduled>();
        fixture.scheduler = mock(Scheduler.class, (proxy, method, args) -> {
            if (method.getName().equals("runTaskLater")) {
                var scheduled = new Scheduled((Runnable) args[0], (long) args[1]);
                queued.add(scheduled);
                return scheduled;
            }
            if (method.getName().equals("cancelTasks")) {
                queued.forEach(Scheduled::cancel);
                return null;
            }
            throw new AssertionError("Unexpected scheduler method");
        });
        fixture.start();
        fixture.command("  scheduler  ");
        require(queued.size() == 2 && queued.getFirst().cancelled, "Cancellation not requested");
        require(queued.getFirst().delay == 20 && queued.getLast().delay == 40, "Wrong tick delays");
        fixture.command("scheduler");
        require(queued.size() == 2, "Concurrent probes allocated more timers");
        queued.getLast().action.run();
        require(fixture.has("PASS | scheduler.delay"), "Delayed completion missing");
        fixture.command("scheduler");
        fixture.command("cleanup");
        require(queued.stream().allMatch(item -> item.cancelled), "Cleanup left queued task");
        fixture.plugin.onDisable();
        require(fixture.commandClosed, "Command not closed");
        System.out.println("PASS | scheduler.mock | Delay, cancellation, overlap guard, cleanup and disable");
    }

    private static void cleanupFailure() {
        var fixture = new Fixture();
        var packetClosed = new AtomicInteger();
        var eventsClosed = new AtomicInteger();
        var schedulerClosed = new AtomicInteger();
        fixture.manager = mock(PluginManager.class, (proxy, method, args) ->
                (Registration) () -> {
                    eventsClosed.incrementAndGet();
                    throw new IllegalStateException("synthetic close failure");
                });
        fixture.network = mock(NetworkManager.class, (proxy, method, args) -> {
            if (method.getName().equals("getCapabilities")) {
                return Set.of(NetworkCapability.PACKET_METADATA);
            }
            if (method.getName().equals("listen")) {
                return (Registration) () -> packetClosed.incrementAndGet();
            }
            throw new AssertionError("Unexpected network method");
        });
        fixture.scheduler = mock(Scheduler.class, (proxy, method, args) -> {
            schedulerClosed.incrementAndGet();
            return null;
        });
        fixture.start();
        fixture.command("events start");
        fixture.command("network start");
        fixture.plugin.onDisable();
        require(eventsClosed.get() == 1, "Event cleanup not attempted");
        require(packetClosed.get() == 1 && schedulerClosed.get() == 1, "Cleanup stopped at first failure");
        require(fixture.commandClosed, "Command leaked after cleanup error");
        require(fixture.has("FAIL | lifecycle.disable"), "Cleanup error reported as success");
        fixture.plugin.onDisable();
        require(packetClosed.get() == 1, "Closed packet observer closed twice");
        System.out.println("PASS | cleanup.mock | Failure isolation, command release and repeated cleanup");
    }

    private static void networkCapabilities() {
        var fixture = new Fixture();
        fixture.network = mock(NetworkManager.class, (proxy, method, args) -> {
            require(method.getName().equals("getCapabilities"), "Unsupported adapter used");
            return Set.of();
        });
        fixture.start();
        fixture.command("network start");
        require(fixture.has("NOT TESTED | network.observe"), "Missing capability not reported");
        fixture.command("forms");
        require(fixture.has("NOT TESTED | forms"), "Console form was not rejected");
        fixture.command("gameplay", "player");
        require(fixture.has("NOT TESTED | gameplay.player"), "Console player probe was not rejected");
        System.out.println("PASS | capabilities.mock | Unsupported networking and missing client rejected");
    }

    private static void formValidation() {
        for (var values : List.of(List.<Object>of(0), List.<Object>of(1), List.<Object>of(-1),
                List.<Object>of(2), List.<Object>of(), List.<Object>of("0"), List.<Object>of(0, 1))) {
            var fixture = new Fixture();
            fixture.start();
            var response = new CompletableFuture<FormResponse>();
            var cancelled = new AtomicInteger();
            var request = mock(FormRequest.class, (proxy, method, args) -> {
                if (method.getName().equals("response")) {
                    return response;
                }
                cancelled.incrementAndGet();
                return null;
            });
            var player = mock(Player.class, (proxy, method, args) -> switch (method.getName()) {
                case "isOnline" -> true;
                case "showForm" -> request;
                default -> throw new AssertionError("Unexpected player method");
            });
            var sender = mock(PlayerSender.class, (proxy, method, args) -> switch (method.getName()) {
                case "getPlayer" -> player;
                case "hasPermission" -> true;
                default -> null;
            });
            fixture.executor.onCommand(sender, fixture.registered, new String[]{"forms"});
            response.complete(new FormResponse(FormResponse.Status.SUBMITTED, values));
            boolean valid = values.equals(List.of(0)) || values.equals(List.of(1));
            require(fixture.has((valid ? "PASS" : "FAIL") + " | forms.response"),
                    "Form selection was not validated: " + values);
            fixture.command("cleanup");
            require(cancelled.get() == 1, "Form callback not cancelled during cleanup");
        }
        System.out.println("PASS | forms.mock | Valid selections, malformed selections and callback cleanup");
    }

    private static void blockRestoration() {
        for (boolean failReadback : List.of(false, true)) {
            var fixture = new Fixture();
            var original = new BlockData("minecraft:stone", Map.of());
            var state = new AtomicReference<>(original);
            var writes = new AtomicInteger();
            var block = mock(Block.class, (proxy, method, args) -> switch (method.getName()) {
                case "getData" -> {
                    if (failReadback && writes.get() == 1) {
                        throw new IllegalStateException("Synthetic readback failure");
                    }
                    yield state.get();
                }
                case "setType" -> {
                    state.set(new BlockData((String) args[0], Map.of()));
                    writes.incrementAndGet();
                    yield null;
                }
                case "setData" -> {
                    state.set((BlockData) args[0]);
                    writes.incrementAndGet();
                    yield null;
                }
                default -> throw new AssertionError("Unexpected block method");
            });
            var dimension = mock(Dimension.class, (proxy, method, args) -> switch (method.getName()) {
                case "getIdentifier" -> "Overworld";
                case "getBlock" -> Optional.of(block);
                default -> throw new AssertionError("Unexpected dimension method");
            });
            fixture.world = mock(World.class, (proxy, method, args) -> switch (method.getName()) {
                case "getName" -> "zernyth-stable-test";
                case "getDimensions" -> List.of(dimension);
                default -> throw new AssertionError("Unexpected world method");
            });
            fixture.start();
            fixture.command("gameplay block 1 64 -2");
            require(writes.get() == 2 && state.get().equals(original), "Original block was not restored");
            require(fixture.has(failReadback ? "FAIL | gameplay" : "PASS | gameplay.block"),
                    "Block probe result missing");
        }
        System.out.println("PASS | block.mock | Native dimension spelling and restoration "
                + "after readback failure");
    }

    private static void require(boolean condition, String message) {
        if (!condition) {
            throw new AssertionError(message);
        }
    }

    private static <T> T mock(Class<T> type, InvocationHandler handler) {
        return type.cast(Proxy.newProxyInstance(type.getClassLoader(), new Class<?>[]{type}, handler));
    }

    private static final class Scheduled implements Task {
        private final Runnable action;
        private final long delay;
        private boolean cancelled;

        private Scheduled(Runnable action, long delay) {
            this.action = action;
            this.delay = delay;
        }

        @Override
        public void cancel() {
            cancelled = true;
        }

        @Override
        public boolean isCancelled() {
            return cancelled;
        }
    }

    private static final class Fixture {
        private final StableTestPlugin plugin = new StableTestPlugin();
        private final List<String> logs = new ArrayList<>();
        private CommandExecutor executor;
        private Command registered;
        private boolean commandClosed;
        private Scheduler scheduler = mock(Scheduler.class, (proxy, method, args) -> null);
        private PluginManager manager;
        private NetworkManager network;
        private World world;

        private void start() {
            var logger = Logger.getAnonymousLogger();
            logger.setUseParentHandlers(false);
            logger.addHandler(new Handler() {
                @Override
                public void publish(LogRecord record) {
                    logs.add(record.getMessage());
                }

                @Override
                public void flush() {
                }

                @Override
                public void close() {
                }
            });
            registered = mock(Command.class, (proxy, method, args) -> {
                commandClosed = true;
                return null;
            });
            if (world == null) {
                world = mock(World.class, (proxy, method, args) -> "zernyth-stable-test");
            }
            var server = mock(Server.class, (proxy, method, args) -> switch (method.getName()) {
                case "registerCommand" -> {
                    executor = (CommandExecutor) args[3];
                    yield registered;
                }
                case "getWorlds" -> List.of(world);
                case "getScheduler" -> scheduler;
                case "getPluginManager" -> manager;
                case "getNetworkManager" -> network;
                default -> throw new AssertionError("Unexpected native access: " + method.getName());
            });
            plugin.initialize(new PluginDescription("test", "test", "test", List.of(), List.of(), List.of()),
                    logger);
            plugin.initializeServer(server);
            plugin.onLoad();
            plugin.onEnable();
        }

        private void command(String... args) {
            var sender = mock(CommandSender.class, (proxy, method, values) ->
                    method.getName().equals("hasPermission") ? true : null);
            executor.onCommand(sender, registered, args);
        }

        private boolean has(String prefix) {
            return logs.stream().anyMatch(line -> line.startsWith(prefix));
        }
    }

}
