package dev.zernyth.fixture;

import java.lang.reflect.InvocationTargetException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.concurrent.atomic.AtomicBoolean;

import dev.zernyth.api.event.EventPriority;
import dev.zernyth.api.event.Registration;
import dev.zernyth.api.network.NetworkCapability;
import dev.zernyth.api.network.NetworkConnection;
import dev.zernyth.api.network.NetworkManager;
import dev.zernyth.api.network.PacketContext;
import dev.zernyth.api.network.PacketDirection;
import dev.zernyth.api.network.PacketFilter;
import dev.zernyth.api.network.PacketSnapshot;
import dev.zernyth.api.network.PacketType;
import dev.zernyth.api.network.connection.ConnectionState;
import dev.zernyth.api.network.exception.UnsupportedPacketException;
import dev.zernyth.api.plugin.JavaPlugin;

/**
 * Native-mock fixture for read-only packet filters, connection lifetime and listener cleanup.
 */
public final class NetworkPlugin extends JavaPlugin {

    private NetworkManager network;
    private boolean enabled;
    private final List<PacketSnapshot> packets = new ArrayList<>();
    private final StringBuilder order = new StringBuilder();
    private int high, filtered, selfCalls, savedCount;
    private Registration low, upper, throwing, selected, slow, self;
    private NetworkConnection connection;
    private NetworkConnection peerConnection;

    private static void check(boolean condition) {
        if (!condition) {
            throw new AssertionError("Network fixture failed");
        }
    }

    private static void rejected(Runnable r) {
        try {
            r.run();
            throw new AssertionError("Native access accepted");
        } catch (IllegalStateException expected) {
        }
    }

    private PacketType text() {
        return network.getPacketTypes()
                .stream()
                .filter(t -> t.name().equals("Text"))
                .findFirst()
                .orElseThrow();
    }

