"""DevTools GUI process.

Runs in a separate, clean Python interpreter. It receives one plain ``dict``
snapshot and renders it as a tree. ``imgui_bundle`` is imported lazily inside
``main`` so the server process — which references ``main`` only to spawn this one
— never loads the GUI stack.

This module must not import anything from ``endstone``.

For developing the GUI on its own, run it against a saved snapshot:

    python -m endstone_devtools.gui path/to/snapshot.json
"""

from __future__ import annotations

import json
from pathlib import Path


class _State:
    def __init__(self, snapshot: dict) -> None:
        self.snapshot = snapshot
        self.filter = ""
        self.status = ""


def main(snapshot: dict) -> None:
    from imgui_bundle import imgui, immapp

    state = _State(snapshot)

    def frame() -> None:
        _draw_menu_bar(imgui, state)
        imgui.begin("Registries")
        changed, state.filter = imgui.input_text("filter by id", state.filter)
        if state.status:
            imgui.same_line()
            imgui.text_disabled(state.status)
        imgui.separator()
        for section, rows in state.snapshot.items():
            _draw_section(imgui, section, rows, state.filter)
        imgui.end()

    immapp.run(gui_function=frame, window_title="Endstone - DevTools", window_size=(960, 600))


def _draw_menu_bar(imgui, state: _State) -> None:
    if not imgui.begin_main_menu_bar():
        return
    if imgui.begin_menu("File"):
        clicked, _ = imgui.menu_item("Save snapshot.json", "", False)
        if clicked:
            _save(state)
        imgui.end_menu()
    imgui.end_main_menu_bar()


def _draw_section(imgui, name: str, rows, needle: str) -> None:
    if isinstance(rows, list):
        visible = [r for r in rows if _matches(r, needle)]
        if not imgui.tree_node(f"{name} ({len(visible)})"):
            return
        for i, row in enumerate(visible):
            imgui.push_id(i)
            _draw_node(imgui, _label(row, i), row)
            imgui.pop_id()
        imgui.tree_pop()
    else:
        _draw_node(imgui, name, rows)


def _draw_node(imgui, label: str, value) -> None:
    if isinstance(value, dict):
        if imgui.tree_node(label):
            for key, child in value.items():
                _draw_node(imgui, str(key), child)
            imgui.tree_pop()
    elif isinstance(value, list):
        if imgui.tree_node(f"{label} ({len(value)})"):
            for i, item in enumerate(value):
                imgui.push_id(i)
                _draw_node(imgui, _label(item, i), item)
                imgui.pop_id()
            imgui.tree_pop()
    else:
        imgui.bullet_text(f"{label}: {value}")


def _matches(row, needle: str) -> bool:
    if not needle:
        return True
    return needle.lower() in str(row.get("id", "")).lower() if isinstance(row, dict) else True


def _label(value, index: int) -> str:
    if isinstance(value, dict):
        return str(value.get("id") or value.get("name") or f"[{index}]")
    return f"[{index}]"


def _save(state: _State) -> None:
    path = Path.cwd() / "devtools_snapshot.json"
    with path.open("w", encoding="utf-8") as f:
        json.dump(state.snapshot, f, indent=2)
    state.status = f"saved {path}"


if __name__ == "__main__":
    import sys

    with open(sys.argv[1], encoding="utf-8") as f:
        main(json.load(f))
