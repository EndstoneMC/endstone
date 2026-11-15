"""Static checks for the showcase pack. Usage: python validate.py <folder with vanilla ui/*.json of the target version> [pack folder]

Checks: strict JSON, manifest, every texture path, nine-slice sidecars, every element reference (name@namespace.element,
factory ids, grid templates, $body/$scrolling_content) against the pack and the vanilla files, the markers used by the
test plugin. It does not and cannot check how the client renders the UI.
"""
import json
import re
import struct
import sys
import uuid
from pathlib import Path

ROOT = Path(__file__).resolve().parent
PACK = ROOT / "pack"
PLUGIN = ROOT.parent / "src/example/java/dev/zernyth/ui/example/UiShowcasePlugin.java"
problems: list[str] = []


def strip_comments(text: str) -> str:
    """Vanilla UI files contain // and /* */ comments outside strings."""
    out, i, in_str = [], 0, False
    while i < len(text):
        c = text[i]
        if in_str:
            out.append(c)
            if c == "\\":
                out.append(text[i + 1]); i += 1
            elif c == '"':
                in_str = False
        elif c == '"':
            in_str = True; out.append(c)
        elif text.startswith("//", i):
            while i < len(text) and text[i] != "\n":
                i += 1
            continue
        elif text.startswith("/*", i):
            i = text.index("*/", i) + 2
            continue
        else:
            out.append(c)
        i += 1
    return "".join(out)


def elements(doc: dict) -> set[str]:
    ns = doc.get("namespace", "")
    names = set()
    def collect(obj):
        if isinstance(obj, dict):
            for k, v in obj.items():
                if k == "namespace":
                    continue
                collect(v)
        elif isinstance(obj, list):
            for v in obj:
                collect(v)
    for key in doc:
        if key != "namespace":
            names.add(f"{ns}.{key.split('@')[0]}")
    collect(doc)
    return names


def png_ok(path: Path) -> bool:
    data = path.read_bytes()
    return data[:8] == b"\x89PNG\r\n\x1a\n" and struct.unpack(">II", data[16:24])[0] > 0


def main():
    if len(sys.argv) < 2:
        print(__doc__); sys.exit(2)
    global PACK
    vanilla_dir = Path(sys.argv[1])
    if len(sys.argv) > 2:
        PACK = Path(sys.argv[2])
    known: set[str] = set()
    for f in vanilla_dir.glob("*.json"):
        if f.name.startswith("_"):
            continue
        known |= elements(json.loads(strip_comments(f.read_text(encoding="utf-8"))))

    docs = {}
    for f in sorted((PACK / "ui").glob("*.json")):
        try:
            docs[f.name] = json.loads(f.read_text(encoding="utf-8"))
        except json.JSONDecodeError as e:
            problems.append(f"{f.name}: invalid JSON ({e})")
    for name, doc in docs.items():
        if "namespace" in doc:
            known |= elements(doc)

    m = json.loads((PACK / "manifest.json").read_text(encoding="utf-8"))
    ids = [m["header"]["uuid"], m["modules"][0]["uuid"]]
    for u in ids:
        uuid.UUID(u)
    if len(set(ids)) != 2 or m["format_version"] != 2 or m["modules"][0]["type"] != "resources":
        problems.append("manifest: needs format 2, a resources module and two different UUIDs")
    if not png_ok(PACK / "pack_icon.png"):
        problems.append("pack_icon.png is not a PNG")
    defs = json.loads((PACK / "ui/_ui_defs.json").read_text(encoding="utf-8"))["ui_defs"]
    for d in defs:
        if not (PACK / d).exists():
            problems.append(f"_ui_defs lists missing file {d}")

    refs = 0
    def check_ref(target: str, where: str):
        nonlocal refs
        refs += 1
        target = target.lstrip("@")
        if target.startswith("$"):
            return
        if target not in known:
            problems.append(f"{where}: unknown element {target}")

    def walk(obj, where):
        if isinstance(obj, dict):
            for k, v in obj.items():
                if "@" in k and not k.startswith("$"):
                    check_ref(k.split("@", 1)[1], f"{where}/{k}")
                if k in ("grid_item_template", "control_name") and isinstance(v, str):
                    check_ref(v, f"{where}/{k}")
                if k == "control_ids" and isinstance(v, dict):
                    for cid in v.values():
                        check_ref(cid.split("@", 1)[-1] if "@" in cid.lstrip("@") else cid, f"{where}/control_ids")
                if k in ("$body", "$scrolling_content") and isinstance(v, str):
                    check_ref(v, f"{where}/{k}")
                if k == "texture" and isinstance(v, str) and v.startswith("textures/zernyth_ui/"):
                    if not (PACK / f"{v}.png").exists():
                        problems.append(f"{where}: missing texture {v}")
                if isinstance(v, str) and v.startswith("textures/zernyth_ui/") and k.startswith("$"):
                    if not (PACK / f"{v}.png").exists():
                        problems.append(f"{where}/{k}: missing texture {v}")
                walk(v, f"{where}/{k}")
        elif isinstance(obj, list):
            for i, v in enumerate(obj):
                walk(v, f"{where}[{i}]")
    for name, doc in docs.items():
        walk(doc, name)

    for side in (PACK / "textures/zernyth_ui/showcase").glob("*.json"):
        meta = json.loads(side.read_text(encoding="utf-8"))
        if not (side.with_suffix(".png")).exists() or not isinstance(meta.get("nineslice_size"), int):
            problems.append(f"{side.name}: invalid nine-slice sidecar")
    pngs = list(PACK.rglob("*.png"))
    problems.extend(f"{p.name}: not a PNG" for p in pngs if not png_ok(p))

    markers = json.loads((ROOT / "markers.json").read_text(encoding="utf-8"))
    if PLUGIN.exists():
        java = PLUGIN.read_text(encoding="utf-8")
        for key, code in markers["screens"].items():
            literal = code.replace("§", "\\u00a7")
            if literal not in java and code not in java:
                problems.append(f"plugin does not use the marker of screen {key}")
        for path in set(re.findall(r'"(textures/zernyth_ui/showcase/[a-z0-9_]+)"', java)) | {
                f"{markers['texturePath']}/{t}" for t in re.findall(r'tex\("([a-z0-9_]+)"\)', java)}:
            if not (PACK / f"{path}.png").exists():
                problems.append(f"plugin uses missing texture {path}")
    else:
        problems.append(f"test plugin not found at {PLUGIN}")

    print(f"{len(docs)} UI files, {refs} element references, {len(pngs)} PNG files checked")
    if problems:
        print("PROBLEMS:"); print("\n".join(f" - {p}" for p in problems)); sys.exit(1)
    print("OK")


if __name__ == "__main__":
    main()
