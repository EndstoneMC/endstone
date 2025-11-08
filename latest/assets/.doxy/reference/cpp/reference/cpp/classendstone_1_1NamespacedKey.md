

# Class endstone::NamespacedKey



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**NamespacedKey**](classendstone_1_1NamespacedKey.md)




























## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**Minecraft**](#variable-minecraft)   = `"minecraft"`<br>_The namespace representing all inbuilt keys._  |
















## Public Static Functions

| Type | Name |
| ---: | :--- |
|  std::string | [**getKey**](#function-getkey) (const std::string & key) <br>_Get the key name (the part after the last colon)._  |
|  std::string | [**getNamespace**](#function-getnamespace) (const std::string & key) <br>_Get the namespace (the part before the last colon)._  |
|  std::string | [**minecraft**](#function-minecraft) (const std::string & key) <br>_Get a key in the Minecraft namespace._  |


























## Public Static Attributes Documentation




### variable Minecraft 

_The namespace representing all inbuilt keys._ 
```C++
const std::string endstone::NamespacedKey::Minecraft;
```




<hr>
## Public Static Functions Documentation




### function getKey 

_Get the key name (the part after the last colon)._ 
```C++
static inline std::string endstone::NamespacedKey::getKey (
    const std::string & key
) 
```



Examples:
* "minecraft:stone" -&gt; "stone"
* "custom:block" -&gt; "block"
* "minecraft:potion\_type:water" -&gt; "water"
* "stone" -&gt; "stone"






**Parameters:**


* `key` the namespaced key string 



**Returns:**

substring after the last colon, or the whole string if no colon 





        

<hr>



### function getNamespace 

_Get the namespace (the part before the last colon)._ 
```C++
static inline std::string endstone::NamespacedKey::getNamespace (
    const std::string & key
) 
```



Examples:
* "minecraft:stone" -&gt; "minecraft"
* "custom:block" -&gt; "custom"
* "minecraft:potion\_type:water" -&gt; "minecraft:potion\_type"
* "stone" -&gt; "minecraft" (default)






**Parameters:**


* `key` the namespaced key string 



**Returns:**

substring before the last colon, or "minecraft" if none 





        

<hr>



### function minecraft 

_Get a key in the Minecraft namespace._ 
```C++
static inline std::string endstone::NamespacedKey::minecraft (
    const std::string & key
) 
```





**Parameters:**


* `key` the key to use 



**Returns:**

new key in the Minecraft namespace 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/namespaced_key.h`

