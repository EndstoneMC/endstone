//
// Created by Vincent on 17/08/2023.
//

#include "common.h"
#include "endstone/logger.h"

class PyLogger : public Logger
{
  public:
    using Logger::Logger;
    void log(LogLevel level, const char *format, ...) const override
    {
        PYBIND11_OVERRIDE_NAME(void, Logger, "log", log, level, format);
    }
};

PYBIND11_MODULE(_logger, m)
{
    py::enum_<LogLevel>(m, "LogLevel")
        .value("ALL", LogLevel::All)
        .value("VERBOSE", LogLevel::Verbose)
        .value("INFO", LogLevel::Info)
        .value("WARNING", LogLevel::Warning)
        .value("ERROR", LogLevel::Error)
        .export_values();

    py::class_<Logger, PyLogger>(m, "Logger")
        .def(py::init<std::string>())
        .def(
            "log",
            [](const PyLogger &logger, const LogLevel level, const char *msg) //
            {                                                                 //
                return logger.log(level, msg);
            },
            py::arg("level"),
            py::arg("msg"))
        .def("set_level", &Logger::setLevel, py::arg("level"))
        .def("get_name", &Logger::getName)
        .def_static("get_logger", &Logger::getLogger, py::return_value_policy::reference_internal);
}