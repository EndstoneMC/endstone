package dev.zernyth.runtime;

import java.lang.management.ManagementFactory;
import java.nio.file.Files;
import java.nio.file.Path;
import java.time.Duration;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

import com.google.gson.GsonBuilder;
import dev.zernyth.api.network.NetworkProtocol;
import dev.zernyth.api.network.PacketType;
import dev.zernyth.api.performance.PerformanceOptions;
import dev.zernyth.api.scheduler.RuntimeLimits;
import dev.zernyth.api.scheduler.Task;
import dev.zernyth.api.scheduler.TaskKind;
import dev.zernyth.api.scheduler.TaskOptions;

/**
 * Local batch harness, not JMH. Percentiles describe batch means, not individual tail latency.
 */
public final class BenchmarkHarness {

    private static final int WARMUP = bounded("zernyth.benchmark.warmup", 3, 3, 20);
    private static final int ROUNDS = bounded("zernyth.benchmark.rounds", 15, 15, 50);
    private static final int SCALE = bounded("zernyth.benchmark.scale", 1, 1, 4);

    private static int bounded(String name, int fallback, int min, int max) {
        int value = Integer.getInteger(name, fallback);
        if (value < min || value > max) {
            throw new IllegalArgumentException("Invalid " + name);
        }
        return value;
    }

    private static final List<Map<String, Object>> METRICS = new ArrayList<>();
    public static volatile long sink;
    private static volatile Object objectSink;

    public static native int ping(int value);

    public static void record(String name, long[] nanos, int operations) {
        var sorted = Arrays.stream(nanos).mapToDouble(n -> (double) n / operations).sorted().toArray();
        var row = new LinkedHashMap<String, Object>();
        row.put("name", name);
        row.put("operationsPerSample", operations);
        row.put("samples", nanos.length);
        row.put("sampleMeansNs", sorted);
        row.put("medianNs", sorted[sorted.length / 2]);
        row.put("p95BatchMeanNs", sorted[(int) Math.ceil(sorted.length * .95) - 1]);
        row.put("p99Ns", null);
        row.put("opsPerSecondFromMedian", 1e9 / sorted[sorted.length / 2]);
        METRICS.add(row);
    }

    /**
     * Reads allocated bytes for this attached platform thread, outside a timed batch.
     *
     * @return cumulative bytes, or -1 when the JVM does not expose this measurement
     */
    public static long threadAllocatedBytes() {
        var bean = ManagementFactory.getThreadMXBean();
        if (bean instanceof com.sun.management.ThreadMXBean allocation
                && allocation.isThreadAllocatedMemorySupported()) {
            return allocation.getThreadAllocatedBytes(Thread.currentThread().threadId());
        }
        return -1;
    }

    /**
     * Adds an allocation observation to the most recently recorded native batch series.
     *
     * @param bytes measured Java allocation, or -1 when unavailable
     * @param operations total measured operations across the series, positive
     */
    public static void recordAllocatedBytes(long bytes, int operations) {
        METRICS.getLast().put("javaAllocatedBytesPerOperation",
                bytes >= 0 ? (double) bytes / operations : null);
    }

    public static void measure(String name, Runnable action, int operations) {
        String filter = System.getProperty("zernyth.benchmark.filter", "");
        if (!filter.isEmpty() && !name.contains(filter)) {
            return;
        }
        operations = Math.multiplyExact(operations, SCALE);
        int warmup = WARMUP, rounds = ROUNDS;
        for (int w = 0; w < warmup; w++) {
            for (int i = 0; i < operations; i++) {
                action.run();
            }
        }
        long[] times = new long[rounds];
        var bean = ManagementFactory.getThreadMXBean();
        com.sun.management.ThreadMXBean alloc =
                bean instanceof com.sun.management.ThreadMXBean b && b.isThreadAllocatedMemorySupported()
                        ? b
                        : null;
        long thread = Thread.currentThread().threadId();
        long before = alloc != null ? alloc.getThreadAllocatedBytes(thread) : -1;
        long cpuBefore = bean.isCurrentThreadCpuTimeSupported() ? bean.getCurrentThreadCpuTime() : -1;
        for (int r = 0; r < rounds; r++) {
            long start = System.nanoTime();
            for (int i = 0; i < operations; i++) {
                action.run();
            }
            times[r] = System.nanoTime() - start;
        }
        long cpuAfter = bean.isCurrentThreadCpuTimeSupported() ? bean.getCurrentThreadCpuTime() : -1;
        long after = alloc != null ? alloc.getThreadAllocatedBytes(thread) : -1;
        record
                (name, times, operations);
        var row = METRICS.getLast();
        row.put("javaAllocatedBytesPerOperation",
                before >= 0 && after >= before ? (double) (after - before) / (operations * rounds) : null);
        row.put("currentPlatformThreadCpuNsPerOperation",
                cpuBefore >= 0 && cpuAfter >= cpuBefore
                        ? (double) (cpuAfter - cpuBefore) / (operations * rounds)
                        : null);
    }

