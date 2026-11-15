package dev.zernyth.runtime;

import java.nio.file.Files;
import java.nio.file.Path;
import java.time.Duration;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Queue;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicLong;
import java.util.concurrent.atomic.AtomicReference;

import com.google.gson.GsonBuilder;
import dev.zernyth.api.performance.PerformanceOptions;
import dev.zernyth.api.scheduler.RuntimeLimits;
import dev.zernyth.api.scheduler.Task;
import dev.zernyth.api.scheduler.TaskHandle;
import dev.zernyth.api.scheduler.TaskKind;
import dev.zernyth.api.scheduler.TaskOptions;
import dev.zernyth.api.scheduler.TaskRejectedException;
import dev.zernyth.api.scheduler.TaskSnapshot;
import dev.zernyth.api.scheduler.TaskStatus;

/**
 * Pure Java admission/ownership tests; Host is a mock, not BDS or JNI.
 */
final class ManagedRuntimeTest {

    static final AtomicInteger checks = new AtomicInteger();

    static void check(boolean condition, String message) {
        checks.incrementAndGet();
        if (!condition) {
            throw new AssertionError(message);
        }
    }

    static void expect(Class<? extends Throwable> type, Runnable body) {
        try {
            body.run();
        } catch (Throwable e) {
            check(type.isInstance(e), "Unexpected exception: " + e);
            return;
        }
        throw new AssertionError("Expected " + type);
    }

    static final class Host implements ManagedRuntime.Host {

        final Thread main = Thread.currentThread();
        Runnable pulse;
        final Queue<Runnable> cpu = new ArrayDeque<>();

        public void checkMain() {
            if (Thread.currentThread() != main) {
                throw new IllegalStateException("server thread required");
            }
        }

        Task task() {
            return new Task() {
                boolean cancelled;

                public void cancel() {
                    checkMain();
                    cancelled = true;
                }

                public boolean isCancelled() {
                    return cancelled;
                }
            };
        }

        public Task pulse(Runnable r) {
            checkMain();
            pulse = r;
            return task();
        }

        public Task cpu(Runnable r) {
            checkMain();
            cpu.add(r);
            return task();
        }

        void tick() {
            pulse.run();
        }
    }

    static TaskSnapshot await(TaskHandle h) throws Exception {
        return h.completion().toCompletableFuture().get(3, TimeUnit.SECONDS);
    }

