//
// Created by Vincent on 31/08/2023.
//

#include "pybind.h"

PYBIND11_MODULE(_bindings, m)
{
    export_plugin(m);
    export_logger(m);
    export_command(m);
    export_chat_color(m);
}
