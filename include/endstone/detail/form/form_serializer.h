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

#include <nlohmann/json.hpp>

#include "endstone/form/action_form.h"
#include "endstone/form/message_form.h"

namespace endstone::detail {

struct FormSerializer {
    nlohmann::json operator()(const MessageForm &form)
    {
        nlohmann::json json;
        json["type"] = "modal";
        json["title"] = toJson(form.getTitle());
        json["content"] = toJson(form.getContent());
        json["button1"] = toJson(form.getButton1());
        json["button2"] = toJson(form.getButton2());
        return json;
    }

    nlohmann::json operator()(const ActionForm &form)
    {
        nlohmann::json json;
        json["type"] = "form";
        json["title"] = toJson(form.getTitle());
        json["content"] = toJson(form.getContent());
        for (const auto &button : form.getButtons()) {
            nlohmann::json btn;
            btn["text"] = toJson(button.getText());
            if (button.getIcon().has_value()) {
                auto icon = button.getIcon().value();
                if (icon.rfind("http://", 0) == 0 || icon.rfind("https://", 0) == 0) {
                    btn["image"]["type"] = "url";
                }
                else {
                    btn["image"]["type"] = "path";
                }
                btn["image"]["data"] = icon;
            }
            json["buttons"].push_back(btn);
        }
        return json;
    }

private:
    static nlohmann::json toJson(const Message &text)
    {
        return std::visit(entt::overloaded{[&](const std::string &arg) -> nlohmann::json { return arg; },
                                           [&](const Translatable &arg) -> nlohmann::json {
                                               nlohmann::json json;
                                               json["rawtext"].push_back({
                                                   {"translate", arg.getTranslationKey()},
                                                   {"with", arg.getParameters()},
                                               });
                                               return json;
                                           }},
                          text);
    }
};

}  // namespace endstone::detail
