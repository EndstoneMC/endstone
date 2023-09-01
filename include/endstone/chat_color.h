//
// Created by Vincent on 29/08/2023.
//

#ifndef ENDSTONE_CHAT_COLOR_H
#define ENDSTONE_CHAT_COLOR_H

#include "endstone/common.h"

enum class ChatColor : char {
    Black = '0',
    DarkBlue = '1',
    DarkGreen = '2',
    DarkAqua = '3',
    DarkRed = '4',
    DarkPurple = '5',
    Gold = '6',
    Gray = '7',
    DarkGray = '8',
    Blue = '9',
    Green = 'a',
    Aqua = 'b',
    Red = 'c',
    LightPurple = 'd',
    Yellow = 'e',
    White = 'f',
    MinecoinGold = 'g',
    MaterialQuartz = 'h',
    MaterialIron = 'i',
    MaterialNetherite = 'j',
    Magic = 'k',
    Bold = 'l',
    MaterialRedstone = 'm',
    MaterialCopper = 'n',
    Italic = 'o',
    MaterialGold = 'p',
    MaterialEmerald = 'q',
    Reset = 'r',
    MaterialDiamond = 's',
    MaterialLapis = 't',
    MaterialAmethyst = 'u'
};

class ChatColors {
private:
    static std::map<char, ChatColor> lookup_map_;
    static std::pair<char, ChatColor> a(ChatColor color)
    {
        return {static_cast<char>(color), color};
    }

public:
    const inline static std::string ESCAPE = "§";
    const inline static std::regex PATTERN = std::regex("(" + ESCAPE + "[0-9A-U])", std::regex_constants::icase);

    static const ChatColor *get(char ch)
    {
        auto it = lookup_map_.find(ch);
        if (it != lookup_map_.end()) {
            return &it->second;
        }
        return nullptr;
    }

    static std::string toString(const ChatColor &color)
    {
        return ESCAPE + static_cast<char>(color);
    }

    static std::string stripColor(const std::string &input)
    {
        return std::regex_replace(input, PATTERN, "");
    }

    static std::vector<ChatColor> values()
    {
        std::vector<ChatColor> colors;
        colors.reserve(lookup_map_.size());
        for (const auto &pair : lookup_map_) {
            colors.push_back(pair.second);
        }
        return colors;
    }
};

inline std::map<char, ChatColor> ChatColors::lookup_map_ = { //
    ChatColors::a(ChatColor::Black),
    ChatColors::a(ChatColor::DarkBlue),
    ChatColors::a(ChatColor::DarkGreen),
    ChatColors::a(ChatColor::DarkAqua),
    ChatColors::a(ChatColor::DarkRed),
    ChatColors::a(ChatColor::DarkPurple),
    ChatColors::a(ChatColor::Gold),
    ChatColors::a(ChatColor::Gray),
    ChatColors::a(ChatColor::DarkGray),
    ChatColors::a(ChatColor::Blue),
    ChatColors::a(ChatColor::Green),
    ChatColors::a(ChatColor::Aqua),
    ChatColors::a(ChatColor::Red),
    ChatColors::a(ChatColor::LightPurple),
    ChatColors::a(ChatColor::Yellow),
    ChatColors::a(ChatColor::White),
    ChatColors::a(ChatColor::MinecoinGold),
    ChatColors::a(ChatColor::MaterialQuartz),
    ChatColors::a(ChatColor::MaterialIron),
    ChatColors::a(ChatColor::MaterialNetherite),
    ChatColors::a(ChatColor::Magic),
    ChatColors::a(ChatColor::Bold),
    ChatColors::a(ChatColor::MaterialRedstone),
    ChatColors::a(ChatColor::MaterialCopper),
    ChatColors::a(ChatColor::Italic),
    ChatColors::a(ChatColor::MaterialGold),
    ChatColors::a(ChatColor::MaterialEmerald),
    ChatColors::a(ChatColor::Reset),
    ChatColors::a(ChatColor::MaterialDiamond),
    ChatColors::a(ChatColor::MaterialLapis),
    ChatColors::a(ChatColor::MaterialAmethyst)};

inline std::string operator+(const std::string &lhs, const ChatColor &rhs)
{
    return lhs + ChatColors::ESCAPE + static_cast<char>(rhs);
}

inline std::string operator+(const ChatColor &lhs, const std::string &rhs)
{
    return ChatColors::ESCAPE + static_cast<char>(lhs) + rhs;
}

#endif // ENDSTONE_CHAT_COLOR_H
