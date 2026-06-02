

# File pointers.h



[**FileList**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**util**](dir_89b85071337bf933dea6c29b4c6a4410.md) **>** [**pointers.h**](pointers_8h.md)

[Go to the source code of this file](pointers_8h_source.md)



* `#include <cstddef>`
* `#include <functional>`
* `#include <memory>`
* `#include <type_traits>`
* `#include <utility>`
* `#include "endstone/check.h"`













## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**endstone**](namespaceendstone.md) <br>_Represents a button with text and an optional icon._  |


## Classes

| Type | Name |
| ---: | :--- |
| class | [**NotNull**](classendstone_1_1NotNull.md) &lt;[**class**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md)&gt;<br>_A wrapper around a std::shared\_ptr that is guaranteed never to be null._  |
| class | [**Nullable**](classendstone_1_1Nullable.md) &lt;[**class**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md)&gt;<br>_A wrapper around a std::shared\_ptr that may be null._  |
| struct | [**hash&lt; endstone::NotNull&lt; T &gt; &gt;**](structstd_1_1hash_3_01endstone_1_1NotNull_3_01T_01_4_01_4.md) &lt;class T&gt;<br> |
| struct | [**hash&lt; endstone::Nullable&lt; T &gt; &gt;**](structstd_1_1hash_3_01endstone_1_1Nullable_3_01T_01_4_01_4.md) &lt;class T&gt;<br> |

















































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**ENDSTONE\_DEFINE\_PTR\_COMPARISONS**](pointers_8h.md#define-endstone_define_ptr_comparisons) (Wrapper) `/* multi line expression */`<br> |

## Macro Definition Documentation





### define ENDSTONE\_DEFINE\_PTR\_COMPARISONS 

```C++
#define ENDSTONE_DEFINE_PTR_COMPARISONS (
    Wrapper
) `/* multi line expression */`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/util/pointers.h`

