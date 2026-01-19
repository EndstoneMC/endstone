// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "endstone/core/logger_factory.h"

#include <mutex>
#include <string>
#include <unordered_map>

#include <spdlog/spdlog.h>

#include "endstone/core/spdlog/console_log_sink.h"
#include "endstone/core/spdlog/file_log_sink.h"
#include "endstone/core/spdlog/spdlog_adapter.h"

namespace endstone::core {

Logger &LoggerFactory::getLogger(const std::string &name)
{
    static std::mutex mutex;
    static std::unordered_map<std::string, SpdLogAdapter> loggers;

    std::scoped_lock<std::mutex> lock(mutex);
    auto it = loggers.find(name);
    if (it != loggers.end()) {
        return it->second;
    }

    static std::vector<spdlog::sink_ptr> sinks = {
        std::make_shared<ConsoleLogSink>(stdout),
        std::make_shared<FileLogSink>("logs/latest.log", "logs/{:%Y-%m-%d}-{}.log", 1000)};

    auto console = std::make_shared<spdlog::logger>(name, std::begin(sinks), std::end(sinks));
    it = loggers.emplace(name, SpdLogAdapter(console)).first;
    return it->second;
}

}  // namespace endstone::core
