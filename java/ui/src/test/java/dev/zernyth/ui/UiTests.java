package dev.zernyth.ui;

import java.net.URI;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.UUID;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.CompletionStage;
import java.util.logging.Logger;

import dev.zernyth.api.form.ActionForm;
import dev.zernyth.api.form.Form;
import dev.zernyth.api.form.FormRequest;
import dev.zernyth.api.form.FormResponse;
import dev.zernyth.api.form.MessageForm;
import dev.zernyth.api.form.ModalForm;
import dev.zernyth.api.player.GameMode;
import dev.zernyth.api.player.Location;
import dev.zernyth.api.player.Player;

/** Tests for the UI runtime; run with {@code gradle :ui:uiTest}. */
final class UiTests {
    private static int passed;

    private UiTests() {
    }

    private static void check(boolean condition, String what) {
        if (!condition) {
            throw new AssertionError("FAILED: " + what);
        }
        passed++;
    }

    /** A player that records shown forms; responses are completed by the test. */
    static final class FakePlayer implements Player {
        final UUID id = UUID.randomUUID();
        final String name;
        final Set<String> permissions;
        final List<Form> forms = new ArrayList<>();
        final List<CompletableFuture<FormResponse>> pending = new ArrayList<>();
        int cancelled;

        FakePlayer(String name, Set<String> permissions) {
            this.name = name;
            this.permissions = permissions;
        }

        @Override
        public FormRequest showForm(Form form) {
            forms.add(form);
            var future = new CompletableFuture<FormResponse>();
            pending.add(future);
            return new FormRequest() {
                @Override
                public CompletionStage<FormResponse> response() {
                    return future;
                }

                @Override
                public void cancel() {
                    cancelled++;
                    future.complete(new FormResponse(FormResponse.Status.CANCELLED, List.of()));
                }
            };
        }

        void answer(int form, Object... values) {
            pending.get(form).complete(new FormResponse(FormResponse.Status.SUBMITTED, List.of(values)));
        }

        Form last() {
            return forms.getLast();
        }

        @Override
        public UUID getUniqueId() {
            return id;
        }

        @Override
        public boolean isOnline() {
            return true;
        }

        @Override
        public void kick(String message) {
        }

        @Override
        public Location getLocation() {
            return new Location("overworld", 0, 64, 0, 0, 0);
        }

        @Override
        public boolean teleport(Location destination) {
            return true;
        }

        @Override
        public GameMode getGameMode() {
            return GameMode.values()[0];
        }

        @Override
        public void setGameMode(GameMode mode) {
        }

        @Override
        public int getHealth() {
            return 20;
        }

        @Override
        public String getName() {
            return name;
        }

        @Override
        public void sendMessage(String message) {
        }

        @Override
        public boolean hasPermission(String permission) {
            return permissions.contains(permission);
        }
    }

    private static Path contract() {
        return Path.of("src/test/resources/contract");
    }

    private static void contractFixtures() throws Exception {
        try (var valid = Files.list(contract().resolve("fixtures/valid"))) {
            for (var file : valid.toList()) {
                var problems = UiDocument.check(Files.readString(file, StandardCharsets.UTF_8));
                check(problems.isEmpty(), "valid fixture " + file.getFileName() + " " + problems);
            }
        }
        try (var invalid = Files.list(contract().resolve("fixtures/invalid"))) {
            for (var file : invalid.toList()) {
                String text = Files.readString(file, StandardCharsets.UTF_8);
                var expect = text.replaceAll("(?s).*\"\\$expect\": \"([a-z_]+)\".*", "$1");
                // The fixture marker is not part of the document.
                String doc = text.replaceAll(",\\s*\"\\$expect\": \"[a-z_]+\"", "");
                var codes = UiDocument.check(doc).stream().map(UiDiagnostic::code).toList();
                check(codes.contains(expect), "invalid fixture " + file.getFileName() + " expects " + expect + " got " + codes);
            }
        }
    }

    private static void hostileJson() {
        check(UiDocument.check("{\"a\":1,\"a\":2}").getFirst().message().contains("Duplicate key"), "duplicate keys rejected");
        check(!UiDocument.check("[".repeat(100) + "]".repeat(100)).isEmpty(), "deep nesting rejected");
        check(!UiDocument.check("{} trailing").isEmpty(), "trailing content rejected");
        check(!UiDocument.check("{\"x\": NaN}").isEmpty(), "NaN rejected");
        check(!UiDocument.check("x".repeat(Json.MAX_CHARS + 1)).isEmpty(), "oversized input rejected");
    }

