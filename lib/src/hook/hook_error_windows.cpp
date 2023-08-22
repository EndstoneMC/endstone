//
// Created by Vincent on 06/12/2022.
//

#ifdef _WIN32

#include "hook_error.h"

const char *MinHookErrorCategory::name() const noexcept
{
    return "MinHookError";
}

std::string MinHookErrorCategory::message(int errVal) const
{
    auto errorCode = static_cast<MH_STATUS>(errVal);
    switch (errorCode)
    {
    case MH_ERROR_ALREADY_INITIALIZED:
        return "MinHook is already initialized.";
    case MH_ERROR_NOT_INITIALIZED:
        return "MinHook is not initialized yet, or already uninitialized.";
    case MH_ERROR_ALREADY_CREATED:
        return "The hook for the specified target function is already created.";
    case MH_ERROR_NOT_CREATED:
        return "The hook for the specified target function is not created yet.";
    case MH_ERROR_ENABLED:
        return "The hook for the specified target function is already enabled.";
    case MH_ERROR_DISABLED:
        return "The hook for the specified target function is not enabled yet, "
               "or already disabled.";
    case MH_ERROR_NOT_EXECUTABLE:
        return "The specified pointer is invalid. It points the address of "
               "non-allocated and/or non-executable "
               "region.";
    case MH_ERROR_UNSUPPORTED_FUNCTION:
        return "The specified target function cannot be hooked.";
    case MH_ERROR_MEMORY_ALLOC:
        return "Failed to allocate memory.";
    case MH_ERROR_MEMORY_PROTECT:
        return "Failed to change the memory protection.";
    case MH_ERROR_MODULE_NOT_FOUND:
        return "The specified module is not loaded.";
    case MH_ERROR_FUNCTION_NOT_FOUND:
        return "The specified function is not found.";
    default:
        return "Unknown error.";
    }
}

std::error_code minhook::make_error_code(MH_STATUS e)
{
    static auto category = MinHookErrorCategory();
    return {static_cast<int>(e), category};
}

#endif // _WIN32
