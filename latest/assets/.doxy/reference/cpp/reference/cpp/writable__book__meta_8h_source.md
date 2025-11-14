

# File writable\_book\_meta.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**inventory**](dir_d1e84b530b14f41e8b6f5ec1b5dee76c.md) **>** [**meta**](dir_2d728641c8c30e7cdff7ab60efc98406.md) **>** [**writable\_book\_meta.h**](writable__book__meta_8h.md)

[Go to the documentation of this file](writable__book__meta_8h.md)


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

#include "endstone/inventory/meta/item_meta.h"

namespace endstone {
class WritableBookMeta : public ItemMeta {
public:
    ENDSTONE_ITEM_META_TYPE(WritableBook)

    
    [[nodiscard]] virtual bool hasPages() const = 0;

    [[nodiscard]] virtual std::string getPage(int page) const = 0;

    virtual void setPage(int page, std::string data) = 0;

    [[nodiscard]] virtual std::vector<std::string> getPages() const = 0;

    template <std::convertible_to<std::string>... Args>
    void setPages(Args &&...pages)
    {
        setPages(std::vector<std::string>{&pages...});
    }

    virtual void setPages(std::vector<std::string> pages) = 0;

    template <std::convertible_to<std::string>... Args>
    void addPage(Args &&...pages)
    {
        addPage(std::vector<std::string>{&pages...});
    }

    virtual void addPage(std::vector<std::string> pages) = 0;

    [[nodiscard]] virtual int getPageCount() const = 0;
};
}  // namespace endstone
```


