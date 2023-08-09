//
// Created by Vincent on 07/08/2023.
//

#include <memory>

#include "endstone.h"
#include "python/logger.h"

#define BEDROCK_LOG(level)                                                                                             \
    if (level >= level_)                                                                                               \
    {                                                                                                                  \
        va_list args;                                                                                                  \
        va_start(args, format);                                                                                        \
        BedrockLog::log_va(BedrockLog::LogCategory::All,                                                               \
                           1,                                                                                          \
                           BedrockLog::LogRule::Default,                                                               \
                           BedrockLog::LogAreaID::Server,                                                              \
                           level,                                                                                      \
                           __FUNCTION__,                                                                               \
                           __LINE__,                                                                                   \
                           format,                                                                                     \
                           args);                                                                                      \
        va_end(args);                                                                                                  \
    }

void Logger::log(LogLevel level, const char *message) const
{
    log_va(level, message);
}

void Logger::log_va(LogLevel level, char const *format, ...) const
{
    BEDROCK_LOG(level)
}

void Logger::verbose(const char *format, ...) const
{
    BEDROCK_LOG(LogLevel::Verbose)
}

void Logger::info(const char *format, ...) const
{
    BEDROCK_LOG(LogLevel::Info)
}

void Logger::warning(const char *format, ...) const
{
    BEDROCK_LOG(LogLevel::Warning)
}

void Logger::error(const char *format, ...) const
{
    BEDROCK_LOG(LogLevel::Error)
}

void Logger::setLevel(LogLevel level)
{
    level_ = level;
}

const std::string &Logger::getName() const
{
    return name_;
}

Logger &Logger::getLogger(const std::string &name)
{
    static std::map<std::string, Logger> loggers;
    static std::mutex mutex;

    std::scoped_lock<std::mutex> lock(mutex);
    auto it = loggers.find(name);
    if (it == loggers.end())
    {
        it = loggers.insert({name, Logger(name)}).first;
    }

    return it->second;
}

void PyLogger::log(LogLevel level, const char *message) const
{
    PYBIND11_OVERRIDE_NAME(void,   // Return type
                           Logger, // Parent class
                           "log",  // Name of method in Python
                           log,    // Name of method in C++
                           level,  // Argument(s)
                           message);
}

namespace LoggerWrapper
{
void log(const Logger &logger, LogLevel level, const char *message)
{
    logger.log(level, message);
}

void verbose(const Logger &logger, const char *message)
{
    log(logger, LogLevel::Verbose, message);
}

void info(const Logger &logger, const char *message)
{
    log(logger, LogLevel::Info, message);
}

void warning(const Logger &logger, const char *message)
{
    log(logger, LogLevel::Warning, message);
}

void error(const Logger &logger, const char *message)
{
    log(logger, LogLevel::Error, message);
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

    py::class_<Logger, PyLogger>(m, "Logger")
        .def(py::init<std::string>())
        .def("log", &LoggerWrapper::log, py::arg("level"), py::arg("msg"))
        .def("verbose", &LoggerWrapper::verbose, py::arg("msg"))
        .def("info", &LoggerWrapper::info, py::arg("msg"))
        .def("warning", &LoggerWrapper::warning, py::arg("msg"))
        .def("error", &LoggerWrapper::error, py::arg("msg"))
        .def("set_level", &Logger::setLevel, py::arg("level"))
        .def("get_name", &Logger::getName)
        .def_static("get_logger", &Logger::getLogger, py::return_value_policy::reference_internal);
}
