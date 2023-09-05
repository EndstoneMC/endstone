//
// Created by Vincent on 05/09/2023.
//

#ifndef ENDSTONE_SYM_H
#define ENDSTONE_SYM_H

namespace endstone::sym {

struct internals {
    std::unordered_map<std::string, void *> symbol_map{};
};

inline internals &get_internals()
{
    static internals i{};
    return i;
}

} // namespace endstone::sym

#ifdef _WIN32
#include <Windows.h>
//
#include <DbgHelp.h>
#include <Psapi.h>

namespace endstone::sym {
inline void sym_initialize(void *handle, int options = 0, const char *search_path = nullptr,
                           bool invade_process = false)
{
    SymSetOptions(options);

    if (!SymInitialize(handle, search_path, invade_process)) {
        throw std::system_error(static_cast<int>(GetLastError()), std::system_category(), "SymInitialize failed");
    }

    auto internals = get_internals();

    wchar_t image_name[MAX_PATH];
    auto len = GetModuleFileNameEx(handle, nullptr, image_name, MAX_PATH);
    if (len == 0 || len == MAX_PATH) {
        throw std::system_error(static_cast<int>(GetLastError()), std::system_category(), "GetModuleFileNameEx failed");
    }

    auto module_base = SymLoadModuleExW(handle, nullptr, image_name, nullptr, 0, 0, nullptr, 0);
    if (!module_base) {
        throw std::system_error(static_cast<int>(GetLastError()), std::system_category(), "SymLoadModuleExW failed");
    }

    SymEnumSymbols(
        handle, 0, "*!*",
        [](auto info, auto size, auto user_context) -> auto {
            // std::string name;
            // if (SymGetOptions() & SYMOPT_UNDNAME) {
            //     name = info->Name;
            // }
            // else {
            //     name.reserve(512);
            //     if (!UnDecorateSymbolName(info->Name, name.data(), name.capacity(), UNDNAME_COMPLETE)) {
            //         throw std::system_error(static_cast<int>(GetLastError()), std::system_category(),
            //                                 "UnDecorateSymbolName failed");
            //     }
            // }
            auto internals = static_cast<sym::internals *>(user_context);
            internals->symbol_map.insert({std::string(info->Name), static_cast<char *>(nullptr) + info->Address});
            return 1;
        },
        &internals);

    MODULEINFO mi = {nullptr};
    if (!GetModuleInformation(GetCurrentProcess(), GetModuleHandle(nullptr), &mi, sizeof(mi))) {
        throw std::system_error(static_cast<int>(GetLastError()), std::system_category(),
                                "GetModuleInformation failed");
    }

    for (auto &item : internals.symbol_map) {
        item.second = static_cast<char *>(mi.lpBaseOfDll) + (reinterpret_cast<size_t>(item.second) - module_base);
    }
}

inline void sym_cleanup(void *handle)
{
    SymCleanup(handle);
}

inline void *sym_from_name(const char *name)
{
    static std::mutex mtx;
    std::lock_guard<std::mutex> lock(mtx);

    static std::unordered_map<std::string, void *> cache;
    auto name_str = std::string(name);
    auto it = cache.find(name_str);
    if (it != cache.end()) {
        return it->second;
    }

    return nullptr;
}

} // namespace endstone::sym
#endif

namespace endstone {

class symbol_handler {
public:
    explicit symbol_handler(void *handle = ((void *)-1l), int options = 0, const char *search_path = nullptr,
                            bool invade_process = false)
        : handle_(handle)
    {
        sym::sym_initialize(handle, options, search_path, invade_process);
    }

    symbol_handler(const symbol_handler &) = delete;
    symbol_handler(symbol_handler &&other) noexcept
    {
        other.valid_ = false;
    }
    symbol_handler &operator=(const symbol_handler &) = delete;
    symbol_handler &operator=(symbol_handler &&) = delete;

    ~symbol_handler()
    {
        if (valid_) {
            sym::sym_cleanup(handle_);
        }
    }

private:
    bool valid_ = true;
    void *handle_ = nullptr;
};

} // namespace endstone

#endif // ENDSTONE_SYM_H
