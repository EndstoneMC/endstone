

# Class endstone::Identifier

**template &lt;[**typename**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md)&gt;**



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Identifier**](classendstone_1_1Identifier.md)



_Represents a identifier consisting of two components: a namespace and a key._ 

* `#include <endstone/identifier.h>`























## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  [**constexpr**](classendstone_1_1Identifier.md) std::string\_view | [**Minecraft**](#variable-minecraft)   = `"minecraft"`<br>_The namespace representing all inbuilt keys._  |














## Public Functions

| Type | Name |
| ---: | :--- |
|  [**constexpr**](classendstone_1_1Identifier.md) | [**Identifier**](#function-identifier-14) ([**const**](classendstone_1_1Identifier.md) [**char**](classendstone_1_1Identifier.md) \* s) noexcept<br> |
|  [**constexpr**](classendstone_1_1Identifier.md) | [**Identifier**](#function-identifier-24) ([**const**](classendstone_1_1Identifier.md) std::string & s) noexcept<br> |
|  [**constexpr**](classendstone_1_1Identifier.md) | [**Identifier**](#function-identifier-34) ([**const**](classendstone_1_1Identifier.md) std::string\_view & identifier) noexcept<br> |
|  [**constexpr**](classendstone_1_1Identifier.md) | [**Identifier**](#function-identifier-44) ([**const**](classendstone_1_1Identifier.md) std::string\_view & namespace\_, [**const**](classendstone_1_1Identifier.md) std::string\_view & key) noexcept<br> |
|  [**constexpr**](classendstone_1_1Identifier.md) std::string\_view | [**getKey**](#function-getkey) () noexcept const<br> |
|  [**constexpr**](classendstone_1_1Identifier.md) std::string\_view | [**getNamespace**](#function-getnamespace) () noexcept const<br> |
|   | [**string**](#function-string) () const<br> |
|  [**constexpr**](classendstone_1_1Identifier.md) [**bool**](classendstone_1_1Identifier.md) | [**operator!=**](#function-operator) ([**const**](classendstone_1_1Identifier.md) [**Identifier**](classendstone_1_1Identifier.md) & other) noexcept const<br> |
|  [**constexpr**](classendstone_1_1Identifier.md) [**bool**](classendstone_1_1Identifier.md) | [**operator==**](#function-operator_1) ([**const**](classendstone_1_1Identifier.md) [**Identifier**](classendstone_1_1Identifier.md) & other) noexcept const<br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**constexpr**](classendstone_1_1Identifier.md) [**Identifier**](classendstone_1_1Identifier.md) | [**minecraft**](#function-minecraft) ([**const**](classendstone_1_1Identifier.md) std::string\_view key) noexcept<br> |


























## Public Static Attributes Documentation




### variable Minecraft 

_The namespace representing all inbuilt keys._ 
```C++
constexpr std::string_view endstone::Identifier< T >::Minecraft;
```




<hr>
## Public Functions Documentation




### function Identifier [1/4]

```C++
inline constexpr endstone::Identifier::Identifier (
    const  char * s
) noexcept
```




<hr>



### function Identifier [2/4]

```C++
inline constexpr endstone::Identifier::Identifier (
    const std::string & s
) noexcept
```




<hr>



### function Identifier [3/4]

```C++
inline constexpr endstone::Identifier::Identifier (
    const std::string_view & identifier
) noexcept
```




<hr>



### function Identifier [4/4]

```C++
inline constexpr endstone::Identifier::Identifier (
    const std::string_view & namespace_,
    const std::string_view & key
) noexcept
```




<hr>



### function getKey 

```C++
inline constexpr std::string_view endstone::Identifier::getKey () noexcept const
```




<hr>



### function getNamespace 

```C++
inline constexpr std::string_view endstone::Identifier::getNamespace () noexcept const
```




<hr>



### function string 

```C++
inline endstone::Identifier::string () const
```




<hr>



### function operator!= 

```C++
inline constexpr  bool endstone::Identifier::operator!= (
    const  Identifier & other
) noexcept const
```




<hr>



### function operator== 

```C++
inline constexpr  bool endstone::Identifier::operator== (
    const  Identifier & other
) noexcept const
```




<hr>
## Public Static Functions Documentation




### function minecraft 

```C++
static inline constexpr  Identifier endstone::Identifier::minecraft (
    const std::string_view key
) noexcept
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/identifier.h`

