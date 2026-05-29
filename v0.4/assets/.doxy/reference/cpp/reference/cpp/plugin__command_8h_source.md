

# File plugin\_command.h

[**File List**](files.md) **>** [**command**](dir_5c7b2dbfabcd1115569d1e20a260545c.md) **>** [**plugin\_command.h**](plugin__command_8h.md)

[Go to the documentation of this file](plugin__command_8h.md)


```C++
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

#include "endstone/command/command.h"
#include "endstone/command/command_executor.h"
#include "endstone/plugin/plugin.h"

namespace endstone {

class PluginCommand : public Command {
public:
    PluginCommand(const Command &command, Plugin &owner) : Command(command), owner_(owner) {}

    bool execute(CommandSender &sender, const std::vector<std::string> &args) const override
    {
        if (!owner_.isEnabled()) {
            sender.sendMessage("Cannot execute command '{}' in plugin {}. Plugin is disabled.", getName(),
                               getPlugin().getDescription().getFullName());
            return false;
        }

        if (!testPermission(sender)) {
            return true;
        }

        try {
            return getExecutor().onCommand(sender, *this, args);
        }
        catch (std::exception &e) {
            getPlugin().getLogger().error("Unhandled exception executing command '{}' in plugin {}", getName(),
                                          owner_.getDescription().getFullName());
            getPlugin().getLogger().error(e.what());
            return false;
        }
    }

    virtual void setExecutor(std::shared_ptr<CommandExecutor> executor)
    {
        executor_ = std::move(executor);
    }

    [[nodiscard]] virtual CommandExecutor &getExecutor() const
    {
        if (executor_) {
            return *executor_;
        }
        return owner_;
    }

    [[maybe_unused]] [[nodiscard]] Plugin &getPlugin() const
    {
        return owner_;
    }

    [[nodiscard]] PluginCommand *asPluginCommand() override
    {
        return this;
    }

private:
    Plugin &owner_;
    std::shared_ptr<CommandExecutor> executor_;
};
}  // namespace endstone
```


