"""Builds the Zernyth UI showcase resource pack: original pixel-art textures and JSON UI for server forms and the HUD.

Run: python generate.py  ->  pack/ (folder) and build/zernyth_ui_showcase.mcpack
Only numpy and the standard library are used. JSON UI targets the vanilla files of Bedrock 1.26.60 (Mojang/bedrock-samples).
"""
import json
import math
import random
import shutil
import struct
import zipfile
import zlib
from pathlib import Path

import numpy as np

ROOT = Path(__file__).resolve().parent
PACK = ROOT / "pack"
HUD_DIR = ROOT / "build" / "hud_experimental"
TEX = "textures/zernyth_ui/showcase"
HEADER_UUID = "6f1c2a8e-4b7d-4e2a-9c3f-2d8b5e7a1c40"
MODULE_UUID = "a3d9e4b2-7c51-4f08-8e6a-91b2c4d7f053"
VERSION = [1, 0, 2]  # raised after the first client test, so clients do not reuse the cached broken pack

# Invisible title markers (valid colour codes followed by a reset). The pack checks for them with string subtraction.
PREFIX = "§1§2§3"
MARKERS = {"main": "§a", "teams": "§b", "gods": "§c", "god": "§d", "stats": "§e", "shop": "§f"}
INFO = "§9§8§r"  # button text marker: information card (god detail, right column)
TAB = "§9§7§r"   # button text marker: shop category tab


def marker(screen: str) -> str:
    return PREFIX + MARKERS[screen] + "§r"


# ---------------------------------------------------------------------------------------------------- PNG writing
def save_png(path: Path, img: np.ndarray):
    img = np.clip(img, 0, 255).astype(np.uint8)
    h, w = img.shape[:2]
    raw = b"".join(b"\x00" + img[y].tobytes() for y in range(h))
    chunk = lambda t, d: struct.pack(">I", len(d)) + t + d + struct.pack(">I", zlib.crc32(t + d) & 0xFFFFFFFF)
    data = b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", struct.pack(">IIBBBBB", w, h, 8, 6, 0, 0, 0)) + chunk(b"IDAT", zlib.compress(raw, 9)) + chunk(b"IEND", b"")
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(data)


def hexc(c: str, a: int = 255):
    c = c.lstrip("#")
    return [int(c[0:2], 16), int(c[2:4], 16), int(c[4:6], 16), a]


def canvas(w, h, color=None):
    img = np.zeros((h, w, 4), dtype=np.float64)
    if color is not None:
        img[:, :] = hexc(color) if isinstance(color, str) else color
    return img


def rect(img, x, y, w, h, color):
    c = hexc(color) if isinstance(color, str) else color
    img[max(0, y):max(0, y + h), max(0, x):max(0, x + w)] = c


def blend(dst, src, x, y):
    h, w = src.shape[:2]
    H, W = dst.shape[:2]
    x0, y0, x1, y1 = max(0, x), max(0, y), min(W, x + w), min(H, y + h)
    if x0 >= x1 or y0 >= y1:
        return
    s = src[y0 - y:y1 - y, x0 - x:x1 - x]
    d = dst[y0:y1, x0:x1]
    a = s[..., 3:4] / 255.0
    d[..., :3] = s[..., :3] * a + d[..., :3] * (1 - a)
    d[..., 3:4] = np.maximum(d[..., 3:4], s[..., 3:4])


def upscale(img, k):
    return np.repeat(np.repeat(img, k, axis=0), k, axis=1)


def frame(w, h, border, inner, edge=None, light=None, width=1, transparent_center=False):
    """Pixel frame: outer border, optional bevel light/edge, filled or transparent inside."""
    img = canvas(w, h, None if transparent_center else inner)
    for i in range(width):
        rect(img, i, i, w - 2 * i, 1, border); rect(img, i, h - 1 - i, w - 2 * i, 1, border)
        rect(img, i, i, 1, h - 2 * i, border); rect(img, w - 1 - i, i, 1, h - 2 * i, border)
    if light:
        rect(img, width, width, w - 2 * width, 1, light); rect(img, width, width, 1, h - 2 * width, light)
    if edge:
        rect(img, width, h - 1 - width, w - 2 * width, 1, edge); rect(img, w - 1 - width, width, 1, h - 2 * width, edge)
    return img


def nineslice(name, img, size):
    save_png(PACK / f"{TEX}/{name}.png", img)
    h, w = img.shape[:2]
    (PACK / f"{TEX}/{name}.json").write_text(json.dumps({"nineslice_size": size, "base_size": [w, h]}, indent=2), encoding="utf-8")


