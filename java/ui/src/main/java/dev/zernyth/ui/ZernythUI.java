package dev.zernyth.ui;

import java.io.IOException;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.Optional;
import java.util.UUID;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicLong;
import java.util.function.Consumer;
import java.util.function.Function;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import dev.zernyth.api.form.ActionForm;
import dev.zernyth.api.form.Form;
import dev.zernyth.api.form.FormRequest;
import dev.zernyth.api.form.FormResponse;
import dev.zernyth.api.form.MessageForm;
import dev.zernyth.api.form.ModalForm;
import dev.zernyth.api.player.Player;
import dev.zernyth.api.plugin.JavaPlugin;
import dev.zernyth.ui.UiDocument.Action;
import dev.zernyth.ui.UiDocument.Component;
import dev.zernyth.ui.UiDocument.Screen;

/**
 * Runtime for Zernyth UI documents: shows screens as Bedrock server forms, keeps one session per player and dispatches
 * declared actions to plugin handlers.
 *
 * <p>Screens map to the forms the Bedrock client supports: {@code action_form} becomes an {@link ActionForm} (buttons with
 * resource-pack textures), {@code modal_form} a {@link ModalForm} (toggles, sliders, dropdowns, text fields) and
 * {@code message_form} a {@link MessageForm} (two buttons). {@code hud_preview} screens are design-only and cannot be opened.
 * Positions, colors and fonts from UI Studio are not part of server forms; the client lays forms out itself.
 *
 * <p>Security: a response only counts when it answers the player's current session (same player, same generation), so
 * stale or replayed responses are ignored. Only actions declared in the document and registered with
 * {@link #onAction(String, Consumer)} are dispatched, and their permissions are checked on the server.
 *
 * <p>Threading: form responses complete on the server thread, so handlers may use the game API directly. Call
 * {@link #close()} in {@code onDisable}; it cancels open forms and drops all sessions.
 */
public final class ZernythUI implements AutoCloseable {
    private static final Pattern PLACEHOLDER = Pattern.compile("\\{([a-z][a-z0-9_.]{0,63})}");
    private static final int MAX_BINDINGS = 256;

    private final Logger logger;
    private final Map<String, Screen> screens = new ConcurrentHashMap<>();
    private final Map<String, UiDocument.ActionDefinition> declared = new ConcurrentHashMap<>();
    private final Map<String, Consumer<UiActionEvent>> handlers = new ConcurrentHashMap<>();
    private final Map<String, Function<Player, String>> bindings = new ConcurrentHashMap<>();
    private final Map<String, Map<String, String>> translations = new ConcurrentHashMap<>();
    private volatile Function<Player, String> locale = player -> null;
    private final Map<UUID, Session> sessions = new ConcurrentHashMap<>();
    private final AtomicLong generations = new AtomicLong();
    private final AtomicLong dispatched = new AtomicLong();
    private final AtomicLong rejected = new AtomicLong();
    private volatile boolean closed;
    private volatile String startScreen;

    private record Session(long generation, String screen, List<Component> shown, FormRequest request) {
    }

    /**
     * Counters for diagnostics.
     *
     * @param screens registered screens
     * @param openSessions players with an open UI
     * @param dispatched actions delivered to handlers
     * @param rejected responses ignored (stale, foreign, unknown or unauthorized)
     */
    public record Stats(int screens, int openSessions, long dispatched, long rejected) {
    }

    ZernythUI(Logger logger) {
        this.logger = Objects.requireNonNull(logger);
    }

    /**
     * Creates a UI runtime for a plugin.
     *
     * @param plugin the owning plugin
     * @return a new runtime; close it in {@code onDisable}
     */
    public static ZernythUI create(JavaPlugin plugin) {
        return new ZernythUI(plugin.getLogger());
    }

    /**
     * Loads every screen and action of a UI Studio export ({@code zernyth-ui.json}).
     *
     * @param document a validated document
     * @return this runtime
     */
    public ZernythUI load(UiDocument document) {
        ensureOpen();
        screens.putAll(document.screens());
        declared.putAll(document.actions());
        document.translations().forEach((lang, texts) -> translations.merge(lang, texts, (a, b) -> {
            var merged = new java.util.HashMap<>(a);
            merged.putAll(b);
            return Map.copyOf(merged);
        }));
        document.startScreen().ifPresent(s -> startScreen = s);
        return this;
    }

    /**
     * Replaces everything loaded from documents and code with this document, for example after a new publication.
     * Handlers and bindings stay; open forms of removed screens are ignored when answered.
     *
     * @param document a validated document
     * @return this runtime
     */
    public ZernythUI replace(UiDocument document) {
        ensureOpen();
        screens.clear();
        declared.clear();
        translations.clear();
        startScreen = null;
        return load(document);
    }

