//
// Created by Vincent on 21/08/2023.
//

#ifdef _WIN32

#include "endstone/plugin/cpp/cpp_plugin.h"
#include "endstone/plugin/cpp/cpp_plugin_loader.h"
#include "endstone/plugin/plugin_logger.h"

#include <Windows.h>

Plugin *CppPluginLoader::loadPlugin(const std::string &file) const
{
    HMODULE module = LoadLibraryA(file.c_str());
    if (!module)
    {
        throw std::system_error(GetLastError(), std::system_category(), "LoadLibrary failed");
    }

    using createPlugin_t = CppPlugin *(*)();
    auto createPlugin_addr = GetProcAddress(module, "createPlugin");

    if (!createPlugin_addr)
    {
        FreeLibrary(module);
        throw std::runtime_error("Failed to find createPlugin function in DLL: " + file +
                                 ". Did you forget ENDSTONE_PLUGIN_CLASS?");
    }

    auto createPlugin = reinterpret_cast<createPlugin_t>(createPlugin_addr);
    auto plugin = createPlugin();
    plugin->loader_ = shared_from_this();
    plugin->logger_ = std::make_shared<PluginLogger>(*plugin);
    return plugin;
}

std::vector<std::string> CppPluginLoader::getPluginFilters() const noexcept
{
    return {"\\.dll$"};
}

void CppPluginLoader::enablePlugin(Plugin &plugin) const
{
    try
    {
        auto &cpp_plugin = dynamic_cast<CppPlugin &>(plugin);
        if (!cpp_plugin.isEnabled())
        {
            cpp_plugin.getLogger()->info("Enabling {}", cpp_plugin.getDescription().getFullName());
            cpp_plugin.setEnabled(true);
        }
    }
    catch (const std::bad_cast &e)
    {
        throw std::runtime_error("Plugin is not associated with this PluginLoader");
    }
}

void CppPluginLoader::disablePlugin(Plugin &plugin) const
{
    try
    {
        auto &cpp_plugin = dynamic_cast<CppPlugin &>(plugin);
        if (cpp_plugin.isEnabled())
        {
            cpp_plugin.getLogger()->info("Disabling {}", cpp_plugin.getDescription().getFullName());
            cpp_plugin.setEnabled(false);
        }
    }
    catch (const std::bad_cast &e)
    {
        throw std::runtime_error("Plugin is not associated with this PluginLoader");
    }
}

#endif //_WIN32