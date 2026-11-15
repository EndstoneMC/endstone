package dev.zernyth.ui;

import java.util.ArrayList;
import java.util.Collections;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

/**
 * Strict JSON reader for UI documents: objects become maps, arrays lists, numbers doubles.
 * Input size and nesting depth are bounded; duplicate keys and trailing content are rejected.
 */
final class Json {
    static final int MAX_CHARS = 2 * 1024 * 1024;
    static final int MAX_DEPTH = 64;

    private final String s;
    private int i;

    private Json(String s) {
        this.s = s;
    }

    static Object parse(String text) {
        if (text.length() > MAX_CHARS) {
            throw new IllegalArgumentException("UI document is larger than 2 MiB");
        }
        var json = new Json(text);
        json.space();
        Object value = json.value(0);
        json.space();
        if (json.i != text.length()) {
            throw json.error("Unexpected content after the document");
        }
        return value;
    }

    private IllegalArgumentException error(String message) {
        return new IllegalArgumentException(message + " at offset " + i);
    }

    private void space() {
        while (i < s.length()) {
            char c = s.charAt(i);
            if (c != ' ' && c != '\t' && c != '\n' && c != '\r') {
                return;
            }
            i++;
        }
    }

    private Object value(int depth) {
        if (depth > MAX_DEPTH) {
            throw error("Nesting is deeper than " + MAX_DEPTH);
        }
        if (i >= s.length()) {
            throw error("Unexpected end");
        }
        char c = s.charAt(i);
        return switch (c) {
            case '{' -> object(depth);
            case '[' -> array(depth);
            case '"' -> string();
            case 't' -> literal("true", Boolean.TRUE);
            case 'f' -> literal("false", Boolean.FALSE);
            case 'n' -> literal("null", null);
            default -> number();
        };
    }

    private Object literal(String word, Object result) {
        if (!s.startsWith(word, i)) {
            throw error("Invalid literal");
        }
        i += word.length();
        return result;
    }

    private Map<String, Object> object(int depth) {
        i++;
        var map = new LinkedHashMap<String, Object>();
        space();
        if (i < s.length() && s.charAt(i) == '}') {
            i++;
            return Collections.unmodifiableMap(map);
        }
        while (true) {
            space();
            if (i >= s.length() || s.charAt(i) != '"') {
                throw error("Expected a key");
            }
            String key = string();
            space();
            expect(':');
            space();
            if (map.containsKey(key)) {
                throw error("Duplicate key \"" + key + "\"");
            }
            map.put(key, value(depth + 1));
            space();
            if (i < s.length() && s.charAt(i) == ',') {
                i++;
                continue;
            }
            expect('}');
            return Collections.unmodifiableMap(map);
        }
    }

    private List<Object> array(int depth) {
        i++;
        var list = new ArrayList<Object>();
        space();
        if (i < s.length() && s.charAt(i) == ']') {
            i++;
            return Collections.unmodifiableList(list);
        }
        while (true) {
            space();
            list.add(value(depth + 1));
            space();
            if (i < s.length() && s.charAt(i) == ',') {
                i++;
                continue;
            }
            expect(']');
            return Collections.unmodifiableList(list);
        }
    }

    private void expect(char c) {
        if (i >= s.length() || s.charAt(i) != c) {
            throw error("Expected '" + c + "'");
        }
        i++;
    }

    private String string() {
        i++;
        var out = new StringBuilder();
        while (true) {
            if (i >= s.length()) {
                throw error("Unterminated string");
            }
            char c = s.charAt(i++);
            if (c == '"') {
                return out.toString();
            }
            if (c < 0x20) {
                throw error("Control character in string");
            }
            if (c != '\\') {
                out.append(c);
                continue;
            }
            if (i >= s.length()) {
                throw error("Unterminated escape");
            }
            char e = s.charAt(i++);
            switch (e) {
                case '"', '\\', '/' -> out.append(e);
                case 'b' -> out.append('\b');
                case 'f' -> out.append('\f');
                case 'n' -> out.append('\n');
                case 'r' -> out.append('\r');
                case 't' -> out.append('\t');
                case 'u' -> {
                    if (i + 4 > s.length()) {
                        throw error("Short unicode escape");
                    }
                    try {
                        out.append((char) Integer.parseInt(s.substring(i, i + 4), 16));
                    } catch (NumberFormatException bad) {
                        throw error("Invalid unicode escape");
                    }
                    i += 4;
                }
                default -> throw error("Invalid escape");
            }
        }
    }

    private Double number() {
        int start = i;
        if (i < s.length() && s.charAt(i) == '-') {
            i++;
        }
        while (i < s.length() && "0123456789.eE+-".indexOf(s.charAt(i)) >= 0) {
            i++;
        }
        String text = s.substring(start, i);
        if (text.isEmpty() || !text.matches("-?(0|[1-9][0-9]*)(\\.[0-9]+)?([eE][+-]?[0-9]+)?")) {
            throw error("Invalid number");
        }
        double value = Double.parseDouble(text);
        if (!Double.isFinite(value)) {
            throw error("Number out of range");
        }
        return value;
    }
}