    public static void basic(String jar) throws Exception {
        measure("java-direct-control", () -> sink++, 10000);
        if (!Boolean.getBoolean("zernyth.benchmark.javaOnly")) {
            measure("jni-synthetic-ping", () -> sink = ping(17), 10000);
        }
        Runnable listener = () -> sink++;
        var method = listener.getClass().getMethod("run");
        measure("java-reflection-listener", () -> {
            try {
                method.invoke(listener);
            } catch (Exception e) {
                throw new AssertionError(e);
            }
        }, 10000);
        packetMetadata();
        measure("production-classloader-create-close", () -> {
            try (var loader = new PluginClassLoader(Path.of(jar), List.of(), List.of(), List.of())) {
                sink = loader.classes.size();
            } catch (Exception e) {
                throw new AssertionError(e);
            }
        }, 20);
    }

    private static void packetMetadata() {
        var protocol = new NetworkProtocol("26.60", 2225);
        var types = Map.of(9, new PacketType("Text", 9, 2225));
        var repeated = new Network.MetadataDecoder(protocol, types);
        String player = "00000000-0000-0000-0000-000000000001";
        measure("java-packet-metadata-repeated-player", () -> objectSink = repeated.decode(
                9, 1, 1234, 16, 0, false,
                new String(player.toCharArray()), new String("17".toCharArray())), 10000);
        var anonymous = new Network.MetadataDecoder(protocol, types);
        measure("java-packet-metadata-unknown-anonymous", () -> objectSink = anonymous.decode(
                1023, 0, 1234, 16, 0, false, "", ""), 10000);
        packetPlayers(protocol, types, 64);
        packetPlayers(protocol, types, 128);
    }

    private static void packetPlayers(NetworkProtocol protocol, Map<Integer, PacketType> types, int count) {
        var alternating = new Network.MetadataDecoder(protocol, types);
        String[] players = new String[count];
        String[] connections = new String[count];
        for (int i = 0; i < players.length; i++) {
            players[i] = "00000000-0000-0000-0000-" + String.format("%012x", i + 1);
            connections[i] = Integer.toString(i + 1);
        }
        int[] next = {0};
        measure("java-packet-metadata-" + count + "-players", () -> {
            int index = next[0]++ % players.length;
            objectSink = alternating.decode(9, 1, 1234, 16, 0, false,
                    new String(players[index].toCharArray()), new String(connections[index].toCharArray()));
        }, 10000);
    }

    public static void finish() throws Exception {
        var result = new LinkedHashMap<String, Object>();
        result.put("java", System.getProperty("java.version"));
        result.put("vendor", System.getProperty("java.vendor"));
        result.put("os",
                System.getProperty("os.name") + " " + System.getProperty("os.version") + " "
                        + System.getProperty("os.arch"));
        result.put("jvmArguments", ManagementFactory.getRuntimeMXBean().getInputArguments());
        result.put("warmupRounds", WARMUP);
        result.put("measurementRounds", ROUNDS);
        result.put("operationScale", SCALE);
        result.put("methodology",
                "Single-process local batch harness; batch means, no independent forks or JMH isolation; P99 "
                        + "deliberately omitted; no BDS.");
        result.put("metrics", METRICS);
        result.put("heapUsedBytes", ManagementFactory.getMemoryMXBean().getHeapMemoryUsage().getUsed());
        result.put("gc",
                ManagementFactory.getGarbageCollectorMXBeans()
                        .stream()
                        .map(b
                                -> Map.of("name", b.getName(), "count", b.getCollectionCount(), "timeMillis",
                                b.getCollectionTime()))
                        .toList());
        Path output =
                Path.of(System.getProperty("zernyth.benchmark.output", System.getenv("ZERNYTH_PERF_OUTPUT")));
        Files.createDirectories(output.toAbsolutePath().getParent());
        Files.writeString(
                output, new GsonBuilder().serializeNulls().setPrettyPrinting().create().toJson(result));
    }

