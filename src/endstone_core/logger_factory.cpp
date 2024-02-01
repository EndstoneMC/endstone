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

#include "endstone_core/logger_factory.h"

#include <mutex>
#include <string>
#include <unordered_map>

#include "endstone_core/spdlog/bedrock_log_sink.h"
#include "endstone_core/spdlog/spdlog_adapter.h"

Logger &LoggerFactory::getLogger(const std::string &name)
{
    static std::mutex mutex;
    static std::unordered_map<std::string, SpdLogAdapter> loggers;

    std::scoped_lock<std::mutex> lock(mutex);
    auto it = loggers.find(name);
    if (it != loggers.end()) {
        return it->second;
    }

    spdlog::sink_ptr console_sink = std::make_shared<BedrockLogSink>(stdout);
    auto console = std::make_shared<spdlog::logger>(name, console_sink);
    spdlog::register_logger(console);
    it = loggers.emplace(name, SpdLogAdapter(console)).first;
    return it->second;
}
