//
// Created by Vincent on 06/09/2023.
//

#pragma once

#include "bedrock_common.h"

class I18n {
public:
    BEDROCK_API static std::string get(const std::string &message, class Localization const *language = nullptr);
};

