package dev.zernyth.runtime;

import java.io.IOException;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;
import java.util.logging.Handler;
import java.util.logging.LogRecord;
import java.util.logging.Logger;

import dev.zernyth.api.command.PlayerSender;
import dev.zernyth.api.plugin.JavaPlugin;
import dev.zernyth.api.plugin.PluginDescription;
import dev.zernyth.api.plugin.PluginDiagnostics;

/**
 * JNI lifecycle adapter. Native Endstone owns dependency and activation decisions.
 */
public final class Bootstrap implements AutoCloseable {

    /**
     * Checks the API/runtime provenance before native registration or plugin execution.
     *
     * @return their matching implementation version
     * @throws IllegalStateException if either package has no version or the versions differ
     */
    public static String runtimeVersion() {
        String runtime = Bootstrap.class.getPackage().getImplementationVersion();
        String api = JavaPlugin.class.getPackage().getImplementationVersion();
        if (runtime == null || api == null || !runtime.equals(api)) {
            throw new IllegalStateException("Matching versioned API/runtime JARs are required");
        }
        return runtime;
    }

    private static final Map<String, Bootstrap> LOADED = new ConcurrentHashMap<>();
    private final PluginClassLoader loader;
    private final JavaPlugin plugin;
    private final GameServer server;
    private final PluginFiles files;
    private final Manifest manifest;
    private final long handle;
    private final long constructedAt = System.nanoTime();
    private volatile Thread lifecycleThread = Thread.currentThread();
    private String status = "CREATED", failure = "";
    private long loadNanos, enableNanos, disableNanos;
    private boolean closed;

    private static native void log(long handle, String message);

    /**
     * Reads bounded plugin metadata without loading the plugin main class.
     *
     * @param path the local plugin JAR path
     * @return metadata encoded for the existing native loader
     * @throws IOException if the JAR or manifest is missing, invalid or exceeds its limits
     */
    public static String[] describe(String path) throws IOException {
        return Manifest.read(Path.of(path)).describe();
    }

    /**
     * Creates a generation-owned plugin classloader, instance and injected runtime context.
     * The native manager remains responsible for dependencies and lifecycle ordering.
     *
     * @param path the local plugin JAR
     * @param handle the native owner generation; zero is reserved for isolated Java tests
     * @throws Exception if metadata, class visibility, libraries or plugin construction fail
     */
    public Bootstrap(String path, long handle) throws Exception {
        this.handle = handle;
        Path jar = Path.of(path).toAbsolutePath().normalize();
        manifest = Manifest.read(jar);
        String[] info = handle == 0 ? new String[]{jar.getParent().resolve(manifest.name()).toString(),
                manifest.name(), manifest.version()}
                : Native.ecosystemInfo(handle);
        if (!info[1].equals(manifest.name()) || !info[2].equals(manifest.version())) {
            throw new IOException("Manifest changed after native discovery");
        }
        var imports = new ArrayList<PluginClassLoader>();
        for (String name : manifest.imports()) {
            var provider = LOADED.get(name);
            if (provider == null) {
                if (manifest.soft().contains(name)) {
                    continue;
                }
                throw new IOException("Java import provider unavailable: " + name);
            }
            imports.add(provider.loader);
        }
        var libraries = new ArrayList<Path>();
        var paths = new PluginFiles(Path.of(info[0]), JavaPlugin.class.getClassLoader(), () -> {
        });
        for (String name : manifest.libraries()) {
            libraries.add(paths.resolve("libraries/" + name));
        }
        loader = new PluginClassLoader(jar, libraries, imports, manifest.exports());
        try {
            var main = Class.forName(manifest.main(), false, loader).asSubclass(JavaPlugin.class);
            if (main.getClassLoader() != loader
                    || !Path.of(main.getProtectionDomain().getCodeSource().getLocation().toURI())
                    .toAbsolutePath()
                    .normalize()
                    .equals(jar)) {
                throw new IOException("Main must belong to plugin JAR, not a dependency library");
            }
            plugin = main.getConstructor().newInstance();
            server = new GameServer(handle, plugin);
            Thread thread = Thread.currentThread();
            files = new PluginFiles(Path.of(info[0]), loader, () -> {
                if (handle != 0) {
                    Native.ecosystemInfo(handle);
                } else if (Thread.currentThread() != thread) {
                    throw new IllegalStateException("Storage requires lifecycle thread");
                }
            });
            Logger logger = new Logger(manifest.name(), null) {
            };
            logger.setUseParentHandlers(false);
            logger.addHandler(new Handler() {
                public void publish(LogRecord logRecord) {
                    if (isLoggable(logRecord)) {
                        log(handle, logRecord.getLevel() + ": " + logRecord.getMessage());
                    }
                }

                public void flush() {
                }

                public void close() {
                }
            });
            plugin.initialize(new PluginDescription(manifest.name(), manifest.version(), manifest.main(),
                            manifest.depend(), manifest.soft(), manifest.before()),
                    logger);
            plugin.initializeServer(server);
            plugin.initializeStorage(files, this::diagnostics);
        } catch (Throwable error) {
            loader.close();
            throw error;
        }
    }

