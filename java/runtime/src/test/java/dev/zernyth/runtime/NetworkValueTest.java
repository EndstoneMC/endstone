package dev.zernyth.runtime;

import java.time.Instant;
import java.util.Arrays;
import java.util.HashSet;
import java.util.Map;
import java.util.Objects;
import java.util.Optional;
import java.util.Set;
import java.util.UUID;

import dev.zernyth.api.network.NetworkProtocol;
import dev.zernyth.api.network.PacketContext;
import dev.zernyth.api.network.PacketDirection;
import dev.zernyth.api.network.PacketFilter;
import dev.zernyth.api.network.PacketSnapshot;
import dev.zernyth.api.network.PacketType;

final class NetworkValueTest {

    private static void invalid(Runnable run) {
        try {
            run.run();
            throw new AssertionError("Invalid packet value accepted");
        } catch (IllegalArgumentException expected) {
        }
    }

    static void run() {
        invalid(() -> new PacketType("invalid", -1, 2225));
        invalid(() -> new PacketType("invalid", 1024, 2225));
        invalid(() -> new PacketFilter(Set.of(), Set.of()));
        var source = new HashSet<PacketDirection>();
        source.add(PacketDirection.INBOUND);
        var filter = new PacketFilter(Set.of(), source);
        source.clear();
        if (filter.directions().size() != 1) {
            throw new AssertionError("Aliased filter");
        }
        var type = new PacketType("UNKNOWN", 1023, 2225);
        var context = new PacketContext(Optional.empty(), Optional.empty(), Instant.EPOCH,
                PacketContext.Phase.INBOUND_BEFORE_NATIVE_DESERIALIZATION, 0, false);
        invalid(()
                -> new PacketSnapshot(
                type, PacketDirection.INBOUND, new NetworkProtocol("other", 1), context, 0));
        invalid(()
                -> new PacketSnapshot(type, PacketDirection.INBOUND,
                new NetworkProtocol("26.60", 2225), context, -1));
        decoderSnapshots();
        System.out.println("Network value/filter/version/decoder checks passed (no BDS/JNI).");
    }

    private static void decoderSnapshots() {
        var protocol = new NetworkProtocol("26.60", 2225);
        var decoder = new Network.MetadataDecoder(protocol, Map.of(9, new PacketType("Text", 9, 2225)));
        String firstPlayer = "00000000-0000-0000-0000-000000000001";
        String secondPlayer = "00000000-0000-0000-0000-000000000002";
        var first = decoder.decode(9, 1, 100, 12, 0, false, firstPlayer, "17");
        var second = decoder.decode(9, 0, 101, 14, 1, true, secondPlayer, "18");
        if (!first.context().playerId().orElseThrow().equals(UUID.fromString(firstPlayer))
                || !first.context().connectionId().orElseThrow().equals("17")
                || first.direction() != PacketDirection.OUTBOUND
                || !second.context().playerId().orElseThrow().equals(UUID.fromString(secondPlayer))
                || !second.context().connectionId().orElseThrow().equals("18")
                || second.direction() != PacketDirection.INBOUND || !second.context().alreadyCancelled()) {
            throw new AssertionError("Decoder aliased retained snapshots or mixed player metadata");
        }
        var reconnect = decoder.decode(9, 1, 102, 0, 0, false, secondPlayer, "19");
        if (!reconnect.context().connectionId().orElseThrow().equals("19")
                || !second.context().connectionId().orElseThrow().equals("18")) {
            throw new AssertionError("Decoder reused a retired connection ID");
        }
        var anonymous = decoder.decode(1023, 0, 103, 0, 0, false, "", "");
        var repeated = decoder.decode(1023, 0, 104, 0, 0, false, "", "");
        if (anonymous.context().playerId().isPresent() || anonymous.context().connectionId().isPresent()
                || anonymous == repeated || anonymous.type() != repeated.type()
                || !anonymous.type().name().equals("UNKNOWN")) {
            throw new AssertionError("Decoder did not retain independent anonymous snapshots");
        }
        invalid(() -> decoder.decode(1024, 0, 0, 0, 0, false, "", ""));
        invalid(() -> decoder.decode(9, 0, 0, 0, 0, false, "malformed-uuid", "20"));
        var recovered = decoder.decode(9, 1, 105, 0, 0, false, secondPlayer, "19");
        if (!recovered.context().playerId().orElseThrow().equals(UUID.fromString(secondPlayer))) {
            throw new AssertionError("Invalid input corrupted the decoder cache");
        }
        for (int i = 0; i < 100; i++) {
            var overflow = decoder.decode(9, 0, 100 + i, 0, 0, false, firstPlayer, "session-" + i);
            if (!overflow.context().playerId().orElseThrow().equals(UUID.fromString(firstPlayer))) {
                throw new AssertionError("Identity cache capacity changed packet contents");
            }
        }
        for (int id = 0; id <= 1023; id++) {
            decoder.decode(id, 0, 1, 0, 0, false, "", "");
        }
        if (cacheSize(decoder, "identities") != 64 || cacheSize(decoder, "types") != 1024) {
            throw new AssertionError("Decoder caches exceeded their documented bounds");
        }
        var corrected = decoder.decode(9, 1, 200, 0, 0, false, firstPlayer, "19");
        if (!corrected.context().playerId().orElseThrow().equals(UUID.fromString(firstPlayer))) {
            throw new AssertionError("Decoder trusted a connection with different player metadata");
        }
        var other = new Network.MetadataDecoder(protocol, Map.of());
        if (!other.decode(9, 1, 100, 0, 0, false, "", "").type().name().equals("UNKNOWN")) {
            throw new AssertionError("Metadata decoder state leaked between registrations");
        }
    }


    private static int cacheSize(Network.MetadataDecoder decoder, String name) {
        try {
            var field = Network.MetadataDecoder.class.getDeclaredField(name);
            field.setAccessible(true);
            Object cached = field.get(decoder);
            if (cached instanceof Map<?, ?> entries) {
                return entries.size();
            }
            return (int) Arrays.stream((Object[]) cached).filter(Objects::nonNull).count();
        } catch (ReflectiveOperationException failure) {
            throw new AssertionError(failure);
        }
    }

}
