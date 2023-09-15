// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "bedrock/bedrock_log.h"

#include "endstone/chat_color.h"
#include "lib/hook/hook.h"

void BedrockLog::log_va(BedrockLog::LogCategory category, std::bitset<3> flags, BedrockLog::LogRule rule,
                        LogAreaID area, LogLevel level, char const *function, int line, char const *format,
                        va_list args)
{
    static std::unordered_map<std::string, std::string> replacements = {
        {ChatColors::toString(ChatColor::Black), "\x1b[30m"},
        {ChatColors::toString(ChatColor::DarkBlue), "\x1b[34m"},
        {ChatColors::toString(ChatColor::DarkGreen), "\x1b[32m"},
        {ChatColors::toString(ChatColor::DarkAqua), "\x1b[36m"},
        {ChatColors::toString(ChatColor::DarkRed), "\x1b[31m"},
        {ChatColors::toString(ChatColor::DarkPurple), "\x1b[35m"},
        {ChatColors::toString(ChatColor::Gold), "\x1b[33m"},
        {ChatColors::toString(ChatColor::Gray), "\x1b[37m"},
        {ChatColors::toString(ChatColor::DarkGray), "\x1b[90m"},
        {ChatColors::toString(ChatColor::Blue), "\x1b[94m"},
        {ChatColors::toString(ChatColor::Green), "\x1b[92m"},
        {ChatColors::toString(ChatColor::Aqua), "\x1b[96m"},
        {ChatColors::toString(ChatColor::Red), "\x1b[91m"},
        {ChatColors::toString(ChatColor::LightPurple), "\x1b[95m"},
        {ChatColors::toString(ChatColor::Yellow), "\x1b[93m"},
        {ChatColors::toString(ChatColor::White), "\x1b[97m"},
        {ChatColors::toString(ChatColor::MinecoinGold), "\x1b[38;2;221;214;5m"},
        {ChatColors::toString(ChatColor::MaterialQuartz), "\x1b[38;2;227;212;209m"},
        {ChatColors::toString(ChatColor::MaterialIron), "\x1b[38;2;206;202;202m"},
        {ChatColors::toString(ChatColor::MaterialNetherite), "\x1b[38;2;68;58;59m"},
        {ChatColors::toString(ChatColor::Magic), "\x1b[8m"},
        {ChatColors::toString(ChatColor::Bold), "\x1b[1m"},
        {ChatColors::toString(ChatColor::MaterialRedstone), "\x1b[38;2;151;22;7m"},
        {ChatColors::toString(ChatColor::MaterialCopper), "\x1b[38;2;180;104;77m"},
        {ChatColors::toString(ChatColor::Italic), "\x1b[3m"},
        {ChatColors::toString(ChatColor::MaterialGold), "\x1b[38;2;222;177;45m"},
        {ChatColors::toString(ChatColor::MaterialEmerald), "\x1b[38;2;222;177;45m"},
        {ChatColors::toString(ChatColor::Reset), "\x1b[0m"},
        {ChatColors::toString(ChatColor::MaterialDiamond), "\x1b[38;2;44;186;168m"},
        {ChatColors::toString(ChatColor::MaterialLapis), "\x1b[38;2;33;73;123m"},
        {ChatColors::toString(ChatColor::MaterialAmethyst), "\x1b[38;2;154;92;198m"},
    };

    std::string_view subject = format;
    std::string output;
    output.reserve(subject.size() * 2);
    std::ptrdiff_t last_pos = 0;
    std::match_results<std::string_view::const_iterator> match;

    while (std::regex_search(subject.begin() + last_pos, subject.end(), match, ChatColors::PATTERN)) {
        output.append(subject.substr(last_pos, match.position()));

        auto it = replacements.find(match.str());
        if (it != replacements.end()) {
            output.append(it->second);
        }
        else {
            output.append(match.str());
        }

        last_pos += (match.position() + match.length());
    }
    output.append(subject.substr(last_pos));

    static std::mutex mtx;
    std::lock_guard<std::mutex> lock(mtx);
    ENDSTONE_HOOK_CALL_ORIGINAL(&BedrockLog::log_va,  //
                                category, flags, rule, area, level, function, line, output.c_str(), args)
    printf("\x1b[0;3;32m");  // set to green and italic
}
