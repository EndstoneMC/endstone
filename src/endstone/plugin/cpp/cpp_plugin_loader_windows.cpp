//
// Created by Vincent on 21/08/2023.
//

#ifdef _WIN32

#include "endstone/plugin/cpp/cpp_plugin_loader.h"
#include "endstone/plugin/plugin.h"
#include "endstone/plugin/plugin_logger.h"

#include <Windows.h>

std::unique_ptr<Plugin> CppPluginLoader::loadPlugin(const std::string &file)
{
    HMODULE module = LoadLibraryA(file.c_str());
    if (!module) {
        throw std::system_error(GetLastError(), std::system_category(), "LoadLibrary failed");
    }

    using fp = Plugin *(*)();
    auto func = GetProcAddress(module, "createPlugin");

    if (!func) {
        FreeLibrary(module);
        throw std::runtime_error("Failed to find createPlugin function in DLL: " + file +
                                 ". Did you forget ENDSTONE_PLUGIN_CLASS?");
    }

    auto createPlugin = reinterpret_cast<fp>(func);
    auto plugin = createPlugin();

    if (!plugin) {
        FreeLibrary(module); // First, free the loaded library to clean up resources.
        throw std::runtime_error("Failed to create a plugin instance from DLL: " + file + ". Invalid plugin instance.");
    }

    initPlugin(*plugin, std::make_shared<PluginLogger>(*plugin));
    return std::unique_ptr<Plugin>(plugin);
}

std::vector<std::string> CppPluginLoader::getPluginFileFilters() const noexcept
{
    return {"\\.dll$"};
}

#endif //_WIN32
