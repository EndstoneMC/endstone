// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#pragma once

#include <spdlog/pattern_formatter.h>
#include <spdlog/spdlog.h>

#include "endstone/color_format.h"

namespace endstone::core {

class TextFormatter : public spdlog::custom_flag_formatter {
public:
    explicit TextFormatter(bool should_do_colors) : should_do_colors_(should_do_colors){};
    void format(const spdlog::details::log_msg &msg, const std::tm &, spdlog::memory_buf_t &dest) override;
    [[nodiscard]] std::unique_ptr<custom_flag_formatter> clone() const override;

private:
    static const std::unordered_map<unsigned char, spdlog::string_view_t> ansi_codes;
    bool should_do_colors_;
};

}  // namespace endstone::core
