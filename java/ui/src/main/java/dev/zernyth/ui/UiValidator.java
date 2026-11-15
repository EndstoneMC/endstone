package dev.zernyth.ui;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.regex.Pattern;

/**
 * Validates parsed UI documents. Mirrors {@code zernyth-ui.schema.json} 1.0/1.1 and the cross-reference rules of UI Studio;
 * both sides check the same contract fixtures.
 */
final class UiValidator {
    private static final Pattern IDENTIFIER = Pattern.compile("^[a-z][a-z0-9_]{0,47}$");
    private static final Pattern ACTION = Pattern.compile("^[a-z][a-z0-9_]{0,31}(\\.[a-z][a-z0-9_]{0,31}){0,3}$");
    private static final Pattern UUID = Pattern.compile("^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$");
    private static final Pattern COLOR = Pattern.compile("^#[0-9a-fA-F]{6}([0-9a-fA-F]{2})?$");
    private static final Pattern PERMISSION = Pattern.compile("^[a-z0-9_.-]{1,64}$");
    private static final Pattern ASSET_PATH = Pattern.compile("^textures/zernyth_ui/[a-z0-9_]{1,48}$");
    private static final Pattern LOCALE = Pattern.compile("^[a-z]{2}(_[A-Z]{2})?$");
    private static final Pattern TEXT_KEY = Pattern.compile("^[a-z][a-z0-9_]{0,47}\\.((title|content)|[a-z][a-z0-9_]{0,47}\\.(text|description))$");
    private static final Pattern BINDING = Pattern.compile("^[a-z0-9_.]{1,64}$");
    private static final Set<String> OPS = Set.of("eq", "ne", "gt", "gte", "lt", "lte");
    private static final Pattern VERSION = Pattern.compile("^[0-9]+\\.[0-9]+(\\.[0-9]+)?([-+][0-9A-Za-z.-]+)?$");
    private static final Set<String> COMPONENT_TYPES = Set.of("text", "rich_text", "button", "image_button", "icon", "image", "panel",
            "container", "scroll", "list", "grid", "tabs", "navbar", "progress", "status", "hud_element", "scoreboard", "shop_card",
            "item_display", "player_stats", "team_panel", "selection_menu", "dialog", "confirm", "background", "toggle", "slider",
            "dropdown", "text_field");
    private static final Set<String> KINDS = Set.of("action_form", "modal_form", "message_form", "hud_preview");
    private static final Set<String> ANCHORS = Set.of("top_left", "top", "top_right", "left", "center", "right", "bottom_left", "bottom",
            "bottom_right");

    private final List<UiDiagnostic> out = new ArrayList<>();

    private UiValidator() {
    }

    static List<UiDiagnostic> validate(Object root) {
        var v = new UiValidator();
        v.document(root);
        if (v.out.isEmpty()) {
            v.references((Map<?, ?>) root);
        }
        return List.copyOf(v.out);
    }

    private void fail(String path, String message) {
        if (out.size() < 50) {
            out.add(new UiDiagnostic("schema", path + " " + message, path));
        }
    }

    private Map<?, ?> object(Object value, String path, Set<String> allowed, Set<String> required) {
        if (!(value instanceof Map<?, ?> map)) {
            fail(path, "must be an object");
            return null;
        }
        for (var key : map.keySet()) {
            if (!allowed.contains(key)) {
                fail(path, "has an unknown property \"" + key + "\"");
            }
        }
        for (var key : required) {
            if (!map.containsKey(key)) {
                fail(path, "is missing \"" + key + "\"");
            }
        }
        return map;
    }

    private void string(Map<?, ?> map, String key, String path, int min, int max, Pattern pattern) {
        if (!map.containsKey(key)) {
            return;
        }
        if (!(map.get(key) instanceof String s) || s.length() < min || s.length() > max || pattern != null && !pattern.matcher(s).matches()) {
            fail(path + "." + key, "is not a valid value");
        }
    }

