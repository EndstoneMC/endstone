# Create your first plugin

## Create a new project

=== "Python"

    TODO

=== "C++"

    TODO

## Create the main plugin class

=== "Python"

    ``` python
    from endstone.plugin import Plugin

    class ExamplePlugin(Plugin):
        pass
    ```

=== "C++"

    ``` c++
    #include "endstone/plugin/plugin.h"

    class ExamplePlugin : public endstone::Plugin {}
    ```

## Add methods

=== "Python"

    ``` python
    from endstone.plugin import Plugin

    class ExamplePlugin(Plugin):
        def on_load(self) -> None:
            self.logger.info("on_load is called!")

        def on_enable(self) -> None:
            self.logger.info("on_enable is called!")

        def on_disable(self) -> None:
            self.logger.info("on_disable is called!")
    ```

=== "C++"

    ``` c++
    #include "endstone/plugin/plugin.h"

    class ExamplePlugin : public endstone::Plugin {
    public:
        void onLoad() override
        {
            getLogger().info("onLoad is called");
        }
    
        void onEnable() override
        {
            getLogger().info("onEnable is called");
        }
    
        void onDisable() override
        {
            getLogger().info("onDisable is called");
        }
    }
    ```

## Configure plugin metadata

=== "Python"

    TODO

=== "C++"

    TODO