    static void run() throws Exception {
        delayedQueueRegressions();
        boundedWorkerLoad();
        var logs = new CopyOnWriteArrayList<String>();
        var profile = new Profile(logs::add);
        profile.options = new PerformanceOptions(true, Duration.ofNanos(1), 100);
        var host = new Host();
        var loader = new ClassLoader() {
        };
        var runtime = new ManagedRuntime(host, loader, 701, profile);
        expect(TaskRejectedException.class, () -> runtime.submitCpu(t -> {
        }));
        runtime.activate();
        runtime.ensureStarted();
        runtime.configure(new RuntimeLimits(2, 1, 1, 2));
        var a = runtime.submit(TaskKind.SERVER,
                t -> check(Thread.currentThread() == host.main, "sync thread"), TaskOptions.immediate());
        var pending = runtime.submitCpu(t -> {
            throw new AssertionError("cancelled task ran");
        });
        expect(TaskRejectedException.class, () -> runtime.submitIo(t -> {
        }));
        pending.cancel();
        check(await(pending).status() == TaskStatus.CANCELLED, "queued cancellation");
        host.tick();
        check(await(a).status() == TaskStatus.SUCCEEDED, "sync success");
        check(a.snapshot().generation() == 701, "generation");
        var count = new AtomicInteger();
        var repeating = runtime.submit(
                TaskKind.SERVER, t -> count.incrementAndGet(), new TaskOptions(2, 2, Duration.ofSeconds(1)));
        host.tick();
        check(count.get() == 0, "delay");
        host.tick();
        check(count.get() == 1, "first run");
        host.tick();
        check(count.get() == 1, "period gap");
        host.tick();
        check(count.get() == 2, "repeat");
        repeating.cancel();
        check(await(repeating).runs() == 2, "repeat cancellation");
        var cpu = runtime.submitCpu(
                t -> check(Thread.currentThread().getContextClassLoader() == loader, "CPU TCCL"));
        host.tick();
        check(cpu.snapshot().status() == TaskStatus.DISPATCHED, "CPU delegated");
        Thread worker = Thread.ofPlatform().daemon().unstarted(host.cpu.remove());
        worker.start();
        worker.join(2000);
        check(await(cpu).status() == TaskStatus.SUCCEEDED, "CPU completed");
        var handoff = new AtomicReference<TaskHandle>();
        var io = runtime.submitIo(t -> {
            check(Thread.currentThread().isVirtual(), "virtual I/O thread");
            check(Thread.currentThread().getContextClassLoader() == loader, "I/O TCCL");
            expect(IllegalStateException.class, () -> runtime.submitCpu(c -> {
            }));
            handoff.set(runtime.returnToServer(
                    () -> check(Thread.currentThread() == host.main, "handoff thread")));
        });
        host.tick();
        check(await(io).status() == TaskStatus.SUCCEEDED, "I/O success");
        host.tick();
        check(await(handoff.get()).status() == TaskStatus.SUCCEEDED, "handoff success");
        var failed = runtime.submitCpu(t -> {
            throw new IllegalArgumentException("expected");
        });
        host.tick();
        var failureWorker = Thread.ofPlatform().daemon().start(host.cpu.remove());
        failureWorker.join(2000);
        check(await(failed).status() == TaskStatus.FAILED, "exception status");
        var entered = new CountDownLatch(1);
        var release = new CountDownLatch(1);
        var running = runtime.submit(TaskKind.IO, t -> {
            entered.countDown();
            // Deliberately ignore interruption until released: cancellation must remain nonterminal.
            while (release.getCount() > 0) {
                try {
                    release.await();
                } catch (InterruptedException ignored) {
                }
            }
        }, new TaskOptions(0, 0, Duration.ofNanos(1)));
        host.tick();
        check(entered.await(2, TimeUnit.SECONDS), "worker entered");
        host.tick();
        check(running.snapshot().timedOut(), "timeout diagnosis");
        runtime.close();
        check(running.snapshot().status() == TaskStatus.CANCEL_REQUESTED,
                "running cancellation remains pending");
        check(!running.completion().toCompletableFuture().isDone(),
                "noncooperative task not reported finished");
        expect(TaskRejectedException.class, () -> runtime.returnToServer(() -> {
        }));
        release.countDown();
        check(await(running).status() == TaskStatus.CANCELLED, "eventual cancellation");
        check(runtime.snapshot().outstanding() == 0, "cleanup outstanding");
        check(runtime.snapshot().globalOutstanding() == 0, "global cleanup");
        check(runtime.snapshot().rejected() >= 2, "rejection metrics");
        check(profile.snapshot().stream().anyMatch(p -> p.exceptions() > 0), "exception metric");
        check(profile.snapshot().stream().anyMatch(p -> p.slowCalls() > 0), "slow metric");
        check(logs.stream().anyMatch(s -> s.contains("task-timeout")), "timeout warning");
        check(logs.size() < 20, "aggregated warnings");
        runtime.close();
        var next = new ManagedRuntime(new Host(), loader, 702, new Profile(s -> {
        }));
        next.activate();
        check(next.snapshot().generation() != runtime.snapshot().generation(), "new generation");
        next.close();
        var ioHost = new Host();
        var bounded = new ManagedRuntime(ioHost, loader, 703, new Profile(s -> {
        }));
        bounded.activate();
        bounded.configure(new RuntimeLimits(2, 1, 1, 2));
        var firstEntered = new CountDownLatch(1);
        var firstRelease = new CountDownLatch(1);
        var first = bounded.submitIo(t -> {
            firstEntered.countDown();
            firstRelease.await();
        });
        var second = bounded.submitIo(t -> {
        });
        ioHost.tick();
        check(firstEntered.await(2, TimeUnit.SECONDS), "bounded I/O started");
        check(second.snapshot().status() == TaskStatus.QUEUED, "I/O concurrency limit");
        firstRelease.countDown();
        await(first);
        ioHost.tick();
        await(second);
        bounded.close();
        var quota = new ManagedRuntime(new Host(), loader, 704, new Profile(s -> {
        }));
        quota.activate();
        quota.configure(new RuntimeLimits(4096, 1, 1, 1));
        int globalLimit = Integer.getInteger("zernyth.async.globalOutstanding", 4096);
        int allocated = Math.min(globalLimit, 4096);
        for (int i = 0; i < allocated; i++) {
            quota.submitCpu(t -> {
            });
        }
        check(quota.snapshot().outstanding() == allocated, "bounded global admission");
        if (globalLimit <= 4096) {
            var other = new ManagedRuntime(new Host(), loader, 705, new Profile(s -> {
            }));
            other.activate();
            expect(TaskRejectedException.class, () -> other.submitCpu(t -> {
            }));
            other.close();
        }
        quota.cancelAll();
        quota.close();
        check(quota.snapshot().globalOutstanding() == 0, "global permits returned");
        expect(IllegalArgumentException.class, () -> new RuntimeLimits(0, 1, 1, 1));
        expect(IllegalArgumentException.class, () -> new TaskOptions(-1, 0, Duration.ofSeconds(1)));
        expect(IllegalArgumentException.class, () -> new TaskOptions(0, 0, Duration.ZERO));
        var raceHost = new Host();
        var racing = new ManagedRuntime(raceHost, loader, 706, new Profile(s -> {
        }));
        racing.activate();
        boolean terminal = true;
        for (int i = 0; i < 100; i++) {
            var handle = racing.submitIo(token -> {
            });
            var canceller = Thread.ofPlatform().daemon().start(handle::cancel);
            raceHost.tick();
            canceller.join(2000);
            terminal &= await(handle).terminal();
        }
        racing.close();
        check(terminal, "dispatch/cancellation race terminates");
        check(racing.snapshot().globalOutstanding() == 0, "race releases permits");
        var memory = LoaderTracker.snapshot();
        check(memory.heapUsed() >= 0, "heap metric");
        System.out.println(
                "Managed async runtime: " + checks.get() + " Java checks passed (mock Host; no JNI/BDS).");
    }