    @Override
    public void onEnable() {
        enabled = true;
        network = getServer().getNetworkManager();
        getServer().registerCommand(getDescription().name().equals("network_peer") ? "netpeer" : "nettest",
                "", "", (sender, command, args) -> {
                    switch (args[0]) {
                        case "init" -> {
                            check(network.getProtocol().reportedRelease().equals("26.60")
                                    && network.getProtocol().protocolVersion() == 2225);
                            check(network.getCapabilities().equals(Set.of(NetworkCapability.PACKET_METADATA,
                                    NetworkCapability.PLAYER_CONNECTION_LIFETIME)));
                            check(!network.getCapabilities().contains(NetworkCapability.ASYNC_OBSERVATION));
                            check(network.getPacketTypes().size() == 4);
                            connection = network.getConnections().getFirst();
                            check(connection.isActive());
                            network.setDiagnosticsEnabled(true);
                            low = network.listen(PacketFilter.all(), EventPriority.LOW, p -> {
                                check(enabled);
                                packets.add(p);
                                order.append("L");
                            });
                            throwing = network.listen(PacketFilter.all(), EventPriority.NORMAL, p -> {
                                throw new IllegalArgumentException("expected packet listener failure");
                            });
                            upper = network.listen(PacketFilter.all(), EventPriority.HIGH, p -> {
                                ++high;
                                order.append("H");
                            });
                            selected = network.listen(
                                    new PacketFilter(Set.of(text()), Set.of(PacketDirection.OUTBOUND)),
                                    EventPriority.MONITOR, p -> ++filtered);
                        }
                        case "check" -> {
                            check(packets.size() == 4 && high == 4 && filtered == 1
                                    && order.toString().equals("LHLHLHLH"));
                            var first = packets.getFirst();
                            check(first.type().identifier() == text().identifier()
                                    && first.direction() == PacketDirection.INBOUND);
                            check(first.context().phase()
                                    == PacketContext.Phase.INBOUND_BEFORE_NATIVE_DESERIALIZATION
                                    && first.context().playerId().isPresent());
                            check(first.context().connectionId().orElseThrow().equals(
                                    connection.getInfo().identifier()));
                            check(first.context().alreadyCancelled() && first.payloadSize() == 6
                                    && first.context().observedAt().toEpochMilli() > 0);
                            var unknown = packets.getLast();
                            check(unknown.type().name().equals("UNKNOWN")
                                    && unknown.type().identifier() == 1023
                                    && unknown.context().playerId().isEmpty());
                            check(packets.get(1).direction() == PacketDirection.OUTBOUND
                                    && packets.get(1).context().phase()
                                    == PacketContext.Phase.OUTBOUND_SERIALIZED_BEFORE_BATCH);
                            var m = network.getMetrics();
                            check(m.callbackExceptions() == 4 && m.bridgeNanos() > 0
                                    && m.listenerNanos() > 0);
                            throwing.close();
                            selected.close();
                            selected.close();
                        }
                        case "worker" -> {
                            var rejected = new AtomicBoolean();
                            var thread = new Thread(() -> {
                                try {
                                    network.getConnections();
                                } catch (IllegalStateException expected) {
                                    rejected.set(true);
                                }
                            });
                            thread.start();
                            try {
                                thread.join();
                            } catch (InterruptedException error) {
                                throw new AssertionError(error);
                            }
                            check(rejected.get());
                        }
                        case "self" -> self = network.listen(PacketFilter.all(), EventPriority.NORMAL, p -> {
                            ++selfCalls;
                            self.close();
                        });
                        case "self-check" -> check(selfCalls == 1);
                        case "slow" -> slow = network.listen(PacketFilter.all(), EventPriority.HIGHEST, p -> {
                            try {
                                Thread.sleep(20);
                            } catch (InterruptedException error) {
                                throw new AssertionError(error);
                            }
                        });
                        case "slow-check" -> {
                            check(network.getMetrics().slowCallbacks() >= 1);
                            slow.close();
                            network.setDiagnosticsEnabled(false);
                        }
                        case "multiple-connections" -> {
                            var connections = network.getConnections();
                            check(connections.size() == 2);
                            peerConnection = connections.get(1);
                            check(!peerConnection.getInfo().identifier()
                                    .equals(connection.getInfo().identifier()));
                            check(!peerConnection.getInfo().playerId()
                                    .equals(connection.getInfo().playerId()));
                        }
                        case "multiple-player-packet" -> {
                            check(packets.getLast().context().playerId().orElseThrow()
                                    .equals(peerConnection.getInfo().playerId()));
                            check(packets.getFirst().context().playerId().orElseThrow()
                                    .equals(connection.getInfo().playerId()));
                        }
                        case "multiple-removed" -> {
                            check(!peerConnection.isActive() && connection.isActive());
                            var connections = network.getConnections();
                            check(connections.size() == 1 && connections.getFirst().getInfo().identifier()
                                    .equals(connection.getInfo().identifier()));
                        }
                        case "disconnect" -> {
                            check(!connection.isActive()
                                    && connection.getInfo().state() == ConnectionState.CLOSED);
                            check(network.getConnections().isEmpty());
                            check(packets.getFirst().context().playerId().isPresent()); // Retained snapshot
                            // survives native
                            // event and player.
                        }
                        case "rejoin" -> {
                            var next = network.getConnections().getFirst();
                            check(!connection.isActive());
                            check(next.isActive()
                                    && !next.getInfo().identifier().equals(
                                    connection.getInfo().identifier()));
                        }
                        case "unsupported" -> {
                            check(network.getCapabilities().isEmpty() && network.getPacketTypes().isEmpty());
                            try {
                                network.listen(PacketFilter.all(), EventPriority.NORMAL, p -> {
                                });
                                throw new AssertionError();
                            } catch (UnsupportedPacketException expected) {
                            }
                        }
                        case "version-count" -> savedCount = packets.size();
                        case "version-unchanged" -> check(savedCount == packets.size());
                        case "remember-connection" -> connection = network.getConnections().getFirst();
                        case "runtime-changed" -> {
                            check(!connection.isActive());
                            check(!network.getConnections().getFirst().getInfo().identifier().equals(
                                    connection.getInfo().identifier()));
                        }
                        case "runtime-retired" -> check(!connection.isActive());
                        case "foreign-filter" -> {
                            try {
                                network.listen(new PacketFilter(
                                                Set.of(new PacketType("Text", text().identifier(), 1)),
                                                Set.of(PacketDirection.INBOUND)),
                                        EventPriority.NORMAL, p -> {
                                        });
                                throw new AssertionError();
                            } catch (UnsupportedPacketException expected) {
                            }
                        }
                        case "limit" -> {
                            low.close();
                            upper.close();
                            var registrations = new ArrayList<Registration>();
                            for (int i = 0; i < 64; i++) {
                                registrations.add(
                                        network.listen(PacketFilter.all(), EventPriority.NORMAL, p -> {
                                        }));
                            }
                            rejected(() -> network.listen(PacketFilter.all(), EventPriority.NORMAL, p -> {
                            }));
                            registrations.forEach(Registration::close);
                            low = network.listen(PacketFilter.all(), EventPriority.NORMAL, p -> {
                                try {
                                    Files.writeString(Path.of("network-late.marker"), "unexpected");
                                } catch (Exception error) {
                                    throw new AssertionError(error);
                                }
                            });
                        }
                        case "invalid" -> {
                            try {
                                var method = Class.forName("dev.zernyth.runtime.Native")
                                        .getDeclaredMethod("networkInfo", long.class);
                                method.setAccessible(true);
                                try {
                                    method.invoke(null, Long.MAX_VALUE);
                                    throw new AssertionError();
                                } catch (InvocationTargetException expected) {
                                    check(expected.getCause() instanceof IllegalStateException);
                                }
                                var field = network.getClass().getDeclaredField("owner");
                                field.setAccessible(true);
                                var active =
                                        Class.forName("dev.zernyth.runtime.Native")
                                                .getDeclaredMethod("networkActive", long.class, String.class);
                                active.setAccessible(true);
                                check(Boolean.FALSE.equals(
                                        active.invoke(null, field.getLong(network), "invalid-connection")));
                            } catch (ReflectiveOperationException error) {
                                throw new AssertionError(error);
                            }
                        }
                        case "peer" ->
                                upper = network.listen(PacketFilter.all(), EventPriority.NORMAL, p -> ++high);
                        case "peer-check" -> check(high == 1);
                        case "close" -> {
                            low.close();
                            upper.close();
                            check(packets.getFirst().payloadSize() == 6);
                        }
                        default -> throw new AssertionError("Unknown phase");
                    }
                    return true;
                });
    }

    @Override
    public void onDisable() {
        enabled = false;
    }
}
