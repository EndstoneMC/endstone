# Register commands

In this section, we'll walk you through creating a simple command. By the end of this section, you'll have
a basic understanding of how to define and register commands with Endstone.

## Create a command

Let's start with a simple command `/hello` that greets the command sender.

=== ":fontawesome-brands-python: Python"

    ``` python title="src/endstone_my_plugin/my_plugin.py" linenums="1" hl_lines="6-11"
    from endstone.plugin import Plugin

    class MyPlugin(Plugin):
        api_version = "0.4"

        commands = {
            "hello": {
                "description": "Greet the command sender.",
                "usages": ["/hello"],
            }
        }

        # ...
    ```

=== ":simple-cplusplus: C++"

    ``` c++ title="src/my_plugin.cpp" linenums="1" hl_lines="7-9"
    #include "my_plugin.h"

    ENDSTONE_PLUGIN("my_plugin", "0.1.0", MyPlugin)
    {
        description = "My first C++ plugin for Endstone servers";

        command("hello")
            .description("Greet the command sender.")
            .usages("/hello");
    }
    ```

**And it's just as simple as that!** You've just created a new command with description and usages.

## Add permissions

By the default, all the commands in endstone requires the `operator` permission to execute for security reasons.
Since we are creating a simple command here, we might want to change its permission for everyone to use.

Let's make the following changes to our code.

=== ":fontawesome-brands-python: Python"

    ``` python title="src/endstone_my_plugin/my_plugin.py" linenums="1" hl_lines="11 15-20"
    from endstone.command import Command, CommandSender
    from endstone.plugin import Plugin

    class MyPlugin(Plugin):
        api_version = "0.4"

        commands = {
            "hello": {
                "description": "Greet the command sender.",
                "usages": ["/hello"],
                "permissions": ["my_plugin.command.hello"],
            }
        }

        permissions = {
            "my_plugin.command.hello": {
                "description": "Allow users to use the /hello command.",
                "default": True, #(1)!
            }
        }

        # ...
    ```

    1.  :star: See tips

    !!! tip

        The `default` field sets the permission level required for executing the command. Possible values are:

        - `True`: everyone can execute this command
        - `False`: on one can execute this command, unless explicitly granted the permission.
        - `"op"`: only operators can execute this command
        - `"not_op"`: only non-operators can execute this command

=== ":simple-cplusplus: C++"

    ``` c++ title="src/my_plugin.cpp" linenums="1" hl_lines="10 12-14"
    #include "my_plugin.h"

    ENDSTONE_PLUGIN("my_plugin", "0.1.0", MyPlugin)
    {
        description = "My first C++ plugin for Endstone servers";

        command("hello")
            .description("Greet the command sender.")
            .usages("/hello")
            .permissions("my_plugin.command.hello");

        permission("my_plugin.command.hello")
            .description("Allow users to use the /hello command.")
            .default_(endstone::PermissionDefault::True); /*(1)!*/
    }
    ```

    1.  :star: See tips

    !!! tip

        The `default_` method sets the permission level required for executing the command. Possible values are:

        - `endstone::PermissionDefault::True`: everyone can execute this command
        - `endstone::PermissionDefault::False`: on one can execute this command, unless explicitly granted the permission.
        - `endstone::PermissionDefault::Operator`: only operators can execute this command
        - `endstone::PermissionDefault::NotOperator`: only non-operators can execute this command

## Handle the commands

Now, the next step is to handle the commands we just created. Let's say we want to send a "Hello, World!" greeting
message to whoever execute this command. It's just a few more lines away from that.

=== ":fontawesome-brands-python: Python"

    ``` python title="src/endstone_my_plugin/my_plugin.py" linenums="1" hl_lines="1 22-26"
    from endstone.command import Command, CommandSender
    from endstone.plugin import Plugin

    class MyPlugin(Plugin):
        api_version = "0.4"

        commands = {
            "hello": {
                "description": "Greet the command sender.",
                "usages": ["/hello"],
                "permissions": ["my_plugin.command.hello"],
            }
        }

        permissions = {
            "my_plugin.command.hello": {
                "description": "Allow users to use the /hello command.",
                "default": True, #(1)!
            }
        }

        def on_command(self, sender: CommandSender, command: Command, args: list[str]) -> bool:
            if command.name == "hello":
                sender.send_message("Hello World!")

            return True

        # ...
    ```

=== ":simple-cplusplus: C++"

    ``` c++ title="include/my_plugin.h" linenums="1" hl_lines="5-12"
    #include <endstone/plugin/plugin.h>

    class MyPlugin : public endstone::Plugin {
    public:
        bool onCommand(endstone::CommandSender &sender, const endstone::Command &command, const std::vector<std::string> &args) override
        {
            if (command.getName() == "hello") 
            {
                sender.sendMessage("Hello World!");
            }
            return true;
        }

        // ...
    };
    ```

Now, [install] your plugin and restart your server. Join the game and type `/help hello`. You should see the usage
information, which indicates that our command has been successfully added to the game!

Now, type `/hello`, and you should receive the message "Hello World!".

![Example command](screenshots/command-example.png)

[install]: install-your-plugin.md