    private static void delayedQueueRegressions() throws Exception {
        var host = new Host();
        var runtime = new ManagedRuntime(host, ManagedRuntimeTest.class.getClassLoader(), 800,
                new Profile(message -> { }));
        runtime.activate();
        runtime.configure(new RuntimeLimits(4096, 1, 1, 256));
        var delayed = new ArrayList<TaskHandle>();
        var calls = new AtomicInteger();
        try {
            for (int i = 0; i < 4096; i++) {
                delayed.add(runtime.submit(TaskKind.SERVER, token -> calls.incrementAndGet(),
                        new TaskOptions(100_000, 0, Duration.ofSeconds(30))));
            }
            for (int i = 0; i < 1000; i++) {
                host.tick();
            }
            check(calls.get() == 0 && runtime.snapshot().outstanding() == 4096,
                    "delayed-only pulses preserve bounded queue");
            runtime.cancelAll();
            check(delayed.stream().allMatch(task -> task.snapshot().status() == TaskStatus.CANCELLED),
                    "bulk cancellation completes all delayed handles");
            check(runtime.snapshot().globalOutstanding() == 0, "bulk cancellation returns capacity");
            var future = runtime.submit(TaskKind.SERVER, token -> calls.addAndGet(100),
                    new TaskOptions(10, 0, Duration.ofSeconds(30)));
            host.tick();
            var immediate = runtime.returnToServer(calls::incrementAndGet);
            host.tick();
            check(await(immediate).status() == TaskStatus.SUCCEEDED && calls.get() == 1,
                    "new immediate work wakes delayed queue");
            var earlier = runtime.submit(TaskKind.SERVER, token -> calls.addAndGet(10),
                    new TaskOptions(2, 0, Duration.ofSeconds(30)));
            host.tick();
            check(calls.get() == 1, "earlier task respects delay");
            host.tick();
            check(await(earlier).status() == TaskStatus.SUCCEEDED && calls.get() == 11,
                    "new earlier deadline wakes queue");
            for (int i = 0; i < 6; i++) {
                host.tick();
            }
            check(await(future).status() == TaskStatus.SUCCEEDED && calls.get() == 111,
                    "older later deadline survives queue rotation");
            var cancelled = runtime.submit(TaskKind.SERVER, token -> {
                throw new AssertionError("Cancelled earliest task ran");
            }, new TaskOptions(1, 0, Duration.ofSeconds(30)));
            cancelled.cancel();
            host.tick();
            var followup = runtime.returnToServer(calls::incrementAndGet);
            host.tick();
            check(await(followup).status() == TaskStatus.SUCCEEDED,
                    "cancelling earliest deadline cannot suppress later admission");
        } finally {
            runtime.close();
        }
        check(runtime.snapshot().globalOutstanding() == 0, "delayed scenario cleanup");
    }

