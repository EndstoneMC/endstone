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

#include <string>

#include "bedrock/core/string/string_hash.h"

struct ActorDefinitionIdentifier {
public:
    static constexpr char NAMESPACE_SEPARATOR = ':';
    static constexpr char EVENT_BEGIN = '<';
    static constexpr char EVENT_END = '>';
    ActorDefinitionIdentifier() = default;
    explicit ActorDefinitionIdentifier(const std::string &full_name);
    void initialize(const std::string &full_name);
    [[nodiscard]] const std::string &getNamespace() const;
    [[nodiscard]] const std::string &getIdentifier() const;
    [[nodiscard]] const std::string &getInitEvent() const;
    [[nodiscard]] const std::string &getCanonicalName() const;
    [[nodiscard]] const HashedString &getCanonicalHash() const;
    [[nodiscard]] const std::string &getFullName() const;
    [[nodiscard]] bool isVanilla() const;

private:
    void _initialize();
    static void _extractIdentifier(const std::string &name, ActorDefinitionIdentifier &id);

    std::string namespace_;                 // +0
    std::string identifier_;                // +24
    std::string init_event_;                // +48
    std::string full_name_;                 // +72
    HashedString canonical_name_{nullptr};  // +96
};
