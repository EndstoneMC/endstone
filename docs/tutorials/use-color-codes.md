# Use color codes

Color codes are used to customize the color and formatting of text in the game, such as on signs, in chat, in titles, in forms and more. These color codes are represented by the section sign (§) followed by a character. Each character represents a different color or formatting option. Endstone wants to make it easy to use them, that is why an easy to use api is provided.

=== ":fontawesome-brands-python: Python"
    
    This code shows how easy it is to use colored text in endstone:

    ```py
    my_beautiful_text = f"This is {ColorFormat.YELLOW} yellow, {ColorFormat.AQUA} aqua and {ColorFormat.GOLD} gold{ColorFormat.RESET}."
    ```

    *Pythons f-strings are awesome.*

    Don't forget to end the colored text with `{ColorFormat.RESET}`,
    it ends all extra color format options and resets them. 

=== ":simple-cplusplus: C++"
    
    This code shows how easy it is to use colored text in endstone:
    
    ```cpp
    std::string my_beautiful_text = "This is " + endstone::ColorFormat::DarkGreen + "dark green." + endstone::ColorFormat::Reset;
    ```

    Don't forget to end the colored text with `endstone::ColorFormat::Reset`,
    it ends all extra color format options and resets them. 

A simple list of all colors and their equivalent color codes can be found here:

| Color/Style         | Code |
|---------------------|------|
| AQUA                | §b   |
| BLACK               | §0   |
| BLUE                | §9   |
| BOLD                | §l   |
| DARK_AQUA           | §3   |
| DARK_BLUE           | §1   |
| DARK_GRAY           | §8   |
| DARK_GREEN          | §2   |
| DARK_PURPLE         | §5   |
| DARK_RED            | §4   |
| GOLD                | §6   |
| GRAY                | §7   |
| GREEN               | §a   |
| ITALIC              | §o   |
| LIGHT_PURPLE        | §d   |
| MATERIAL_AMETHYST   | §u   |
| MATERIAL_COPPER     | §n   |
| MATERIAL_DIAMOND    | §s   |
| MATERIAL_EMERALD    | §q   |
| MATERIAL_GOLD       | §p   |
| MATERIAL_IRON       | §i   |
| MATERIAL_LAPIS      | §t   |
| MATERIAL_NETHERITE  | §j   |
| MATERIAL_QUARTZ     | §h   |
| MATERIAL_REDSTONE   | §m   |
| MINECOIN_GOLD       | §g   |
| OBFUSCATED          | §k   |
| RED                 | §c   |
| RESET               | §r   |
| WHITE               | §f   |
| YELLOW              | §e   |
