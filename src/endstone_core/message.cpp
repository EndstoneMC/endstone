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

#include "endstone/detail/message.h"

#include "bedrock/locale/i18n.h"
#include "endstone/endstone.h"

namespace endstone::detail {

std::string EndstoneMessage::toString(Message message)
{
    return std::visit(overloaded{[](const std::string &string) { return string; },
                                 [](const Translatable &tr) {
                                     return getI18n().get(tr.getTranslate(), tr.getWith(), nullptr);
                                 }},
                      message);
}

Translatable EndstoneMessage::toTranslatable(Message message)
{
    return std::visit(overloaded{[](const std::string &string) {
                                     return Translatable{string, {}};
                                 },
                                 [](const Translatable &tr) {
                                     return tr;
                                 }},
                      message);
}

}  // namespace endstone::detail
