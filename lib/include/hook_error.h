//
// Created by Vincent on 31/07/2023.
//

#ifndef ENDSTONE_HOOK_ERROR_H
#define ENDSTONE_HOOK_ERROR_H

#include <system_error>

#ifdef _WIN32

#include <MinHook.h>

class MinHookErrorCategory : public std::error_category
{
    [[nodiscard]] const char *name() const noexcept override;

    [[nodiscard]] std::string message(int errVal) const override;
};

namespace minhook
{

inline std::error_code make_error_code(MH_STATUS e)
{
    static auto category = MinHookErrorCategory();
    return {static_cast<int>(e), category};
}

} // namespace minhook

#endif // _WIN32

#endif // ENDSTONE_HOOK_ERROR_H
