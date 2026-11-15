package dev.zernyth.runtime;

import java.lang.ref.WeakReference;
import java.util.ArrayDeque;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.concurrent.CancellationException;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.CompletionStage;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicLong;

import dev.zernyth.api.scheduler.AsyncRuntime;
import dev.zernyth.api.scheduler.AsyncSnapshot;
import dev.zernyth.api.scheduler.RuntimeLimits;
import dev.zernyth.api.scheduler.Task;
import dev.zernyth.api.scheduler.TaskAction;
import dev.zernyth.api.scheduler.TaskHandle;
import dev.zernyth.api.scheduler.TaskKind;
import dev.zernyth.api.scheduler.TaskOptions;
import dev.zernyth.api.scheduler.TaskRejectedException;
import dev.zernyth.api.scheduler.TaskSnapshot;
import dev.zernyth.api.scheduler.TaskStatus;

/**
 * Admission/ownership layer. Tick timing and CPU execution are delegated to Endstone.
 */
final class ManagedRuntime implements AsyncRuntime, AutoCloseable {

    interface Host {

        void checkMain();

        Task pulse(Runnable callback);

        Task cpu(Runnable callback);
    }

    private static final int GLOBAL_LIMIT = positiveProperty("zernyth.async.globalOutstanding", 4096, 65536);
    private static final int CPU_LIMIT = positiveProperty("zernyth.async.globalCpu",
            Math.max(1, Math.min(4, Runtime.getRuntime().availableProcessors() - 1)), 64);
    private static final int IO_LIMIT = positiveProperty("zernyth.async.globalIo", 32, 1024);
    private static final AtomicInteger OUTSTANDING = new AtomicInteger(), CPU = new AtomicInteger(),
            IO = new AtomicInteger();
    private static final AtomicLong IDS = new AtomicLong();

    private static int positiveProperty(String name, int fallback, int max) {
        int configuredLimit = Integer.getInteger(name, fallback);
        if (configuredLimit < 1 || configuredLimit > max) {
            throw new IllegalArgumentException("Invalid " + name);
        }
        return configuredLimit;
    }

    private static boolean reserve(AtomicInteger counter, int max) {
        int old;
        do {
            old = counter.get();
            if (old >= max) {
                return false;
            }
        } while (!counter.compareAndSet(old, old + 1));
        return true;
    }

    private final Object gate = new Object();
    private final Host host;
    private final Profile profile;
    private final long generation;
    private ClassLoader loader;
    private RuntimeLimits limits = RuntimeLimits.defaults();
    private final Map<Long, Control> tasks = new LinkedHashMap<>();
    private final ArrayDeque<Control> queue = new ArrayDeque<>();
    private final ConcurrentLinkedQueue<Task> nativeCancels = new ConcurrentLinkedQueue<>();
    private boolean active, closed;
    private Task pump;
    private long tick, accepted, rejected, timeouts;
    // Conservative lower bound. Cancellation may leave it early, never later than queued work.
    private long nextDueTick = Long.MAX_VALUE;
    private int runningCpu, runningIo;

    ManagedRuntime(Host host, ClassLoader loader, long generation, Profile profile) {
        this.host = host;
        this.loader = loader;
        this.generation = generation;
        this.profile = profile;
    }

    void activate() {
        synchronized (gate) {
            if (closed) {
                throw new IllegalStateException("Runtime closed");
            }
            active = true;
        }
    }

    void ensureStarted() {
        host.checkMain();
        synchronized (gate) {
            if (!active || closed) {
                rejected++;
                throw new TaskRejectedException("Plugin generation is not active");
            }
            if (pump == null) {
                pump = host.pulse(this::pulse);
            }
        }
    }

    public TaskHandle submit(TaskKind kind, TaskAction action, TaskOptions options) {
        host.checkMain();
        ensureStarted();
        return accept(Objects.requireNonNull(kind), Objects.requireNonNull(action),
                Objects.requireNonNull(options));
    }

    public TaskHandle returnToServer(Runnable action) {
        Objects.requireNonNull(action);
        synchronized (gate) {
            if (pump == null) {
                rejected++;
                throw new TaskRejectedException("Initialize AsyncRuntime on the server thread first");
            }
        }
        return accept(TaskKind.SERVER, token -> action.run(), TaskOptions.immediate());
    }

