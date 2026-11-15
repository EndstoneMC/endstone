package dev.zernyth.ui.example;

import java.io.IOException;
import java.io.Reader;
import java.net.URI;
import java.nio.file.Files;
import java.util.Map;
import java.util.Properties;
import java.util.UUID;
import java.util.concurrent.ConcurrentHashMap;

import dev.zernyth.api.command.CommandSender;
import dev.zernyth.api.command.PlayerSender;
import dev.zernyth.api.plugin.JavaPlugin;
import dev.zernyth.ui.UiRemote;
import dev.zernyth.ui.ZernythUI;

/**
 * Example: shows screens made in Zernyth UI Studio. With {@code remote.properties} ({@code url} and {@code key}) in the
 * data folder it loads the project published on the platform, and {@code /zui reload} fetches a new publication.
 * Otherwise it reads {@code zernyth-ui.json} from the data folder, or falls back to a screen defined in code.
 */
public final class LobbyMenuPlugin extends JavaPlugin {
    private final Map<UUID, Integer> coins = new ConcurrentHashMap<>();
    private ZernythUI ui;
    private UiRemote remote;

    @Override
    public void onEnable() {
        ui = ZernythUI.create(this);
        var settings = getDataFolder().resolve("remote.properties");
        var exported = getDataFolder().resolve("zernyth-ui.json");
        if (Files.exists(settings)) {
            try (Reader reader = Files.newBufferedReader(settings)) {
                var p = new Properties();
                p.load(reader);
                remote = UiRemote.of(URI.create(p.getProperty("url", "https://zernyth.net")), p.getProperty("key", ""));
            } catch (IOException | IllegalArgumentException failure) {
                getLogger().severe("remote.properties is invalid: " + failure.getMessage());
            }
        }
        if (remote != null) {
            reload(null);
        } else if (Files.exists(exported)) {
            try {
                ui.load(exported);
            } catch (IOException | IllegalArgumentException failure) {
                getLogger().severe("zernyth-ui.json could not be loaded: " + failure.getMessage());
            }
        } else {
            ui.screen("main_menu").title("Main Menu").content("Welcome, {player.name}! Coins: {coins}")
                    .button("shop_button", b -> b.text("Shop").action("shop.open"))
                    .button("close_button", b -> b.text("Close").close())
                    .register();
        }
        ui.bind("coins", player -> String.valueOf(coins.getOrDefault(player.getUniqueId(), 100)));
        ui.onAction("shop.open", event -> event.player().sendMessage("Shop opened!"));
        ui.onAction("shop.buy", event -> {
            int balance = coins.merge(event.player().getUniqueId(), -10, Integer::sum);
            event.player().sendMessage("Bought for 10 coins. Balance: " + balance);
        });
        getServer().registerCommand("menu", "Open the lobby menu", "zernyth.ui.menu", (sender, command, args) -> {
            if (!(sender instanceof PlayerSender playerSender) || !ui.open(playerSender.getPlayer(), "main_menu")) {
                sender.sendMessage("The menu is not available.");
            }
            return true;
        });
        getServer().registerCommand("zui", "Reload the menus published in UI Studio", "zernyth.ui.admin", (sender, command, args) -> {
            if (remote == null) {
                sender.sendMessage("No remote.properties: menus are loaded from files.");
            } else {
                reload(sender);
            }
            return true;
        });
    }

    // The download runs off the server thread; loading happens back on it.
    private void reload(CommandSender sender) {
        var scheduler = getServer().getScheduler();
        scheduler.runTaskAsync(() -> {
            try {
                var document = remote.fetch();
                scheduler.runTask(() -> {
                    document.ifPresent(ui::replace);
                    String message = document.isPresent() ? "UI revision " + remote.version() + " loaded." : "The menus are up to date.";
                    getLogger().info(message);
                    if (sender != null) {
                        sender.sendMessage(message);
                    }
                });
            } catch (IOException | InterruptedException | IllegalArgumentException failure) {
                getLogger().warning("UI reload failed: " + failure.getMessage());
                if (sender != null) {
                    scheduler.runTask(() -> sender.sendMessage("UI reload failed: " + failure.getMessage()));
                }
            }
        });
    }

    @Override
    public void onDisable() {
        if (ui != null) {
            ui.close();
        }
    }
}