    private void number(Map<?, ?> map, String key, String path, double min, double max, boolean integer) {
        if (!map.containsKey(key)) {
            return;
        }
        if (!(map.get(key) instanceof Double d) || d < min || d > max || integer && d != Math.rint(d)) {
            fail(path + "." + key, "must be a number from " + min + " to " + max);
        }
    }

    private void bool(Map<?, ?> map, String key, String path) {
        if (map.containsKey(key) && !(map.get(key) instanceof Boolean)) {
            fail(path + "." + key, "must be true or false");
        }
    }

    private void oneOf(Map<?, ?> map, String key, String path, Set<String> values) {
        if (map.containsKey(key) && !(map.get(key) instanceof String s && values.contains(s))) {
            fail(path + "." + key, "has an unsupported value");
        }
    }

    private List<?> array(Map<?, ?> map, String key, String path, int min, int max) {
        if (!map.containsKey(key)) {
            return List.of();
        }
        if (!(map.get(key) instanceof List<?> list) || list.size() < min || list.size() > max) {
            fail(path + "." + key, "must be a list with " + min + " to " + max + " entries");
            return List.of();
        }
        return list;
    }

    private void triple(Map<?, ?> map, String key, String path, int max) {
        if (!map.containsKey(key)) {
            return;
        }
        if (!(map.get(key) instanceof List<?> list) || list.size() != 3
                || !list.stream().allMatch(n -> n instanceof Double d && d >= 0 && d <= max && d == Math.rint(d))) {
            fail(path + "." + key, "must be three whole numbers");
        }
    }

    private void document(Object root) {
        var map = object(root, "document", Set.of("schemaVersion", "engine", "id", "name", "description", "target", "pack", "screens", "assets",
                "actions", "startScreen", "defaultLocale", "translations"), Set.of("schemaVersion", "engine", "id", "name", "target", "screens", "assets", "actions"));
        if (map == null) {
            return;
        }
        if (!"1.0".equals(map.get("schemaVersion")) && !"1.1".equals(map.get("schemaVersion"))) {
            fail("document.schemaVersion", "must be \"1.0\" or \"1.1\"");
        }
        string(map, "defaultLocale", "document", 2, 5, LOCALE);
        if (map.containsKey("translations")) {
            if (!(map.get("translations") instanceof Map<?, ?> locales) || locales.size() > 20) {
                fail("document.translations", "must be an object with at most 20 locales");
            } else {
                for (var entry : locales.entrySet()) {
                    String path = "document.translations." + entry.getKey();
                    if (!(entry.getKey() instanceof String locale) || !LOCALE.matcher(locale).matches()) {
                        fail(path, "is not a valid locale");
                    }
                    if (!(entry.getValue() instanceof Map<?, ?> texts) || texts.size() > 5000) {
                        fail(path, "must be an object with at most 5000 texts");
                        continue;
                    }
                    for (var text : texts.entrySet()) {
                        if (!(text.getKey() instanceof String key) || !TEXT_KEY.matcher(key).matches()) {
                            fail(path, "has an invalid key");
                        } else if (!(text.getValue() instanceof String value) || value.length() > 4000) {
                            fail(path + "." + key, "must be a text of at most 4000 characters");
                        }
                    }
                }
            }
        }
        if (!"zernyth".equals(map.get("engine"))) {
            fail("document.engine", "must be \"zernyth\"");
        }
        string(map, "id", "document", 1, 48, IDENTIFIER);
        string(map, "name", "document", 1, 80, null);
        string(map, "description", "document", 0, 500, null);
        string(map, "startScreen", "document", 1, 48, IDENTIFIER);
        var target = object(map.get("target"), "document.target", Set.of("minEngineVersion", "zernythVersion"),
                Set.of("minEngineVersion", "zernythVersion"));
        if (target != null) {
            triple(target, "minEngineVersion", "document.target", 999);
            string(target, "zernythVersion", "document.target", 1, 64, VERSION);
        }
        if (map.containsKey("pack")) {
            var pack = object(map.get("pack"), "document.pack", Set.of("headerUuid", "moduleUuid", "version", "iconAsset"),
                    Set.of("headerUuid", "moduleUuid", "version"));
            if (pack != null) {
                string(pack, "headerUuid", "document.pack", 36, 36, UUID);
                string(pack, "moduleUuid", "document.pack", 36, 36, UUID);
                triple(pack, "version", "document.pack", 9999);
                string(pack, "iconAsset", "document.pack", 1, 48, IDENTIFIER);
            }
        }
        var assets = array(map, "assets", "document", 0, 500);
        for (int n = 0; n < assets.size(); n++) {
            String path = "assets[" + n + "]";
            var a = object(assets.get(n), path, Set.of("id", "name", "path", "width", "height", "sha256", "source", "sourceId"),
                    Set.of("id", "name", "path", "width", "height"));
            if (a != null) {
                string(a, "id", path, 1, 48, IDENTIFIER);
                string(a, "name", path, 1, 80, null);
                string(a, "path", path, 1, 80, ASSET_PATH);
                number(a, "width", path, 1, 1024, true);
                number(a, "height", path, 1, 1024, true);
                string(a, "sha256", path, 64, 64, Pattern.compile("^[0-9a-f]{64}$"));
                oneOf(a, "source", path, Set.of("upload", "builtin"));
                string(a, "sourceId", path, 0, 64, null);
            }
        }
        var actions = array(map, "actions", "document", 0, 500);
        for (int n = 0; n < actions.size(); n++) {
            String path = "actions[" + n + "]";
            var a = object(actions.get(n), path, Set.of("name", "description", "permission"), Set.of("name"));
            if (a != null) {
                string(a, "name", path, 1, 131, ACTION);
                string(a, "description", path, 0, 200, null);
                string(a, "permission", path, 1, 64, PERMISSION);
            }
        }
        var screens = array(map, "screens", "document", 1, 100);
        for (int n = 0; n < screens.size(); n++) {
            screen(screens.get(n), "screens[" + n + "]");
        }
    }

