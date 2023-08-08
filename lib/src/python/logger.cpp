//
// Created by Vincent on 07/08/2023.
//

#include <memory>

#include "endstone.h"
#include "python/logger.h"

void Logger::log(LogLevel level, char const *format, ...) const
{
    if (level >= level_)
    {
        va_list args;
        va_start(args, format);
        BedrockLog::log_va(BedrockLog::LogCategory::All,  // 0
                           1,                             // 1
                           BedrockLog::LogRule::Default,  // 0
                           BedrockLog::LogAreaID::Server, // 12
                           level,
                           __FUNCTION__,
                           __LINE__,
                           format,
                           args);
        va_end(args);
    }
}

void Logger::setLevel(LogLevel level)
{
    level_ = level;
}

std::shared_ptr<Logger> Logger::getLogger(const std::string &name)
{
    return std::make_shared<Logger>(name);
}

void Logger::verbose(const char *format, ...) const
{
    log(LogLevel::Verbose, format);
}

void Logger::info(const char *format, ...) const
{
    log(LogLevel::Info, format);
}

void Logger::warning(const char *format, ...) const
{
    log(LogLevel::Warning, format);
}

void Logger::error(const char *format, ...) const
{
    log(LogLevel::Error, format);
}

void PyLogger::log(LogLevel level, const char *format, ...) const
{
    PYBIND11_OVERRIDE_PURE(void,   // Return type
                           Logger, // Parent class
                           log,    // Function name in C++
    );
}

namespace LoggerWrapper
{
void log(const Logger &logger, LogLevel level, const char *format)
{
    logger.log(level, format);
}

void verbose(const Logger &logger, const char *format)
{
    logger.verbose(format);
}

void info(const Logger &logger, const char *format)
{
    logger.info(format);
}

void warning(const Logger &logger, const char *format)
{
    logger.warning(format);
}

void error(const Logger &logger, const char *format)
{
    logger.error(format);
}
} // namespace LoggerWrapper

PYBIND11_MODULE(_logger, m)
{
    py::enum_<LogLevel>(m, "LogLevel")
        .value("VERBOSE", LogLevel::Verbose)
        .value("INFO", LogLevel::Info)
        .value("WARNING", LogLevel::Warning)
        .value("ERROR", LogLevel::Error)
        .export_values();

    py::class_<Logger, PyLogger, std::shared_ptr<Logger>>(m, "Logger")
        .def(py::init<std::string>())
        .def("log", &LoggerWrapper::log, py::arg("level"), py::arg("msg"))
        .def("verbose", &LoggerWrapper::verbose, py::arg("msg"))
        .def("info", &LoggerWrapper::info, py::arg("msg"))
        .def("warning", &LoggerWrapper::warning, py::arg("msg"))
        .def("error", &LoggerWrapper::error, py::arg("msg"))
        .def("set_level", &Logger::setLevel, py::arg("level"))
        .def_static("get_logger", &Logger::getLogger);
}
