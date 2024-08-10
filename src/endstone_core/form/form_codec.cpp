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

#include "endstone/detail/form/form_codec.h"

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

#include "endstone/form/action_form.h"
#include "endstone/form/controls/dropdown.h"
#include "endstone/form/controls/label.h"
#include "endstone/form/controls/slider.h"
#include "endstone/form/controls/step_slider.h"
#include "endstone/form/controls/text_input.h"
#include "endstone/form/controls/toggle.h"
#include "endstone/form/message_form.h"
#include "endstone/form/modal_form.h"
#include "endstone/message.h"

namespace endstone::detail {

template <>
nlohmann::json FormCodec::toJson(const Message &message)
{
    return std::visit(entt::overloaded{[&](const std::string &arg) -> nlohmann::json {
                                           nlohmann::json json;
                                           json["rawtext"].push_back({
                                               {"text", arg},
                                           });
                                           return json;
                                       },
                                       [&](const Translatable &arg) -> nlohmann::json {
                                           nlohmann::json json;
                                           json["rawtext"].push_back({
                                               {"translate", arg.getTranslationKey()},
                                               {"with", arg.getParameters()},
                                           });
                                           return json;
                                       }},
                      message);
}

/**
 * Controls
 */
template <>
nlohmann::json FormCodec::toJson(const Label &label)
{
    nlohmann::json json;
    json["type"] = "label";
    json["text"] = toJson(label.getText());
    return json;
}

template <>
nlohmann::json FormCodec::toJson(const Dropdown &dropdown)
{
    nlohmann::json json;
    json["type"] = "dropdown";
    json["text"] = toJson(dropdown.getLabel());
    json["options"] = dropdown.getOptions();
    if (auto default_index = dropdown.getDefaultIndex()) {
        json["default"] = default_index.value();
    }
    return json;
}

template <>
nlohmann::json FormCodec::toJson(const Slider &slider)
{
    nlohmann::json json;
    json["type"] = "slider";
    json["text"] = toJson(slider.getLabel());
    json["min"] = slider.getMin();
    json["max"] = slider.getMax();
    json["step"] = slider.getStep();
    if (auto default_value = slider.getDefaultValue()) {
        json["default"] = default_value.value();
    }
    return json;
}

template <>
nlohmann::json FormCodec::toJson(const StepSlider &slider)
{
    nlohmann::json json;
    json["type"] = "step_slider";
    json["text"] = toJson(slider.getLabel());
    json["steps"] = slider.getOptions();
    if (auto default_index = slider.getDefaultIndex()) {
        json["default"] = default_index.value();
    }
    return json;
}

template <>
nlohmann::json FormCodec::toJson(const TextInput &input)
{
    nlohmann::json json;
    json["type"] = "input";
    json["text"] = toJson(input.getLabel());
    json["placeholder"] = toJson(input.getPlaceholder());
    if (auto default_value = input.getDefaultValue()) {
        json["default"] = default_value.value();
    }
    return json;
}

template <>
nlohmann::json FormCodec::toJson(const Toggle &toggle)
{
    nlohmann::json json;
    json["type"] = "toggle";
    json["text"] = toJson(toggle.getLabel());
    json["default"] = toggle.getDefaultValue();
    return json;
}

/**
 * Forms
 */
template <>
nlohmann::json FormCodec::toJson(const MessageForm &form)
{
    nlohmann::json json;
    json["type"] = "modal";
    json["title"] = toJson(form.getTitle());
    json["content"] = toJson(form.getContent());
    json["button1"] = toJson(form.getButton1());
    json["button2"] = toJson(form.getButton2());
    return json;
}

template <>
nlohmann::json FormCodec::toJson(const ActionForm::Button &button)
{
    nlohmann::json json;
    json["text"] = toJson(button.getText());
    if (auto icon = button.getIcon(); icon.has_value()) {
        if (icon.value().rfind("http://", 0) == 0 || icon.value().rfind("https://", 0) == 0) {
            json["image"]["type"] = "url";
        }
        else {
            json["image"]["type"] = "path";
        }
        json["image"]["data"] = icon.value();
    }
    return json;
}

template <>
nlohmann::json FormCodec::toJson(const ActionForm &form)
{
    nlohmann::json json;
    json["type"] = "form";
    json["title"] = toJson(form.getTitle());
    json["content"] = toJson(form.getContent());
    json["buttons"] = nlohmann::json::array();
    for (const auto &button : form.getButtons()) {
        json["buttons"].push_back(toJson(button));
    }
    return json;
}

template <>
nlohmann::json FormCodec::toJson(const ModalForm &form)
{
    nlohmann::json json;
    json["type"] = "custom_form";
    json["title"] = toJson(form.getTitle());
    json["content"] = nlohmann::json::array();
    for (const auto &control : form.getControls()) {
        json["content"].push_back(std::visit(entt::overloaded{[](auto &&arg) {
                                                 return toJson(arg);
                                             }},
                                             control));
    }

    if (auto submit_button = form.getSubmitButton(); submit_button.has_value()) {
        json["submit"] = toJson(submit_button.value());
    }

    if (auto icon = form.getIcon(); icon.has_value()) {
        if (icon.value().rfind("http://", 0) == 0 || icon.value().rfind("https://", 0) == 0) {
            json["icon"]["type"] = "url";
        }
        else {
            json["icon"]["type"] = "path";
        }
        json["icon"]["data"] = icon.value();
    }
    return json;
}

}  // namespace endstone::detail