    private static void boundedWorkerLoad() throws Exception {
        var results = new ArrayList<Map<String, Object>>();
        for (var kind : List.of(TaskKind.CPU, TaskKind.IO)) {
            var host = new Host();
            var runtime = new ManagedRuntime(host, ManagedRuntimeTest.class.getClassLoader(),
                    900 + kind.ordinal(),
                    new Profile(message -> { }));
            runtime.activate();
            runtime.configure(new RuntimeLimits(64, 2, 4, 32));
            var controls = new ArrayList<TaskHandle>();
            var active = new AtomicInteger();
            var maximum = new AtomicInteger();
            var virtual = new AtomicInteger();
            var checksum = new AtomicLong();
            var workers = Executors.newFixedThreadPool(2);
            long started = System.nanoTime();
            try {
                for (int i = 0; i < 64; i++) {
                    controls.add(runtime.submit(kind, token -> {
                        maximum.accumulateAndGet(active.incrementAndGet(), Math::max);
                        try {
                            if (Thread.currentThread().isVirtual()) {
                                virtual.incrementAndGet();
                            }
                            if (kind == TaskKind.IO) {
                                Thread.sleep(3);
                            } else {
                                int sum = 0;
                                for (int n = 0; n < 20_000; n++) {
                                    sum += n;
                                }
                                checksum.addAndGet(sum);
                            }
                            token.throwIfCancellationRequested();
                        } finally {
                            active.decrementAndGet();
                        }
                    }, TaskOptions.immediate()));
                }
                expect(TaskRejectedException.class, () -> runtime.submit(kind, token -> { },
                        TaskOptions.immediate()));
                while (runtime.snapshot().outstanding() > 0) {
                    if (System.nanoTime() - started > TimeUnit.SECONDS.toNanos(10)) {
                        throw new AssertionError("Bounded worker scenario exceeded deadline");
                    }
                    host.tick();
                    for (Runnable work; (work = host.cpu.poll()) != null; ) {
                        workers.submit(work);
                    }
                    Thread.sleep(1);
                }
                check(controls.stream().allMatch(task -> task.snapshot().status() == TaskStatus.SUCCEEDED),
                        "bounded worker load completes every admitted task");
                check(maximum.get() > 0 && maximum.get() <= (kind == TaskKind.IO ? 4 : 2),
                        "worker parallelism remains bounded");
                check(virtual.get() == (kind == TaskKind.IO ? 64 : 0),
                        "I/O uses virtual threads exclusively");
                check(checksum.get() == (kind == TaskKind.CPU ? 12_799_360_000L : 0),
                        "bounded CPU workload checksum");
                var waits = controls.stream()
                        .mapToLong(task -> task.snapshot().queueWaitNanos()).sorted().toArray();
                var wall = controls.stream()
                        .mapToLong(task -> task.snapshot().wallNanos()).sorted().toArray();
                results.add(Map.of("kind", kind.name(), "tasks", 64, "maxConcurrent", maximum.get(),
                        "virtualTasks", virtual.get(), "scenarioWallNanos", System.nanoTime() - started,
                        "p95TaskQueueNanos", waits[60], "p95TaskWallNanos", wall[60],
                        "checksum", checksum.get()));
            } finally {
                runtime.close();
                workers.shutdownNow();
                check(workers.awaitTermination(2, TimeUnit.SECONDS), "mock CPU workers terminate");
            }
            check(runtime.snapshot().globalOutstanding() == 0, "worker load returns all permits");
        }
        Path output = Path.of("build/managed-load.json");
        Files.createDirectories(output.toAbsolutePath().getParent());
        Files.writeString(output, new GsonBuilder().setPrettyPrinting().create().toJson(results));
    }
}
