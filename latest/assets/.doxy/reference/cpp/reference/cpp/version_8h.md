

# File version.h



[**FileList**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**version.h**](version_8h.md)

[Go to the source code of this file](version_8h_source.md)



































































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**ENDSTONE\_API\_VERSION**](version_8h.md#define-endstone_api_version)  ENDSTONE\_TOSTRING(ENDSTONE\_VERSION\_MAJOR) "." ENDSTONE\_TOSTRING(ENDSTONE\_VERSION\_MINOR)<br> |
| define  | [**ENDSTONE\_STRINGIFY**](version_8h.md#define-endstone_stringify) (x) #x<br> |
| define  | [**ENDSTONE\_TOSTRING**](version_8h.md#define-endstone_tostring) (x) ENDSTONE\_STRINGIFY(x)<br> |
| define  | [**ENDSTONE\_VERSION\_MAJOR**](version_8h.md#define-endstone_version_major)  0<br> |
| define  | [**ENDSTONE\_VERSION\_MINOR**](version_8h.md#define-endstone_version_minor)  5<br> |
| define  | [**ENDSTONE\_VERSION\_PATCH**](version_8h.md#define-endstone_version_patch)  7<br> |

## Macro Definition Documentation





### define ENDSTONE\_API\_VERSION 

```C++
#define ENDSTONE_API_VERSION ENDSTONE_TOSTRING(ENDSTONE_VERSION_MAJOR) "." ENDSTONE_TOSTRING(ENDSTONE_VERSION_MINOR)
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
#define ENDSTONE_VERSION_PATCH 7
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/version.h`

