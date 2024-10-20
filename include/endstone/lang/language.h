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
#include <vector>

#include "endstone/lang/translatable.h"

namespace endstone {

/**
 * @brief An interface for translating text into different languages.
 */
class Language {
public:
    virtual ~Language() = default;

    /**
     * @brief Translates a given text into the current locale.
     *
     * @param text The text to be translated.
     * @return The translated text in the current locale.
     */
    [[nodiscard]] virtual std::string translate(std::string text) const;

    /**
     * @brief Translates a given text into a specific locale.
     *
     * @param text The text to be translated.
     * @param locale The locale identifier for the desired translation (e.g., "en_US").
     * @return The translated text in the specified locale.
     */
    [[nodiscard]] virtual std::string translate(std::string text, std::string locale) const;

    /**
     * @brief Translates a given text using a set of parameters for the current locale.
     *
     * @param text The text to be translated.
     * @param params A list of parameters to be used in the translation.
     * @return The translated text in the current locale, with parameters applied.
     */
    [[nodiscard]] virtual std::string translate(std::string text, std::vector<std::string> params) const;

    /**
     * @brief Translates a given text using a set of parameters for a specific locale.
     *
     * @param text The text to be translated.
     * @param params A list of parameters to be used in the translation.
     * @param locale The locale identifier for the desired translation (e.g., "en_US").
     * @return The translated text in the specified locale, with parameters applied.
     */
    [[nodiscard]] virtual std::string translate(std::string text, std::vector<std::string> params,
                                                std::string locale) const;

    /**
     * @brief Translates a Translatable object into the current locale.
     *
     * @param translatable A Translatable object containing text and parameters.
     * @return The translated text in the current locale.
     */
    [[nodiscard]] virtual std::string translate(Translatable translatable) const;

    /**
     * @brief Translates a Translatable object into a specific locale.
     *
     * @param translatable A Translatable object containing text and parameters.
     * @param locale The locale identifier for the desired translation (e.g., "en_US").
     * @return The translated text in the specified locale.
     */
    [[nodiscard]] virtual std::string translate(Translatable translatable, std::string locale) const;

    /**
     * @brief Gets the current locale.
     *
     * @return A string representing the current locale (e.g., "en_US").
     */
    [[nodiscard]] virtual std::string getLocale() const;
};

}  // namespace endstone
