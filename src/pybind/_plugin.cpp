//
// Created by Vincent on 17/08/2023.
//

#include "pybind.h"

/*
 * All the plugin-related python trampoline classes are removed since the following commit
 * https://github.com/EndstoneMC/endstone/commit/c579df43332abf9c7f73553648ca47e34256fec7
 * Record here in case we need them back in the future...
 */

PYBIND11_MODULE(_plugin, m) {}