    /**
     * Reads, validates and loads a UI Studio export.
     *
     * @param file path of {@code zernyth-ui.json}, usually in the plugin's data folder
     * @return this runtime
     * @throws IOException when the file cannot be read
     * @throws UiValidationException when it is not a valid document
     */
    public ZernythUI load(Path file) throws IOException {
        return load(UiDocument.read(file));
    }

    /**
     * Starts a screen defined in code (an action form).
     *
     * @param id screen identifier
     * @return a builder; call {@link ScreenBuilder#register()}
     */
    public ScreenBuilder screen(String id) {
        ensureOpen();
        return new ScreenBuilder(this, id);
    }

    void register(Screen screen, List<String> actions) {
        ensureOpen();
        screens.put(screen.id(), screen);
        for (var name : actions) {
            declared.putIfAbsent(name, new UiDocument.ActionDefinition(name, Optional.empty()));
        }
    }

    /**
     * Registers the handler for a declared action. Actions without a handler are ignored with a warning.
     *
     * @param action action name, for example {@code shop.open}
     * @param handler runs on the server thread
     * @return this runtime
     */
    public ZernythUI onAction(String action, Consumer<UiActionEvent> handler) {
        ensureOpen();
        handlers.put(Objects.requireNonNull(action), Objects.requireNonNull(handler));
        return this;
    }

    /**
     * Supplies a value for the placeholder {@code {key}} in titles, texts and button labels.
     * {@code {player.name}} is built in. Values are read when a screen opens; reopen it to show new values.
     *
     * @param key placeholder name, for example {@code coins}
     * @param provider value for a player; must be quick and must not block
     * @return this runtime
     */
    public ZernythUI bind(String key, Function<Player, String> provider) {
        ensureOpen();
        if (!PLACEHOLDER.matcher("{" + key + "}").matches()) {
            throw new IllegalArgumentException("Invalid placeholder name: " + key);
        }
        if (!bindings.containsKey(key) && bindings.size() >= MAX_BINDINGS) {
            throw new IllegalStateException("Too many placeholders");
        }
        bindings.put(key, Objects.requireNonNull(provider));
        return this;
    }

    /**
     * Sets how to find a player's language for translated texts (schema 1.1). The engine API has no client locale, so
     * the plugin decides, for example from its own settings. Without it, or for unknown languages, the document texts are used.
     *
     * @param provider returns a locale such as {@code de_DE} or {@code de}, or null
     * @return this runtime
     */
    public ZernythUI locale(Function<Player, String> provider) {
        locale = Objects.requireNonNull(provider);
        return this;
    }

    /**
     * Opens the document's start screen.
     *
     * @param player the player
     * @return whether a form was shown
     */
    public boolean open(Player player) {
        String start = startScreen;
        return start != null && open(player, start);
    }

    /**
     * Opens a screen for a player, replacing their current UI session.
     *
     * @param player the player
     * @param screenId screen identifier
     * @return whether a form was shown (false when the screen is unknown, design-only, empty or not permitted)
     */
    public boolean open(Player player, String screenId) {
        if (closed || !player.isOnline()) {
            return false;
        }
        Screen screen = screens.get(screenId);
        if (screen == null || screen.kind().equals("hud_preview")) {
            logger.warning(() -> "UI screen \"" + screenId + "\" cannot be shown as a form");
            return false;
        }
        if (screen.permission().isPresent() && !player.hasPermission(screen.permission().get())) {
            return false;
        }
        var shown = new ArrayList<Component>();
        Form form = build(player, screen, shown);
        if (form == null) {
            return false;
        }
        long generation = generations.incrementAndGet();
        var previous = sessions.remove(player.getUniqueId());
        if (previous != null) {
            previous.request().cancel();
        }
        FormRequest request = player.showForm(form);
        sessions.put(player.getUniqueId(), new Session(generation, screen.id(), List.copyOf(shown), request));
        request.response().whenComplete((response, error) -> {
            if (error != null) {
                end(player.getUniqueId(), generation);
                logger.log(Level.WARNING, "UI form failed", error);
                return;
            }
            respond(player, generation, response);
        });
        return true;
    }

    /**
     * Closes the player's UI session; an open form is cancelled.
     *
     * @param player the player
     */
    public void close(Player player) {
        var session = sessions.remove(player.getUniqueId());
        if (session != null) {
            session.request().cancel();
        }
    }

