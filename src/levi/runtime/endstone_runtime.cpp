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

#include <ll/api/io/Sink.h>
#include <ll/api/mod/NativeMod.h>
#include <ll/api/mod/RegisterHelper.h>
#include <ll/api/utils/StringUtils.h>
#include <pybind11/embed.h>

#include "endstone/core/devtools/devtools.h"
#include "endstone/runtime/hook.h"

namespace endstone {

class McColorSink : public ll::io::SinkBase {
    std::vector<std::shared_ptr<ll::io::SinkBase>> sinks;

public:
    template <class R>
    McColorSink(R &&range)
    {
        for (auto &sink : std::forward<R>(range)) {
            sinks.emplace_back(sink.shared_from_this());
        }
    }

    virtual void append(ll::io::LogMessageView const &view)
    {
        ll::io::LogMessage msg(ll::string_utils::replaceMcToAnsiCode(view.msg), view.tit, view.lvl, view.tm);
        for (auto &sink : sinks) {
            sink->append(msg);
        }
    }

    virtual void flush()
    {
        for (auto &sink : sinks) {
            sink->flush();
        }
    }

    virtual void setFormatter(ll::Polymorphic<ll::io::Formatter> fmter)
    {
        for (auto &sink : sinks) {
            sink->setFormatter(fmter);
        }
    }

    virtual void setFlushLevel(ll::io::LogLevel l)
    {
        for (auto &sink : sinks) {
            sink->setFlushLevel(l);
        }
    }
};

class EndstoneRuntime {

public:
    static EndstoneRuntime &getInstance();
    EndstoneRuntime() : mSelf(*ll::mod::NativeMod::current())
    {
        auto &logger = getSelf().getLogger();
        auto sink = std::make_shared<McColorSink>(getSelf().getLogger().sinks());
        logger.clearSink();
        logger.addSink(std::move(sink));
    }
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
        logger.info("PyConfig_InitIsolatedConfig...");
        py::initialize_interpreter(&config);
        logger.info("initialize_interpreter...");

        py::module_::import("threading");  // https://github.com/pybind/pybind11/issues/2197
        logger.info("threading...");
        py::module_::import("numpy");  // https://github.com/numpy/numpy/issues/24833
        logger.info("numpy...");
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
    return true;
}

bool EndstoneRuntime::enable()
{
    getSelf().getLogger().debug("Enabling...");
    // Code for enabling the mod goes here.
    return true;
}

bool EndstoneRuntime::disable()
{
    getSelf().getLogger().debug("Disabling...");
    // Code for disabling the mod goes here.
    return true;
}

}  // namespace endstone

LL_REGISTER_MOD(endstone::EndstoneRuntime, endstone::EndstoneRuntime::getInstance());
