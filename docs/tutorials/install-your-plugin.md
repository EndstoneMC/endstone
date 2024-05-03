---
comments: true
toc_depth: 2
---

=== "Python"

    To install your first plugin in Python, click on the :octicons-terminal-16: icon on
    the side bar to open the terminal and type:
    ```
    pip install -e .
    ```
    
    !!! tip
        `-e` means to install the python package in **editable** mode which allows any changes in the source code to be
        effective immediately without the need to use `pip install` again.

    You should see something like this in your console:
    ```
    (venv) pip install -e .
    Obtaining file:///D:/Projects/Python/endstone-my-plugin
    Installing build dependencies ... done
    Checking if build backend supports build_editable ... done
    Getting requirements to build editable ... done
    Installing backend dependencies ... done
    Preparing editable metadata (pyproject.toml) ... done
    Building wheels for collected packages: endstone-my-plugin
    Building editable for endstone-my-plugin (pyproject.toml) ... done
    Created wheel for endstone-my-plugin: filename=endstone_my_plugin-0.1.0-0.editable-py3-none-any.whl size=1577 sha256=83938509c0d941430c5fe5a11be2b9715e188ac9d34d776fcdefe9661d98c6e6
    Stored in directory: C:\Users\Vincent\AppData\Local\Temp\pip-ephem-wheel-cache-1vjqvfaq\wheels\7a\32\83\e6e7cacfcda5443b9f97a99605a19af6bb715389503054a8ca
    Successfully built endstone-my-plugin
    Installing collected packages: endstone-my-plugin
    Successfully installed endstone-my-plugin-0.1.0
    ```

=== "C++"

    Before installing your first plugin, you will need to build it.

    ![Build Plugin](screenshots/clion-build-plugin.png)
    
    Then, copy the built shared library to the plugins folder `path/to/bedrock_server/plugins`.

    !!! tip
        For example, if you are running Endstone server for Minecraft 1.20.81 on Windows. To install the plugin, copy
        `endstone_my_plugin.dll` to `endstone/bedrock_server/windows/1.20.81/plugins`.

Now, [start your server] again. You should see your plugin has been successfully loaded.

![Install plugin](screenshots/install-plugin.png)

**:partying_face: Congratulations!** You've made your first plugin for Endstone servers!

[start your server]: ../getting-started/start-your-server.md