# ---------------------------------------------------------------------------------------------------- pixel icons
ICONS = {
    "sword": ["..........ww", ".........wsw", "........wsw.", ".......wsw..", "......wsw...", ".g...wsw....", ".gg.wsw.....", "..gwsw......", "..bgg.......", ".bb.gg......", "bb...g......", "b..........."],
    "cart": ["............", "ww..........", ".w..........", ".wssssssssw.", ".wssssssssw.", "..wssssssw..", "..wwwwwwww..", "..w.........", "..wwwwwwww..", "............", "...yy...yy..", "...yy...yy.."],
    "crown": ["............", "..y...y...y.", "..yy..y..yy.", "..yyy.y.yyy.", "..yyyyyyyyy.", "..yrryybbyy.", "..yrryybbyy.", "..yyyyyyyyy.", "..ooooooooo.", "............", "............", "............"],
    "people": ["............", "..ww....ww..", ".wwww..wwww.", ".wwww..wwww.", "..ww....ww..", "............", ".wwww..wwww.", "wwwwwwwwwwww", "wwwwwwwwwwww", "wwwwwwwwwwww", "............", "............"],
    "chart": ["............", ".........ww.", ".........ww.", "......ww.ww.", "......ww.ww.", "...ww.ww.ww.", "...ww.ww.ww.", "...ww.ww.ww.", "...ww.ww.ww.", ".wwwwwwwwwww", "............", "............"],
    "gear": ["............", ".....ww.....", "..w.wwww.w..", "..wwwwwwww..", "...ww..ww...", ".www....www.", ".www....www.", "...ww..ww...", "..wwwwwwww..", "..w.wwww.w..", ".....ww.....", "............"],
    "info": ["............", ".....ww.....", ".....ww.....", "............", "....www.....", ".....ww.....", ".....ww.....", ".....ww.....", ".....ww.....", "....wwww....", "............", "............"],
    "shield": ["............", "..wwwwwwww..", "..wbbbbbbw..", "..wbbyybbw..", "..wbyyyybw..", "..wbbyybbw..", "..wbbbbbbw..", "...wbbbbw...", "....wbbw....", ".....ww.....", "............", "............"],
    "flag": ["............", "..wrrrrrr...", "..wrrrrrrr..", "..wrrrrrr...", "..wrrrrr....", "..w.........", "..w.........", "..w.........", "..w.........", ".www........", "............", "............"],
    "door": ["............", "...wwwwww...", "...wbbbbw...", "...wbbbbw...", "...wbbbbw...", "...wbbyyw...", "...wbbbbw...", "...wbbbbw...", "...wbbbbw...", "..wwwwwwww..", "............", "............"],
    "plus": ["............", ".....gg.....", ".....gg.....", ".....gg.....", "..gggggggg..", "..gggggggg..", ".....gg.....", ".....gg.....", ".....gg.....", "............", "............", "............"],
    "tower": ["............", "..w.ww.w....", "..wwwwwww...", "...wwwww....", "...wkwkw....", "...wwwww....", "...wwwww....", "...wwkww....", "..wwwwwww...", ".wwwwwwwww..", "............", "............"],
    "back": ["............", "....w.......", "...ww.......", "..wwwwwwww..", ".wwwwwwwwww.", "..wwwwwwww..", "...ww.......", "....w.......", "............", "............", "............", "............"],
    "logo": ["oooooooooooo", "oooooooooooo", "........ooo.", ".......ooo..", "......ooo...", ".....ooo....", "....ooo.....", "...ooo......", "..ooo.......", ".ooo........", "oooooooooooo", "oooooooooooo"],
}
PAL = {"w": "#f4f2f8", "s": "#c9e7f5", "g": "#7a4a1d", "b": "#6b3f1f", "k": "#2b2b2b", "y": "#ffcf3a", "r": "#e8423a", "o": "#ff7a00"}


def icon(name, scale=1, palette=None, shadow=True):
    rows = ICONS[name]
    pal = {**PAL, **(palette or {})}
    img = canvas(len(rows[0]) + 1, len(rows) + 1)
    for y, row in enumerate(rows):
        for x, ch in enumerate(row):
            if ch != ".":
                if shadow:
                    img[y + 1, x + 1] = [0, 0, 0, 140] if img[y + 1, x + 1, 3] == 0 else img[y + 1, x + 1]
                img[y, x] = hexc(pal[ch])
    return upscale(img, scale)