    /** Cancels all open forms and releases every session and handler. Further use throws. */
    @Override
    public void close() {
        closed = true;
        for (var session : sessions.values()) {
            session.request().cancel();
        }
        sessions.clear();
        handlers.clear();
        bindings.clear();
        translations.clear();
    }

    /**
     * Current counters.
     *
     * @return statistics
     */
    public Stats stats() {
        return new Stats(screens.size(), sessions.size(), dispatched.get(), rejected.get());
    }

    private void ensureOpen() {
        if (closed) {
            throw new IllegalStateException("UI runtime is closed");
        }
    }

    private boolean visible(Player player, Component c) {
        return (c.visibleIf().isEmpty() || player.hasPermission(c.visibleIf().get())) && c.showIf().map(cond -> matches(player, cond)).orElse(true);
    }

    private String bound(Player player, String key) {
        if (key.equals("player.name")) {
            return player.getName();
        }
        var provider = bindings.get(key);
        if (provider == null) {
            return null;
        }
        try {
            return String.valueOf(provider.apply(player));
        } catch (RuntimeException failure) {
            logger.log(Level.WARNING, "UI binding " + key + " failed", failure);
            return null;
        }
    }

    // Unbound values never match, so a missing binding hides the component instead of showing it to everyone.
    boolean matches(Player player, UiDocument.Condition cond) {
        String actual = bound(player, cond.key());
        if (actual == null) {
            return false;
        }
        int cmp;
        if (cond.numeric()) {
            double a;
            try {
                a = Double.parseDouble(actual.trim());
            } catch (NumberFormatException notNumber) {
                return false;
            }
            cmp = Double.compare(a, Double.parseDouble(cond.value()));
        } else {
            if (!cond.op().equals("eq") && !cond.op().equals("ne")) {
                return false;
            }
            cmp = actual.equals(cond.value()) ? 0 : 1;
        }
        return switch (cond.op()) {
            case "eq" -> cmp == 0;
            case "ne" -> cmp != 0;
            case "gt" -> cmp > 0;
            case "gte" -> cmp >= 0;
            case "lt" -> cmp < 0;
            default -> cmp <= 0;
        };
    }

    // Exact locale first, then the language part ("de" for "de_DE", or any "de_XX" for "de").
    String text(Player player, String key, String fallback) {
        String wanted;
        try {
            wanted = locale.apply(player);
        } catch (RuntimeException failure) {
            logger.log(Level.WARNING, "UI locale provider failed", failure);
            wanted = null;
        }
        if (wanted == null || translations.isEmpty()) {
            return fallback;
        }
        var exact = translations.get(wanted);
        if (exact != null && exact.containsKey(key)) {
            return exact.get(key);
        }
        String language = wanted.length() >= 2 ? wanted.substring(0, 2) : wanted;
        for (var entry : translations.entrySet()) {
            if ((entry.getKey().equals(language) || entry.getKey().startsWith(language + "_")) && entry.getValue().containsKey(key)) {
                return entry.getValue().get(key);
            }
        }
        return fallback;
    }

    String resolve(Player player, String text) {
        if (text.indexOf('{') < 0) {
            return text;
        }
        Matcher m = PLACEHOLDER.matcher(text);
        var out = new StringBuilder();
        while (m.find()) {
            String key = m.group(1);
            String value;
            if (key.equals("player.name")) {
                value = player.getName();
            } else {
                var provider = bindings.get(key);
                try {
                    value = provider == null ? m.group() : String.valueOf(provider.apply(player));
                } catch (RuntimeException failure) {
                    logger.log(Level.WARNING, "UI placeholder {" + key + "} failed", failure);
                    value = "";
                }
            }
            m.appendReplacement(out, Matcher.quoteReplacement(value));
        }
        m.appendTail(out);
        return out.toString();
    }

    private static boolean isButton(Component c) {
        return switch (c.type()) {
            case "button", "image_button", "shop_card", "item_display", "selection_menu", "confirm" -> true;
            default -> c.action().isPresent();
        };
    }

    private static boolean isBodyText(Component c) {
        return switch (c.type()) {
            case "text", "rich_text", "player_stats", "team_panel" -> true;
            default -> false;
        };
    }

    private static boolean isControl(Component c) {
        return switch (c.type()) {
            case "toggle", "slider", "dropdown", "text_field" -> true;
            default -> false;
        };
    }

    private String label(Player player, Screen screen, Component c) {
        return text(player, screen.id() + "." + c.id() + ".text", c.text());
    }

