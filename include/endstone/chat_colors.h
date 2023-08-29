//
// Created by Vincent on 29/08/2023.
//

#ifndef ENDSTONE_CHAT_COLORS_H
#define ENDSTONE_CHAT_COLORS_H

#include "endstone/common.h"
class ChatColor {

    friend class ChatColors;

public:
    const inline static std::string COLOR_CHAR = "§";
    const inline static std::regex COLOR_PATTERN =
        std::regex("(" + COLOR_CHAR + "[0-9A-U])", std::regex_constants::icase);

private:
    explicit ChatColor(char code) : code_(code), str_{COLOR_CHAR + code} {}

public:
    constexpr char getChar() const
    {
        return code_;
    }

    const std::string &toString() const
    {
        return str_;
    }

    constexpr bool isFormatCode() const
    {
        return code_ != 'k'  // MAGIC
            && code_ != 'l'  // BOLD
            && code_ != 'o'  // ITALIC
            && code_ != 'r'; // RESET
    }

    constexpr bool isColorCode() const
    {
        return !isFormatCode();
    }

    constexpr bool operator==(const ChatColor &other) const
    {
        return code_ == other.code_;
    }

    constexpr bool operator!=(const ChatColor &other) const
    {
        return !(*this == other);
    }

private:
    char code_;
    std::string str_;
};

class ChatColors {
public:
    static std::vector<ChatColor *> values()
    {
        std::vector<ChatColor *> colors;
        colors.reserve(char_lookup_.size()); // Optional, but can improve performance

        for (const auto &pair : char_lookup_) {
            colors.push_back(pair.second);
        }

        return colors;
    }

    static ChatColor *getByChar(char code)
    {
        if (char_lookup_.find(code) != char_lookup_.end()) {
            return char_lookup_[code];
        }
        return nullptr;
    }

    static std::string stripColor(const std::string &input)
    {
        return std::regex_replace(input, ChatColor::COLOR_PATTERN, "");
    }

private:
    inline static std::unordered_map<char, ChatColor *> char_lookup_;

    static ChatColor newChatColor(char code)
    {
        ChatColor color(code);
        char_lookup_[code] = &color;
        return color;
    }

public:
    const inline static auto BLACK = newChatColor('0');
    const inline static auto DARK_BLUE = newChatColor('1');
    const inline static auto DARK_GREEN = newChatColor('2');
    const inline static auto DARK_AQUA = newChatColor('3');
    const inline static auto DARK_RED = newChatColor('4');
    const inline static auto DARK_PURPLE = newChatColor('5');
    const inline static auto GOLD = newChatColor('6');
    const inline static auto GRAY = newChatColor('7');
    const inline static auto DARK_GRAY = newChatColor('8');
    const inline static auto BLUE = newChatColor('9');
    const inline static auto GREEN = newChatColor('a');
    const inline static auto AQUA = newChatColor('b');
    const inline static auto RED = newChatColor('c');
    const inline static auto LIGHT_PURPLE = newChatColor('d');
    const inline static auto YELLOW = newChatColor('e');
    const inline static auto WHITE = newChatColor('f');
    const inline static auto MINECOIN_GOLD = newChatColor('g');
    const inline static auto MATERIAL_QUARTZ = newChatColor('h');
    const inline static auto MATERIAL_IRON = newChatColor('i');
    const inline static auto MATERIAL_NETHERITE = newChatColor('j');
    const inline static auto MAGIC = newChatColor('k');
    const inline static auto BOLD = newChatColor('l');
    const inline static auto MATERIAL_REDSTONE = newChatColor('m');
    const inline static auto MATERIAL_COPPER = newChatColor('n');
    const inline static auto ITALIC = newChatColor('o');
    const inline static auto MATERIAL_GOLD = newChatColor('p');
    const inline static auto MATERIAL_EMERALD = newChatColor('q');
    const inline static auto RESET = newChatColor('r');
    const inline static auto MATERIAL_DIAMOND = newChatColor('s');
    const inline static auto MATERIAL_LAPIS = newChatColor('t');
    const inline static auto MATERIAL_AMETHYST = newChatColor('u');
};

#endif // ENDSTONE_CHAT_COLORS_H
