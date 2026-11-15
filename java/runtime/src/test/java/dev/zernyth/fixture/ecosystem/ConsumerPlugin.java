package dev.zernyth.fixture.ecosystem;

import java.io.IOException;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.List;
import java.util.concurrent.atomic.AtomicBoolean;

import dev.zernyth.api.command.Command;
import dev.zernyth.api.command.CommandDefinition;
import dev.zernyth.api.command.CommandSender;
import dev.zernyth.api.command.Commands;
import dev.zernyth.api.plugin.JavaPlugin;
import dev.zernyth.api.service.ServicePriority;
import dev.zernyth.fixture.shared.Greeting;

/**
 * Consumes shared services in class-identity, priority and provider-revocation regressions.
 */
public final class ConsumerPlugin extends JavaPlugin {

    private Greeting service;

    private static void check(boolean condition) {
        if (!condition) {
            throw new AssertionError("Ecosystem fixture failed");
        }
    }

    @Override
    public void onEnable() {
        service = getServer().getServicesManager().load(Greeting.class);
        check(service != null && service.greet().equals("high")
                && getServer().getServicesManager().getProviders(Greeting.class).size() == 2);
        try {
            saveDefaultConfig();
            check(getConfig().getInt("server.max-players", 0) == 42);
        } catch (IOException error) {
            throw new IllegalStateException(error);
        }
        if (getDescription().name().equals("eco_consumer")) {
            Commands.register(getServer(),
                    new CommandDefinition("typed", "", "ecosystem.use", false, List.of("t"), List.of(),
                            List.of(new CommandDefinition("count", "", "ecosystem.count", false, List.of(),
                                            List.of(new CommandDefinition.Argument(
                                                    "value", CommandDefinition.Type.INTEGER)),
                                            List.of(),
                                            context -> {
                                                check(context.integer("value") == 42);
                                                context.sender().sendMessage("typed-ok");
                                                return true;
                                            }),
                                    new CommandDefinition("player", "", "", true, List.of(), List.of(),
                                            List.of(),
                                            context -> {
                                                throw new AssertionError(
                                                        "Console passed player-only command");
                                            })),
                            null));
        }
    }

    @Override
    public boolean onCommand(CommandSender sender, Command command, String[] args) {
        switch (args[0]) {
            case "check" -> {
                var d = getDiagnostics();
                check(d.status().equals("ENABLED") && d.loadNanos() > 0 && d.enableNanos() > 0
                        && d.commands() >= 2);
                try {
                    getConfig().set("server.name", "saved");
                    saveConfig();
                    reloadConfig();
                    check(getConfig().getString("server.name", "").equals("saved"));
                } catch (IOException error) {
                    throw new IllegalStateException(error);
                }
            }
            case "temporary-service" -> {
                var manager = getServer().getServicesManager();
                var registration = manager.register(
                        Greeting.class, () -> "temporary", ServicePriority.HIGHEST);
                var temporary = manager.load(Greeting.class);
                check(temporary.greet().equals("temporary")
                        && manager.getProviders(Greeting.class).size() == 3);
                registration.close();
                registration.close();
                try {
                    temporary.greet();
                    throw new AssertionError("Unregistered service still invoked");
                } catch (IllegalStateException expected) {
                }
                check(manager.load(Greeting.class).greet().equals("high"));
            }
            case "alias-rollback" -> {
                try {
                    Commands.register(getServer(),
                            new CommandDefinition("tempalias", "", "", false, List.of("ecoalias"), List.of(),
                                    List.of(), context -> true));
                    throw new AssertionError("Duplicate alias accepted");
                } catch (IllegalStateException expected) {
                }
            }
            case "revoked" -> {
                try {
                    service.greet();
                    throw new AssertionError("Stale service invoked");
                } catch (IllegalStateException expected) {
                }
                check(getServer().getServicesManager().find(Greeting.class).isEmpty());
            }
            case "bad-reload" -> {
                try {
                    reloadConfig();
                    throw new AssertionError("Corrupt config accepted");
                } catch (IOException expected) {
                }
                try {
                    check(getConfig().getString("server.name", "").equals("saved"));
                } catch (IOException error) {
                    throw new AssertionError(error);
                }
            }
            case "worker" -> {
                var failed = new AtomicBoolean();
                var diagnosticsRejected = new AtomicBoolean();
                Thread thread = new Thread(() -> {
                    try {
                        service.greet();
                    } catch (IllegalStateException expected) {
                        failed.set(true);
                    }
                    try {
                        getDiagnostics();
                    } catch (IllegalStateException expected) {
                        diagnosticsRejected.set(true);
                    }
                });
                thread.start();
                try {
                    thread.join();
                } catch (InterruptedException error) {
                    throw new AssertionError(error);
                }
                check(failed.get() && diagnosticsRejected.get());
            }
            case "class-conflict" -> {
                try (var duplicate = new URLClassLoader(
                        new URL[]{
                                Greeting.class.getProtectionDomain().getCodeSource().getLocation()},
                        JavaPlugin.class.getClassLoader())) {
                    var type = duplicate.loadClass(Greeting.class.getName());
                    try {
                        getServer().getServicesManager().load(type);
                        throw new AssertionError("Duplicated interface accepted");
                    } catch (IllegalStateException expected) {
                    }
                } catch (Exception error) {
                    throw new AssertionError(error);
                }
            }
            default -> throw new AssertionError("Unknown ecosystem phase");
        }
        return true;
    }
}