    private Form build(Player player, Screen screen, List<Component> shown) {
        String title = resolve(player, text(player, screen.id() + ".title", screen.title()));
        String content = text(player, screen.id() + ".content", screen.content());
        var visible = screen.components().stream().filter(c -> visible(player, c)).toList();
        switch (screen.kind()) {
            case "action_form" -> {
                var buttons = new ArrayList<ActionForm.Button>();
                var body = new StringBuilder(resolve(player, content));
                for (var c : visible) {
                    if (isButton(c) && buttons.size() < 256) {
                        buttons.add(new ActionForm.Button(resolve(player, label(player, screen, c)), c.texture().orElse(null)));
                        shown.add(c);
                    } else if (isBodyText(c) && !c.text().isEmpty()) {
                        body.append(body.isEmpty() ? "" : "\n").append(resolve(player, label(player, screen, c)));
                    }
                }
                return buttons.isEmpty() ? null : new ActionForm(title, body.toString(), buttons);
            }
            case "message_form" -> {
                var buttons = visible.stream().filter(ZernythUI::isButton).limit(2).toList();
                if (buttons.size() < 2) {
                    return null;
                }
                shown.addAll(buttons);
                return new MessageForm(title, resolve(player, content), resolve(player, label(player, screen, buttons.get(0))),
                        resolve(player, label(player, screen, buttons.get(1))));
            }
            case "modal_form" -> {
                var controls = new ArrayList<ModalForm.Control>();
                for (var c : visible) {
                    if (!isControl(c) || controls.size() >= 256) {
                        continue;
                    }
                    String label = resolve(player, label(player, screen, c));
                    controls.add(switch (c.type()) {
                        case "toggle" -> new ModalForm.Toggle(label, c.value() != 0);
                        case "slider" -> {
                            float max = (float) Math.max(c.max(), 1);
                            yield new ModalForm.Slider(label, 0, max, 1, (float) Math.min(Math.max(c.value(), 0), max));
                        }
                        case "dropdown" -> new ModalForm.Dropdown(label, c.options().isEmpty() ? List.of("-") : c.options(),
                                (int) Math.min(Math.max(c.value(), 0), Math.max(c.options().size() - 1, 0)));
                        default -> new ModalForm.TextField(label, c.placeholder(), "");
                    });
                    shown.add(c);
                }
                return controls.isEmpty() ? null : new ModalForm(title, controls);
            }
            default -> {
                return null;
            }
        }
    }

    private void end(UUID player, long generation) {
        sessions.computeIfPresent(player, (id, s) -> s.generation() == generation ? null : s);
    }

    void respond(Player player, long generation, FormResponse response) {
        Session session = sessions.get(player.getUniqueId());
        // Only the newest form of this player counts; older, cancelled or foreign responses are ignored.
        if (closed || session == null || session.generation() != generation) {
            if (response.status() == FormResponse.Status.SUBMITTED) {
                rejected.incrementAndGet();
            }
            return;
        }
        end(player.getUniqueId(), generation);
        if (response.status() != FormResponse.Status.SUBMITTED) {
            return;
        }
        Screen screen = screens.get(session.screen());
        if (screen == null) {
            rejected.incrementAndGet();
            return;
        }
        if (screen.kind().equals("modal_form")) {
            if (response.values().size() != session.shown().size()) {
                rejected.incrementAndGet();
                return;
            }
            var values = new LinkedHashMap<String, Object>();
            for (int i = 0; i < session.shown().size(); i++) {
                values.put(session.shown().get(i).id(), response.values().get(i));
            }
            screen.submit().ifPresent(a -> perform(player, screen, "", a, values));
            return;
        }
        if (response.values().size() != 1 || !(response.values().getFirst() instanceof Number index)
                || index.intValue() < 0 || index.intValue() >= session.shown().size()) {
            rejected.incrementAndGet();
            return;
        }
        Component pressed = session.shown().get(index.intValue());
        if (!pressed.enabled()) {
            return;
        }
        pressed.action().ifPresent(a -> perform(player, screen, pressed.id(), a, Map.of()));
    }

    private void perform(Player player, Screen screen, String component, Action action, Map<String, Object> values) {
        switch (action.type()) {
            case "navigate" -> action.target().ifPresent(target -> open(player, target));
            case "server_event" -> {
                String name = action.name().orElse("");
                var definition = declared.get(name);
                var handler = handlers.get(name);
                if (definition == null || handler == null) {
                    rejected.incrementAndGet();
                    logger.warning(() -> "UI action \"" + name + "\" has no handler");
                    return;
                }
                if (definition.permission().isPresent() && !player.hasPermission(definition.permission().get())) {
                    rejected.incrementAndGet();
                    return;
                }
                dispatched.incrementAndGet();
                try {
                    handler.accept(new UiActionEvent(player, screen.id(), component, name, new HashMap<>(values)));
                } catch (RuntimeException failure) {
                    logger.log(Level.SEVERE, "UI action \"" + name + "\" failed", failure);
                }
            }
            default -> {
            }
        }
    }

