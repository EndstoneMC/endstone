

# File detail.h



[**FileList**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**detail.h**](detail_8h.md)

[Go to the source code of this file](detail_8h_source.md)



* `#include <stdexcept>`
* `#include <dlfcn.h>`













## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**endstone**](namespaceendstone.md) <br>_Represents a button with text and an optional icon._  |
| namespace | [**detail**](namespaceendstone_1_1detail.md) <br> |



















































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**ENDSTONE\_API\_VERSION**](detail_8h.md#define-endstone_api_version)  `ENDSTONE\_TOSTRING(ENDSTONE\_VERSION\_MAJOR) "." ENDSTONE\_TOSTRING(ENDSTONE\_VERSION\_MINOR)`<br> |
| define  | [**ENDSTONE\_STRINGIFY**](detail_8h.md#define-endstone_stringify) (x) `#x`<br> |
| define  | [**ENDSTONE\_TOSTRING**](detail_8h.md#define-endstone_tostring) (x) `ENDSTONE\_STRINGIFY(x)`<br> |
| define  | [**ENDSTONE\_VERSION**](detail_8h.md#define-endstone_version)  `/* multi line expression */`<br> |
| define  | [**ENDSTONE\_VERSION\_MAJOR**](detail_8h.md#define-endstone_version_major)  `0`<br> |
| define  | [**ENDSTONE\_VERSION\_MINOR**](detail_8h.md#define-endstone_version_minor)  `11`<br> |
| define  | [**ENDSTONE\_VERSION\_PATCH**](detail_8h.md#define-endstone_version_patch)  `0`<br> |
| define  | [**MINECRAFT\_VERSION**](detail_8h.md#define-minecraft_version)  `ENDSTONE\_TOSTRING(MINECRAFT\_VERSION\_MINOR) "." ENDSTONE\_TOSTRING(MINECRAFT\_VERSION\_PATCH)`<br> |
| define  | [**MINECRAFT\_VERSION\_MAJOR**](detail_8h.md#define-minecraft_version_major)  `1`<br> |
| define  | [**MINECRAFT\_VERSION\_MINOR**](detail_8h.md#define-minecraft_version_minor)  `26`<br> |
| define  | [**MINECRAFT\_VERSION\_PATCH**](detail_8h.md#define-minecraft_version_patch)  `0`<br> |
| define  | [**NETWORK\_PROTOCOL\_VERSION**](detail_8h.md#define-network_protocol_version)  `924`<br> |

## Macro Definition Documentation





### define ENDSTONE\_API\_VERSION 

```C++
#define ENDSTONE_API_VERSION `ENDSTONE_TOSTRING(ENDSTONE_VERSION_MAJOR) "." ENDSTONE_TOSTRING(ENDSTONE_VERSION_MINOR)`
```




<hr>



### define ENDSTONE\_STRINGIFY 

```C++
#define ENDSTONE_STRINGIFY (
    x
) `#x`
```




<hr>



### define ENDSTONE\_TOSTRING 

```C++
#define ENDSTONE_TOSTRING (
    x
) `ENDSTONE_STRINGIFY(x)`
```




<hr>



### define ENDSTONE\_VERSION 

```C++
#define ENDSTONE_VERSION `/* multi line expression */`
```




<hr>



### define ENDSTONE\_VERSION\_MAJOR 

```C++
#define ENDSTONE_VERSION_MAJOR `0`
```




<hr>



### define ENDSTONE\_VERSION\_MINOR 

```C++
#define ENDSTONE_VERSION_MINOR `11`
```




<hr>



### define ENDSTONE\_VERSION\_PATCH 

```C++
#define ENDSTONE_VERSION_PATCH `0`
```




<hr>



### define MINECRAFT\_VERSION 

```C++
#define MINECRAFT_VERSION `ENDSTONE_TOSTRING(MINECRAFT_VERSION_MINOR) "." ENDSTONE_TOSTRING(MINECRAFT_VERSION_PATCH)`
```




<hr>



### define MINECRAFT\_VERSION\_MAJOR 

```C++
#define MINECRAFT_VERSION_MAJOR `1`
```




<hr>



### define MINECRAFT\_VERSION\_MINOR 

```C++
#define MINECRAFT_VERSION_MINOR `26`
```




<hr>



### define MINECRAFT\_VERSION\_PATCH 

```C++
#define MINECRAFT_VERSION_PATCH `0`
```




<hr>



### define NETWORK\_PROTOCOL\_VERSION 

```C++
#define NETWORK_PROTOCOL_VERSION `924`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/detail.h`