    private static void groupedReadingOrder() {
        // The group sits below the top button; its child at relative y=0 must come second, and stats text joins the body.
        String json = """
                {"schemaVersion":"1.0","engine":"zernyth","id":"order","name":"Order","target":{"minEngineVersion":[1,21,0],"zernythVersion":"0.9.0"},
                 "startScreen":"main","assets":[],"actions":[],
                 "screens":[{"id":"main","title":"Main","kind":"action_form","canvas":{"width":640,"height":360},"components":[
                   {"id":"group","type":"container","x":0,"y":200,"width":300,"height":100,"children":[
                     {"id":"low","type":"button","x":0,"y":0,"width":200,"height":40,"text":"Low","action":{"type":"close"}}]},
                   {"id":"top","type":"button","x":0,"y":100,"width":200,"height":40,"text":"Top","action":{"type":"close"}},
                   {"id":"stats","type":"player_stats","x":0,"y":10,"width":200,"height":40,"text":"Kills: 3"}]}]}
                """;
        var ui = new ZernythUI(Logger.getLogger("ui-test")).load(UiDocument.parse(json));
        var p = new FakePlayer("Steve", Set.of());
        check(ui.open(p), "grouped screen opens");
        var form = (ActionForm) p.last();
        check(form.buttons().get(0).text().equals("Top") && form.buttons().get(1).text().equals("Low"), "children ordered by canvas position");
        check(form.content().equals("Kills: 3"), "player statistics text is form body");
    }

    private static void conditionsAndTranslations() throws Exception {
        var ui = runtime(contract().resolve("fixtures/valid/conditions.json"));
        var p = new FakePlayer("Steve", Set.of());
        check(ui.open(p, "hello") && ((ActionForm) p.last()).buttons().size() == 1, "unbound condition hides the component");
        var coins = new java.util.concurrent.atomic.AtomicInteger(50);
        ui.bind("coins", player -> String.valueOf(coins.get()));
        ui.open(p, "hello");
        check(((ActionForm) p.last()).buttons().size() == 1, "condition coins >= 100 false at 50");
        coins.set(150);
        ui.open(p, "hello");
        check(((ActionForm) p.last()).buttons().size() == 2, "condition coins >= 100 true at 150");
        var lang = new java.util.concurrent.atomic.AtomicReference<String>("de_DE");
        ui.locale(player -> lang.get());
        ui.open(p, "hello");
        var de = (ActionForm) p.last();
        check(de.title().equals("Hallo") && de.content().equals("Willkommen") && de.buttons().get(1).text().equals("VIP-Laden"), "German texts by exact locale");
        check(de.buttons().get(0).text().equals("OK"), "untranslated text falls back to the document");
        lang.set("de");
        ui.open(p, "hello");
        check(((ActionForm) p.last()).title().equals("Hallo"), "language fallback de -> de_DE");
        lang.set("fr_FR");
        ui.open(p, "hello");
        check(((ActionForm) p.last()).title().equals("Hello"), "unknown language uses the document text");
        var nonNumeric = new UiDocument.Condition("coins", "gt", "5.0", true);
        ui.bind("coins", player -> "many");
        ui.replace(UiDocument.parse(Files.readString(contract().resolve("fixtures/valid/minimal.json"))));
        check(!ui.open(p, "main_menu") && ui.stats().screens() == 1, "replace drops screens of the previous document");
        check(!ui.matches(p, nonNumeric), "non-numeric value never matches a numeric condition");
        check(!ui.matches(p, new UiDocument.Condition("coins", "gt", "a", false)), "text conditions only support eq and ne");
        check(ui.matches(p, new UiDocument.Condition("player.name", "eq", "Steve", false)), "player.name can be compared");
    }

