

# Class endstone::NamespacedKey



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**NamespacedKey**](classendstone_1_1NamespacedKey.md)



_Represents a string-based key which consists of two components - a namespace and a key._ [More...](#detailed-description)

* `#include <endstone/namespaced_key.h>`























## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  constexpr std::string\_view | [**MINECRAFT**](#variable-minecraft)   = `"minecraft"`<br> |














## Public Functions

| Type | Name |
| ---: | :--- |
|  const std::string & | [**getKey**](#function-getkey) () noexcept const<br> |
|  const std::string & | [**getNamespace**](#function-getnamespace) () noexcept const<br> |
|  bool | [**operator!=**](#function-operator) (const [**NamespacedKey**](classendstone_1_1NamespacedKey.md) & other) noexcept const<br> |
|  bool | [**operator==**](#function-operator_1) (const [**NamespacedKey**](classendstone_1_1NamespacedKey.md) & other) noexcept const<br> |
|  std::string | [**toString**](#function-tostring) () noexcept const<br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  Result&lt; [**NamespacedKey**](classendstone_1_1NamespacedKey.md) &gt; | [**create**](#function-create) (const [**Plugin**](classendstone_1_1Plugin.md) & plugin, std::string\_view key) <br>_Create a key in the plugin's namespace._  |
|  Result&lt; [**NamespacedKey**](classendstone_1_1NamespacedKey.md) &gt; | [**fromString**](#function-fromstring) (std::string\_view input, const [**Plugin**](classendstone_1_1Plugin.md) \* plugin=nullptr) <br> |


























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
inline bool endstone::NamespacedKey::operator!= (
    const NamespacedKey & other
) noexcept const
```




<hr>



### function operator== 

```C++
inline bool endstone::NamespacedKey::operator== (
    const NamespacedKey & other
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

_Create a key in the plugin's namespace._ 
```C++
static inline Result< NamespacedKey > endstone::NamespacedKey::create (
    const Plugin & plugin,
    std::string_view key
) 
```





**Parameters:**


* `plugin` the plugin to use for the namespace 
* `key` the key to create 




        

<hr>



### function fromString 

```C++
static inline Result< NamespacedKey > endstone::NamespacedKey::fromString (
    std::string_view input,
    const Plugin * plugin=nullptr
) 
```



Parse a [**NamespacedKey**](classendstone_1_1NamespacedKey.md) from a string, with an optional default namespace.


Examples:
* fromString("foo", plugin) -&gt; "plugin:foo"
* fromString("foo:bar", nullptr) -&gt; "foo:bar"
* fromString(":foo", nullptr) -&gt; "minecraft:foo"
* fromString("foo", nullptr) -&gt; "minecraft:foo"
* fromString("Foo", plugin) -&gt; std::nullopt






**Parameters:**


* `input` the string to parse 
* `plugin` optional default namespace; if nullptr, uses the minecraft namespace 



**Returns:**

optional containing the parsed key, or std::nullopt if invalid 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/namespaced_key.h`