    private void action(Object value, String path) {
        var a = object(value, path, Set.of("type", "name", "target"), Set.of("type"));
        if (a == null) {
            return;
        }
        oneOf(a, "type", path, Set.of("server_event", "navigate", "close"));
        string(a, "name", path, 1, 131, ACTION);
        string(a, "target", path, 1, 48, IDENTIFIER);
        if ("server_event".equals(a.get("type")) && !a.containsKey("name")) {
            fail(path, "is missing \"name\"");
        }
        if ("navigate".equals(a.get("type")) && !a.containsKey("target")) {
            fail(path, "is missing \"target\"");
        }
    }

    private void screen(Object value, String path) {
        var s = object(value, path, Set.of("id", "title", "kind", "content", "permission", "submit", "canvas", "components"),
                Set.of("id", "title", "kind", "canvas", "components"));
        if (s == null) {
            return;
        }
        string(s, "id", path, 1, 48, IDENTIFIER);
        string(s, "title", path, 1, 120, null);
        oneOf(s, "kind", path, KINDS);
        string(s, "content", path, 0, 1000, null);
        string(s, "permission", path, 1, 64, PERMISSION);
        if (s.containsKey("submit")) {
            action(s.get("submit"), path + ".submit");
        }
        var canvas = object(s.get("canvas"), path + ".canvas", Set.of("width", "height", "background", "backgroundAsset"),
                Set.of("width", "height"));
        if (canvas != null) {
            number(canvas, "width", path + ".canvas", 160, 3840, true);
            number(canvas, "height", path + ".canvas", 120, 2160, true);
            string(canvas, "background", path + ".canvas", 7, 9, COLOR);
            string(canvas, "backgroundAsset", path + ".canvas", 1, 48, IDENTIFIER);
        }
        var components = array(s, "components", path, 0, 500);
        for (int n = 0; n < components.size(); n++) {
            component(components.get(n), path + ".components[" + n + "]", 0);
        }
    }

