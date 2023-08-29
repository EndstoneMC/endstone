//
// Created by Vincent on 29/08/2023.
//

#ifndef ENDSTONE_CHAT_COLORS_H
#define ENDSTONE_CHAT_COLORS_H

#include "endstone/common.h"

class ChatColor {
public:
    inline static const auto COLOR_CHAR = char(167); // §
    inline static const auto COLOR_PATTERN =
        std::regex(std::string(1, COLOR_CHAR) + "[0-9A-FK-ORX]", std::regex_constants::icase);

private:
    friend class ChatColors;

    enum Code : int {
        BLACK = 0,
        DARK_BLUE,
        DARK_GREEN,
        DARK_AQUA,
        DARK_RED,
        DARK_PURPLE,
        GOLD,
        GRAY,
        DARK_GRAY,
        BLUE,
        GREEN,
        AQUA,
        RED,
        LIGHT_PURPLE,
        YELLOW,
        WHITE,
        MAGIC,
        BOLD,
        STRIKETHROUGH,
        UNDERLINE,
        ITALIC,
        RESET
    };

    ChatColor(char code, int int_code, bool is_format = false)
        : code_(code), int_code_(int_code), is_format_(is_format), str_(std::string(1, COLOR_CHAR) + code)
    {
        id_lookup_[int_code] = this;
        char_lookup_[code] = this;
    }

public:
    char getChar() const
    {
        return code_;
    }

    std::string toString() const
    {
        return str_;
    }

    bool isFormatCode() const
    {
        return is_format_;
    }

    bool isColorCode() const
    {
        return !is_format_ && int_code_ != RESET;
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
        return std::regex_replace(input, COLOR_PATTERN, "");
    }

    static std::string translateColorCodes(char color_char, const std::string &input)
    {
        std::string result = input;
        for (size_t i = 0; i < result.length() - 1; ++i) {
            if (result[i] == color_char &&
                std::string_view("0123456789AaBbCcDdEeFfKkLlMmNnOoRrXx").find(result[i + 1]) != std::string::npos) {
                result[i] = COLOR_CHAR;
                result[i + 1] = static_cast<char>(std::tolower(static_cast<unsigned char>(result[i + 1])));
            }
        }
        return result;
    }

    static std::string getLastColors(const std::string &input)
    {
        std::string result;
        size_t length = input.length();

        for (size_t index = length - 1; index != std::string::npos; --index) {
            if (input[index] == COLOR_CHAR && index < length - 1) {
                std::string hex_color = getHexColor(input, index);
                if (!hex_color.empty()) {
                    result += hex_color;
                    break;
                }

                char c = input[index + 1];
                ChatColor *color = getByChar(c);
                if (color) {
                    result += color->toString();
                    if (color->isColorCode() || color->int_code_ == RESET) {
                        break;
                    }
                }
            }
        }

        return result;
    }

private:
    static std::string getHexColor(const std::string &input, size_t index)
    {
        if (index >= 12 && input[index - 11] == 'x' && input[index - 12] == COLOR_CHAR) {
            for (size_t i = index - 10; i <= index; i += 2) {
                if (input[i] != COLOR_CHAR) {
                    return "";
                }
            }

            for (size_t i = index - 9; i <= index + 1; i += 2) {
                char toCheck = input[i];
                if (toCheck < '0' || toCheck > 'f' || (toCheck > '9' && toCheck < 'A') ||
                    (toCheck > 'F' && toCheck < 'a')) {
                    return "";
                }
            }

            return input.substr(index - 12, 14);
        }
        else {
            return "";
        }
    }

private:
    char code_;
    int int_code_;
    bool is_format_;
    std::string str_;

    inline static std::unordered_map<int, ChatColor *> id_lookup_;
    inline static std::unordered_map<char, ChatColor *> char_lookup_;
};

class ChatColors {
public:
    const inline static ChatColor BLACK{'0', ChatColor::BLACK};
    const inline static ChatColor DARK_BLUE{'1', ChatColor::DARK_BLUE};
    const inline static ChatColor DARK_GREEN{'2', ChatColor::DARK_GREEN};
    const inline static ChatColor DARK_AQUA{'3', ChatColor::DARK_AQUA};
    const inline static ChatColor DARK_RED{'4', ChatColor::DARK_RED};
    const inline static ChatColor DARK_PURPLE{'5', ChatColor::DARK_PURPLE};
    const inline static ChatColor GOLD{'6', ChatColor::GOLD};
    const inline static ChatColor GRAY{'7', ChatColor::GRAY};
    const inline static ChatColor DARK_GRAY{'8', ChatColor::DARK_GRAY};
    const inline static ChatColor BLUE{'9', ChatColor::BLUE};
    const inline static ChatColor GREEN{'a', ChatColor::GREEN};
    const inline static ChatColor AQUA{'b', ChatColor::AQUA};
    const inline static ChatColor RED{'c', ChatColor::RED};
    const inline static ChatColor LIGHT_PURPLE{'d', ChatColor::LIGHT_PURPLE};
    const inline static ChatColor YELLOW{'e', ChatColor::YELLOW};
    const inline static ChatColor WHITE{'f', ChatColor::WHITE};
    const inline static ChatColor MAGIC{'k', ChatColor::MAGIC, true};
    const inline static ChatColor BOLD{'l', ChatColor::BOLD, true};
    const inline static ChatColor STRIKETHROUGH{'m', ChatColor::STRIKETHROUGH, true};
    const inline static ChatColor UNDERLINE{'n', ChatColor::UNDERLINE, true};
    const inline static ChatColor ITALIC{'o', ChatColor::ITALIC, true};
    const inline static ChatColor RESET{'r', ChatColor::RESET};
};

#endif // ENDSTONE_CHAT_COLORS_H