    public static void main(String[] args) throws Exception {
        System.setProperty("zernyth.benchmark.javaOnly", "true");
        basic(args[0]);
        if (args.length > 1) {
            var host = new ManagedRuntime.Host() {
                public void checkMain() {
                }

                public Task pulse(Runnable callback) {
                    return stub();
                }

                public Task cpu(Runnable callback) {
                    throw new AssertionError("Not a worker benchmark");
                }

                private Task stub() {
                    return new Task() {
                        public void cancel() {
                        }

                        public boolean isCancelled() {
                            return false;
                        }
                    };
                }
            };
            var profile = new Profile(message -> {
            });
            var runtime = new ManagedRuntime(host, BenchmarkHarness.class.getClassLoader(), 1, profile);
            runtime.activate();
            runtime.ensureStarted();
            measure("managed-admission-cancel-java-mock", () -> runtime.submitCpu(t -> {
            }).cancel(), 2000);
            measure("managed-server-enqueue-pulse-java-mock", () -> {
                runtime.returnToServer(() -> sink++);
                runtime.pulse();
            }, 2000);
            runtime.close();
            delayedQueue(host, 256);
            delayedQueue(host, 4096);
            bulkCancellation(host);
            profile.options = new PerformanceOptions(
                    true, Duration.ofDays(1), 100);
            measure("java-profile-enabled-callback",
                    () -> profile.measure("event", "probe", () -> ++sink), 10000);
            measure("java-isolated-lifecycle-and-cleanup", () -> {
                try (var plugin = new Bootstrap(args[1], 0)) {
                    plugin.load();
                    plugin.enable();
                    plugin.disable();
                } catch (Exception e) {
                    throw new AssertionError(e);
                }
            }, 20);
        }
        finish();
    }

    private static void delayedQueue(ManagedRuntime.Host host, int count) {
        var runtime = new ManagedRuntime(host, BenchmarkHarness.class.getClassLoader(), count,
                new Profile(message -> { }));
        runtime.activate();
        runtime.configure(new RuntimeLimits(4096, 1, 1, 256));
        var delayed = new TaskOptions(1_000_000_000, 0, Duration.ofSeconds(30));
        try {
            for (int i = 0; i < count; i++) {
                runtime.submit(TaskKind.SERVER, token -> {
                    throw new AssertionError("Delayed callback executed during idle measurement");
                }, delayed);
            }
            measure("managed-delayed-" + count + "-pulse", runtime::pulse, 200);
            if (runtime.snapshot().outstanding() != count) {
                throw new AssertionError("Delayed queue lost tasks");
            }
        } finally {
            runtime.close();
        }
    }

    private static void bulkCancellation(ManagedRuntime.Host host) {
        var runtime = new ManagedRuntime(host, BenchmarkHarness.class.getClassLoader(), 5000,
                new Profile(message -> { }));
        runtime.activate();
        runtime.configure(new RuntimeLimits(256, 1, 1, 256));
        var options = new TaskOptions(1_000_000_000, 0, Duration.ofSeconds(30));
        try {
            measure("managed-256-admit-cancel-all", () -> {
                for (int i = 0; i < 256; i++) {
                    runtime.submit(TaskKind.SERVER, token -> { }, options);
                }
                runtime.cancelAll();
                if (runtime.snapshot().outstanding() != 0) {
                    throw new AssertionError("Bulk cancellation retained tasks");
                }
            }, 10);
        } finally {
            runtime.close();
        }
    }
}
