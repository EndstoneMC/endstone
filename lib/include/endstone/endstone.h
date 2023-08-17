//
// Created by Vincent on 17/08/2023.
//

#ifndef ENDSTONE_ENDSTONE_H
#define ENDSTONE_ENDSTONE_H

#include "common.h"
#include "server.h"

class Endstone
{
  public:
    ENDSTONE_API static std::string getVersion();
    ENDSTONE_API static std::string getMinecraftVersion();
    ENDSTONE_API static Server &getServer();
    ENDSTONE_API static void setServer(std::unique_ptr<Server> server);

  private:
    static std::unique_ptr<Server> server_;
};

#endif // ENDSTONE_ENDSTONE_H