    private static void remote() throws Exception {
        String key = "zui_" + "a".repeat(43);
        byte[] doc = Files.readAllBytes(contract().resolve("fixtures/valid/lobby.json"));
        byte[] pack = "pack-bytes".getBytes(StandardCharsets.UTF_8);
        String sha = java.util.HexFormat.of().formatHex(java.security.MessageDigest.getInstance("SHA-256").digest(pack));
        var badSha = new java.util.concurrent.atomic.AtomicBoolean();
        var server = com.sun.net.httpserver.HttpServer.create(new java.net.InetSocketAddress("127.0.0.1", 0), 0);
        server.createContext("/api/v1/ui-runtime/", exchange -> {
            try (exchange) {
                if (!key.equals(exchange.getRequestHeaders().getFirst(UiRemote.KEY_HEADER))) {
                    exchange.sendResponseHeaders(401, -1);
                    return;
                }
                exchange.getResponseHeaders().add("X-Zernyth-UI-Version", "3");
                exchange.getResponseHeaders().add("X-Zernyth-UI-Pack-SHA256", badSha.get() ? "0".repeat(64) : sha);
                if (exchange.getRequestURI().getPath().endsWith("/document")) {
                    if ("\"v3\"".equals(exchange.getRequestHeaders().getFirst("If-None-Match"))) {
                        exchange.sendResponseHeaders(304, -1);
                        return;
                    }
                    exchange.getResponseHeaders().add("ETag", "\"v3\"");
                    exchange.sendResponseHeaders(200, doc.length);
                    exchange.getResponseBody().write(doc);
                } else {
                    exchange.sendResponseHeaders(200, pack.length);
                    exchange.getResponseBody().write(pack);
                }
            }
        });
        server.start();
        try {
            var base = URI.create("http://127.0.0.1:" + server.getAddress().getPort());
            var remote = UiRemote.of(base, key);
            var first = remote.fetch();
            check(first.isPresent() && first.get().id().equals("lobby_ui") && remote.version() == 3, "remote document fetched and validated");
            check(remote.fetch().isEmpty(), "unchanged document is not downloaded again (ETag)");
            Path dir = Files.createTempDirectory("zui-pack");
            Path target = dir.resolve("lobby.mcpack");
            check(remote.downloadPack(target).equals(sha) && java.util.Arrays.equals(Files.readAllBytes(target), pack), "pack downloaded with checksum");
            badSha.set(true);
            try {
                remote.downloadPack(dir.resolve("other.mcpack"));
                check(false, "wrong checksum rejected");
            } catch (java.io.IOException expected) {
                check(!Files.exists(dir.resolve("other.mcpack")), "wrong checksum rejected, nothing written");
            }
            try {
                UiRemote.of(base, "zui_" + "b".repeat(43)).fetch();
                check(false, "unknown key reported");
            } catch (java.io.IOException expected) {
                check(expected.getMessage().contains("rejected"), "unknown key reported");
            }
        } finally {
            server.stop(0);
        }
        try {
            UiRemote.of(URI.create("http://zernyth.net"), key);
            check(false, "plain http refused for remote hosts");
        } catch (IllegalArgumentException expected) {
            check(true, "plain http refused for remote hosts");
        }
    }

    private static ZernythUI runtime(Path doc) throws Exception {
        return new ZernythUI(Logger.getLogger("ui-test")).load(doc);
    }

    private static void formsAndActions() throws Exception {
        var ui = runtime(contract().resolve("fixtures/valid/lobby.json"));
        var events = new ArrayList<UiActionEvent>();
        ui.onAction("shop.open", events::add);
        ui.onAction("shop.buy", events::add);
        var steve = new FakePlayer("Steve", Set.of());

        check(ui.open(steve), "start screen opens");
        var menu = (ActionForm) steve.last();
        check(menu.title().equals("Main Menu"), "title");
        check(menu.content().startsWith("Welcome, Steve!"), "built-in placeholder resolved");
        check(menu.buttons().size() == 3, "three buttons in reading order");
        check(menu.buttons().getFirst().text().equals("Shop"), "first button is the top one");
        check("textures/zernyth_ui/coin".equals(menu.buttons().getFirst().image()), "button texture path from the asset");

        steve.answer(0, 0);
        check(events.size() == 1 && events.getFirst().action().equals("shop.open") && events.getFirst().component().equals("shop_button"),
                "server action dispatched with its component");
        check(events.getFirst().player() == steve, "event carries the right player");

        // A second delivery of the same answer (a replay) is ignored: the session for generation 1 is over.
        ui.respond(steve, 1, new FormResponse(FormResponse.Status.SUBMITTED, List.of(0)));
        check(events.size() == 1, "replay ignored");
        // An answer for a generation that never belonged to this player is ignored as well.
        ui.respond(steve, 999, new FormResponse(FormResponse.Status.SUBMITTED, List.of(0)));
        check(events.size() == 1, "foreign generation ignored");

        // Navigation opens the modal form; its submit action needs a permission Steve lacks.
        ui.open(steve);
        steve.answer(1, 1);
        check(steve.last() instanceof ModalForm m && m.controls().size() == 4, "navigation opens the modal form with four controls");
        steve.answer(2, true, 50.0, 1, "Alex");
        check(events.size() == 1, "permission-protected action not dispatched");
        check(ui.stats().rejected() >= 2, "rejections counted");

        var admin = new FakePlayer("Admin", Set.of("lobby.shop"));
        ui.open(admin, "settings");
        admin.answer(0, true, 75.0, 0, "Alex");
        var submit = events.getLast();
        check(submit.action().equals("shop.buy") && submit.values().get("volume").equals(75.0) && submit.values().get("nick").equals("Alex"),
                "modal values keyed by component id");

        // Message form: first button is 0 (as in the native bridge).
        ui.open(admin, "confirm_buy");
        check(admin.last() instanceof MessageForm mf && mf.firstButton().equals("Buy"), "message form buttons");
        admin.answer(1, 0);
        check(events.getLast().component().equals("yes"), "message form first button dispatched");

        // Out-of-range and malformed answers are rejected.
        ui.open(steve);
        long before = ui.stats().rejected();
        steve.answer(3, 99);
        check(ui.stats().rejected() == before + 1, "out-of-range index rejected");

        // Opening a new form supersedes the old one: answering the old one does nothing.
        ui.open(steve);
        ui.open(steve);
        int count = events.size();
        steve.answer(steve.pending.size() - 2, 0);
        check(events.size() == count, "stale form ignored");
        check(steve.cancelled >= 1, "superseded form cancelled");

        // A handler failure is contained.
        ui.onAction("shop.open", e -> {
            throw new IllegalStateException("boom");
        });
        steve.answer(steve.pending.size() - 1, 0);
        check(ui.stats().openSessions() == 0, "session ended after a failing handler");

        ui.open(steve);
        ui.close();
        check(ui.stats().openSessions() == 0, "close drops sessions");
        check(steve.pending.getLast().isDone(), "close cancels open forms");
        try {
            ui.onAction("x.y", e -> { });
            check(false, "closed runtime refuses use");
        } catch (IllegalStateException expected) {
            check(true, "closed runtime refuses use");
        }
        check(!ui.open(steve, "main_menu"), "closed runtime opens nothing");
    }