    /** Builds an action form in code. */
    public static final class ScreenBuilder {
        private final ZernythUI ui;
        private final String id;
        private String title;
        private String content = "";
        private String permission;
        private final List<Component> components = new ArrayList<>();
        private final List<String> actions = new ArrayList<>();

        private ScreenBuilder(ZernythUI ui, String id) {
            if (!id.matches("^[a-z][a-z0-9_]{0,47}$")) {
                throw new IllegalArgumentException("Invalid screen id: " + id);
            }
            this.ui = ui;
            this.id = id;
            this.title = id;
        }

        /**
         * Sets the title.
         *
         * @param title form title; placeholders allowed
         * @return this builder
         */
        public ScreenBuilder title(String title) {
            this.title = Objects.requireNonNull(title);
            return this;
        }

        /**
         * Sets the body text.
         *
         * @param content text above the buttons; placeholders allowed
         * @return this builder
         */
        public ScreenBuilder content(String content) {
            this.content = Objects.requireNonNull(content);
            return this;
        }

        /**
         * Requires a permission to open the screen.
         *
         * @param permission the permission
         * @return this builder
         */
        public ScreenBuilder permission(String permission) {
            this.permission = Objects.requireNonNull(permission);
            return this;
        }

        /**
         * Adds a button.
         *
         * @param buttonId component identifier
         * @param spec configures the button
         * @return this builder
         */
        public ScreenBuilder button(String buttonId, Consumer<ButtonBuilder> spec) {
            var b = new ButtonBuilder(buttonId);
            spec.accept(b);
            if (b.action == null) {
                throw new IllegalStateException("Button \"" + buttonId + "\" needs an action");
            }
            if (b.action.type().equals("server_event")) {
                actions.add(b.action.name().orElseThrow());
            }
            components.add(new Component(buttonId, "button", b.text, Optional.ofNullable(b.texture), Optional.ofNullable(b.visibleIf), true,
                    0, 0, List.of(), "", Optional.of(b.action), Optional.empty()));
            return this;
        }

        /** Registers the screen; it replaces a screen with the same identifier. */
        public void register() {
            ui.register(new Screen(id, title, "action_form", content, Optional.ofNullable(permission), Optional.empty(), components), actions);
        }
    }

    /** Configures one button of a code-defined screen. */
    public static final class ButtonBuilder {
        private final String id;
        private String text;
        private String texture;
        private String visibleIf;
        private Action action;

        private ButtonBuilder(String id) {
            if (!id.matches("^[a-z][a-z0-9_]{0,47}$")) {
                throw new IllegalArgumentException("Invalid button id: " + id);
            }
            this.id = id;
            this.text = id;
        }

        /**
         * Sets the label.
         *
         * @param text label; placeholders allowed
         * @return this builder
         */
        public ButtonBuilder text(String text) {
            this.text = Objects.requireNonNull(text);
            return this;
        }

        /**
         * Shows a resource-pack texture on the button.
         *
         * @param path texture path without extension, for example {@code textures/zernyth_ui/coin}
         * @return this builder
         */
        public ButtonBuilder texture(String path) {
            this.texture = Objects.requireNonNull(path);
            return this;
        }

        /**
         * Shows the button only to players with a permission.
         *
         * @param permission the permission
         * @return this builder
         */
        public ButtonBuilder visibleIf(String permission) {
            this.visibleIf = Objects.requireNonNull(permission);
            return this;
        }

        /**
         * Sends a server action when pressed.
         *
         * @param name action name, for example {@code shop.open}
         * @return this builder
         */
        public ButtonBuilder action(String name) {
            if (!name.matches("^[a-z][a-z0-9_]{0,31}(\\.[a-z][a-z0-9_]{0,31}){0,3}$")) {
                throw new IllegalArgumentException("Invalid action name: " + name);
            }
            this.action = new Action("server_event", Optional.of(name), Optional.empty());
            return this;
        }

        /**
         * Opens another screen when pressed.
         *
         * @param screenId target screen
         * @return this builder
         */
        public ButtonBuilder navigate(String screenId) {
            this.action = new Action("navigate", Optional.empty(), Optional.of(screenId));
            return this;
        }

        /**
         * Closes the UI when pressed.
         *
         * @return this builder
         */
        public ButtonBuilder close() {
            this.action = new Action("close", Optional.empty(), Optional.empty());
            return this;
        }

        @Override
        public String toString() {
            return "ButtonBuilder[" + id + "]";
        }
    }
}
