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

#include <string_view>

namespace endstone {

/**
 * @brief Represents an error with a message and the stack trace.
 */
class Error {
public:
  Error(std::string_view message, std::string_view stack_trace) : message_(message), stack_trace_(stack_trace) {}

  /**
   * @brief Returns the error message.
   *
   * @return The error message.
   */
  [[nodiscard]] std::string_view getMessage() const noexcept
  {
    return message_;
  }

  /**
   * @brief Returns the stack trace associated with the error.
   *
   * @return The stack trace associated with the error.
   */
  [[nodiscard]] std::string_view getStackTrace() const noexcept
  {
    return stack_trace_;
  }

private:
  std::string message_;
  std::string stack_trace_;
};

}  // namespace endstone
