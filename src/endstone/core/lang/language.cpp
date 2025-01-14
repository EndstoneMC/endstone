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

#include "endstone/core/lang/language.h"

#include "bedrock/locale/i18n.h"

namespace endstone::core {

std::string EndstoneLanguage::translate(std::string text) const
{
    return translate(text, getLocale());
}

std::string EndstoneLanguage::translate(std::string text, std::string locale) const
{
    return translate(text, {}, locale);
}

std::string EndstoneLanguage::translate(std::string text, std::vector<std::string> params) const
{
    return translate(text, params, getLocale());
}

std::string EndstoneLanguage::translate(std::string text, std::vector<std::string> params, std::string locale) const
{
    auto &i18n = getI18n();
    auto localization = i18n.getLocaleFor(locale);
    if (!localization) {
        localization = i18n.getLocaleFor(getLocale());
    }
    return i18n.get(text, params, localization);
}

std::string EndstoneLanguage::translate(Translatable translatable) const
{
    return translate(translatable, getLocale());
}

std::string EndstoneLanguage::translate(Translatable translatable, std::string locale) const
{
    return translate(translatable.getText(), translatable.getParameters(), locale);
}

std::string EndstoneLanguage::getLocale() const
{
    return getI18n().getCurrentLanguage()->getLanguageCode();
}

}  // namespace endstone::core
