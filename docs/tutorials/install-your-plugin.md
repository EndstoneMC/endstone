---
comments: true
---

Before installing your first plugin, you will need to build it.

=== ":fontawesome-brands-python: Python"

    Click on the :octicons-terminal-16: icon on the side bar to open the 
    terminal and type:

    ```bash
    pip install pipx
    pipx run build --wheel
    ```

    You should see something like this in your console:

    ![PyCharm build plugin](screenshots/pycharm-build-plugin.png)

    Then, copy the built wheel package `endstone_my_plugin.whl` under the `dist`
    folder to the plugins folder `path/to/bedrock_server/plugins`.

    Now, [start your server] again. You should see your plugin has been successfully
    loaded.
    
    ![Start server with plugin](screenshots/start-server-with-plugin.png)

    **:partying_face: Congratulations!** You've made your first plugin for Endstone servers!

    ## Development Mode (a.k.a Editable Installs)
    
    When creating a plugin, developers usually want to implement and test changes iteratively, before cutting a release 
    and preparing a distribution archive. In normal circumstances this can be quite cumbersome and require the 
    developers to continuously re-build and re-install the plugin.

    **But we've got you!** Endstone also allows developers to load the code under development directly from the project folder 
    without having to copy the `whl` files to the `plugins` folder.

    To do so, you will need the activate the [virtual environment]. Please make sure Endstone is also installed inside the environment.

    === ":fontawesome-brands-windows: Windows"

        ``` sh
        . venv/Scripts/activate
        ```

    === ":fontawesome-brands-linux: Linux"

        ``` sh
        . venv/bin/activate
        ```
    
    You can now enter this “development mode” by performing an [editable installation] inside the virtual environment, 
    using pip’s `-e/--editable` flag, as shown below:

    ``` sh title="(venv)"
    pip install --editable .
    ```

    Now, if you change the source code on the disk, you can simply run the `/reload` command in Endstone and you change
    will be effective immediately.

    When you’re done with a given development task, you can simply uninstall your package 
    (as you would normally do with `pip uninstall <package name>`).

=== ":simple-cplusplus: C++"

    Click on the menu icon on the top bar and select **Build > Build Project** 
    to build your plugin.

    ![CLion build plugin](screenshots/clion-build-plugin.png)
    
    Then, copy the built shared library `endstone_my_plugin.dll` to the plugins 
    folder `path/to/bedrock_server/plugins`.

    Now, [start your server] again. You should see your plugin has been successfully
    loaded.
    
    ![Start server with plugin](screenshots/start-server-with-plugin.png)

    **:partying_face: Congratulations!** You've made your first plugin for Endstone servers!


[start your server]: ../getting-started/start-your-server.md

[editable installation]: https://pip.pypa.io/en/latest/topics/local-project-installs/

[virtual environment]: ../../getting-started/installation/#environment