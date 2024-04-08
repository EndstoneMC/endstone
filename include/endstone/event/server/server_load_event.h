// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#pragma once

#include <string>

#include "endstone/command/command_sender.h"
#include "endstone/event/event.h"

namespace endstone {

class ServerLoadEvent : public Event {
public:
    enum class LoadType {
        Startup,  // TODO(event): add Reload
    };

    explicit ServerLoadEvent(LoadType type) : type_(type) {}

    [[nodiscard]] LoadType getType() const
    {
        return type_;
    }

    inline static const std::string NAME = "ServerLoadEvent";
    [[nodiscard]] std::string getEventName() const override
    {
        return NAME;
    }

    [[nodiscard]] bool isCancellable() const override
    {
        return false;
    }

private:
    LoadType type_;
};

}  // namespace endstone
