

# Class endstone::Skin



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Skin**](classendstone_1_1Skin.md)



_Represents a player skin._ 

* `#include <endstone/skin.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Skin**](#function-skin) (std::string id, [**Image**](classendstone_1_1Image.md) image, std::optional&lt; std::string &gt; cape\_id=std::nullopt, std::optional&lt; [**Image**](classendstone_1_1Image.md) &gt; cape\_image=std::nullopt) <br>_Constructs a skin from its id and image, with an optional cape._  |
|  [**const**](classendstone_1_1Identifier.md) std::optional&lt; std::string &gt; & | [**getCapeId**](#function-getcapeid) () const<br>_Gets the cape id._  |
|  [**const**](classendstone_1_1Identifier.md) [**Image**](classendstone_1_1Image.md) \* | [**getCapeImage**](#function-getcapeimage) () const<br>_Gets the cape image._  |
|  [**const**](classendstone_1_1Identifier.md) std::string & | [**getId**](#function-getid) () const<br>_Gets the skin id._  |
|  [**const**](classendstone_1_1Identifier.md) [**Image**](classendstone_1_1Image.md) & | [**getImage**](#function-getimage) () const<br>_Gets the skin image._  |




























## Public Functions Documentation




### function Skin 

_Constructs a skin from its id and image, with an optional cape._ 
```C++
inline endstone::Skin::Skin (
    std::string id,
    Image image,
    std::optional< std::string > cape_id=std::nullopt,
    std::optional< Image > cape_image=std::nullopt
) 
```





**Parameters:**


* `id` The skin id. 
* `image` The skin image. 
* `cape_id` The cape id, or std::nullopt if the skin has no cape. 
* `cape_image` The cape image, or std::nullopt if the skin has no cape. 




        

<hr>



### function getCapeId 

_Gets the cape id._ 
```C++
inline const std::optional< std::string > & endstone::Skin::getCapeId () const
```





**Returns:**

the cape id. 





        

<hr>



### function getCapeImage 

_Gets the cape image._ 
```C++
inline const  Image * endstone::Skin::getCapeImage () const
```





**Returns:**

the cape image. 





        

<hr>



### function getId 

_Gets the skin id._ 
```C++
inline const std::string & endstone::Skin::getId () const
```





**Returns:**

the skin id. 





        

<hr>



### function getImage 

_Gets the skin image._ 
```C++
inline const  Image & endstone::Skin::getImage () const
```





**Returns:**

the skin image. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/skin.h`

