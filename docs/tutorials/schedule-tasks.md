# Schedule tasks

Endstone provides a task scheduling system that allows plugins to schedule tasks for future execution, possibly at
regular intervals. In this tutorial, we'll guide you on scheduling a straightforward task.

Here, we want an on-screen popup displaying "Hi!" to appear for every online player at a 1-second interval.

=== ":fontawesome-brands-python: Python"

    ``` python title="src/endstone_my_plugin/my_plugin.py" linenums="1" hl_lines="10 12-14"
    from endstone.plugin import Plugin

    class MyPlugin(Plugin):
        api_version = "0.4"

        # ...

        def on_enable(self) -> None:
            self.logger.info("on_enable is called!")
            self.server.scheduler.run_task(self, self.send_time, delay=0, period=20)

        def send_time(self) -> None:
            for player in self.server.online_players:
                player.send_popup("Hi!")
    ```

=== ":simple-cplusplus: C++"

    ``` c++ title="include/my_plugin.h" linenums="1" hl_lines="10 13-19"
    #include <endstone/plugin/plugin.h>

    class MyPlugin : public endstone::Plugin {
    public:
        // ...

        void onEnable() override
        {
            getLogger().info("onEnable is called");
            getServer().getScheduler().runTaskTimer([&]() { sendTime(); }, 0, 20);
        }

        void sendTime()
        {
            for (auto& player : getServer().getOnlinePlayers())
            {
                player->sendPopup("Hi");
            }
        }
    };
    ```

**:partying_face: And that's it!** The server will now send a "Hi" message to all players online at an interval of 20
ticks or approximately every second.