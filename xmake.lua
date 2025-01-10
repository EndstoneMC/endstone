add_rules("mode.debug", "mode.release")

add_repositories("LiteLDev-repo https://github.com/LiteLDev/xmake-repo.git")

add_requires("sentry-native")
add_requires("libbase64")
add_requires("boost 1.85.0")
add_requires("moodycamelconqueue")
add_requires("cpptrace 0.7.1")
add_requires("date 3.0.3")
add_requires("pybind11 2.13.6")
add_requires("toml++ v3.4.0")
add_requires("python 3.12.3")

add_requires("levilamina 42b75be796e8481394a8dd380c2500f76287919f")
add_requires("levibuildscript")

-- Define common packages to avoid repetition
local common_packages = {
    "libbase64", "boost", "moodycamelconqueue",
    "cpptrace", "date", "toml++", "sentry-native", "levilamina"
}

local get_version = function (oss)
        local tags = oss.iorun("git describe --tags --long")
        local tag, num_commits, commit_hash = tags:match("v?(%S+)-(%d+)-g([a-f0-9]+)")
        if tonumber(num_commits) > 0 then
            tag = tag:match("(%S+)%.%d+")
            tag = tag.."-dev"..num_commits
        end
        return tag
    end

set_project("endstone")
set_languages("c++20")
set_runtimes("MD")
add_defines("PYBIND11_DETAILED_ERROR_MESSAGES", "NOMINMAX", "ENDSTONE_LL")

target("endstone_headers")
    set_kind("headeronly")
    set_languages("c++20")
    add_includedirs("include", "src", "$(buildir)/generated", {public = true})
    add_headerfiles("include/(**.h)")
    on_load(function (target)
        local toml = import("scripts.toml")
        local symbols = toml.parse(io.readfile( "src/bedrock/symbol_generator/symbols.toml"))[target:plat()]
        local count = 0
        for _ in pairs(symbols) do count = count + 1 end
        local file = assert(io.open("$(buildir)/generated/bedrock_symbols.generated.h", "w"), "Failed to open symbol file")
        file:write("#pragma once\n\n")
        file:write("// clang-format off\n")
        file:write("#include <array>\n")
        file:write("#include <string_view>\n\n")
        file:write("static constexpr std::array<std::pair<std::string_view, std::size_t>, ")
        file:write(count)
        file:write("> symbols = {{\n")
        for k, v in pairs(symbols) do
            file:print("    { \"%s\", %d },", k, v)
        end
        file:write("}};\n")
        file:write("// clang-format on\n")
        file:close()
    end)

target("endstone_python")
    add_rules("python.library")
    add_files("src/levi/memory_operators.cpp")
    add_files("src/endstone/python/**.cpp")
    add_deps("endstone_headers")
    add_packages(common_packages)
    add_packages("python", "pybind11", {links = "python312"})
    set_symbols("debug")
    after_build(function (target)
        local file = assert(io.open("endstone/_internal/version.py", "w"))
        file:write("TYPE_CHECKING = False\n")
        file:write("if TYPE_CHECKING:\n")
        file:write("    from typing import Tuple, Union\n")
        file:write("    VERSION_TUPLE = Tuple[Union[int, str], ...]\n")
        file:write("else:\n")
        file:write("    VERSION_TUPLE = object\n\n")
        file:write("version: str\n")
        file:write("__version__: str\n")
        file:write("__version_tuple__: VERSION_TUPLE\n")
        file:write("version_tuple: VERSION_TUPLE\n\n")
        local version = get_version(os)
        file:print("__version__ = version = '%s'", version)
        local major, minor, patch, suffix = version:match("(%d+)%.(%d+)%.(%d+)(.*)")
        file:printf("__version_tuple__ = version_tuple = (%d, %d, %d", major, minor, patch)
        if suffix == nil then
            file:print(")")
        else
            file:print(", '%s')", suffix:sub(2))
        end
        file:close()
        os.cp("endstone", "bin/EndstoneRuntime/Lib")
        os.cp(target:targetfile(), "bin/EndstoneRuntime/Lib/endstone/_internal")
    end)
    

target("endstone_core")
    set_kind("static")
    set_languages("c++20")
    add_files("src/bedrock/**.cpp")
    add_files("src/endstone/core/**.cpp")
    add_files("src/levi/core/**.cpp")
    remove_files("src/endstone/core/devtools/**.cpp")
    remove_files("src/endstone/core/spdlog/**.cpp")
    remove_files("src/endstone/core/logger_factory.cpp")
    add_deps("endstone_headers")
    add_packages(common_packages)
    add_packages("python", "pybind11", {links = "python312"})
    on_load(function (target)
        target:add("defines", "ENDSTONE_VERSION=\""..get_version(os).."\"")
    end)

target("endstone_runtime")
    set_kind("shared")
    on_load(function (target)
        target:add("rules", "@levibuildscript/modpacker", {modName = "EndstoneRuntime", modVersion = get_version(os)})
    end)
    add_files("src/levi/memory_operators.cpp")
    add_files("src/endstone/runtime/windows.cpp")
    add_files("src/endstone/runtime/bedrock_hooks/**.cpp")
    remove_files("src/endstone/runtime/bedrock_hooks/bedrock_event_logger.cpp")
    add_files("src/levi/runtime/**.cpp")
    add_deps("endstone_headers", "endstone_core")
    add_packages(common_packages)
    add_packages("python", "pybind11", {links = "python312"})
    add_links("dbghelp", "ws2_32","Psapi")
    set_symbols("debug")
