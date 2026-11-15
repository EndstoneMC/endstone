package dev.zernyth.ui;

import java.io.IOException;
import java.io.InputStream;
import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.time.Duration;
import java.util.HexFormat;
import java.util.Objects;
import java.util.Optional;

/**
 * Fetches a project published in Zernyth UI Studio, using a server key ({@code zui_…}) created in the project's
 * publish dialog. Calls block: run them in an async task and load the result on the server thread.
 *
 * <pre>{@code
 * var remote = UiRemote.of(URI.create("https://zernyth.net"), key);
 * scheduler.runTaskAsync(() -> remote.fetch().ifPresent(doc -> scheduler.runTask(() -> ui.load(doc))));
 * }</pre>
 */
public final class UiRemote {
    /** Header that carries the server key. */
    public static final String KEY_HEADER = "X-Zernyth-UI-Key";
    static final long MAX_PACK_BYTES = 20L * 1024 * 1024;

    private final URI base;
    private final String key;
    private final HttpClient http;
    private volatile String etag;
    private volatile int version = -1;

    private UiRemote(URI base, String key) {
        this.base = base;
        this.key = key;
        this.http = HttpClient.newBuilder().connectTimeout(Duration.ofSeconds(10)).followRedirects(HttpClient.Redirect.NEVER).build();
    }

    /**
     * Creates a client for a platform address.
     *
     * @param base platform address such as {@code https://zernyth.net}; plain http only for localhost
     * @param key server key from UI Studio
     * @return the client
     * @throws IllegalArgumentException when the address is not https (except localhost) or the key is malformed
     */
    public static UiRemote of(URI base, String key) {
        Objects.requireNonNull(base);
        Objects.requireNonNull(key);
        String host = base.getHost();
        boolean local = "localhost".equals(host) || "127.0.0.1".equals(host) || "[::1]".equals(host);
        if (!"https".equals(base.getScheme()) && !("http".equals(base.getScheme()) && local)) {
            throw new IllegalArgumentException("The platform address must use https");
        }
        if (!key.matches("^zui_[A-Za-z0-9_-]{20,76}$")) {
            throw new IllegalArgumentException("The UI key is malformed");
        }
        String path = base.getPath() == null ? "" : base.getPath().replaceAll("/+$", "");
        return new UiRemote(URI.create(base.getScheme() + "://" + base.getRawAuthority() + path), key);
    }

    /**
     * Published revision seen by the last successful {@link #fetch()}, or -1.
     *
     * @return the revision number
     */
    public int version() {
        return version;
    }

    private HttpRequest.Builder request(String path) {
        return HttpRequest.newBuilder(URI.create(base + "/api/v1/ui-runtime/" + path)).timeout(Duration.ofSeconds(30)).header(KEY_HEADER, key);
    }

    private static IOException failure(int status) {
        return new IOException(switch (status) {
            case 401 -> "The UI key was rejected (revoked, or the project is not available)";
            case 404 -> "The project is not published";
            case 429 -> "Too many requests; try again later";
            default -> "The platform answered with HTTP " + status;
        });
    }

    private static byte[] read(InputStream in, long limit) throws IOException {
        try (in) {
            byte[] data = in.readNBytes((int) Math.min(limit + 1, Integer.MAX_VALUE));
            if (data.length > limit) {
                throw new IOException("The response is larger than " + limit + " bytes");
            }
            return data;
        }
    }

    /**
     * Downloads the published document if it changed since the last call.
     *
     * @return the validated document, or empty when it is unchanged
     * @throws IOException when the request fails or the platform refuses it
     * @throws InterruptedException when the thread is interrupted
     * @throws UiValidationException when the document does not match the contract
     */
    public Optional<UiDocument> fetch() throws IOException, InterruptedException {
        var builder = request("document").GET();
        String known = etag;
        if (known != null) {
            builder.header("If-None-Match", known);
        }
        var response = http.send(builder.build(), HttpResponse.BodyHandlers.ofInputStream());
        if (response.statusCode() == 304) {
            response.body().close();
            return Optional.empty();
        }
        if (response.statusCode() != 200) {
            response.body().close();
            throw failure(response.statusCode());
        }
        var document = UiDocument.parse(new String(read(response.body(), Json.MAX_CHARS * 2L), StandardCharsets.UTF_8));
        etag = response.headers().firstValue("ETag").orElse(null);
        version = response.headers().firstValue("X-Zernyth-UI-Version").map(Integer::parseInt).orElse(-1);
        return Optional.of(document);
    }

    /**
     * Downloads the published resource pack and replaces {@code target} only after the SHA-256 checksum matched.
     * Bedrock servers read resource packs at start, so restart the server to offer a new pack to players.
     *
     * @param target file to write, for example {@code resource_packs/lobby_ui.mcpack}
     * @return the SHA-256 checksum of the pack
     * @throws IOException when the request fails or the checksum differs
     * @throws InterruptedException when the thread is interrupted
     */
    public String downloadPack(Path target) throws IOException, InterruptedException {
        var response = http.send(request("pack").GET().build(), HttpResponse.BodyHandlers.ofInputStream());
        if (response.statusCode() != 200) {
            response.body().close();
            throw failure(response.statusCode());
        }
        byte[] data = read(response.body(), MAX_PACK_BYTES);
        String expected = response.headers().firstValue("X-Zernyth-UI-Pack-SHA256").orElseThrow(() -> new IOException("The pack checksum is missing"));
        String actual;
        try {
            actual = HexFormat.of().formatHex(MessageDigest.getInstance("SHA-256").digest(data));
        } catch (NoSuchAlgorithmException impossible) {
            throw new IllegalStateException(impossible);
        }
        if (!actual.equals(expected)) {
            throw new IOException("The pack checksum does not match");
        }
        Path parent = target.toAbsolutePath().getParent();
        Files.createDirectories(parent);
        Path temp = Files.createTempFile(parent, ".zernyth-ui", ".tmp");
        try {
            Files.write(temp, data);
            Files.move(temp, target, StandardCopyOption.REPLACE_EXISTING, StandardCopyOption.ATOMIC_MOVE);
        } finally {
            Files.deleteIfExists(temp);
        }
        return actual;
    }
}
