//
// Created by Vincent on 28/08/2023.
//

#ifndef ENDSTONE_BEDROCK_COMMON_H
#define ENDSTONE_BEDROCK_COMMON_H

#include "endstone/common.h"

using CommandPermissionLevel = uint8_t;
using CommandFlag = int16_t;

#if defined(_WIN32)
#ifdef BEDROCK_EXPORTS
#define BEDROCK_API __declspec(dllexport)
#elif defined(BEDROCK_IMPORTS)
#define BEDROCK_API __declspec(dllimport)
#endif
#else
#if defined(BEDROCK_EXPORTS) || defined(BEDROCK_IMPORTS)
#if defined(__GNUC__) || defined(__clang__)
#define BEDROCK_API __attribute__((visibility("default")))
#endif
#endif
#endif

#ifndef BEDROCK_API
#define BEDROCK_API
#endif

#endif // ENDSTONE_BEDROCK_COMMON_H
