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

#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include <gsl/gsl>

#include "bedrock/bedrock.h"
#include "bedrock/core/memory.h"
#include "bedrock/core/utility/non_owner_pointer.h"
#include "bedrock/forward.h"

class I18n {
public:
    virtual ~I18n() = 0;
    virtual void clearLanguages() = 0;
    virtual std::vector<std::string> findAvailableLanguages(ResourcePackManager &) = 0;
    virtual std::unordered_map<std::string, std::string> findAvailableLanguageNames(ResourcePackManager &) = 0;
    virtual void loadLanguages(ResourcePackManager &, gsl::not_null<Bedrock::NonOwnerPointer<ResourceLoadManager>>,
                               std::string const &) = 0;
    virtual void loadAllLanguages(ResourcePackManager &) = 0;
    virtual std::vector<std::string> getLanguageCodesFromPack(PackAccessStrategy const &) = 0;
    virtual void loadLanguageKeywordsFromPack(PackManifest const &, PackAccessStrategy const &) = 0;
    virtual void loadLanguageKeywordsFromPack(PackManifest const &, PackAccessStrategy const &,
                                              std::vector<std::string> const &) = 0;
    virtual void appendLanguageStringsFromPack(
        PackManifest const &, std::multimap<std::string, std::pair<std::string, std::string>> const &) = 0;
    virtual std::unordered_map<std::string, std::string> getLanguageKeywordsFromPack(PackManifest const &,
                                                                                     std::string const &) = 0;
    virtual void loadLanguagesByLocale(
        std::unordered_multimap<std::string, std::pair<std::string, std::string>> const &) = 0;
    virtual void appendAdditionalTranslations(std::unordered_map<std::string, std::string> const &,
                                              std::string const &) = 0;
    virtual void appendLanguageStrings(PackAccessStrategy *) = 0;
    virtual void appendTranslations(std::string const &, std::unordered_map<std::string, std::string> const &) = 0;
    virtual void addI18nObserver(I18nObserver &) = 0;
    virtual void chooseLanguage(std::string const &) = 0;
    virtual std::string get(std::string const &, std::vector<std::string> const &, std::shared_ptr<Localization>) = 0;
    virtual std::string get(std::string const &, std::shared_ptr<Localization>) = 0;
    virtual std::string getPackKeywordValue(PackManifest const &, std::string const &) = 0;
    virtual std::string getPackKeywordValueForTelemetry(PackManifest const &, std::string const &) = 0;
    virtual bool hasPackKeyEntry(PackManifest const &, std::string const &) = 0;
    [[nodiscard]] virtual std::string const &getSupportedLanguageCodes() const = 0;
    virtual std::string const &getLanguageName(std::string const &) = 0;
    virtual std::shared_ptr<Localization> getLocaleFor(std::string const &) = 0;
    virtual std::string const &getLocaleCodeFor(std::string const &) = 0;
    virtual optional_ref<Localization const> getCurrentLanguage() = 0;
    virtual bool languageSupportsHypenSplitting() = 0;
    virtual std::string getLocalizedAssetFileWithFallback(std::string const &, std::string const &) = 0;
    virtual bool isPackKeyword(std::string const &) = 0;
};

ENDSTONE_HOOK I18n &getI18n();
