//
// Created by Vincent on 17/08/2023.
//

#ifndef ENDSTONE_ENDSTONE_H
#define ENDSTONE_ENDSTONE_H

#include "common.h"
#include "server.h"

class Endstone {
public:
    static std::string getVersion();
    static std::string getMinecraftVersion();
    static Server &getServer();
    static void setServer(std::unique_ptr<Server> server);

private:
    static std::unique_ptr<Server> server_;
};

#endif // ENDSTONE_ENDSTONE_H