    private void component(Object value, String path, int depth) {
        var c = object(value, path, Set.of("id", "type", "name", "x", "y", "width", "height", "anchor", "hidden", "locked", "visibleIf",
                "enabled", "text", "description", "icon", "texture", "style", "value", "max", "options", "placeholder", "action", "children", "showIf"),
                Set.of("id", "type", "x", "y", "width", "height"));
        if (c == null) {
            return;
        }
        string(c, "id", path, 1, 48, IDENTIFIER);
        oneOf(c, "type", path, COMPONENT_TYPES);
        string(c, "name", path, 0, 80, null);
        number(c, "x", path, -4000, 8000, false);
        number(c, "y", path, -4000, 8000, false);
        number(c, "width", path, 1, 8000, false);
        number(c, "height", path, 1, 8000, false);
        oneOf(c, "anchor", path, ANCHORS);
        bool(c, "hidden", path);
        bool(c, "locked", path);
        bool(c, "enabled", path);
        string(c, "visibleIf", path, 1, 64, PERMISSION);
        string(c, "text", path, 0, 1000, null);
        string(c, "description", path, 0, 1000, null);
        string(c, "icon", path, 1, 48, IDENTIFIER);
        string(c, "texture", path, 1, 48, IDENTIFIER);
        string(c, "placeholder", path, 0, 200, null);
        number(c, "value", path, -Double.MAX_VALUE, Double.MAX_VALUE, false);
        number(c, "max", path, -Double.MAX_VALUE, Double.MAX_VALUE, false);
        for (var option : array(c, "options", path, 0, 64)) {
            if (!(option instanceof String o) || o.length() > 80) {
                fail(path + ".options", "must contain short texts");
            }
        }
        if (c.containsKey("style")) {
            var st = object(c.get("style"), path + ".style", Set.of("fill", "textColor", "border", "borderWidth", "radius", "opacity",
                    "fontSize", "align", "padding", "pixelated"), Set.of());
            if (st != null) {
                for (var key : List.of("fill", "textColor", "border")) {
                    string(st, key, path + ".style", 7, 9, COLOR);
                }
                number(st, "borderWidth", path + ".style", 0, 32, false);
                number(st, "radius", path + ".style", 0, 64, false);
                number(st, "opacity", path + ".style", 0, 1, false);
                number(st, "fontSize", path + ".style", 6, 96, false);
                number(st, "padding", path + ".style", 0, 128, false);
                oneOf(st, "align", path + ".style", Set.of("left", "center", "right"));
                bool(st, "pixelated", path + ".style");
            }
        }
        if (c.containsKey("action")) {
            action(c.get("action"), path + ".action");
        }
        if (c.containsKey("showIf")) {
            var cond = object(c.get("showIf"), path + ".showIf", Set.of("key", "op", "value"), Set.of("key", "op", "value"));
            if (cond != null) {
                string(cond, "key", path + ".showIf", 1, 64, BINDING);
                oneOf(cond, "op", path + ".showIf", OPS);
                if (!(cond.get("value") instanceof Double || cond.get("value") instanceof String v && v.length() <= 200)) {
                    fail(path + ".showIf.value", "must be a number or a short text");
                }
            }
        }
        if (depth >= 16) {
            fail(path, "is nested too deeply");
            return;
        }
        var children = array(c, "children", path, 0, 200);
        for (int n = 0; n < children.size(); n++) {
            component(children.get(n), path + ".children[" + n + "]", depth + 1);
        }
    }

    private static boolean usesConditions(List<?> components) {
        for (var c : components) {
            var m = (Map<?, ?>) c;
            if (m.containsKey("showIf") || m.get("children") instanceof List<?> children && usesConditions(children)) {
                return true;
            }
        }
        return false;
    }

