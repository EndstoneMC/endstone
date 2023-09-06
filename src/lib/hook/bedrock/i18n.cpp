//
// Created by Vincent on 06/09/2023.
//

#include "bedrock/i18n.h"

#include "hook/hook.h"

std::string I18n::get(const std::string &message, const class Localization *language)
{
    return ENDSTONE_HOOK_CALL_ORIGINAL(&I18n::get, message, language);
}
