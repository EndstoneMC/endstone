

# Class endstone::Skin



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Skin**](classendstone_1_1Skin.md)



_Represents a player skin._ 

* `#include <endstone/skin.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Skin**](#function-skin) (std::string id, [**Image**](classendstone_1_1Image.md) image, std::optional&lt; std::string &gt; cape\_id=std::nullopt, std::optional&lt; [**Image**](classendstone_1_1Image.md) &gt; cape\_image=std::nullopt) <br> |
|  const std::optional&lt; std::string &gt; & | [**getCapeId**](#function-getcapeid) () const<br>_Gets the cape id._  |
|  const [**Image**](classendstone_1_1Image.md) \* | [**getCapeImage**](#function-getcapeimage) () const<br>_Gets the cape image._  |
|  const std::string & | [**getId**](#function-getid) () const<br>_Gets the skin id._  |
|  const [**Image**](classendstone_1_1Image.md) & | [**getImage**](#function-getimage) () const<br>_Gets the skin image._  |




























## Public Functions Documentation




### function Skin 

```C++
inline endstone::Skin::Skin (
    std::string id,
    Image image,
    std::optional< std::string > cape_id=std::nullopt,
    std::optional< Image > cape_image=std::nullopt
) 
```




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
inline const Image * endstone::Skin::getCapeImage () const
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
inline const Image & endstone::Skin::getImage () const
```





**Returns:**

the skin image. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/skin.h`