## Add parameters to commands

Now, let's say we want to send a custom message to greet the sender instead of "Hello World!". We can add a parameter
to the command that accepts a message.

In Endstone, there are two basic kinds of parameters:

| Kind          | Description                                   | Syntax         |
|---------------|-----------------------------------------------|----------------|
| **Mandatory** | This parameter must be provided               | `<name: type>` |
| **Optional**  | This parameter is optional and can be omitted | `[name: type]` |

Let's change our code to add an **optional** parameter `msg` with `message` type. Based on the table above, the syntax
should be `[msg: message]`. Let's add it to our code.

=== ":fontawesome-brands-python: Python"

    ``` python title="src/endstone_my_plugin/my_plugin.py" linenums="1" hl_lines="10 24-27"
    from endstone.command import Command, CommandSender
    from endstone.plugin import Plugin

    class MyPlugin(Plugin):
        api_version = "0.4"

        commands = {
            "hello": {
                "description": "Greet the command sender.",
                "usages": ["/hello [msg: message]"],
                "permissions": ["my_plugin.command.hello"],
            }
        }

        permissions = {
            "my_plugin.command.hello": {
                "description": "Allow users to use the /hello command.",
                "default": True,
            }
        }

        def on_command(self, sender: CommandSender, command: Command, args: list[str]) -> bool:
            if command.name == "hello":
                if len(args) == 0: #(1)!
                    sender.send_message("Hello World!")
                else:
                    sender.send_message(args[0])

            return True

        # ...
    ```

    1.  :white_check_mark: Here, we checked if the optional parameter is provided.

=== ":simple-cplusplus: C++"

    ``` c++ title="src/my_plugin.cpp" linenums="1" hl_lines="9"
    #include "my_plugin.h"

    ENDSTONE_PLUGIN("my_plugin", "0.1.0", MyPlugin)
    {
        description = "My first C++ plugin for Endstone servers";

        command("hello")
            .description("Greet the command sender.")
            .usages("/hello [msg: message]")
            .permissions("my_plugin.command.hello");

        permission("my_plugin.command.hello")
            .description("Allow users to use the /hello command.")
            .default_(endstone::PermissionDefault::True);
    }
    ```

    ``` c++ title="include/my_plugin.h" linenums="1" hl_lines="9-14"
    #include <endstone/plugin/plugin.h>

    class MyPlugin : public endstone::Plugin {
    public:
        bool onCommand(endstone::CommandSender &sender, const endstone::Command &command, const std::vector<std::string> &args) override
        {
            if (command.getName() == "hello") 
            {
                if (args.empty()) {
                    sender.sendMessage("Hello World!");
                }
                else {
                    sender.sendMessage(args[0]);
                }
            }
            return true;
        }

        // ...
    };
    ```

**:partying_face:  And it's done!** Now when the users use `/hello This is my message!`, "This is my message!" will
be shown to them instead of "Hello World!".

!!! tip

    To make the parameter mandatory, change the parameter from `[msg: message]` to `<msg: message>`

### Built-in types

In the section above, we added a parameter with type `message` which is a **built-in type** supported by Endstone.
Here is a full list of all built-in types currently supported.

| Type         | Alias           | Description                                                  | Example            |
|--------------|-----------------|--------------------------------------------------------------|--------------------|
| `int`        |                 | Represents an integer                                        | `10`               |
| `float`      |                 | Represents a floating point number                           | `3.14`             |
| `bool`       |                 | Represents a boolean                                         | `true`             |
| `actor`      | `entity`        | Represents an actor selector                                 | `@e`               |
| `player`     |                 | Represents a player selector                                 | `PlayerName`       |
| `target`     |                 | Represents a target selector, same as `actor` and `player`   | `@a`               |
| `str`        | `string`        | Represents a string, terminated by space                     | `Hello`            |
| `block_pos`, | `vec3i`         | Represents a 3-Dimensional integer position                  | `1 2 3`            |
| `pos`        | `vec3`, `vec3f` | Represents a 3-Dimensional float position                    | `1.0 2.0 3.0`      |
| `message`    |                 | Represents a message, grabs all text input until end of line | `Hello World!`     |
| `json`       |                 | Represents a JSON string                                     | `{"key": "value"}` |

### User-defined enum types

Enum Types can be used to represent a set of predefined string constants. This behavior is similar to how literal types
function in modern programming languages such as Node.js and Python. Enum types provide a way to define a "type"
comprised of a set of named constants, referred to as the enumeration, or in short, "enum".

Endstone allows developer to add their own enums. The basic syntax follows the pattern:

- Mandatory: `(value1|value2|value3)<name: EnumType>`
- Optional: `(value1|value2|value3)[name: EnumType]`

Here, all possible values, separated by the pipeline operator `|`, are scoped inside the parentheses `()`, 
followed by the parameter kind, name and type.

!!! example

    An example of using enum types in command usages is: `/home (add|list|del)<action: HomeAction>`
    
    In this example, the parameter is named `action` and has a **user-defined enum** type `HomeAction`. 
    When using the command, the user must select one of the specified action from the set: `add`, `list`, or `del`.