    private Control accept(TaskKind kind, TaskAction action, TaskOptions options) {
        synchronized (gate) {
            if (closed || !active || tasks.size() >= limits.maxOutstanding()
                    || !reserve(OUTSTANDING, GLOBAL_LIMIT)) {
                rejected++;
                throw new TaskRejectedException("Async runtime closed or capacity exhausted");
            }
            long due;
            try {
                due = Math.addExact(tick, options.delayTicks());
            } catch (ArithmeticException error) {
                OUTSTANDING.decrementAndGet();
                throw new IllegalArgumentException("Tick overflow");
            }
            var task = new Control(this, IDS.incrementAndGet(), generation, kind, action, options, due);
            tasks.put(task.id, task);
            enqueue(task);
            accepted++;
            return task;
        }
    }

    public RuntimeLimits limits() {
        synchronized (gate) {
            return limits;
        }
    }

    public void configure(RuntimeLimits limits) {
        host.checkMain();
        Objects.requireNonNull(limits);
        synchronized (gate) {
            if (closed || !tasks.isEmpty()) {
                throw new IllegalStateException("Configure limits only with no outstanding tasks");
            }
            this.limits = limits;
        }
    }

    public AsyncSnapshot snapshot() {
        synchronized (gate) {
            int running = 0;
            for (var control : tasks.values()) {
                if (control.status == TaskStatus.RUNNING || control.status == TaskStatus.CANCEL_REQUESTED) {
                    running++;
                }
            }
            return new AsyncSnapshot(generation, closed, tasks.size(), tasks.size() - running, running,
                    accepted, rejected, timeouts, OUTSTANDING.get(), CPU.get(), IO.get());
        }
    }

    private boolean permit(Control control) {
        if (control.kind == TaskKind.CPU) {
            if (runningCpu >= limits.cpuParallelism() || !reserve(CPU, CPU_LIMIT)) {
                return false;
            }
            runningCpu++;
            control.permit = true;
        } else if (control.kind == TaskKind.IO) {
            if (runningIo >= limits.ioParallelism() || !reserve(IO, IO_LIMIT)) {
                return false;
            }
            runningIo++;
            control.permit = true;
        }
        return true;
    }

    private void releasePermit(Control control) {
        if (!control.permit) {
            return;
        }
        control.permit = false;
        if (control.kind == TaskKind.CPU) {
            runningCpu--;
            CPU.decrementAndGet();
        } else {
            runningIo--;
            IO.decrementAndGet();
        }
    }

    /** Adds work while holding gate and keeps delayed-only pulses constant-time. */
    private void enqueue(Control control) {
        queue.add(control);
        if (control.due < nextDueTick) {
            nextDueTick = control.due;
        }
    }

    void pulse() {
        host.checkMain();
        for (Task task; (task = nativeCancels.poll()) != null; ) {
            try {
                task.cancel();
            } catch (IllegalStateException ignored) {
            }
        }
        int warnings = 0;
        int remaining;
        synchronized (gate) {
            if (closed) {
                return;
            }
            tick++;
            remaining = nextDueTick <= tick ? queue.size() : 0;
            if (remaining > 0) {
                nextDueTick = Long.MAX_VALUE;
            }
            // Server callbacks finish on this thread and check their duration on exit.
            // Only asynchronous work can still be running when a normal tick pulse begins.
            if (runningCpu > 0 || runningIo > 0) {
                long now = System.nanoTime();
                for (var control : tasks.values()) {
                    if ((control.status == TaskStatus.RUNNING
                            || control.status == TaskStatus.CANCEL_REQUESTED)
                            && !control.timedOut
                            && now - control.started > control.options.warnAfter().toNanos()) {
                        control.timedOut = true;
                        timeouts++;
                        warnings++;
                    }
                }
            }
        }
        for (int i = 0; i < warnings; i++) {
            profile.warning("task-timeout", "running task");
        }
        if (remaining == 0) {
            return;
        }
        long mainStart = System.nanoTime();
        int mainCallbacks = 0;
        while (remaining-- > 0) {
            Control control;
            synchronized (gate) {
                control = queue.poll();
                if (control == null || closed) {
                    break;
                }
                if (control.terminal()) {
                    continue;
                }
                if (control.due > tick
                        || (control.kind == TaskKind.SERVER
                        && (mainCallbacks >= limits.maxServerCallbacksPerTick()
                        || System.nanoTime() - mainStart > 2_000_000))
                        || !permit(control)) {
                    enqueue(control);
                    continue;
                }
                control.status = TaskStatus.DISPATCHED;
            }
            if (control.kind == TaskKind.SERVER) {
                mainCallbacks++;
                run(control);
            } else if (control.kind == TaskKind.CPU) {
                try {
                    Task nativeTask = host.cpu(() -> run(control));
                    synchronized (gate) {
                        if (!control.terminal()) {
                            control.nativeTask = nativeTask;
                        } else {
                            nativeCancels.add(nativeTask);
                        }
                    }
                } catch (Throwable error) {
                    failDispatch(control, error);
                }
            } else {
                try {
                    var thread = Thread.ofVirtual()
                            .name("zernyth-io-" + generation + "-" + control.id)
                            .inheritInheritableThreadLocals(false)
                            .unstarted(() -> run(control));
                    synchronized (gate) {
                        // Cancellation may have completed while the virtual thread was being constructed.
                        // Never reattach a thread reference to a terminal handle.
                        if (control.terminal()) {
                            continue;
                        }
                        control.thread = thread;
                    }
                    thread.start();
                } catch (Throwable error) {
                    failDispatch(control, error);
                }
            }
        }
    }

