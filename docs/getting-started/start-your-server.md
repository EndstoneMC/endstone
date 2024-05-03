---
comments: true
---

# Start your server

After you've [installed] Endstone, you can bootstrap your server using the `endstone` executable. Go to the directory
where you want your server to be located and enter:

```
endstone
```

Alternatively, if you're running Endstone from within Docker, use:

=== ":fontawesome-brands-linux: Linux / :fontawesome-brands-windows: Powershell"

    ```
    docker run --rm -it -v ${PWD}:/home/endstone -p 19132:19132/udp endstone/endstone
    ```

=== ":fontawesome-brands-windows: Command Prompt"

    ```
    docker run --rm -it -v "%cd%":/home/endstone -p 19132:19132/udp endstone/endstone
    ```

You should see this in your console:

![Start your server](start-your-server.png)

!!! tip
    The first time you run the bootstrap, it will need to download the [Bedrock Dedicated Server] from the official
    mirror. Press ++y++ and ++enter++ to continue.


[installed]: installation.md

[Bedrock Dedicated Server]: https://www.minecraft.net/en-us/download/server/bedrock