# ---------------------------------------------------------------------------------------------------- scenes (tile art)
def scene(seed, sky_top, sky_bottom, hill, far, ground, sun=None, w=52, h=42, night=False):
    """Original procedural landscape in chunky pixels (no external art)."""
    rnd = random.Random(seed)
    img = canvas(w, h)
    top, bot = np.array(hexc(sky_top), float), np.array(hexc(sky_bottom), float)
    for y in range(h):
        img[y, :] = top + (bot - top) * (y / h)
    if night:
        for _ in range(40):
            img[rnd.randrange(0, h // 2), rnd.randrange(0, w)] = hexc("#fff6c8")
    if sun:
        cx, cy, r = rnd.randrange(8, w - 8), rnd.randrange(6, 14), 4
        for y in range(h):
            for x in range(w):
                d = math.hypot(x - cx, y - cy)
                if d < r:
                    img[y, x] = hexc(sun)
                elif d < r + 2:
                    img[y, x, :3] = img[y, x, :3] * 0.6 + np.array(hexc(sun)[:3]) * 0.4
    phase, phase2 = rnd.random() * 6, rnd.random() * 6
    for x in range(w):
        far_h = int(h * 0.48 + math.sin(x / 5 + phase) * 4 + math.sin(x / 2.3 + phase2) * 1.5)
        img[far_h:, x] = hexc(far)
        hill_h = int(h * 0.64 + math.sin(x / 7 + phase2) * 3)
        img[hill_h:, x] = hexc(hill)
    for _ in range(9):  # trees
        tx, base = rnd.randrange(0, w), int(h * 0.62) + rnd.randrange(-2, 3)
        th = rnd.randrange(5, 10)
        for i in range(th):
            half = max(0, (i * 3) // th)
            img[max(0, base - th + i), max(0, tx - half):min(w, tx + half + 1)] = hexc(hill)
    gy = int(h * 0.8)
    img[gy:, :] = hexc(ground)
    for x in range(0, w, 3):
        img[gy, x] = np.array(hexc(ground)) * 0.8
    if night:
        for _ in range(5):
            img[rnd.randrange(gy - 6, gy), rnd.randrange(0, w)] = hexc("#ffb347")
    return img


def tile_art(name, icon_name, palette, size=(52, 42), icon_palette=None, scale=4):
    art = scene(hash(name) & 0xFFFF, *palette, w=size[0], h=size[1])
    if icon_name:
        ic = icon(icon_name, 2, icon_palette)
        blend(art, ic, (size[0] - ic.shape[1]) // 2, 3)
    save_png(PACK / f"{TEX}/{name}.png", upscale(art, scale))


PALETTES = {
    "dusk": ("#2b2350", "#f08a4b", "#203a2a", "#4b3a6b", "#2e4a2a", "#ffd27a"),
    "day": ("#5a9de6", "#b9dcf7", "#3f7a37", "#6f93b8", "#4f8a3a", "#fff3b0"),
    "night": ("#0d1030", "#3a2a6a", "#151d2c", "#2a2650", "#1e2a24", "#fff6c8"),
    "nether": ("#2a0a0a", "#7a2015", "#3a0f0f", "#5a1a12", "#4a1212", "#ff9b3a"),
    "forest": ("#3d6fa8", "#d9a35e", "#2c5a2c", "#5b7a8a", "#3d6b30", "#ffe08a"),
}


def scene_args(p):
    sky_top, sky_bottom, hill, far, ground, sun = PALETTES[p]
    return (sky_top, sky_bottom, hill, far, ground, sun)


# ---------------------------------------------------------------------------------------------------- textures
def build_textures():
    o, dark, slate = "#ff7a00", "#1c1c22", "#3f4b59"
    nineslice("window", frame(32, 32, "#101014", slate, edge="#2e3742", light="#56657a", width=2), 8)
    nineslice("panel", frame(16, 16, "#1d1d22", "#474a52", edge="#3a3c43", light="#5a5d66"), 4)
    nineslice("card", frame(16, 16, "#1d1d22", "#45474e", edge="#383a40", light="#575a62"), 4)
    nineslice("card_hover", frame(16, 16, o, "#55575f", edge="#40424a", light="#6a6d76"), 4)
    nineslice("tile_frame", frame(16, 16, "#111114", None, light=o, edge="#b85500", width=1, transparent_center=True), 3)
    nineslice("tile_frame_hover", frame(16, 16, "#111114", None, light="#ffd27a", edge=o, width=1, transparent_center=True), 3)
    nineslice("button", frame(16, 16, "#18181c", "#3c3d43", edge="#2b2c31", light="#55565d"), 4)
    nineslice("button_hover", frame(16, 16, o, "#4c4d54", edge="#36373d", light="#6a6b73"), 4)
    nineslice("button_pressed", frame(16, 16, o, "#2c2d32", light="#222327"), 4)
    nineslice("tab", frame(16, 16, "#18181c", "#2f3036", light="#47484f"), 4)
    nineslice("tab_hover", frame(16, 16, "#2f7cff", "#3a3b42", light="#5c8fe8"), 4)
    nineslice("hud_frame", frame(16, 16, o, [12, 12, 16, 215], light="#ffb066", width=1), 4)
    nineslice("hud_ribbon", frame(16, 12, "#7a3300", o, light="#ffb066", edge="#c45a00"), 3)
    nineslice("hud_border", frame(16, 16, o, None, light="#ffb066", edge="#c45a00", transparent_center=True), 3)
    nineslice("actionbar", frame(16, 16, o, [12, 12, 16, 200], light="#ffb066"), 4)

    # Text shade for tiles: transparent at the top, dark at the bottom.
    shade = canvas(1, 32)
    for y in range(32):
        shade[y] = [0, 0, 0, int(220 * (y / 31) ** 1.6)]
    save_png(PACK / f"{TEX}/shade.png", shade)
    save_png(PACK / f"{TEX}/dim.png", canvas(1, 1, [0, 0, 0, 150]))

    # Header: orange part with a slanted edge, dark part, 1 px outline. Drawn at 340 x 24 (no stretching in the form).
    w, h = 352, 24
    head = canvas(w, h, "#1f1f24")
    split = int(w * 0.6)
    for y in range(h):
        edge = split - y
        head[y, :edge] = hexc("#e2572b")
        head[y, edge:edge + 1] = hexc("#2a2a30")
    head[1, :split] = hexc("#ff8a55")
    for border in (0, h - 1):
        head[border, :] = hexc("#0e0e11")
    head[:, 0] = hexc("#0e0e11"); head[:, -1] = hexc("#0e0e11")
    save_png(PACK / f"{TEX}/header.png", head)

    save_png(PACK / f"{TEX}/logo.png", icon("logo", 2, shadow=True))

    # Main menu tiles (scenes with icons).
    for name, ic, pal in [("tile_play", "sword", "dusk"), ("tile_shop", "cart", "night"), ("tile_gods", "crown", "dusk"),
                          ("tile_teams", "people", "forest"), ("tile_stats", "chart", "night"), ("tile_settings", "gear", "nether")]:
        tile_art(name, ic, scene_args(pal))
    # Team menu tiles (scene only, text sits on top).
    for name, pal in [("team_info", "day"), ("team_members", "forest"), ("team_alliances", "day"), ("team_chunks", "night"),
                      ("team_leave", "dusk"), ("team_settings", "forest"), ("team_invite", "dusk"), ("team_raids", "nether"), ("team_back", "night")]:
        tile_art(name, None, scene_args(pal), size=(52, 30))

    # God sigils: diamond emblems in distinct colours.
    gods = {"anarmir": ("#7b3fd1", "#d9a6ff"), "infernir": ("#c53a1d", "#ffb36b"), "tossenkim": ("#1f5fb8", "#9ad1ff"), "padandir": ("#8a1f2a", "#ff8a8a"),
            "stelhym": ("#2e8a3d", "#a6f0a0"), "conssesir": ("#4a4a55", "#e8e8f0"), "oxal": ("#b8861f", "#ffe08a"), "kiwiki": ("#1f8a8a", "#9af0e8")}
    for name, (base, light) in gods.items():
        s = canvas(12, 12)
        for y in range(12):
            for x in range(12):
                d = abs(x - 5.5) + abs(y - 5.5)
                if d <= 5.5:
                    s[y, x] = hexc(base if d > 2.5 else light)
                if 5 <= d <= 5.5:
                    s[y, x] = hexc("#101014")
        s[5:7, 5:7] = hexc("#ffffff")
        save_png(PACK / f"{TEX}/god_{name}.png", upscale(s, 4))

    # God banner (left column of the detail view).
    bw, bh = 12, 30
    banner = canvas(bw, bh)
    rnd = random.Random(7)
    for y in range(bh):
        for x in range(bw):
            tip = y > bh - 7 and abs(x - 5.5) > (bh - 1 - y) * 0.9
            if not tip:
                v = rnd.random()
                banner[y, x] = hexc("#4b2a8a" if v < 0.6 else "#5d36a8" if v < 0.85 else "#2a1950")
        if y < bh - 6:
            banner[y, 0] = banner[y, -1] = hexc("#c77dff")
    for y in range(2, bh - 8, 6):
        for x in range(bw):
            if abs(x - 5.5) < 3 - abs((y % 6) - 2):
                banner[y, x] = hexc("#e7b6ff")
    save_png(PACK / f"{TEX}/god_banner.png", upscale(banner, 4))

    # Shop: block textures (16 x 16 noise) and tab icons.
    blocks = {"netherrack": ("#6e2a24", "#8a3a33", "#4f1c18"), "basalt": ("#56565c", "#6c6c74", "#3e3e44"), "soul_sand": ("#5a4433", "#6e5440", "#463426"),
              "soul_soil": ("#4b3b2e", "#5c4a3a", "#3a2d23"), "obsidian": ("#1a1328", "#2a1f40", "#0f0b18"), "crying_obsidian": ("#1a1328", "#7a2fd1", "#0f0b18"),
              "bricks": ("#9a4a3a", "#b35a47", "#7a3a2e"), "stone": ("#7a7a80", "#8e8e95", "#64646a"), "portal": ("#4a1f8a", "#8a4fe0", "#2c1258")}
    for name, (a, b, c) in blocks.items():
        rnd = random.Random(name)
        t = canvas(16, 16)
        for y in range(16):
            for x in range(16):
                v = rnd.random()
                t[y, x] = hexc(a if v < 0.55 else b if v < 0.8 else c)
        if name == "bricks":
            for y in range(0, 16, 4):
                t[y, :] = hexc("#c9c1b5")
                off = 0 if (y // 4) % 2 == 0 else 4
                for x in range(off, 16, 8):
                    t[y:y + 4, x] = hexc("#c9c1b5")
        save_png(PACK / f"{TEX}/block_{name}.png", upscale(t, 4))

    # Pack icon.
    pi = canvas(32, 32, "#1f1f24")
    rect(pi, 0, 0, 32, 3, "#e2572b")
    blend(pi, icon("logo", 2), 3, 5)
    save_png(PACK / "pack_icon.png", upscale(pi, 4))


# ---------------------------------------------------------------------------------------------------- JSON UI
NS = "zernyth_showcase"


def visible_if_title_has(code: str):
    """Bindings that show a control only when the form title contains `code`."""
    return [
        {"binding_name": "#title_text"},
        {"binding_type": "view", "source_property_name": f"(not ((#title_text - '{code}') = #title_text))", "target_property_name": "#visible"},
    ]


def collection(name):
    return {"binding_name": name, "binding_type": "collection", "binding_collection_name": "form_buttons"}


def button_visible(expr):
    return [collection("#form_button_text"), {"binding_type": "view", "source_property_name": expr, "target_property_name": "#visible"}]


NOT_EMPTY = "(not (#form_button_text = ''))"
NO_MARKERS = f"((not (#form_button_text = '')) and ((#form_button_text - '{INFO}') = #form_button_text) and ((#form_button_text - '{TAB}') = #form_button_text))"
IS_INFO = f"(not ((#form_button_text - '{INFO}') = #form_button_text))"
IS_TAB = f"(not ((#form_button_text - '{TAB}') = #form_button_text))"


def label(text, **kw):
    base = {"type": "label", "text": text, "color": [1, 1, 1], "shadow": True, "localize": False}
    base.update(kw)
    return base


# Texture variable of each face per button state (default / hover / pressed).
FACE_STATES = {
    "tile_face": ("$frame", ("tile_frame", "tile_frame_hover", "tile_frame_hover")),
    "team_face": ("$frame", ("tile_frame", "tile_frame_hover", "tile_frame_hover")),
    "god_card_face": ("$card", ("card", "card_hover", "card_hover")),
    "shop_face": ("$card", ("card", "card_hover", "card_hover")),
    "choice_face": ("$bg", ("button", "button_hover", "button_pressed")),
    "tab_face": ("$bg", ("tab", "tab_hover", "tab_hover")),
    "info_face": ("$bg", ("panel", "panel", "panel")),
}


def form_button(face, size, extra_bindings=None, visible=NOT_EMPTY):
    """A form button (sends button.form_button_click with its collection index) drawn with `face` in three states."""
    return {
        "type": "panel",
        "size": size,
        "bindings": button_visible(visible),
        "controls": [{
            "button@common.button": {
                "size": ["100% - 4px", "100% - 4px"],
                "$pressed_button_name": "button.form_button_click",
                "bindings": [{"binding_type": "collection_details", "binding_collection_name": "form_buttons"}] + (extra_bindings or []),
                "controls": [
                    {f"{state}@{NS}.{face}": {FACE_STATES[face][0]: f"{TEX}/{FACE_STATES[face][1][i]}"}}
                    for i, state in enumerate(("default", "hover", "pressed"))
                ],
            }
        }],
    }


def button_image(**kw):
    img = {"type": "image", "bindings": [collection("#form_button_texture") | {"binding_name_override": "#texture"},
                                         collection("#form_button_texture_file_system") | {"binding_name_override": "#texture_file_system"}]}
    img.update(kw)
    return img


def button_text(**kw):
    return label("#form_button_text", bindings=[collection("#form_button_text")], **kw)


def stack_factory(item, orientation="vertical", size=None):
    return {
        "type": "stack_panel", "orientation": orientation, "size": size or ["100%", "100%c"],
        "factory": {"name": "buttons", "control_ids": {"button": f"{NS}.{item}"}},
        "collection_name": "form_buttons",
        "bindings": [{"binding_name": "#form_button_contents", "binding_name_override": "#collection_length"}],
    }


def grid(item, cols, rows, cell):
    return {
        "type": "grid", "size": [cols * cell[0], rows * cell[1]], "grid_dimensions": [cols, rows],
        "grid_item_template": f"{NS}.{item}", "collection_name": "form_buttons",
        "anchor_from": "top_middle", "anchor_to": "top_middle",
        "bindings": [{"binding_name": "#form_button_contents", "binding_name_override": "#maximum_grid_items"}],
    }


def build_ui():
    tex = lambda n: f"{TEX}/{n}"
    ui = {"namespace": NS}

    # ---- window shared by every screen
    ui["window"] = {
        "type": "panel", "size": [340, 226], "anchor_from": "center", "anchor_to": "center", "layer": 20,
        "$body|default": f"{NS}.empty",
        "controls": [
            {"background": {"type": "image", "texture": tex("window"), "size": ["100% - 8px", "100% - 18px"], "offset": [0, 18], "anchor_from": "top_middle", "anchor_to": "top_middle"}},
            {"header": {"type": "image", "texture": tex("header"), "size": [352, 24], "anchor_from": "top_middle", "anchor_to": "top_middle", "layer": 2, "controls": [
                {"logo": {"type": "image", "texture": tex("logo"), "size": [16, 16], "offset": [8, 4], "anchor_from": "top_left", "anchor_to": "top_left"}},
                {"brand": label("ZERNYTH.NET", font_type="MinecraftTen", offset=[28, 8], anchor_from="top_left", anchor_to="top_left")},
                {"title": label("#title_text", font_type="MinecraftTen", offset=[-30, 8], anchor_from="top_right", anchor_to="top_right", text_alignment="right",
                                bindings=[{"binding_name": "#title_text"}])},
            ]}},
            {"close@common.close_button": {"anchor_from": "top_right", "anchor_to": "top_right", "$close_button_offset": [-4, 2], "layer": 30}},
            {"body@$body": {"size": ["100% - 20px", "100% - 38px"], "offset": [0, 30], "anchor_from": "top_middle", "anchor_to": "top_middle", "layer": 3}},
        ],
    }
    ui["empty"] = {"type": "panel"}
    ui["dim"] = {"type": "image", "texture": tex("dim"), "size": ["100%", "100%"], "layer": 1, "bindings": visible_if_title_has(PREFIX)}

    # ---- tile faces (main menu, teams)
    def tile_face(name, text_anchor, text_offset, align):
        return {
            "type": "panel", "size": ["100%", "100%"],
            "controls": [
                {"art": button_image(size=["100%", "100%"], layer=1)},
                {"shade": {"type": "image", "texture": tex("shade"), "size": ["100%", "70%"], "anchor_from": "bottom_middle", "anchor_to": "bottom_middle", "layer": 2}},
                {"text": button_text(size=["100% - 8px", "default"], anchor_from=text_anchor, anchor_to=text_anchor, offset=text_offset, text_alignment=align, layer=4)},
                {"frame": {"type": "image", "texture": "$frame", "size": ["100%", "100%"], "layer": 5}},
            ],
        }
    ui["tile_face"] = tile_face("tile_face", "bottom_middle", [0, -5], "center")
    ui["team_face"] = tile_face("team_face", "top_left", [5, 5], "left")
    for face in ("tile_face", "team_face"):
        ui[face]["$frame|default"] = tex("tile_frame")

    ui["main_item"] = form_button("tile_face", [104, 84])
    ui["main_body"] = {"type": "panel", "controls": [{"grid": grid("main_item", 3, 2, (104, 84)) | {"offset": [0, 4]}}]}

    ui["team_item"] = form_button("team_face", [104, 58])
    ui["teams_body"] = {"type": "panel", "controls": [{"grid": grid("team_item", 3, 3, (104, 58))}]}

    # ---- cards (god list, shop)
    def card_face(icon_size, icon_offset, text_offset, text_anchor="top_left", icon_anchor="left_middle", align="left"):
        return {
            "type": "image", "texture": "$card", "size": ["100%", "100%"], "$card|default": tex("card"),
            "controls": [
                {"icon": button_image(size=icon_size, offset=icon_offset, anchor_from=icon_anchor, anchor_to=icon_anchor, layer=2)},
                {"text": button_text(size=["100% - 46px" if align == "left" else "100% - 6px", "default"], offset=text_offset, anchor_from=text_anchor, anchor_to=text_anchor,
                                     text_alignment=align, layer=3)},
            ],
        }
    ui["god_card_face"] = card_face([24, 24], [5, 0], [33, 6])
    ui["god_item"] = form_button("god_card_face", [106, 50])
    ui["gods_grid"] = grid("god_item", 2, 8, (106, 50))
    ui["gods_body"] = {"type": "stack_panel", "orientation": "horizontal", "controls": [
        {"scores": {"type": "image", "texture": tex("panel"), "size": [96, "100%"], "controls": [
            {"text": label("#form_text", size=["100% - 10px", "default"], offset=[5, 5], anchor_from="top_left", anchor_to="top_left", bindings=[{"binding_name": "#form_text"}])}]}},
        {"gap": {"type": "panel", "size": [4, "100%"]}},
        {"list@common.scrolling_panel": {"size": ["fill", "100%"], "$show_background": False, "$scrolling_content": f"{NS}.gods_grid",
                                          "$scroll_size": [5, "100% - 4px"], "$scrolling_pane_size": ["100% - 4px", "100%"]}},
    ]}

    # ---- god detail: banner | description + Yes/No | information cards (buttons marked with INFO)
    ui["choice_face"] = {
        "type": "image", "texture": "$bg", "size": ["100%", "100%"], "$bg|default": tex("button"),
        "controls": [{"text": button_text(anchor_from="center", anchor_to="center", text_alignment="center", layer=2)}],
    }
    ui["choice_item"] = form_button("choice_face", [92, 26], visible=NO_MARKERS)
    ui["info_face"] = {"type": "image", "texture": "$bg", "size": ["100%", "100%"], "$bg|default": tex("panel"),
                       "controls": [{"text": button_text(size=["100% - 10px", "default"], offset=[5, 5], anchor_from="top_left", anchor_to="top_left", layer=2)}]}
    ui["info_item"] = form_button("info_face", ["100%", 84], visible=IS_INFO)
    ui["god_body"] = {"type": "stack_panel", "orientation": "horizontal", "controls": [
        {"banner_panel": {"type": "image", "texture": tex("panel"), "size": [62, "100%"], "controls": [
            {"banner": {"type": "image", "texture": tex("god_banner"), "size": [48, 120], "anchor_from": "center", "anchor_to": "center"}}]}},
        {"gap1": {"type": "panel", "size": [4, "100%"]}},
        {"center": {"type": "image", "texture": tex("panel"), "size": ["fill", "100%"], "controls": [
            {"text": label("#form_text", size=["100% - 12px", "default"], offset=[0, 8], anchor_from="top_middle", anchor_to="top_middle", text_alignment="center",
                           bindings=[{"binding_name": "#form_text"}])},
            {"choices": grid("choice_item", 2, 1, (92, 26)) | {"anchor_from": "bottom_middle", "anchor_to": "bottom_middle", "offset": [0, -8]}},
        ]}},
        {"gap2": {"type": "panel", "size": [4, "100%"]}},
        {"info": stack_factory("info_item", size=[96, "100%"])},
    ]}

    # ---- stats: scrolling text and the buttons (e.g. Back) at the bottom
    ui["stats_text"] = {"type": "panel", "size": ["100%", "100%c + 8px"], "controls": [
        {"text": label("#form_text", size=["100% - 12px", "default"], offset=[6, 6], anchor_from="top_left", anchor_to="top_left", bindings=[{"binding_name": "#form_text"}])}]}
    ui["stats_button"] = form_button("choice_face", [160, 26])
    ui["stats_body"] = {"type": "stack_panel", "orientation": "vertical", "controls": [
        {"scroll@common.scrolling_panel": {"size": ["100%", "fill"], "$show_background": False, "$scrolling_content": f"{NS}.stats_text",
                                            "$scroll_size": [5, "100% - 4px"], "$scrolling_pane_size": ["100% - 4px", "100%"]}},
        {"buttons": stack_factory("stats_button", orientation="horizontal", size=["100%c", 28]) | {"anchor_from": "bottom_middle", "anchor_to": "bottom_middle"}},
    ]}

    # ---- shop: category tabs (buttons marked with TAB) on the left, item cards on the right
    ui["tab_face"] = {
        "type": "image", "texture": "$bg", "size": ["100%", "100%"], "$bg|default": tex("tab"),
        "controls": [{"icon": button_image(size=[24, 24], anchor_from="center", anchor_to="center", layer=2)}],
    }
    ui["tab_item"] = form_button("tab_face", [36, 36], visible=IS_TAB)
    ui["shop_face"] = card_face([32, 32], [0, 5], [0, 40], text_anchor="top_middle", icon_anchor="top_middle", align="center")
    ui["shop_item"] = form_button("shop_face", [90, 84], visible=NO_MARKERS)
    ui["shop_grid"] = grid("shop_item", 3, 6, (90, 84))
    ui["shop_body"] = {"type": "stack_panel", "orientation": "horizontal", "controls": [
        {"tabs": stack_factory("tab_item", size=[36, "100%"])},
        {"gap": {"type": "panel", "size": [4, "100%"]}},
        {"items@common.scrolling_panel": {"size": ["fill", "100%"], "$show_background": False, "$scrolling_content": f"{NS}.shop_grid",
                                           "$scroll_size": [5, "100% - 4px"], "$scrolling_pane_size": ["100% - 4px", "100%"]}},
    ]}

    # ---- router: vanilla form unless the title carries a Zernyth marker
    screens = [("main", "main_body"), ("teams", "teams_body"), ("gods", "gods_body"), ("god", "god_body"), ("stats", "stats_body"), ("shop", "shop_body")]
    # Markers are written into each expression literally: variables inside binding expressions are not used in the
    # vanilla files, and an unresolved expression hid every form (first client test, 1.26.63).
    full = {name: PREFIX + MARKERS[name] for name, _ in screens}
    no_marker = " and ".join(f"((#title_text - '{m}') = #title_text)" for m in full.values())
    ui["router"] = {"type": "panel", "size": ["100%", "100%"], "controls": [
        {"vanilla@server_form.long_form": {"bindings": [{"binding_name": "#title_text"},
                                                        {"binding_type": "view", "source_property_name": f"({no_marker})", "target_property_name": "#visible"}]}},
        {"dim@" + NS + ".dim": {"bindings": [{"binding_name": "#title_text"},
                                             {"binding_type": "view", "source_property_name": f"(not ({no_marker}))", "target_property_name": "#visible"}]}},
    ] + [{f"screen_{name}@{NS}.window": {"$body": f"{NS}.{body}", "bindings": visible_if_title_has(full[name])}} for name, body in screens]}

    # ---- HUD
    # HUD v3: minimal changes. v1 replaced the whole sidebar (children sized from the parent while the parent was sized
    # from its children) and blocked joining with client 1.26.63. Now the vanilla layout stays; "modifications" only
    # add decoration sized like vanilla's own objective background ("100%sm": the largest sibling).
    scoreboards = {"namespace": "scoreboard", "scoreboard_sidebar": {"modifications": [
        {"array_name": "controls", "operation": "insert_front", "value": [
            {"zernyth_ribbon": {"type": "image", "texture": tex("hud_ribbon"), "size": ["100%sm + 4px", 11], "offset": [0, -1],
                                "anchor_from": "top_middle", "anchor_to": "top_middle", "layer": 1}}]},
        {"array_name": "controls", "operation": "insert_back", "value": [
            {"zernyth_border": {"type": "image", "texture": tex("hud_border"), "size": ["100%sm + 4px", "100%sm + 2px"], "offset": [0, -1],
                                "anchor_from": "top_middle", "anchor_to": "top_middle", "layer": 5}}]},
    ]}}
    # Actionbar: only the background texture changes (one property; the vanilla element and its animation stay).
    hud = {"namespace": "hud", "hud_actionbar_text": {"texture": tex("actionbar")}}
    server_form = {"namespace": "server_form", "main_screen_content": {
        "type": "panel", "size": [0, 0],
        "controls": [{"server_form_factory": {"type": "factory", "control_ids": {"long_form": f"@{NS}.router", "custom_form": "@server_form.custom_form"}}}]}}
    return ui, server_form, scoreboards, hud


def write_json(path: Path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, indent=2, ensure_ascii=False) + "\n", encoding="utf-8")


def main():
    if PACK.exists():
        shutil.rmtree(PACK)
    PACK.mkdir(parents=True)
    build_textures()
    ui, server_form, scoreboards, hud = build_ui()
    write_json(PACK / "ui/zernyth_showcase.json", ui)
    write_json(PACK / "ui/server_form.json", server_form)
    # The HUD files blocked joining with client 1.26.63 (InitialConnection-149, first client test). They are kept out of
    # the pack and only go into the separately labelled "with_hud" variant until the cause is known.
    if HUD_DIR.exists():
        shutil.rmtree(HUD_DIR)
    write_json(HUD_DIR / "sidebar/ui/scoreboards.json", scoreboards)
    write_json(HUD_DIR / "actionbar/ui/hud_screen.json", hud)
    write_json(PACK / "ui/_ui_defs.json", {"ui_defs": ["ui/zernyth_showcase.json"]})
    write_json(PACK / "manifest.json", {
        "format_version": 2,
        "header": {"name": "Zernyth UI Showcase", "description": "Experimental JSON UI for Zernyth server forms (Bedrock 1.26.60)",
                   "uuid": HEADER_UUID, "version": VERSION, "min_engine_version": [1, 21, 0]},
        "modules": [{"type": "resources", "uuid": MODULE_UUID, "version": VERSION, "description": "UI and textures"}],
    })
    write_json(ROOT / "markers.json", {"prefix": PREFIX, "screens": {k: marker(k) for k in MARKERS}, "info": INFO, "tab": TAB, "texturePath": TEX})
    out = ROOT / "build" / "zernyth_ui_showcase.mcpack"
    out.parent.mkdir(parents=True, exist_ok=True)
    with zipfile.ZipFile(out, "w", zipfile.ZIP_DEFLATED) as z:
        for f in sorted(PACK.rglob("*")):
            if f.is_file():
                z.write(f, f.relative_to(PACK).as_posix())
    print("pack:", PACK, "mcpack:", out, "files:", sum(1 for f in PACK.rglob("*") if f.is_file()))
    build_variants()


# Variants for narrowing down client problems. Each has its own UUIDs so the client never reuses a cached copy.
VARIANTS = {
    "textures_only": ("Zernyth UI Showcase (textures only)", lambda rel: not rel.startswith("ui/"), []),
    "forms_v2": ("Zernyth UI Showcase (forms v2)", lambda rel: True, []),
    "hud_actionbar_v3": ("Zernyth UI Showcase (forms v2 + actionbar v3)", lambda rel: True, ["actionbar"]),
    "hud_sidebar_v3": ("Zernyth UI Showcase (forms v2 + sidebar v3)", lambda rel: True, ["sidebar"]),
    "complete_v3": ("Zernyth UI Showcase (forms, actionbar, sidebar)", lambda rel: True, ["actionbar", "sidebar"]),
}


def build_variants():
    import uuid
    base = ROOT / "build" / "variants"
    if base.exists():
        shutil.rmtree(base)
    for name, (title, keep, hud_parts) in VARIANTS.items():
        folder = base / f"zernyth_ui_{name}"
        sources = [(PACK, f) for f in sorted(PACK.rglob("*"))]
        for part in hud_parts:
            sources += [(HUD_DIR / part, f) for f in sorted((HUD_DIR / part).rglob("*"))]
        for src, f in sources:
            rel = f.relative_to(src).as_posix()
            if f.is_file() and rel != "manifest.json" and keep(rel):
                (folder / rel).parent.mkdir(parents=True, exist_ok=True)
                shutil.copyfile(f, folder / rel)
        header = str(uuid.uuid5(uuid.UUID(HEADER_UUID), f"{name}-header"))
        module = str(uuid.uuid5(uuid.UUID(HEADER_UUID), f"{name}-module"))
        write_json(folder / "manifest.json", {
            "format_version": 2,
            "header": {"name": title, "description": "Zernyth UI showcase test variant", "uuid": header, "version": VERSION, "min_engine_version": [1, 21, 0]},
            "modules": [{"type": "resources", "uuid": module, "version": VERSION, "description": title}],
        })
        with zipfile.ZipFile(base / f"zernyth_ui_{name}.mcpack", "w", zipfile.ZIP_DEFLATED) as z:
            for f in sorted(folder.rglob("*")):
                if f.is_file():
                    z.write(f, f.relative_to(folder).as_posix())
        print(f"variant {name}: pack_id {header}")


if __name__ == "__main__":
    main()
