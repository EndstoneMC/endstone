

# File registry.h



[**FileList**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**registry.h**](registry_8h.md)

[Go to the source code of this file](registry_8h_source.md)



* `#include <concepts>`
* `#include <functional>`
* `#include <stdexcept>`
* `#include <string>`
* `#include <fmt/format.h>`
* `#include "detail.h"`
* `#include "identifier.h"`
* `#include "server.h"`













## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**endstone**](namespaceendstone.md) <br>_Represents a button with text and an optional icon._  |


## Classes

| Type | Name |
| ---: | :--- |
| class | [**IRegistry**](classendstone_1_1IRegistry.md) <br> |
| class | [**Registry**](classendstone_1_1Registry.md) &lt;[**typename**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md)&gt;<br>_Abstract registry interface for objects with identifiers._  |
| class | [**Type**](classendstone_1_1Registry_1_1Type.md) <br>_CRTP base for registry-backed types identified by an_ [_**Identifier**_](classendstone_1_1Identifier.md) _._ |
| struct | [**formatter&lt; T &gt;**](structfmt_1_1formatter_3_01T_01_4.md) &lt;typename T&gt;<br> |

















































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**ENDSTONE\_REGISTRY\_TYPE**](registry_8h.md#define-endstone_registry_type) (type) `static constexpr auto RegistryType = #type;`<br> |

## Macro Definition Documentation





### define ENDSTONE\_REGISTRY\_TYPE 

```C++
#define ENDSTONE_REGISTRY_TYPE (
    type
) `static constexpr auto RegistryType = #type;`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/registry.h`

