---
comments: true
---

Color codes are used to customize the color and formatting of text in the game, such as on signs, in chat, in titles, in
forms and more. These color codes are represented by the section sign (`§`) followed by a character. Each character
represents a different color or formatting option. Endstone wants to make it easy to use them, that is why an easy to
use api is provided.

## Usage

=== ":fontawesome-brands-python: Python"

    This code shows how easy it is to use colored text in endstone:

    ```python
    from endstone import ColorFormat

    my_beautiful_text = f"This is {ColorFormat.YELLOW}yellow, {ColorFormat.AQUA}aqua and {ColorFormat.GOLD}gold{ColorFormat.RESET}."
    ```

    *[Pythons f-strings] are awesome.*
    
    !!! tip

        Don't forget to end the colored text with `ColorFormat.RESET`,
        it ends all extra color format options and resets them. 

    [Pythons f-strings]: https://docs.python.org/3/tutorial/inputoutput.html#tut-f-strings

=== ":simple-cplusplus: C++"

    This code shows how easy it is to use colored text in endstone:
    
    ```cpp
    #include <endstone/color_format.h>

    auto my_beautiful_text = "This is " + endstone::ColorFormat::DarkGreen + "dark green." + endstone::ColorFormat::Reset;
    ```
    
    !!! tip
    
        Don't forget to end the colored text with `endstone::ColorFormat::Reset`,
        it ends all extra color format options and resets them. 

## All color and format codes

<div class="center-table" markdown>

| Code |        Name        |
|:----:|:------------------:|
| `§0` |       black        |
| `§1` |     dark_blue      |
| `§2` |     dark_green     |
| `§3` |     dark_aqua      |
| `§4` |      dark_red      |
| `§5` |    dark_purple     |
| `§6` |        gold        |
| `§7` |        gray        |
| `§8` |     dark_gray      |
| `§9` |        blue        |
| `§a` |       green        |
| `§b` |        aqua        |
| `§c` |        red         |
| `§d` |    light_purple    |
| `§e` |       yellow       |
| `§f` |       white        |
| `§g` |   minecoin_gold    |
| `§h` |  material_quartz   |
| `§i` |   material_iron    |
| `§j` | material_netherite |
| `§k` |     obfuscated     |
| `§l` |        bold        |
| `§m` | material_redstone  |
| `§n` |  material_copper   |
| `§o` |       italic       |
| `§p` |   material_gold    |
| `§q` |  material_emerald  |
| `§r` |       reset        |
| `§s` |  material_diamond  |
| `§t` |   material_lapis   |
| `§u` | material_amethyst  |

</div>
