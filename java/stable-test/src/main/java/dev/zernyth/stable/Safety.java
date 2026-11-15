package dev.zernyth.stable;

import java.util.List;

/** Pure fail-closed selection policy, independent of native handles. */
final class Safety {

    private Safety() {
    }

    static String[] arguments(String[] args) {
        if (args.length != 1) {
            return args;
        }
        String text = args[0].strip();
        return text.isEmpty() ? new String[0] : text.split("\\s+");
    }

    static boolean isolated(List<String> names) {
        return names.equals(List.of("zernyth-stable-test"));
    }
}