    private void failDispatch(Control control, Throwable error) {
        TaskSnapshot done;
        synchronized (gate) {
            if (control.terminal()) {
                return;
            }
            control.failure = error.getClass().getName();
            done = finish(control, TaskStatus.FAILED);
        }
        profile.warning("task-dispatch", error.getClass().getName());
        control.completion.complete(done);
    }

    private TaskSnapshot finish(Control control, TaskStatus status) {
        releasePermit(control);
        control.status = status;
        control.action = null;
        control.nativeTask = null;
        control.thread = null;
        if (tasks.remove(control.id) != null) {
            OUTSTANDING.decrementAndGet();
        }
        return control.snapshot();
    }

    private void run(Control control) {
        ClassLoader context = null;
        TaskAction action = null;
        TaskSnapshot skipped = null;
        synchronized (gate) {
            if (closed || control.terminal() || control.cancel.get()) {
                if (!control.terminal()) {
                    skipped = finish(control, TaskStatus.CANCELLED);
                }
            } else {
                control.status = TaskStatus.RUNNING;
                control.started = System.nanoTime();
                control.queueWait += control.started - control.queuedAt;
                control.thread = Thread.currentThread();
                context = loader;
                action = control.action;
            }
        }
        if (skipped != null) {
            control.completion.complete(skipped);
        }
        if (action == null) {
            return;
        }
        final TaskAction work = action;
        Thread current = Thread.currentThread();
        ClassLoader previous = current.getContextClassLoader();
        Throwable failure = null;
        try {
            current.setContextClassLoader(context);
            profile.measure("task", control.kind.name(), () -> {
                work.run(control);
                return null;
            });
        } catch (Throwable error) {
            failure = error;
        } finally {
            current.setContextClassLoader(previous);
            TaskSnapshot done = null;
            boolean timeoutWarning = false;
            synchronized (gate) {
                long elapsed = System.nanoTime() - control.started;
                control.wall += elapsed;
                control.runs++;
                control.thread = null;
                control.nativeTask = null;
                if (!control.timedOut && elapsed > control.options.warnAfter().toNanos()) {
                    control.timedOut = true;
                    timeouts++;
                    timeoutWarning = true;
                }
                if (failure != null) {
                    control.failure = failure.getClass().getName();
                }
                releasePermit(control);
                if (control.cancel.get() || closed || failure instanceof CancellationException
                        || failure instanceof InterruptedException) {
                    done = finish(control, TaskStatus.CANCELLED);
                } else if (failure != null) {
                    done = finish(control, TaskStatus.FAILED);
                } else if (control.options.periodTicks() > 0) {
                    control.status = TaskStatus.QUEUED;
                    control.queuedAt = System.nanoTime();
                    control.due = tick > Long.MAX_VALUE - control.options.periodTicks() ? Long.MAX_VALUE
                            : tick + control.options.periodTicks();
                    enqueue(control);
                } else {
                    done = finish(control, TaskStatus.SUCCEEDED);
                }
            }
            if (timeoutWarning) {
                profile.warning("task-timeout", control.kind.name());
            }
            if (failure != null && !(failure instanceof CancellationException)
                    && !(failure instanceof InterruptedException)) {
                profile.warning("task-failure", failure.getClass().getName());
            }
            if (done != null) {
                control.completion.complete(done);
            }
        }
    }

