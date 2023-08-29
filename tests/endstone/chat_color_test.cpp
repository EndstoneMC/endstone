//
// Created by Vincent on 29/08/2023.
//

#include "endstone/chat_colors.h"

#include <gtest/gtest.h>

TEST(ChatColorTest, GetByChar)
{
    for (auto &color : ChatColors::values()) {
        EXPECT_EQ(ChatColors::getByChar(color->getChar()), color);
    }
}

TEST(ChatColorTest, StripColorOnEmptyString)
{
    EXPECT_EQ(ChatColors::stripColor(""), "");
}

TEST(ChatColorTest, StripColor)
{
    std::string subject;
    std::string expected;
    const std::string filler = "test";
    for (auto &color : ChatColors::values()) {
        subject += color->toString() + filler;
        expected += filler;
    }
    EXPECT_EQ(ChatColors::stripColor(subject), expected);
}

TEST(ChatColorTest, ToString)
{
    for (auto &color : ChatColors::values()) {
        std::string expected = ChatColor::COLOR_CHAR + std::string(1, color->getChar());
        EXPECT_EQ(expected, color->toString());
    }
}

TEST(ChatColorTest, RegexSearch)
{
    std::string subject;
    const std::string filler = "test";
    for (auto &color : ChatColors::values()) {
        subject += color->toString() + filler;
    }

    std::sregex_iterator begin = std::sregex_iterator(subject.begin(), subject.end(), ChatColor::COLOR_PATTERN);
    std::sregex_iterator end = std::sregex_iterator();
    size_t count = std::distance(begin, end);
    EXPECT_EQ(count, ChatColors::values().size());
}

TEST(ChatColorTest, RegexReplaceWithString)
{
    std::string subject;
    const std::string filler = "test";
    for (auto &color : ChatColors::values()) {
        subject += color->toString() + filler;
    }

    std::string replaced;
    std::sregex_iterator begin = std::sregex_iterator(subject.begin(), subject.end(), ChatColor::COLOR_PATTERN);
    std::sregex_iterator end = std::sregex_iterator();
    std::ptrdiff_t lastPos = 0;

    for (std::sregex_iterator it = begin; it != end; ++it) {
        replaced += subject.substr(lastPos, it->position() - lastPos);
        lastPos = it->position() + it->length();
    }
    replaced += subject.substr(lastPos);

    EXPECT_EQ(replaced, ChatColors::stripColor(subject));
}

TEST(ChatColorTest, RegexReplaceWithStringView)
{
    const std::string filler = "test";
    std::string subject_str;
    for (auto &color : ChatColors::values()) {
        subject_str += color->toString() + filler;
    }
    std::string_view subject = subject_str;

    std::string replaced_str;
    std::ptrdiff_t last_pos = 0;
    std::match_results<std::string_view::const_iterator> match;

    while (std::regex_search(subject.begin() + last_pos, subject.end(), match, ChatColor::COLOR_PATTERN)) {
        replaced_str += std::string(subject.substr(last_pos, match.position()));
        last_pos += (match.position() + match.length());
    }
    replaced_str += std::string(subject.substr(last_pos));

    // Convert 'replaced' to string_view for the comparison
    std::string_view replaced = replaced_str;

    EXPECT_EQ(replaced, ChatColors::stripColor(subject_str));
}
