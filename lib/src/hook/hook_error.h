//
// Created by Vincent on 31/07/2023.
//

#ifndef ENDSTONE_HOOK_ERROR_H
#define ENDSTONE_HOOK_ERROR_H

#include <system_error>

#ifdef _WIN32
#include "MinHook.h"

/**
 * @brief This class provides a custom error category for MinHook errors.
 */
class MinHookErrorCategory : public std::error_category
{
  public:
    /**
     * @brief Returns the name of the error category.
     * @return The name of the error category.
     */
    [[nodiscard]] const char *name() const noexcept override;

    /**
     * @brief Provides a human-readable error message for a given error value.
     * @param errVal The error value.
     * @return A string containing a descriptive error message.
     */
    [[nodiscard]] std::string message(int errVal) const override;
};

/**
 * @brief A namespace containing helper functions related to MinHook.
 */
namespace minhook
{

/**
 * @brief Creates an error code based on a MinHook status.
 * @param e The MinHook status.
 * @return A standard error code.
 */
std::error_code make_error_code(MH_STATUS e);

} // namespace minhook

#endif // _WIN32

#endif // ENDSTONE_HOOK_ERROR_H
