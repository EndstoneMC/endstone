//
// Created by Vincent on 22/08/2023.
//

#ifndef ENDSTONE_LOGGER_FACTORY_H
#define ENDSTONE_LOGGER_FACTORY_H

#include "endstone/logger.h"

class LoggerFactory
{
  public:
    static std::shared_ptr<Logger> getLogger(const std::string &name);
};

#endif // ENDSTONE_LOGGER_FACTORY_H
