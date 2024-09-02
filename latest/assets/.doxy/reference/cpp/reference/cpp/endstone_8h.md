

# File endstone.h



[**FileList**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**endstone.h**](endstone_8h.md)

[Go to the source code of this file](endstone_8h_source.md)
















## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**endstone**](namespaceendstone.md) <br> |


## Classes

| Type | Name |
| ---: | :--- |
| struct | [**overloaded**](structendstone_1_1overloaded.md) &lt;Func&gt;<br>_Helper type for visitors._  |

















































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**ENDSTONE\_API\_VERSION**](endstone_8h.md#define-endstone_api_version)  ENDSTONE\_TOSTRING(ENDSTONE\_VERSION\_MAJOR) "." ENDSTONE\_TOSTRING(ENDSTONE\_VERSION\_MINOR)<br> |
| define  | [**ENDSTONE\_EXPORT**](endstone_8h.md#define-endstone_export)  \_\_attribute\_\_((visibility("default")))<br> |
| define  | [**ENDSTONE\_MAX\_PLAYERS**](endstone_8h.md#define-endstone_max_players)  200<br> |
| define  | [**ENDSTONE\_NOINLINE**](endstone_8h.md#define-endstone_noinline)  \_\_attribute\_\_((noinline)) inline<br> |
| define  | [**ENDSTONE\_STRINGIFY**](endstone_8h.md#define-endstone_stringify) (x) #x<br> |
| define  | [**ENDSTONE\_TOSTRING**](endstone_8h.md#define-endstone_tostring) (x) ENDSTONE\_STRINGIFY(x)<br> |
| define  | [**ENDSTONE\_VERSION\_MAJOR**](endstone_8h.md#define-endstone_version_major)  0<br> |
| define  | [**ENDSTONE\_VERSION\_MINOR**](endstone_8h.md#define-endstone_version_minor)  5<br> |
| define  | [**ENDSTONE\_VERSION\_PATCH**](endstone_8h.md#define-endstone_version_patch)  2<br> |

## Macro Definition Documentation





### define ENDSTONE\_API\_VERSION 

```C++
#define ENDSTONE_API_VERSION ENDSTONE_TOSTRING(ENDSTONE_VERSION_MAJOR) "." ENDSTONE_TOSTRING(ENDSTONE_VERSION_MINOR)
```




<hr>



### define ENDSTONE\_EXPORT 

```C++
#define ENDSTONE_EXPORT __attribute__((visibility("default")))
```




<hr>



### define ENDSTONE\_MAX\_PLAYERS 

```C++
#define ENDSTONE_MAX_PLAYERS 200
```




<hr>



### define ENDSTONE\_NOINLINE 

```C++
#define ENDSTONE_NOINLINE __attribute__((noinline)) inline
```




<hr>



### define ENDSTONE\_STRINGIFY 

```C++
#define ENDSTONE_STRINGIFY (
    x
) #x
```




<hr>



### define ENDSTONE\_TOSTRING 

```C++
#define ENDSTONE_TOSTRING (
    x
) ENDSTONE_STRINGIFY(x)
```




<hr>



### define ENDSTONE\_VERSION\_MAJOR 

```C++
#define ENDSTONE_VERSION_MAJOR 0
```




<hr>



### define ENDSTONE\_VERSION\_MINOR 

```C++
#define ENDSTONE_VERSION_MINOR 5
```




<hr>



### define ENDSTONE\_VERSION\_PATCH 

```C++
#define ENDSTONE_VERSION_PATCH 2
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/endstone.h`