    private void references(Map<?, ?> doc) {
        var screenIds = new HashSet<String>();
        // 1.0 readers reject unknown fields, so documents using 1.1 features must say 1.1.
        if ("1.0".equals(doc.get("schemaVersion")) && (doc.containsKey("translations") || doc.containsKey("defaultLocale")
                || ((List<?>) doc.get("screens")).stream().anyMatch(s -> usesConditions((List<?>) ((Map<?, ?>) s).get("components"))))) {
            out.add(new UiDiagnostic("version_feature", "Conditions and translations need schema version 1.1", "schemaVersion"));
        }
        var screens = (List<?>) doc.get("screens");
        for (var s : screens) {
            var id = (String) ((Map<?, ?>) s).get("id");
            if (!screenIds.add(id)) {
                out.add(new UiDiagnostic("duplicate_id", "Two screens use the id \"" + id + "\"", "screens"));
            }
        }
        var assets = new HashSet<String>();
        for (var a : (List<?>) doc.get("assets")) {
            var id = (String) ((Map<?, ?>) a).get("id");
            if (!assets.add(id)) {
                out.add(new UiDiagnostic("duplicate_id", "Two assets use the id \"" + id + "\"", "assets"));
            }
        }
        var actions = new HashSet<String>();
        for (var a : (List<?>) doc.get("actions")) {
            actions.add((String) ((Map<?, ?>) a).get("name"));
        }
        if (doc.get("startScreen") instanceof String start && !screenIds.contains(start)) {
            out.add(new UiDiagnostic("unknown_screen", "The start screen \"" + start + "\" does not exist", "startScreen"));
        }
        if (doc.get("pack") instanceof Map<?, ?> pack && pack.get("iconAsset") instanceof String icon && !assets.contains(icon)) {
            out.add(new UiDiagnostic("missing_asset", "The pack icon \"" + icon + "\" is not in the project", "pack.iconAsset"));
        }
        for (int n = 0; n < screens.size(); n++) {
            var s = (Map<?, ?>) screens.get(n);
            String path = "screens[" + n + "]";
            if (s.get("canvas") instanceof Map<?, ?> canvas && canvas.get("backgroundAsset") instanceof String bg && !assets.contains(bg)) {
                out.add(new UiDiagnostic("missing_asset", "Background \"" + bg + "\" is not in the project", path + ".canvas"));
            }
            actionRef(s.get("submit"), path + ".submit", screenIds, actions);
            var ids = new HashSet<String>();
            componentRefs((List<?>) s.get("components"), path, ids, screenIds, assets, actions);
        }
    }

    private void componentRefs(List<?> components, String path, Set<String> ids, Set<String> screens, Set<String> assets,
            Set<String> actions) {
        for (int n = 0; n < components.size(); n++) {
            var c = (Map<?, ?>) components.get(n);
            String p = path + ".components[" + n + "]";
            String id = (String) c.get("id");
            if (!ids.add(id)) {
                out.add(new UiDiagnostic("duplicate_id", "Two components use the id \"" + id + "\"", p));
            }
            for (var key : List.of("icon", "texture")) {
                if (c.get(key) instanceof String ref && !assets.contains(ref)) {
                    out.add(new UiDiagnostic("missing_asset", "\"" + id + "\" uses the asset \"" + ref + "\", which is not in the project", p));
                }
            }
            actionRef(c.get("action"), p + ".action", screens, actions);
            if (c.get("children") instanceof List<?> children) {
                componentRefs(children, p, ids, screens, assets, actions);
            }
        }
    }

    private void actionRef(Object value, String path, Set<String> screens, Set<String> actions) {
        if (!(value instanceof Map<?, ?> a)) {
            return;
        }
        if ("navigate".equals(a.get("type")) && a.get("target") instanceof String t && !screens.contains(t)) {
            out.add(new UiDiagnostic("unknown_screen", "Navigation to \"" + t + "\", which does not exist", path));
        }
        if ("server_event".equals(a.get("type")) && a.get("name") instanceof String name && !actions.contains(name)) {
            out.add(new UiDiagnostic("undeclared_action", "The action \"" + name + "\" is not declared", path));
        }
    }
}