    private static void builderAndBindings() {
        var ui = new ZernythUI(Logger.getLogger("ui-test"));
        var events = new ArrayList<UiActionEvent>();
        ui.screen("main_menu").title("Hi {player.name}").content("Coins: {coins}")
                .button("shop_button", b -> b.text("Shop").action("shop.open"))
                .button("vip", b -> b.text("VIP").visibleIf("rank.vip").action("vip.open"))
                .register();
        ui.bind("coins", p -> "42");
        ui.onAction("shop.open", events::add);
        var p = new FakePlayer("Alex", Set.of());
        check(ui.open(p, "main_menu"), "code-defined screen opens");
        var form = (ActionForm) p.last();
        check(form.title().equals("Hi Alex") && form.content().equals("Coins: 42"), "bindings resolved");
        check(form.buttons().size() == 1, "permission-gated button hidden");
        p.answer(0, 0);
        check(events.size() == 1, "code-defined action dispatched");
        check(ui.resolve(p, "{unknown}").equals("{unknown}"), "unknown placeholders stay visible");
        try {
            ui.bind("Bad Key", x -> "");
            check(false, "invalid binding refused");
        } catch (IllegalArgumentException expected) {
            check(true, "invalid binding refused");
        }
        check(!ui.open(p, "missing"), "unknown screen not opened");
    }

    private static void benchmark() {
        var ui = new ZernythUI(Logger.getLogger("ui-test"));
        ui.screen("bench").title("{player.name}").button("a", b -> b.text("A {coins}").action("bench.a")).register();
        ui.bind("coins", p -> "1");
        ui.onAction("bench.a", e -> { });
        var p = new FakePlayer("Bench", Set.of());
        int rounds = 20_000;
        for (int i = 0; i < 2_000; i++) {
            ui.open(p, "bench");
            p.answer(p.pending.size() - 1, 0);
        }
        long start = System.nanoTime();
        for (int i = 0; i < rounds; i++) {
            ui.open(p, "bench");
            p.answer(p.pending.size() - 1, 0);
            if (p.pending.size() > 64) {
                p.pending.clear();
                p.forms.clear();
            }
        }
        double micros = (System.nanoTime() - start) / 1_000.0 / rounds;
        System.out.printf("Benchmark (wall clock, fake player, no BDS): open + dispatch %.2f µs per round over %d rounds%n", micros, rounds);
        check(ui.stats().dispatched() == rounds + 2_000L, "every benchmark round dispatched");
    }

    /**
     * Runs all tests.
     *
     * @param args unused
     * @throws Exception on failure
     */
    public static void main(String[] args) throws Exception {
        contractFixtures();
        hostileJson();
        formsAndActions();
        groupedReadingOrder();
        conditionsAndTranslations();
        remote();
        builderAndBindings();
        benchmark();
        System.out.println("UI tests passed: " + passed + " checks");
        check(Map.of().isEmpty(), "done");
    }
}
