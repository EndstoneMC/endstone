package dev.zernyth.runtime;

import java.time.Instant;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.Optional;
import java.util.Set;
import java.util.UUID;

import dev.zernyth.api.event.EventPriority;
import dev.zernyth.api.event.Registration;
import dev.zernyth.api.network.NetworkCapability;
import dev.zernyth.api.network.NetworkConnection;
import dev.zernyth.api.network.NetworkManager;
import dev.zernyth.api.network.NetworkMetrics;
import dev.zernyth.api.network.NetworkProtocol;
import dev.zernyth.api.network.PacketContext;
import dev.zernyth.api.network.PacketDirection;
import dev.zernyth.api.network.PacketFilter;
import dev.zernyth.api.network.PacketListener;
import dev.zernyth.api.network.PacketSnapshot;
import dev.zernyth.api.network.PacketType;
import dev.zernyth.api.network.connection.ConnectionInfo;
import dev.zernyth.api.network.connection.ConnectionState;
import dev.zernyth.api.network.exception.UnsupportedPacketException;

final class Network implements NetworkManager {

    private final long owner;
    private final Profile profile;
    private long listenerNanos, maxListenerNanos;
    private boolean diagnostics;

    Network(long owner, Profile profile) {
        this.owner = owner;
        this.profile = profile;
    }

    private String[] info() {
        return Native.networkInfo(owner);
    }

    private static NetworkProtocol protocol(String[] info) {
        return new NetworkProtocol(info[0], Integer.parseInt(info[1]));
    }

    public NetworkProtocol getProtocol() {
        return protocol(info());
    }

    public Set<NetworkCapability> getCapabilities() {
        return info()[2].equals("1")
                ? Set.of(NetworkCapability.PACKET_METADATA, NetworkCapability.PLAYER_CONNECTION_LIFETIME)
                : Set.of();
    }

    public List<PacketType> getPacketTypes() {
        String[] data = info();
        if (!data[2].equals("1")) {
            return List.of();
        }
        var result = new ArrayList<PacketType>();
        for (int i = 3; i < data.length; i += 2) {
            result.add(new PacketType(data[i], Integer.parseInt(data[i + 1]), Integer.parseInt(data[1])));
        }
        return List.copyOf(result);
    }

    public Registration listen(PacketFilter filter, EventPriority priority, PacketListener listener) {
        Objects.requireNonNull(filter);
        Objects.requireNonNull(priority);
        Objects.requireNonNull(listener);
        String[] data = info();
        if (!data[2].equals("1")) {
            throw new UnsupportedPacketException("No metadata adapter for " + protocol(data));
        }
        NetworkProtocol protocol = protocol(data);
        for (var type : filter.types()) {
            if (type.protocolVersion() != protocol.protocolVersion()) {
                throw new UnsupportedPacketException("Packet filter belongs to another protocol");
            }
        }
        var types = new HashMap<Integer, PacketType>();
        for (int i = 3; i < data.length; i += 2) {
            types.put(Integer.parseInt(data[i + 1]),
                    new PacketType(data[i], Integer.parseInt(data[i + 1]), protocol.protocolVersion()));
        }
        var decoder = new MetadataDecoder(protocol, types);
        int directions = filter.directions().stream().mapToInt(d -> 1 << d.ordinal()).sum();
        long token = Native.networkListen(owner, directions,
                filter.types().stream().mapToInt(PacketType::identifier).distinct().toArray(),
                priority.ordinal(), (id, direction, millis, size, sub, cancelled, player, connection) -> {
                    var snapshot = decoder.decode(
                            id, direction, millis, size, sub, cancelled, player, connection);
                    boolean timed = diagnostics;
                    long start = timed ? System.nanoTime() : 0;
                    try {
                        profile.measure("packet", "metadata", () -> {
                            listener.onPacket(snapshot);
                            return null;
                        });
                    } finally {
                        if (timed) {
                            long elapsed = System.nanoTime() - start;
                            listenerNanos += elapsed;
                            maxListenerNanos = Math.max(maxListenerNanos, elapsed);
                        }
                    }
                });
        return () -> Native.networkRemove(owner, token);
    }

    /**
     * Server-thread-only decoder owned by one native listener registration.
     * Caches contain detached immutable values and expire with the registered callback.
     */
    static final class MetadataDecoder {
        private static final PacketDirection[] DIRECTIONS = PacketDirection.values();
        private final NetworkProtocol protocol;
        private final PacketType[] types = new PacketType[1024];
        private static final int IDENTITY_CACHE_LIMIT = 64;
        private final Map<String, Identity> identities = new HashMap<>();

        private record Identity(String playerText, Optional<UUID> playerId, Optional<String> connectionId) {
        }

        MetadataDecoder(NetworkProtocol protocol, Map<Integer, PacketType> types) {
            this.protocol = protocol;
            types.forEach((identifier, type) -> this.types[identifier] = type);
        }

        PacketSnapshot decode(int id, int direction, long millis, long size, int sub, boolean cancelled,
                String player, String connection) {
            if (id < 0 || id >= types.length) {
                throw new IllegalArgumentException("Invalid packet identifier");
            }
            var identity = connection.isEmpty() ? null : identities.get(connection);
            Optional<UUID> playerId;
            Optional<String> connectionId;
            if (identity != null && identity.playerText().equals(player)) {
                playerId = identity.playerId();
                connectionId = identity.connectionId();
            } else {
                playerId = player.isEmpty() ? Optional.empty() : Optional.of(UUID.fromString(player));
                connectionId = connection.isEmpty() ? Optional.empty() : Optional.of(connection);
                if (!connection.isEmpty() && (identity != null || identities.size() < IDENTITY_CACHE_LIMIT)) {
                    identities.put(connection, new Identity(player, playerId, connectionId));
                }
            }
            var context = new PacketContext(playerId, connectionId,
                    Instant.ofEpochMilli(millis),
                    direction == 0 ? PacketContext.Phase.INBOUND_BEFORE_NATIVE_DESERIALIZATION
                            : PacketContext.Phase.OUTBOUND_SERIALIZED_BEFORE_BATCH,
                    sub, cancelled);
            var type = types[id];
            if (type == null) {
                type = new PacketType("UNKNOWN", id, protocol.protocolVersion());
                types[id] = type;
            }
            return new PacketSnapshot(type, DIRECTIONS[direction], protocol, context, size);
        }
    }

    public List<NetworkConnection> getConnections() {
        var result = new ArrayList<NetworkConnection>();
        var values = Native.networkConnections(owner);
        for (int i = 0; i < values.length; i += 2) {
            String id = values[i];
            UUID uuid = UUID.fromString(values[i + 1]);
            result.add(new NetworkConnection() {
                public boolean isActive() {
                    return Native.networkActive(owner, id);
                }

                public ConnectionInfo getInfo() {
                    return new ConnectionInfo(id, uuid,
                            isActive() ? ConnectionState.PLAYER_ASSOCIATED : ConnectionState.CLOSED);
                }
            });
        }
        return List.copyOf(result);
    }

    public void setDiagnosticsEnabled(boolean enabled) {
        Native.networkMetrics(owner, enabled ? 1 : 0);
        diagnostics = enabled;
    }

    public NetworkMetrics getMetrics() {
        long[] m = Native.networkMetrics(owner, -1);
        return new NetworkMetrics(
                m[0], m[1], m[2], m[3], m[4], m[5], m[6], listenerNanos, maxListenerNanos, m[7] != 0);
    }
}
