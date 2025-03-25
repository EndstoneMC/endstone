

# File plugin.h



[**FileList**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**plugin**](dir_53ee43673b2467e53c4cb8c30a2e7d89.md) **>** [**plugin.h**](plugin_8h.md)

[Go to the source code of this file](plugin_8h_source.md)



* `#include <algorithm>`
* `#include <filesystem>`
* `#include <set>`
* `#include <string>`
* `#include <unordered_map>`
* `#include <utility>`
* `#include <vector>`
* `#include "endstone/command/command_executor.h"`
* `#include "endstone/logger.h"`
* `#include "endstone/permissions/permission.h"`
* `#include "endstone/plugin/plugin_description.h"`
* `#include "endstone/server.h"`













## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**endstone**](namespaceendstone.md) <br> |
| namespace | [**core**](namespaceendstone_1_1core.md) <br> |
| namespace | [**detail**](namespaceendstone_1_1detail.md) <br> |


## Classes

| Type | Name |
| ---: | :--- |
| class | [**Plugin**](classendstone_1_1Plugin.md) <br>_Represents a_ [_**Plugin**_](classendstone_1_1Plugin.md) _._ |

















































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**ENDSTONE\_EXPORT**](plugin_8h.md#define-endstone_export)  `\_\_attribute\_\_((visibility("default")))`<br> |
| define  | [**ENDSTONE\_PLUGIN**](plugin_8h.md#define-endstone_plugin) (Name, Version, MainClass) `/* multi line expression */`<br> |

## Macro Definition Documentation





### define ENDSTONE\_EXPORT 

```C++
#define ENDSTONE_EXPORT `__attribute__((visibility("default")))`
```




<hr>



### define ENDSTONE\_PLUGIN 

```C++
#define ENDSTONE_PLUGIN (
    Name,
    Version,
    MainClass
) `/* multi line expression */`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/plugin/plugin.h`

