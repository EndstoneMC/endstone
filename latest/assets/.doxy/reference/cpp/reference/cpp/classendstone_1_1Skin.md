

# Class endstone::Skin



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Skin**](classendstone_1_1Skin.md)



_Represents a player skin._ 

* `#include <endstone/skin.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Skin**](#function-skin) (std::string skin\_id, [**Image**](classendstone_1_1Image.md) skin\_data, std::optional&lt; std::string &gt; cape\_id=std::nullopt, std::optional&lt; [**Image**](classendstone_1_1Image.md) &gt; cape\_data=std::nullopt) <br> |
|  [**const**](classendstone_1_1Vector.md) [**Image**](classendstone_1_1Image.md) \* | [**getCapeData**](#function-getcapedata) () const<br>_Gets the cape data._  |
|  [**const**](classendstone_1_1Vector.md) std::optional&lt; std::string &gt; & | [**getCapeId**](#function-getcapeid) () const<br>_Gets the cape id._  |
|  [**const**](classendstone_1_1Vector.md) [**Image**](classendstone_1_1Image.md) & | [**getSkinData**](#function-getskindata) () const<br>_Gets the skin data._  |
|  [**const**](classendstone_1_1Vector.md) std::string & | [**getSkinId**](#function-getskinid) () const<br>_Gets the skin id._  |




























## Public Functions Documentation




### function Skin 

```C++
inline endstone::Skin::Skin (
    std::string skin_id,
    Image skin_data,
    std::optional< std::string > cape_id=std::nullopt,
    std::optional< Image > cape_data=std::nullopt
) 
```




<hr>



### function getCapeData 

_Gets the cape data._ 
```C++
inline const  Image * endstone::Skin::getCapeData () const
```





**Returns:**

the cape data. 





        

<hr>



### function getCapeId 

_Gets the cape id._ 
```C++
inline const std::optional< std::string > & endstone::Skin::getCapeId () const
```





**Returns:**

the cape id. 





        

<hr>



### function getSkinData 

_Gets the skin data._ 
```C++
inline const  Image & endstone::Skin::getSkinData () const
```





**Returns:**

the skin data. 





        

<hr>



### function getSkinId 

_Gets the skin id._ 
```C++
inline const std::string & endstone::Skin::getSkinId () const
```





**Returns:**

the skin id. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/skin.h`