    private void cancel(Control control) {
        TaskSnapshot done = null;
        Thread interrupt = null;
        synchronized (gate) {
            if (control.terminal()) {
                return;
            }
            control.cancel.set(true);
            if (control.status == TaskStatus.RUNNING || control.status == TaskStatus.CANCEL_REQUESTED) {
                control.status = TaskStatus.CANCEL_REQUESTED;
                if (control.kind == TaskKind.IO) {
                    interrupt = control.thread;
                }
            } else {
                queue.remove(control);
                if (control.nativeTask != null) {
                    nativeCancels.add(control.nativeTask);
                }
                done = finish(control, TaskStatus.CANCELLED);
            }
        }
        if (interrupt != null) {
            interrupt.interrupt();
        }
        if (done != null) {
            control.completion.complete(done);
        }
    }

    public void cancelAll() {
        host.checkMain();
        List<Control> copy;
        synchronized (gate) {
            copy = List.copyOf(tasks.values());
            // The captured controls are cancelled below. Avoid repeatedly searching a rotated queue.
            queue.clear();
            nextDueTick = Long.MAX_VALUE;
        }
        copy.forEach(this::cancel);
    }

    void cancelNativeTasks() {
        cancelAll();
        Task timer;
        synchronized (gate) {
            timer = pump;
            pump = null;
        }
        if (timer != null) {
            timer.cancel();
        }
    }

    public void close() {
        synchronized (gate) {
            if (closed) {
                return;
            }
            if (pump == null && tasks.isEmpty()) {
                closed = true;
                active = false;
                loader = null;
                return;
            }
        }
        host.checkMain();
        List<Control> copy;
        Task timer;
        synchronized (gate) {
            if (closed) {
                return;
            }
            closed = true;
            active = false;
            copy = List.copyOf(tasks.values());
            queue.clear();
            nextDueTick = Long.MAX_VALUE;
            timer = pump;
            pump = null;
            loader = null;
        }
        copy.forEach(this::cancel);
        if (timer != null) {
            timer.cancel();
        }
        for (Task task; (task = nativeCancels.poll()) != null; ) {
            try {
                task.cancel();
            } catch (IllegalStateException ignored) {
            }
        }
    }

    static final class Control implements TaskHandle {

        final WeakReference<ManagedRuntime> owner;
        final long id, generation;
        final TaskKind kind;
        final TaskOptions options;
        final AtomicBoolean cancel = new AtomicBoolean();
        final CompletableFuture<TaskSnapshot> completion = new CompletableFuture<>();
        volatile TaskStatus status = TaskStatus.QUEUED;
        volatile long runs, wall, queueWait, started;
        volatile boolean timedOut;
        volatile String failure = "";
        TaskAction action;
        Task nativeTask;
        Thread thread;
        long due, queuedAt = System.nanoTime();
        boolean permit;

        Control(ManagedRuntime owner, long id, long generation, TaskKind kind, TaskAction action,
                TaskOptions options, long due) {
            this.owner = new WeakReference<>(owner);
            this.id = id;
            this.generation = generation;
            this.kind = kind;
            this.action = action;
            this.options = options;
            this.due = due;
        }

        boolean terminal() {
            return status == TaskStatus.SUCCEEDED || status == TaskStatus.FAILED
                    || status == TaskStatus.CANCELLED;
        }

        public boolean isCancellationRequested() {
            return cancel.get();
        }

        public void cancel() {
            var runtime = owner.get();
            if (runtime != null) {
                runtime.cancel(this);
            } else if (!terminal()) {
                cancel.set(true);
            }
        }

        public TaskSnapshot snapshot() {
            return new TaskSnapshot(
                    id, generation, kind, status, runs, queueWait, wall, cancel.get(), timedOut, failure);
        }

        public CompletionStage<TaskSnapshot> completion() {
            return completion.minimalCompletionStage();
        }
    }
}
