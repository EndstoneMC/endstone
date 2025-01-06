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

#include "imgui_json.h"

#include <imgui.h>

#include <regex>

#include "bedrock/core/math/color.h"

namespace {
bool isColor(const std::string &str)
{
    static std::regex pattern("^#([A-Fa-f0-9]{8})$");
    return std::regex_match(str, pattern);
}
}  // namespace

void ImGui::Json(const nlohmann::json &json)  // NOLINT(*-no-recursion)
{
    switch (json.type()) {
    case nlohmann::json::value_t::object: {
        for (const auto &el : json.items()) {
            if (el.value().is_primitive()) {
                ImGui::Text("%s: ", el.key().c_str());
                ImGui::SameLine();
                ImGui::Json(el.value());
            }
            else if (ImGui::TreeNode(el.key().c_str())) {
                ImGui::Json(el.value());
                ImGui::TreePop();
            }
        }
        break;
    }
    case nlohmann::json::value_t::array: {
        int index = 0;
        for (const auto &el : json) {
            auto key = std::to_string(index);
            if (el.is_primitive()) {
                ImGui::Text("%s: ", key.c_str());
                ImGui::SameLine();
                ImGui::Json(el);
            }
            else if (ImGui::TreeNode(key.c_str())) {
                ImGui::Json(el);
                ImGui::TreePop();
            }
            ++index;
        }
        break;
    }
    default: {
        ImGui::Text("%s", json.dump().c_str());
        if (json.is_string()) {
            auto value = json.get<std::string>();
            if (isColor(value)) {
                ImGui::SameLine();
                auto color = mce::Color::fromHexString(value.substr(1));
                ImGui::ColorButton("Color", {color.r, color.g, color.b, color.a});
            }
        }
        break;
    }
    }
}
