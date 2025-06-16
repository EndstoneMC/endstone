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

#include "bedrock/textobject/text_object.h"

#include <utility>

#include "bedrock/locale/i18n.h"

const std::string ITextObject::RAW_TEXT_OBJECT_KEY = "rawText";
const std::string ITextObject::RAW_TEXT_TEXT_KEY = "text";
const std::string ITextObject::RAW_TEXT_TRANSLATE_KEY = "translate";
const std::string ITextObject::RAW_TEXT_WITH_KEY = "with";

ResolveData::ResolveData(const Actor &actor, const Scoreboard &scoreboard) : actor(&actor), scoreboard(&scoreboard) {}

TextObjectRoot::TextObjectRoot() = default;

std::string TextObjectRoot::asString() const
{
    std::stringstream ss;
    for (const auto &child : children_) {
        ss << child->asString();
    }
    return ss.str();
}

Json::Value TextObjectRoot::asJsonValue() const
{
    Json::Value root(Json::objectValue);
    auto children = root[RAW_TEXT_OBJECT_KEY];
    for (const auto &child : children_) {
        children.append(child->asJsonValue());
    }
    return root;
}

Json::Value TextObjectRoot::resolve(const ResolveData &resolve_data) const
{
    Json::Value root(Json::objectValue);
    auto children = root[RAW_TEXT_OBJECT_KEY];
    for (const auto &child : children_) {
        children.append(child->resolve(resolve_data));
    }
    return root;
}

void TextObjectRoot::clear()
{
    children_.clear();
}

bool TextObjectRoot::isEmpty() const
{
    return children_.empty();
}

TextObjectText::TextObjectText(std::string text) : text_(std::move(text)) {}

std::string TextObjectText::asString() const
{
    return text_;
}

Json::Value TextObjectText::asJsonValue() const
{
    Json::Value json(Json::objectValue);
    json[RAW_TEXT_TEXT_KEY] = text_;
    return json;
}

Json::Value TextObjectText::resolve(const ResolveData & /*unused*/) const
{
    return asJsonValue();
}

TextObjectLocalizedText::TextObjectLocalizedText(std::string text) : text_(std::move(text)) {}

std::string TextObjectLocalizedText::asString() const
{
    return getI18n().get(text_, nullptr);
}

Json::Value TextObjectLocalizedText::asJsonValue() const
{
    Json::Value json(Json::objectValue);
    json[RAW_TEXT_TRANSLATE_KEY] = text_;
    return json;
}

Json::Value TextObjectLocalizedText::resolve(const ResolveData & /*unused*/) const
{
    return asJsonValue();
}
