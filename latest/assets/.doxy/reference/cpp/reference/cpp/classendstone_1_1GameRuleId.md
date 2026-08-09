

# Class endstone::GameRuleId

**template &lt;[**typename**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md)&gt;**



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**GameRuleId**](classendstone_1_1GameRuleId.md)



_Represents the identifier of a game rule, carrying the type of the rule's value._ 

* `#include <endstone/game_rule.h>`



Inherits the following classes: [endstone::Identifier](classendstone_1_1Identifier.md)




























## Public Static Attributes inherited from endstone::Identifier

See [endstone::Identifier](classendstone_1_1Identifier.md)

| Type | Name |
| ---: | :--- |
|  [**constexpr**](classendstone_1_1Identifier.md) std::string\_view | [**Minecraft**](classendstone_1_1Identifier.md#variable-minecraft)   = `"minecraft"`<br>_The namespace representing all inbuilt keys._  |


























## Public Functions

| Type | Name |
| ---: | :--- |
|  [**constexpr**](classendstone_1_1Identifier.md) | [**Identifier**](#function-identifier-14) ([**const**](classendstone_1_1Identifier.md) [**char**](classendstone_1_1Identifier.md) \* s) noexcept<br>_Constructs an identifier by parsing a string._  |
|  [**constexpr**](classendstone_1_1Identifier.md) | [**Identifier**](#function-identifier-24) ([**const**](classendstone_1_1Identifier.md) std::string & s) noexcept<br>_Constructs an identifier by parsing a string._  |
|  [**constexpr**](classendstone_1_1Identifier.md) | [**Identifier**](#function-identifier-34) ([**const**](classendstone_1_1Identifier.md) std::string\_view & identifier) noexcept<br>_Constructs an identifier by parsing a string._  |
|  [**constexpr**](classendstone_1_1Identifier.md) | [**Identifier**](#function-identifier-44) ([**const**](classendstone_1_1Identifier.md) std::string\_view & namespace\_, [**const**](classendstone_1_1Identifier.md) std::string\_view & key) noexcept<br>_Constructs an identifier from a separate namespace and key._  |


## Public Functions inherited from endstone::Identifier

See [endstone::Identifier](classendstone_1_1Identifier.md)

| Type | Name |
| ---: | :--- |
|  [**constexpr**](classendstone_1_1Identifier.md) | [**Identifier**](classendstone_1_1Identifier.md#function-identifier-14) ([**const**](classendstone_1_1Identifier.md) [**char**](classendstone_1_1Identifier.md) \* s) noexcept<br>_Constructs an identifier by parsing a string._  |
|  [**constexpr**](classendstone_1_1Identifier.md) | [**Identifier**](classendstone_1_1Identifier.md#function-identifier-24) ([**const**](classendstone_1_1Identifier.md) std::string & s) noexcept<br>_Constructs an identifier by parsing a string._  |
|  [**constexpr**](classendstone_1_1Identifier.md) | [**Identifier**](classendstone_1_1Identifier.md#function-identifier-34) ([**const**](classendstone_1_1Identifier.md) std::string\_view & identifier) noexcept<br>_Constructs an identifier by parsing a string._  |
|  [**constexpr**](classendstone_1_1Identifier.md) | [**Identifier**](classendstone_1_1Identifier.md#function-identifier-44) ([**const**](classendstone_1_1Identifier.md) std::string\_view & namespace\_, [**const**](classendstone_1_1Identifier.md) std::string\_view & key) noexcept<br>_Constructs an identifier from a separate namespace and key._  |
|  [**constexpr**](classendstone_1_1Identifier.md) std::string\_view | [**getKey**](classendstone_1_1Identifier.md#function-getkey) () noexcept const<br>_Gets the key component of this identifier._  |
|  [**constexpr**](classendstone_1_1Identifier.md) std::string\_view | [**getNamespace**](classendstone_1_1Identifier.md#function-getnamespace) () noexcept const<br>_Gets the namespace component of this identifier._  |
|   | [**string**](classendstone_1_1Identifier.md#function-string) () const<br> |
|  [**constexpr**](classendstone_1_1Identifier.md) [**bool**](classendstone_1_1Identifier.md) | [**operator!=**](classendstone_1_1Identifier.md#function-operator) ([**const**](classendstone_1_1Identifier.md) [**Identifier**](classendstone_1_1Identifier.md) & other) noexcept const<br> |
|  [**constexpr**](classendstone_1_1Identifier.md) [**bool**](classendstone_1_1Identifier.md) | [**operator==**](classendstone_1_1Identifier.md#function-operator_1) ([**const**](classendstone_1_1Identifier.md) [**Identifier**](classendstone_1_1Identifier.md) & other) noexcept const<br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**constexpr**](classendstone_1_1Identifier.md) [**GameRuleId**](classendstone_1_1GameRuleId.md) | [**minecraft**](#function-minecraft) ([**const**](classendstone_1_1Identifier.md) std::string\_view key) noexcept<br> |


## Public Static Functions inherited from endstone::Identifier

See [endstone::Identifier](classendstone_1_1Identifier.md)

| Type | Name |
| ---: | :--- |
|  [**constexpr**](classendstone_1_1Identifier.md) [**Identifier**](classendstone_1_1Identifier.md) | [**minecraft**](classendstone_1_1Identifier.md#function-minecraft) ([**const**](classendstone_1_1Identifier.md) std::string\_view key) noexcept<br>_Creates an identifier in the_ `minecraft` _namespace with the given key._ |


















































## Public Functions Documentation




### function Identifier [1/4]

_Constructs an identifier by parsing a string._ 
```C++
inline constexpr endstone::GameRuleId::Identifier (
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
inline constexpr endstone::GameRuleId::Identifier (
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
inline constexpr endstone::GameRuleId::Identifier (
    const std::string_view & identifier
) noexcept
```



If the string contains a colon, the part before the last colon is taken as the namespace and the remainder as the key. Otherwise the whole string is the key and the namespace defaults to `minecraft`.




**Parameters:**


* `identifier` The identifier string. 




        

<hr>



### function Identifier [4/4]

_Constructs an identifier from a separate namespace and key._ 
```C++
inline constexpr endstone::GameRuleId::Identifier (
    const std::string_view & namespace_,
    const std::string_view & key
) noexcept
```





**Parameters:**


* `namespace_` The namespace component. 
* `key` The key component. 




        

<hr>
## Public Static Functions Documentation




### function minecraft 

```C++
static inline constexpr  GameRuleId endstone::GameRuleId::minecraft (
    const std::string_view key
) noexcept
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/game_rule.h`

