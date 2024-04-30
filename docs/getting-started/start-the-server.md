# Start the server

After you've [installed] Endstone, you can bootstrap your server using the `endstone` executable. Go to the directory
where you want your server to be located and enter:

```
endstone
```

Alternatively, if you're running Material for MkDocs from within Docker, use:

=== ":material-linux: Linux / :fontawesome-brands-windows: Powershell"

    ```
    docker run --rm -it -v ${PWD}:/home/endstone -p 19132:19132/udp endstone/endstone
    ```

=== ":fontawesome-brands-windows: Command Prompt"

    ```
    docker run --rm -it -v "%cd%":/home/endstone -p 19132:19132/udp endstone/endstone
    ```

The first time you run the boostrap, it will ask you to download the [Bedrock Dedicated Server] from the official
mirror. Enter ++y++ to continue.

You will see this from the console:

```text
Bedrock Dedicated Server (v1.20.80) is not found in /home/endstone/bedrock_server/linux/1.20.80. Would you like to download it now? [Y/n]: y
[2024-04-30 11:14:16.521 INFO] [LinuxBootstrap] Loading index from the remote server...
[2024-04-30 11:14:16.856 INFO] [LinuxBootstrap] Downloading server from https://minecraft.azureedge.net/bin-linux/bedrock-server-1.20.80.05.zip...
Downloading... ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ 128.3/128.3 MB 0:00:00
[2024-04-30 11:14:20.213 INFO] [LinuxBootstrap] Download complete. Verifying integrity...
[2024-04-30 11:14:20.213 INFO] [LinuxBootstrap] Integrity check passed. Extracting to /home/endstone/bedrock_server/linux/1.20.80...
```

It will take a few seconds to extract the server and start the server.

```text
[2024-04-30 11:16:54.985 INFO] [EndstoneRuntime] Initialising...
[2024-04-30 11:16:57.625 INFO] [Server] setting up server logging...
[2024-04-30 11:16:57.628 INFO] [Server] Starting Server
[2024-04-30 11:16:57.628 INFO] [Server] Version: 1.20.80.05
[2024-04-30 11:16:57.628 INFO] [Server] Session ID: 3acba8a5-88b0-47c6-a2da-41ea51013c03
[2024-04-30 11:16:57.628 INFO] [Server] Build ID: 23555108
[2024-04-30 11:16:57.628 INFO] [Server] Branch: r/20_u8
[2024-04-30 11:16:57.628 INFO] [Server] Commit ID: 2c8fdaef654a88c0efd0908029243d44daca42e5
[2024-04-30 11:16:57.628 INFO] [Server] Configuration: Publish
[2024-04-30 11:16:57.629 INFO] [Server] Level Name: Bedrock level
[2024-04-30 11:16:57.635 INFO] [Server] No CDN config file found for dedicated server
[2024-04-30 11:16:57.637 INFO] [Server] Game mode: 0 Survival
[2024-04-30 11:16:57.637 INFO] [Server] Difficulty: 1 EASY
[2024-04-30 11:16:57.643 INFO] [Server] Content logging to console is enabled.
[2024-04-30 11:17:10.554 INFO] [Server] This server is running Endstone version: 0.4.2.dev39 (Minecraft: v1.20.80)
[2024-04-30 11:17:10.567 INFO] [Database] Opening level 'worlds/Bedrock level/db'
[2024-04-30 11:17:30.732 INFO] [Network] IPv4 supported, port: 19132: Used for gameplay and LAN discovery
[2024-04-30 11:17:30.732 INFO] [Network] IPv6 not supported
[2024-04-30 11:17:30.934 INFO] [Server] Server started.
[2024-04-30 11:17:30.934 INFO] [Server] ================ TELEMETRY MESSAGE ===================
[2024-04-30 11:17:30.934 INFO] [Server] Server Telemetry is currently not enabled.
[2024-04-30 11:17:30.935 INFO] [Server] Enabling this telemetry helps us improve the game.
[2024-04-30 11:17:30.935 INFO] [Server] To enable this feature, add the line 'emit-server-telemetry=true'
[2024-04-30 11:17:30.935 INFO] [Server] to the server.properties file in the handheld/src-server directory
[2024-04-30 11:17:30.935 INFO] [Server] ======================================================
```

After the server is up and running, use command `version` in the console to check the version of Endstone.

```text
version
[2024-04-30 11:17:51.913 INFO] [Server] This server is running Endstone version: 0.4.1 (Minecraft: v1.20.80)
```

[installed]: installation.md

[Bedrock Dedicated Server]: https://www.minecraft.net/en-us/download/server/bedrock