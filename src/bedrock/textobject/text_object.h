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

#include "bedrock/world/actor/actor.h"
#include "bedrock/world/scores/scoreboard.h"

struct ResolveData {
    ResolveData(const Actor &actor, const Scoreboard &scoreboard);
    const gsl::not_null<const Actor *> actor;
    const gsl::not_null<const Scoreboard *> scoreboard;
};
static_assert(sizeof(ResolveData) == 16);

class ITextObject {
public:
    static const std::string RAW_TEXT_OBJECT_KEY;
    static const std::string RAW_TEXT_TEXT_KEY;
    static const std::string RAW_TEXT_TRANSLATE_KEY;
    static const std::string RAW_TEXT_WITH_KEY;
    virtual ~ITextObject() = default;
    [[nodiscard]] virtual std::string asString() const = 0;
    [[nodiscard]] virtual Json::Value asJsonValue() const = 0;
    [[nodiscard]] virtual Json::Value resolve(const ResolveData &) const = 0;
};
static_assert(sizeof(ITextObject) == 8);

class TextObjectText;
class TextObjectLocalizedText;

class TextObjectRoot : public ITextObject {
public:
    TextObjectRoot();

    template <typename T, typename... Args>
    void addChild(Args &&...args)
    {
        addChild(std::make_unique<T>(std::forward<Args>(args)...));
    }

    void addChild(std::unique_ptr<ITextObject> child)
    {
        children_.push_back(std::move(child));
    }

    [[nodiscard]] std::string asString() const override;
    [[nodiscard]] Json::Value asJsonValue() const override;
    [[nodiscard]] Json::Value resolve(const ResolveData &resolve_data) const override;

    void clear();
    [[nodiscard]] bool isEmpty() const;

private:
    std::vector<std::unique_ptr<ITextObject>> children_;
};
static_assert(sizeof(TextObjectRoot) == 32);

class TextObjectText : public ITextObject {
public:
    explicit TextObjectText(std::string text);
    [[nodiscard]] std::string asString() const override;
    [[nodiscard]] Json::Value asJsonValue() const override;
    [[nodiscard]] Json::Value resolve(const ResolveData & /*unused*/) const override;

private:
    std::string text_;
};

class TextObjectLocalizedText : public ITextObject {
public:
    explicit TextObjectLocalizedText(std::string text);
    [[nodiscard]] std::string asString() const override;
    [[nodiscard]] Json::Value asJsonValue() const override;
    [[nodiscard]] Json::Value resolve(const ResolveData & /*unused*/) const override;

private:
    std::string text_;  // +8
};
