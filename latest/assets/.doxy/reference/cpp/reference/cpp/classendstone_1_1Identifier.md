

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
|  [**constexpr**](classendstone_1_1Identifier.md) | [**Identifier**](#function-identifier-14) ([**const**](classendstone_1_1Identifier.md) [**char**](classendstone_1_1Identifier.md) \* s) noexcept<br>_Constructs an identifier by parsing a string._  |
|  [**constexpr**](classendstone_1_1Identifier.md) | [**Identifier**](#function-identifier-24) ([**const**](classendstone_1_1Identifier.md) std::string & s) noexcept<br>_Constructs an identifier by parsing a string._  |
|  [**constexpr**](classendstone_1_1Identifier.md) | [**Identifier**](#function-identifier-34) ([**const**](classendstone_1_1Identifier.md) std::string\_view & identifier) noexcept<br>_Constructs an identifier by parsing a string._  |
|  [**constexpr**](classendstone_1_1Identifier.md) | [**Identifier**](#function-identifier-44) ([**const**](classendstone_1_1Identifier.md) std::string\_view & namespace\_, [**const**](classendstone_1_1Identifier.md) std::string\_view & key) noexcept<br>_Constructs an identifier from a separate namespace and key._  |
|  [**constexpr**](classendstone_1_1Identifier.md) std::string\_view | [**getKey**](#function-getkey) () noexcept const<br>_Gets the key component of this identifier._  |
|  [**constexpr**](classendstone_1_1Identifier.md) std::string\_view | [**getNamespace**](#function-getnamespace) () noexcept const<br>_Gets the namespace component of this identifier._  |
|   | [**string**](#function-string) () const<br> |
|  [**constexpr**](classendstone_1_1Identifier.md) [**bool**](classendstone_1_1Identifier.md) | [**operator!=**](#function-operator) ([**const**](classendstone_1_1Identifier.md) [**Identifier**](classendstone_1_1Identifier.md) & other) noexcept const<br> |
|  [**constexpr**](classendstone_1_1Identifier.md) [**bool**](classendstone_1_1Identifier.md) | [**operator==**](#function-operator_1) ([**const**](classendstone_1_1Identifier.md) [**Identifier**](classendstone_1_1Identifier.md) & other) noexcept const<br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**constexpr**](classendstone_1_1Identifier.md) [**Identifier**](classendstone_1_1Identifier.md) | [**minecraft**](#function-minecraft) ([**const**](classendstone_1_1Identifier.md) std::string\_view key) noexcept<br>_Creates an identifier in the minecraft namespace with the given key._  |


























## Public Static Attributes Documentation




### variable Minecraft 

_The namespace representing all inbuilt keys._ 
```C++
constexpr std::string_view endstone::Identifier< T >::Minecraft;
```




<hr>
## Public Functions Documentation




### function Identifier [1/4]

_Constructs an identifier by parsing a string._ 
```C++
inline constexpr endstone::Identifier::Identifier (
    const  char * s
) noexcept
```



See the string\_view overload for the parsing rules.




**Parameters:**


* `s` The identifier string. 




        

<hr>



### function Identifier [2/4]

_Constructs an identifier by parsing a string._ 
```C++
inline constexpr endstone::Identifier::Identifier (
    const std::string & s
) noexcept
```



See the string\_view overload for the parsing rules.




**Parameters:**


* `s` The identifier string. 




        

<hr>



### function Identifier [3/4]

_Constructs an identifier by parsing a string._ 
```C++
inline constexpr endstone::Identifier::Identifier (
    const std::string_view & identifier
) noexcept
```



If the string contains a colon, the part before the last colon is taken as the namespace and the remainder as the key. Otherwise the whole string is the key and the namespace defaults to minecraft.




**Parameters:**


* `identifier` The identifier string. 




        

<hr>



### function Identifier [4/4]

_Constructs an identifier from a separate namespace and key._ 
```C++
inline constexpr endstone::Identifier::Identifier (
    const std::string_view & namespace_,
    const std::string_view & key
) noexcept
```





**Parameters:**


* `namespace_` The namespace component. 
* `key` The key component. 




        

<hr>



### function getKey 

_Gets the key component of this identifier._ 
```C++
inline constexpr std::string_view endstone::Identifier::getKey () noexcept const
```





**Returns:**

the key 





        

<hr>



### function getNamespace 

_Gets the namespace component of this identifier._ 
```C++
inline constexpr std::string_view endstone::Identifier::getNamespace () noexcept const
```





**Returns:**

the namespace 





        

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

_Creates an identifier in the minecraft namespace with the given key._ 
```C++
static inline constexpr  Identifier endstone::Identifier::minecraft (
    const std::string_view key
) noexcept
```





**Parameters:**


* `key` The key component. 



**Returns:**

an identifier with the namespace set to minecraft. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/identifier.h`