    private PluginDiagnostics diagnostics() {
        // JNI reattachment can create a different Java Thread for the same native server thread.
        // Production access is checked by the native owner/thread contract in ecosystemCounts.
        if (handle == 0 && Thread.currentThread() != lifecycleThread) {
            throw new IllegalStateException("Diagnostics require the lifecycle/server thread");
        }
        long[] c = handle != 0 ? Native.ecosystemCounts(handle) : new long[5];
        return new PluginDiagnostics(manifest.name(), status, loadNanos, enableNanos, disableNanos,
                loader.classes.size(), loader.libraries, manifest.depend(), c[0], c[1], c[2], c[3], c[4],
                failure);
    }

    /**
     * Binds this generation and invokes onLoad on the native lifecycle thread.
     *
     * @throws IllegalStateException if this loader is closed or its plugin name is already bound
     */
    public void load() {
        if (closed || LOADED.putIfAbsent(manifest.name(), this) != null) {
            throw new IllegalStateException("Duplicate Java loader binding");
        }
        status = "LOADING";
        try {
            plugin.onLoad();
            status = "LOADED";
        } catch (Throwable error) {
            status = "FAILED_LOAD";
            failure = error.getClass().getName();
            throw error;
        } finally {
            loadNanos = System.nanoTime() - constructedAt;
        }
    }

    private void commands() throws IOException {
        for (var entry : manifest.commands().entrySet()) {
            Map<String, Object> data = new LinkedHashMap<>();
            ((Map<?, ?>) entry.getValue()).forEach((k, v) -> data.put(String.valueOf(k), v));
            for (String key : data.keySet()) {
                if (!Set.of("description", "permission", "aliases", "player-only").contains(key)) {
                    throw new IOException("Unsupported command metadata: " + key);
                }
            }
            String description =
                    data.containsKey("description") ? Manifest.string(data.get("description")) : "";
            String permission = data.containsKey("permission") ? Manifest.string(data.get("permission")) : "";
            if (data.containsKey("player-only") && !(data.get("player-only") instanceof Boolean)) {
                throw new IOException("player-only must be boolean");
            }
            boolean only = Boolean.TRUE.equals(data.get("player-only"));
            var names = new ArrayList<String>();
            names.add(entry.getKey());
            names.addAll(Manifest.list(data, "aliases"));
            for (String name : names) {
                server.registerCommand(name, description, permission, (sender, command, args) -> {
                    if (only && !(sender instanceof PlayerSender)) {
                        sender.sendMessage("This command requires a player.");
                        return true;
                    }
                    return plugin.onCommand(sender, command, args);
                });
            }
        }
    }

    /**
     * Activates this generation's runtime and commands before invoking onEnable.
     * Native code must call this on the server thread after dependency checks.
     *
     * @throws IOException if command metadata is invalid
     * @throws IllegalStateException if this generation is already closed or native owner checks fail
     */
    public void enable() throws IOException {
        lifecycleThread = Thread.currentThread();
        if (closed) {
            throw new IllegalStateException("ClassLoader closed: native generation recreation required");
        }
        status = "ENABLING";
        long start = System.nanoTime();
        try {
            server.activateRuntime();
            commands();
            plugin.onEnable();
            status = "ENABLED";
        } catch (Throwable error) {
            status = "FAILED_ENABLE";
            failure = error.getClass().getName();
            throw error;
        } finally {
            enableNanos = System.nanoTime() - start;
        }
    }

    /**
     * Requests task shutdown, invokes onDisable and closes this generation even when a hook fails.
     * Runs on the server thread without waiting for non-cooperative workers.
     *
     * @throws IOException if classloader/storage cleanup fails
     */
    public void disable() throws IOException {
        status = "DISABLING";
        long start = System.nanoTime();
        try {
            server.stopRuntime();
            plugin.onDisable();
            status = "DISABLED";
        } catch (Throwable error) {
            status = "FAILED_DISABLE";
            failure = error.getClass().getName();
            throw error;
        } finally {
            disableNanos = System.nanoTime() - start;
            server.reset();
            close();
        }
    }

    /**
     * Closes this generation once, removing its loader binding and releasing owned Java resources.
     * The native manager separately revokes native registrations and handles.
     *
     * @throws IOException if storage/classloader cleanup fails
     */
    @Override
    public void close() throws IOException {
        if (closed) {
            return;
        }
        closed = true;
        LOADED.remove(manifest.name(), this);
        try {
            server.stopRuntime();
        } finally {
            files.close();
            loader.close();
        }
    }
}
