//
// Created by Vincent on 17/08/2023.
//

#include "endstone/common.h"
#include "endstone/logger.h"
#include "endstone/pybind.h"

PYBIND11_MODULE(_logger, m)
{
    py::enum_<LogLevel>(m, "LogLevel")
        .value("ALL", LogLevel::All)
        .value("VERBOSE", LogLevel::Verbose)
        .value("INFO", LogLevel::Info)
        .value("WARNING", LogLevel::Warning)
        .value("ERROR", LogLevel::Error)
        .export_values();
}