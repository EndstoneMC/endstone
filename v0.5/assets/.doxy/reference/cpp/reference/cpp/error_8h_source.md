

# File error.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**util**](dir_89b85071337bf933dea6c29b4c6a4410.md) **>** [**error.h**](error_8h.md)

[Go to the documentation of this file](error_8h.md)


```C++
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

class Error {
public:
  Error(std::string_view message, std::string_view stack_trace) : message_(message), stack_trace_(stack_trace) {}

  [[nodiscard]] std::string_view getMessage() const noexcept
  {
    return message_;
  }

  [[nodiscard]] std::string_view getStackTrace() const noexcept
  {
    return stack_trace_;
  }

private:
  std::string message_;
  std::string stack_trace_;
};

}  // namespace endstone
```


