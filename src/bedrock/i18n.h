//
// Created by Vincent on 06/09/2023.
//

#ifndef ENDSTONE_I18N_H
#define ENDSTONE_I18N_H

#include "bedrock_common.h"

class I18n {
public:
    BEDROCK_API static std::string get(const std::string &message, class Localization const *language = nullptr);
};

#endif // ENDSTONE_I18N_H
