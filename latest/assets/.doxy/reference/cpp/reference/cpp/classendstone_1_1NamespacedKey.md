

# Class endstone::NamespacedKey



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**NamespacedKey**](classendstone_1_1NamespacedKey.md)



_Represents a string-based key which consists of two components - a namespace and a key._ [More...](#detailed-description)

* `#include <endstone/namespaced_key.h>`























## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  [**constexpr**](classendstone_1_1Vector.md) std::string\_view | [**MINECRAFT**](#variable-minecraft)   = `"minecraft"`<br> |














## Public Functions

| Type | Name |
| ---: | :--- |
|  [**const**](classendstone_1_1Vector.md) std::string & | [**getKey**](#function-getkey) () noexcept const<br> |
|  [**const**](classendstone_1_1Vector.md) std::string & | [**getNamespace**](#function-getnamespace) () noexcept const<br> |
|  [**bool**](classendstone_1_1Vector.md) | [**operator!=**](#function-operator) ([**const**](classendstone_1_1Vector.md) [**NamespacedKey**](classendstone_1_1NamespacedKey.md) & other) noexcept const<br> |
|  [**bool**](classendstone_1_1Vector.md) | [**operator==**](#function-operator_1) ([**const**](classendstone_1_1Vector.md) [**NamespacedKey**](classendstone_1_1NamespacedKey.md) & other) noexcept const<br> |
|  std::string | [**toString**](#function-tostring) () noexcept const<br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  std::optional&lt; [**NamespacedKey**](classendstone_1_1NamespacedKey.md) &gt; | [**create**](#function-create) (std::string\_view namespace\_, std::string\_view key) <br>_Factory method to create a_ [_**NamespacedKey**_](classendstone_1_1NamespacedKey.md) _. Returns std::nullopt if namespace or key is invalid._ |
|  std::optional&lt; [**NamespacedKey**](classendstone_1_1NamespacedKey.md) &gt; | [**fromString**](#function-fromstring) (std::string\_view input, std::optional&lt; std::string\_view &gt; default\_ns=std::nullopt) <br> |


























## Detailed Description


Namespaces may only contain lowercase alphanumeric characters, periods, underscores, and hyphens.


Keys may only contain lowercase alphanumeric characters, periods, underscores, hyphens, and forward slashes. 


    
## Public Static Attributes Documentation




### variable MINECRAFT 

```C++
constexpr std::string_view endstone::NamespacedKey::MINECRAFT;
```



The namespace representing all inbuilt keys. 


        

<hr>
## Public Functions Documentation




### function getKey 

```C++
inline const std::string & endstone::NamespacedKey::getKey () noexcept const
```




<hr>



### function getNamespace 

```C++
inline const std::string & endstone::NamespacedKey::getNamespace () noexcept const
```




<hr>



### function operator!= 

```C++
bool endstone::NamespacedKey::operator!= (
    const  NamespacedKey & other
) noexcept const
```




<hr>



### function operator== 

```C++
bool endstone::NamespacedKey::operator== (
    const  NamespacedKey & other
) noexcept const
```




<hr>



### function toString 

```C++
inline std::string endstone::NamespacedKey::toString () noexcept const
```




<hr>
## Public Static Functions Documentation




### function create 

_Factory method to create a_ [_**NamespacedKey**_](classendstone_1_1NamespacedKey.md) _. Returns std::nullopt if namespace or key is invalid._
```C++
static inline std::optional< NamespacedKey > endstone::NamespacedKey::create (
    std::string_view namespace_,
    std::string_view key
) 
```





**Parameters:**


* `namespace_` the namespace 
* `key` the key 



**Returns:**

optional containing the [**NamespacedKey**](classendstone_1_1NamespacedKey.md), or std::nullopt on failure 





        

<hr>



### function fromString 

```C++
static inline std::optional< NamespacedKey > endstone::NamespacedKey::fromString (
    std::string_view input,
    std::optional< std::string_view > default_ns=std::nullopt
) 
```



Parse a [**NamespacedKey**](classendstone_1_1NamespacedKey.md) from a string, with an optional default namespace.


Examples:
* fromString("foo", "plugin") -&gt; "plugin:foo"
* fromString("foo:bar", std::nullopt) -&gt; "foo:bar"
* fromString(":foo", "minecraft") -&gt; "minecraft:foo"
* fromString("foo", std::nullopt) -&gt; "minecraft:foo"
* fromString("Foo", ...) -&gt; std::nullopt






**Parameters:**


* `input` the string to parse 
* `default_ns` optional default namespace; if std::nullopt, uses Minecraft 



**Returns:**

optional containing the parsed key, or std::nullopt if invalid 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/namespaced_key.h`

