package dev.zernyth.ui.example;

import java.util.List;
import java.util.Map;
import java.util.UUID;
import java.util.concurrent.ConcurrentHashMap;

import dev.zernyth.api.command.PlayerSender;
import dev.zernyth.api.plugin.JavaPlugin;
import dev.zernyth.ui.ZernythUI;

/**
 * Test plugin for the experimental showcase resource pack ({@code java/ui/showcase}). Every screen is an ordinary action
 * form; an invisible marker in the title tells the pack's JSON UI which layout to draw. Without the pack, players see
 * the normal Bedrock form. {@code /zuitest [main|teams|gods|god|stats|shop]} opens a screen.
 */
public final class UiShowcasePlugin extends JavaPlugin {
    // Must match showcase/markers.json (colour codes followed by a reset, so they are invisible in the title).
    static final String MAIN = "§1§2§3§a§r";
    static final String TEAMS = "§1§2§3§b§r";
    static final String GODS = "§1§2§3§c§r";
    static final String GOD = "§1§2§3§d§r";
    static final String STATS = "§1§2§3§e§r";
    static final String SHOP = "§1§2§3§f§r";
    static final String INFO = "§9§8§r";
    static final String TAB = "§9§7§r";
    static final List<String> SCREENS = List.of("main", "teams", "gods", "god", "stats", "shop_blocks", "shop_nether", "shop_portal");

    private final Map<UUID, String> chosenGod = new ConcurrentHashMap<>();
    private ZernythUI ui;

    private static String tex(String name) {
        return "textures/zernyth_ui/showcase/" + name;
    }

    @Override
    public void onEnable() {
        ui = ZernythUI.create(this);
        registerScreens();
        ui.bind("god", player -> chosenGod.getOrDefault(player.getUniqueId(), "none"));
        ui.onAction("showcase.play", e -> e.player().sendMessage("§aPlay pressed (showcase)."));
        ui.onAction("showcase.team", e -> e.player().sendMessage("§eTeam action: " + e.component()));
        ui.onAction("showcase.settings", e -> e.player().sendMessage("§7Settings are not part of the showcase."));
        ui.onAction("showcase.god_accept", e -> {
            chosenGod.put(e.player().getUniqueId(), "Anarmir");
            e.player().sendMessage("§dYou follow Anarmir now.");
        });
        ui.onAction("showcase.buy", e -> e.player().sendMessage("§aWould buy: " + e.component() + " (showcase, nothing is charged)."));
        ui.onAction("showcase.info", e -> ui.open(e.player(), "god"));
        getServer().registerCommand("zuitest", "Open a Zernyth UI showcase screen", "zernyth.ui.test", (sender, command, args) -> {
            String screen = args.length == 0 ? "main" : args[0].equals("shop") ? "shop_blocks" : args[0];
            if (!(sender instanceof PlayerSender playerSender)) {
                sender.sendMessage("Only players can open forms.");
            } else if (!SCREENS.contains(screen)) {
                sender.sendMessage("Screens: main, teams, gods, god, stats, shop");
            } else if (!ui.open(playerSender.getPlayer(), screen)) {
                sender.sendMessage("The screen could not be opened.");
            }
            return true;
        });
    }

