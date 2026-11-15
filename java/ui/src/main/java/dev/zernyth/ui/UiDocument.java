package dev.zernyth.ui;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Optional;

/**
 * A validated Zernyth UI document (schema 1.0), as exported by Zernyth UI Studio.
 *
 * @param id project identifier
 * @param name project name
 * @param startScreen screen opened by {@link ZernythUI#open(dev.zernyth.api.player.Player)}, if set
 * @param screens screens by identifier, in document order
 * @param actions declared server actions by name; only these can be dispatched
 * @param assetPaths resource-pack texture paths by asset identifier
 * @param defaultLocale language of the document's own texts, or empty (schema 1.1)
 * @param translations texts by locale and key such as {@code main.title} or {@code main.shop.text} (schema 1.1)
 */
public record UiDocument(String id, String name, Optional<String> startScreen, Map<String, Screen> screens,
        Map<String, ActionDefinition> actions, Map<String, String> assetPaths, Optional<String> defaultLocale,
        Map<String, Map<String, String>> translations) {

    /**
     * Validates the parts.
     *
     * @param id project identifier
     * @param name project name
     * @param startScreen start screen
     * @param screens screens
     * @param actions declared actions
     * @param assetPaths texture paths
     * @param defaultLocale document language
     * @param translations translated texts
     */
    public UiDocument {
        screens = Map.copyOf(screens);
        actions = Map.copyOf(actions);
        assetPaths = Map.copyOf(assetPaths);
        var copy = new LinkedHashMap<String, Map<String, String>>();
        translations.forEach((locale, texts) -> copy.put(locale, Map.copyOf(texts)));
        translations = Map.copyOf(copy);
    }

    /**
     * Shows a component only when a value bound with {@link ZernythUI#bind} matches (schema 1.1).
     *
     * @param key binding key, or {@code player.name}
     * @param op {@code eq}, {@code ne}, {@code gt}, {@code gte}, {@code lt} or {@code lte}
     * @param value value to compare with
     * @param numeric whether {@code value} is a number; then the bound value is compared as a number
     */
    public record Condition(String key, String op, String value, boolean numeric) {
    }

    /**
     * A declared action.
     *
     * @param name action name such as {@code shop.open}
     * @param permission permission the player needs, or empty
     */
    public record ActionDefinition(String name, Optional<String> permission) {
    }

    /**
     * What a component or form submission does.
     *
     * @param type {@code server_event}, {@code navigate} or {@code close}
     * @param name action name for server events
     * @param target screen identifier for navigation
     */
    public record Action(String type, Optional<String> name, Optional<String> target) {
    }

    /**
     * A component, flattened from the document tree in reading order.
     *
     * @param id component identifier
     * @param type component type
     * @param text display text, may contain placeholders such as {@code {player.name}}
     * @param texture resource-pack texture path for buttons, or empty
     * @param visibleIf permission required to show the component, or empty
     * @param enabled whether pressing it does anything
     * @param value numeric value (toggle, slider)
     * @param max slider maximum
     * @param options dropdown options
     * @param placeholder text-field placeholder
     * @param action what pressing it does, or empty
     * @param showIf condition for showing it, or empty
     */
    public record Component(String id, String type, String text, Optional<String> texture, Optional<String> visibleIf, boolean enabled,
            double value, double max, List<String> options, String placeholder, Optional<Action> action, Optional<Condition> showIf) {
        /**
         * Copies the options.
         *
         * @param id identifier
         * @param type type
         * @param text text
         * @param texture texture path
         * @param visibleIf permission
         * @param enabled enabled
         * @param value value
         * @param max maximum
         * @param options options
         * @param placeholder placeholder
         * @param action action
         * @param showIf condition
         */
        public Component {
            options = List.copyOf(options);
        }
    }

    /**
     * A screen.
     *
     * @param id screen identifier
     * @param title form title
     * @param kind {@code action_form}, {@code modal_form}, {@code message_form} or {@code hud_preview}
     * @param content body text (action and message forms)
     * @param permission permission needed to open it, or empty
     * @param submit action for modal-form submissions, or empty
     * @param components components in reading order (top to bottom, then left to right)
     */
    public record Screen(String id, String title, String kind, String content, Optional<String> permission, Optional<Action> submit,
            List<Component> components) {
        /**
         * Copies the components.
         *
         * @param id identifier
         * @param title title
         * @param kind kind
         * @param content content
         * @param permission permission
         * @param submit submit action
         * @param components components
         */
        public Screen {
            components = List.copyOf(components);
        }
    }

    /**
     * Parses and validates a document.
     *
     * @param json document text
     * @return the document
     * @throws UiValidationException when the text is not a valid document
     */
    public static UiDocument parse(String json) {
        Object root;
        try {
            root = Json.parse(json);
        } catch (IllegalArgumentException malformed) {
            throw new UiValidationException(List.of(new UiDiagnostic("schema", "Invalid JSON: " + malformed.getMessage(), "document")));
        }
        var problems = UiValidator.validate(root);
        if (!problems.isEmpty()) {
            throw new UiValidationException(problems);
        }
        return from((Map<?, ?>) root);
    }

    /**
     * Reads, parses and validates a document file (UTF-8).
     *
     * @param file the file
     * @return the document
     * @throws IOException when the file cannot be read
     * @throws UiValidationException when it is not a valid document
     */
    public static UiDocument read(Path file) throws IOException {
        if (Files.size(file) > Json.MAX_CHARS * 2L) {
            throw new UiValidationException(List.of(new UiDiagnostic("schema", "UI document is larger than 2 MiB", "document")));
        }
        return parse(Files.readString(file, StandardCharsets.UTF_8));
    }

    /**
     * Validates without building a document.
     *
     * @param json document text
     * @return the problems, empty when the document is valid
     */
    public static List<UiDiagnostic> check(String json) {
        try {
            return UiValidator.validate(Json.parse(json));
        } catch (IllegalArgumentException malformed) {
            return List.of(new UiDiagnostic("schema", "Invalid JSON: " + malformed.getMessage(), "document"));
        }
    }

    private static Optional<String> opt(Map<?, ?> map, String key) {
        return map.get(key) instanceof String s ? Optional.of(s) : Optional.empty();
    }

    private static Optional<Action> action(Object value) {
        if (!(value instanceof Map<?, ?> a)) {
            return Optional.empty();
        }
        return Optional.of(new Action((String) a.get("type"), opt(a, "name"), opt(a, "target")));
    }

    private static Optional<Condition> condition(Object value) {
        if (!(value instanceof Map<?, ?> c)) {
            return Optional.empty();
        }
        Object v = c.get("value");
        boolean numeric = v instanceof Double;
        String text = numeric ? Double.toString((Double) v) : (String) v;
        return Optional.of(new Condition((String) c.get("key"), (String) c.get("op"), text, numeric));
    }

    private static UiDocument from(Map<?, ?> root) {
        var assetPaths = new LinkedHashMap<String, String>();
        for (var a : (List<?>) root.get("assets")) {
            var m = (Map<?, ?>) a;
            assetPaths.put((String) m.get("id"), (String) m.get("path"));
        }
        var actions = new LinkedHashMap<String, ActionDefinition>();
        for (var a : (List<?>) root.get("actions")) {
            var m = (Map<?, ?>) a;
            actions.put((String) m.get("name"), new ActionDefinition((String) m.get("name"), opt(m, "permission")));
        }
        var screens = new LinkedHashMap<String, Screen>();
        for (var s : (List<?>) root.get("screens")) {
            var m = (Map<?, ?>) s;
            var flat = new ArrayList<Map<?, ?>>();
            flatten((List<?>) m.get("components"), flat, 0, 0);
            // Forms are lists: the visual layout is read top to bottom, then left to right.
            flat.sort((a, b) -> {
                int byY = Double.compare((Double) a.get("y"), (Double) b.get("y"));
                return byY != 0 ? byY : Double.compare((Double) a.get("x"), (Double) b.get("x"));
            });
            var components = new ArrayList<Component>();
            for (var c : flat) {
                String texture = c.get("icon") instanceof String icon ? assetPaths.get(icon)
                        : c.get("texture") instanceof String tex ? assetPaths.get(tex) : null;
                var options = new ArrayList<String>();
                if (c.get("options") instanceof List<?> list) {
                    list.forEach(o -> options.add((String) o));
                }
                components.add(new Component((String) c.get("id"), (String) c.get("type"), c.get("text") instanceof String t ? t : "",
                        Optional.ofNullable(texture), opt(c, "visibleIf"), !Boolean.FALSE.equals(c.get("enabled")),
                        c.get("value") instanceof Double v ? v : 0, c.get("max") instanceof Double x ? x : 100, options,
                        c.get("placeholder") instanceof String p ? p : "", action(c.get("action")), condition(c.get("showIf"))));
            }
            screens.put((String) m.get("id"), new Screen((String) m.get("id"), (String) m.get("title"), (String) m.get("kind"),
                    m.get("content") instanceof String text ? text : "", opt(m, "permission"), action(m.get("submit")), components));
        }
        var translations = new LinkedHashMap<String, Map<String, String>>();
        if (root.get("translations") instanceof Map<?, ?> locales) {
            locales.forEach((locale, texts) -> {
                var map = new LinkedHashMap<String, String>();
                ((Map<?, ?>) texts).forEach((k, v) -> map.put((String) k, (String) v));
                translations.put((String) locale, map);
            });
        }
        return new UiDocument((String) root.get("id"), (String) root.get("name"), opt(root, "startScreen"), screens, actions, assetPaths,
                opt(root, "defaultLocale"), translations);
    }

    // Child positions are relative to their parent; reading order uses canvas positions.
    private static void flatten(List<?> components, List<Map<?, ?>> out, double ox, double oy) {
        for (var c : components) {
            var m = new java.util.LinkedHashMap<Object, Object>((Map<?, ?>) c);
            if (Boolean.TRUE.equals(m.get("hidden"))) {
                continue;
            }
            double x = (Double) m.get("x") + ox;
            double y = (Double) m.get("y") + oy;
            m.put("x", x);
            m.put("y", y);
            out.add(m);
            if (m.get("children") instanceof List<?> children) {
                flatten(children, out, x, y);
            }
        }
    }
}
