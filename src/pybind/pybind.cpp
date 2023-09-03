//
// Created by Vincent on 31/08/2023.
//

#include "pybind.h"

PYBIND11_MODULE(_bindings, m)
{
    def_server(m);
    def_logger(m);
    def_plugin(m);
    def_command_api(m);
    def_chat_color(m);
}