    private void registerScreens() {
        ui.screen("main").title(MAIN + "MAIN MENU").content("")
                .button("play", b -> b.text("§lPLAY§r\n§7Join a game").texture(tex("tile_play")).action("showcase.play"))
                .button("shop", b -> b.text("§lSHOP§r\n§7Buy items").texture(tex("tile_shop")).navigate("shop_blocks"))
                .button("gods", b -> b.text("§lGODS§r\n§7Choose your god").texture(tex("tile_gods")).navigate("gods"))
                .button("teams", b -> b.text("§lTEAMS§r\n§7Manage your team").texture(tex("tile_teams")).navigate("teams"))
                .button("stats", b -> b.text("§lSTATS§r\n§7View your stats").texture(tex("tile_stats")).navigate("stats"))
                .button("settings", b -> b.text("§lSETTINGS§r\n§7Adjust your preferences").texture(tex("tile_settings")).action("showcase.settings"))
                .register();

        var teams = ui.screen("teams").title(TEAMS + "TEAMS").content("");
        String[][] teamTiles = {
            {"info", "TEAM INFO", "Access your team stats"}, {"members", "MANAGE MEMBERS", "Manage your team's members"},
            {"alliances", "ALLIANCES", "Manage your team's alliances"}, {"chunks", "TEAM CHUNKS", "Claim / unclaim chunks"},
            {"leave", "LEAVE TEAM", "Leave / disband your team"}, {"settings", "TEAM SETTINGS", "Manage your team's settings"},
            {"invite", "INVITE PLAYERS", "Invite players to your team"}, {"raids", "RAIDS", "Start raids or view history"},
        };
        for (String[] t : teamTiles) {
            teams.button(t[0], b -> b.text("§l" + t[1] + "§r\n§f" + t[2]).texture(tex("team_" + t[0])).action("showcase.team"));
        }
        teams.button("back", b -> b.text("§lBACK§r\n§fReturn to main menu").texture(tex("team_back")).navigate("main")).register();

        var gods = ui.screen("gods").title(GODS + "GOD AFFINITY").content(
                "§6§lScoreboard§r\n§f1. Infernir §e1\n§f2. Anarmir §e0\n§f3. Ox'al §e0\n§f4. Tossenkim §e0\n"
                        + "§f5. Padandir §e0\n§f6. Stelhym §e0\n§f7. Conssesir §e0\n§f8. Kiwiki §e0\n\n§7Your god: §d{god}");
        String[][] godList = {
            {"anarmir", "Anarmir", "Dian of Chaos", "Speed I"}, {"infernir", "Infernir", "Arkess of Fire", "Fire Resistance I"},
            {"tossenkim", "Tossenkim", "Ark of Tide", "Water Breathing I"}, {"padandir", "Padandir", "Dian of War", "Strength I"},
            {"stelhym", "Stelhym", "Drune of Life", "Regeneration I"}, {"conssesir", "Conssesir", "Drune of Death", "Wither Ward I"},
            {"oxal", "Ox'al", "Arkess of Discipline", "Village Hero I"}, {"kiwiki", "Kiwiki", "Ark of Winds", "Jump Boost I"},
        };
        for (String[] g : godList) {
            gods.button(g[0], b -> b.text("§l" + g[1] + "§r\n§7" + g[2] + "\n§8" + g[3]).texture(tex("god_" + g[0])).navigate("god"));
        }
        gods.button("back", b -> b.text("§lBack§r\n§7Main menu").texture(tex("logo")).navigate("main")).register();

        // Order matters: the two choices first, information cards (INFO marker) after them.
        ui.screen("god").title(GOD + "ANARMIR").content(
                "§l§fANARMIR§r\n§7Anarmir, Dian of Chaos, the Pillagers' deity\nand spiritual enemy of the Kadi.\n\n"
                        + "§eDo you wish to pick this god\nand take on their challenge?")
                .button("yes", b -> b.text("§eYes").action("showcase.god_accept"))
                .button("no", b -> b.text("§cNo").navigate("gods"))
                .button("level1", b -> b.text(INFO + "§9§lLVL 1§r\n\n§fIf your god loses:\nSpeed I").action("showcase.info"))
                .button("level2", b -> b.text(INFO + "§a§lLVL 2§r\n\n§fIf your god wins:\nSpeed II").action("showcase.info"))
                .register();

        ui.screen("stats").title(STATS + "STATS").content(
                "§bHere are your current stats, {player.name}:\n\n§aKills: §f0\n§aDeaths: §f0\n§aK/D ratio: §f0.00\n§aLevel: §f0\n"
                        + "§aCoins: §f0\n§aBlocks broken: §f0\n§aBlocks placed: §f0\n\n§7(Showcase values; connect your own statistics with ui.bind.)\n\n"
                        + "§eKeep playing to improve your stats!")
                .button("back", b -> b.text("Back").navigate("main"))
                .register();

        shop("shop_blocks", "BUILDING BLOCKS", new String[][] {
            {"stone", "Stone", "2", "0.01"}, {"bricks", "Bricks", "6", "0.05"}, {"basalt", "Basalt", "20", "0.02"},
        });
        shop("shop_nether", "NETHER BLOCKS", new String[][] {
            {"netherrack", "Netherrack", "4", "0.01"}, {"soul_sand", "Soul Sand", "40", "0.02"}, {"soul_soil", "Soul Soil", "40", "0.02"},
        });
        shop("shop_portal", "DIMENSIONAL BLOCKS", new String[][] {
            {"obsidian", "Obsidian", "100", "1"}, {"crying_obsidian", "Crying Obsidian", "200", "2"}, {"portal", "Portal Stone", "500", "5"},
        });
    }

    // Item cards first, category tabs (TAB marker) last: the item grid then has no gaps before the items.
    private void shop(String id, String title, String[][] items) {
        var screen = ui.screen(id).title(SHOP + title).content("");
        for (String[] item : items) {
            screen.button(item[0], b -> b.text("§f" + item[1] + "\n§aBuy: " + item[2] + " $\n§cSell: " + item[3] + " $")
                    .texture(tex("block_" + item[0])).action("showcase.buy"));
        }
        screen.button("tab_blocks", b -> b.text(TAB + "Building").texture(tex("block_bricks")).navigate("shop_blocks"))
                .button("tab_nether", b -> b.text(TAB + "Nether").texture(tex("block_netherrack")).navigate("shop_nether"))
                .button("tab_portal", b -> b.text(TAB + "Dimensional").texture(tex("block_portal")).navigate("shop_portal"))
                .register();
    }

    @Override
    public void onDisable() {
        if (ui != null) {
            ui.close();
        }
    }
}
