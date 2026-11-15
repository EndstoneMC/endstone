package dev.zernyth.fixture;

import java.util.function.Supplier;

import dev.zernyth.api.event.EventHandler;
import dev.zernyth.api.event.EventPriority;
import dev.zernyth.api.event.Listener;
import dev.zernyth.api.event.PlayerJoinEvent;
import dev.zernyth.api.network.PacketFilter;
import dev.zernyth.api.plugin.JavaPlugin;
import dev.zernyth.runtime.BenchmarkHarness;

/**
 * Connects controlled native benchmark commands to measurements; results are not server-capacity guarantees.
 */
public final class PerformancePlugin extends JavaPlugin implements Listener {

    @EventHandler
    public void join(PlayerJoinEvent event) {
        BenchmarkHarness.sink++;
    }

    @Override
    public void onEnable() {
        getServer().getPluginManager().registerEvents(this, this);
        getServer().getNetworkManager().listen(
                PacketFilter.all(), EventPriority.NORMAL, p -> BenchmarkHarness.sink++);
        var player = getServer().getOnlinePlayers().getFirst();
        var services = getServer().getServicesManager();
        getServer().registerCommand("perfbench", "", "", (sender, command, args) -> {
            switch (args[0]) {
                case "packet-listeners" -> {
                    for (int i = 0; i < 3; i++) {
                        getServer().getNetworkManager().listen(
                                PacketFilter.all(), EventPriority.NORMAL, packet -> BenchmarkHarness.sink++);
                    }
                }
                case "players" -> BenchmarkHarness.measure("jni-production-player-list",
                        () -> BenchmarkHarness.sink = getServer().getOnlinePlayers().size(), 2000);
                case "strings" -> BenchmarkHarness.measure("jni-production-player-strings",
                        () -> BenchmarkHarness.sink = player.getName().length(), 2000);
                case "services" -> BenchmarkHarness.measure("jni-production-service-lookup",
                        ()
                                -> BenchmarkHarness.sink =
                                services.find(Supplier.class).isPresent()
                                        ? 1
                                        : 0,
                        2000);
                case "registration" -> BenchmarkHarness.measure("jni-listener-register-close",
                        ()
                                -> getServer()
                                .getPluginManager()
                                .registerEvents(
                                        new Listener() {
                                            @EventHandler
                                            public void event(PlayerJoinEvent e) {
                                            }
                                        },
                                        this)
                                .close(),
                        100);
                case "schedule" -> BenchmarkHarness.measure("jni-schedule-cancel",
                        () -> getServer().getScheduler().runTaskLater(() -> {
                        }, 1000000).cancel(), 100);
                default -> throw new AssertionError();
            }
            return true;
        });
    }
}
