# endstone-devtools

A development plugin that inspects the server's registries in a [Dear ImGui](https://github.com/ocornut/imgui)
window. It is the Python successor to the old built-in C++ DevTools.

The window renders in a **separate process** so the heavy GUI stack never loads
into the server, and the GUI can never block the server thread. The server side
takes a one-shot, picklable snapshot of every registry it can read and hands it
to that process.

This is a local-development tool: the window opens on the **server host**, so
`/devtools` only works from the console.

## Usage

From the server console:

```
/devtools        # or: /dev
```

Use **File → Save snapshot.json** in the window to dump the current snapshot to
`devtools_snapshot.json` in the server's working directory.

## What it shows

Whatever the public registry API exposes today — `blocks`, `items`,
`enchantments`, and `actors`. Sections grow as the registry API does. Data that
has no registry yet (block states, recipes, biomes, the block palette, item
components) is intentionally not shown.

## Layout

```
src/endstone_devtools/
    __init__.py     # intentionally empty (import isolation for the GUI process)
    plugin.py       # the Endstone plugin: /devtools -> collect + spawn
    snapshot.py     # collect(server) -> picklable dict
    gui.py          # the GUI process entry point (imgui-bundle)
```

`gui.py` can be run standalone against a saved snapshot, no server required:

```
python -m endstone_devtools.gui devtools_snapshot.json
```

## Development install

Build a wheel and drop it in the server's `plugins/` folder, or install it into
the server's Python environment:

```
pip install ./plugins/devtools
```

The `imgui-bundle` dependency is installed automatically alongside the plugin.
