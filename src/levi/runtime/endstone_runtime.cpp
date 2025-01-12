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

#include <memory>

#include <ll/api/mod/NativeMod.h>
#include <ll/api/mod/RegisterHelper.h>
#include <ll/api/utils/StringUtils.h>
#include <pybind11/embed.h>

#include "endstone/core/devtools/devtools.h"
#include "endstone/runtime/hook.h"

namespace endstone::hook {
void uninstall();
}  // namespace endstone::hook

namespace endstone::core {

void unload_endstone_server();
void disable_endstone_server();
void enable_endstone_server();

class EndstoneRuntime {

public:
    static EndstoneRuntime &getInstance();
    EndstoneRuntime() : mSelf(*ll::mod::NativeMod::current()) {}
    [[nodiscard]] ll::mod::NativeMod &getSelf() const
    {
        return mSelf;
    }
    bool load();
    bool enable();
    bool disable();
    // bool unload();

private:
    ll::mod::NativeMod &mSelf;
    bool enabled{};
};

EndstoneRuntime &EndstoneRuntime::getInstance()
{
    static EndstoneRuntime instance;
    return instance;
}

bool EndstoneRuntime::load()
{
    auto &logger = getSelf().getLogger();
    try {
        logger.info("Initialising...");
        namespace py = pybind11;

        // Initialise an isolated Python environment to avoid installing signal handlers
        // https://docs.python.org/3/c-api/init_config.html#init-isolated-conf
        PyConfig config;
        PyConfig_InitIsolatedConfig(&config);
        PyConfig_SetString(&config, &config.pythonpath_env, (getSelf().getModDir() / "Lib").c_str());
        config.isolated = 0;
        config.use_environment = 1;
        config.install_signal_handlers = 0;
        py::initialize_interpreter(&config);
        py::module_::import("threading");  // https://github.com/pybind/pybind11/issues/2197
        py::module_::import("numpy");      // https://github.com/numpy/numpy/issues/24833
        py::gil_scoped_release release{};
        release.disarm();

        // Install hooks
        endstone::hook::install();

#ifdef ENDSTONE_WITH_DEVTOOLS
        // Create devtools window
        auto thread = std::thread(&endstone::devtools::render);
        thread.detach();
#endif
    }
    catch (const std::exception &e) {
        logger.error("An exception occurred while initialising Endstone runtime.");
        logger.error("{}", e.what());
        throw;
    }
    enabled = true;
    return true;
}

bool EndstoneRuntime::enable()
{
    if (!enabled) {
        getSelf().getLogger().debug("Enabling...");
        enable_endstone_server();
        enabled = true;
    }

    return true;
}

bool EndstoneRuntime::disable()
{

    if (enabled) {
        getSelf().getLogger().debug("Disabling...");
        disable_endstone_server();
        enabled = false;
    }
    return true;
}

// TODO:
// bool EndstoneRuntime::unload()
// {
//     getSelf().getLogger().debug("Unloading...");
//     unload_endstone_server();
//     getSelf().getLogger().debug("uninstall hooks...");
//     hook::uninstall();
//     return true;
// }

}  // namespace endstone::core

LL_REGISTER_MOD(endstone::core::EndstoneRuntime, endstone::core::EndstoneRuntime::getInstance());
