<div align="center">
  <a href="https://github.com/EndstoneMC/endstone/releases">
    <img src="https://static.wikia.nocookie.net/minecraft_gamepedia/images/4/43/End_Stone_JE3_BE2.png" alt="Logo" width="80" height="80">
  </a>

<h3>Endstone</h3>

<p>
  <b>High-performance Minecraft Bedrock server software</b><br>
  Extensible with Python and C++ plugins
</p>

[![CI](https://github.com/EndstoneMC/endstone/actions/workflows/ci.yml/badge.svg)](https://github.com/EndstoneMC/endstone/actions/workflows/ci.yml)
[![Minecraft](https://img.shields.io/badge/minecraft-v26.0_(Bedrock)-black)](https://feedback.minecraft.net/hc/en-us/sections/360001186971-Release-Changelogs)
[![PyPI - Version](https://img.shields.io/pypi/v/endstone)](https://pypi.org/project/endstone)
[![Python](https://img.shields.io/pypi/pyversions/endstone?logo=python&logoColor=white)](https://www.python.org/)
[![GitHub License](https://img.shields.io/github/license/endstonemc/endstone)](LICENSE)
[![Discord](https://img.shields.io/discord/1230982180742631457?logo=discord&logoColor=white&color=5865F2)](https://discord.gg/xxgPuc2XN9)

</div>

## Why Endstone?

Bedrock's official addon and script APIs let you add content, but can hardly modify core gameplay. Custom servers like
PocketMine and Nukkit offer that control, but sacrifice vanilla features. Endstone gives you both: cancellable events,
packet control, and deep gameplay access with full vanilla compatibility. Think of it as Paper for Bedrock. If you've
ever wished Bedrock servers had the same modding power as Java Edition, this is it.

## Quick Start

Get your server running in seconds:

```shell
pip install endstone
endstone
```

Then create your first plugin:

```python
from endstone.plugin import Plugin
from endstone.event import event_handler, PlayerJoinEvent


class MyPlugin(Plugin):
    api_version = "0.10"

    def on_enable(self):
        self.logger.info("MyPlugin enabled!")
        self.register_events(self)

    @event_handler
    def on_player_join(self, event: PlayerJoinEvent):
        event.player.send_message(f"Welcome, {event.player.name}!")
```

**Get started faster with our templates:**
[Python](https://github.com/EndstoneMC/python-plugin-template) | [C++](https://github.com/EndstoneMC/cpp-plugin-template)

## Features

- **Cross-platform** - Runs natively on both Windows and Linux without emulation, making deployment flexible and
  straightforward.

- **Always up-to-date** - Designed to stay compatible with the latest Minecraft Bedrock releases so you're never left
  behind.

- **Python & C++ plugins** - Write plugins in Python for rapid development, or use C++ when you need maximum
  performance. The choice is yours.

- **Powerful API** - A comprehensive API with 60+ events covering players, blocks, actors, and more. Includes commands,
  forms, scoreboards, inventories, and a full permission system.

- **Drop-in replacement** - Works with your existing Bedrock worlds and configurations. Just install and run.

- **Familiar to Bukkit developers** - If you've developed plugins for Java Edition servers, you'll feel right at home
  with Endstone's API design.

## Installation

Requires Python 3.10+ on Windows 10+ or Linux (Ubuntu 22.04+, Debian 12+).

### Using pip (recommended)

```shell
pip install endstone
endstone
```

### Using Docker

```shell
docker pull endstone/endstone
docker run --rm -it -p 19132:19132/udp endstone/endstone
```

### Building from source

```shell
git clone https://github.com/EndstoneMC/endstone.git
cd endstone
pip install .
endstone
```

For detailed installation guides, system requirements, and configuration options, see
our [documentation](https://endstone.dev/).

## Documentation

Visit [endstone.dev](https://endstone.dev/) for comprehensive guides, API reference, and tutorials.

## Contributing

We welcome contributions from the community! Whether it's bug reports, feature requests, or code contributions:

- **Found a bug?** Open an [issue](https://github.com/EndstoneMC/endstone/issues)
- **Want to contribute code?** Submit a [pull request](https://github.com/EndstoneMC/endstone/pulls)
- **Want to support the project?** [Buy me a coffee](https://ko-fi.com/EndstoneMC)

## License

Endstone is licensed under the [Apache-2.0 license](LICENSE).

<div align="center">

**Sponsored by [Bisect Hosting](https://bisecthosting.com/endstone)**

[![Bisect Hosting](docs/assets/bisecthosting-banner.webp)](https://bisecthosting.com/endstone)

</div>
