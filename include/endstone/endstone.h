//
// Created by Vincent on 17/08/2023.
//

#pragma once

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

