

# Class endstone::Skin



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Skin**](classendstone_1_1Skin.md)



_Represents a player skin._ 

* `#include <endstone/skin.h>`















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**ImageData**](structendstone_1_1Skin_1_1ImageData.md) <br> |






















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Skin**](#function-skin-12) () = default<br> |
|   | [**Skin**](#function-skin-22) (std::string skin\_id, [**ImageData**](structendstone_1_1Skin_1_1ImageData.md) skin\_data, std::optional&lt; std::string &gt; cape\_id=std::nullopt, std::optional&lt; [**ImageData**](structendstone_1_1Skin_1_1ImageData.md) &gt; cape\_data=std::nullopt) <br> |
|  const std::optional&lt; [**ImageData**](structendstone_1_1Skin_1_1ImageData.md) &gt; & | [**getCapeData**](#function-getcapedata) () const<br>_Gets the cape data._  |
|  const std::optional&lt; std::string &gt; & | [**getCapeId**](#function-getcapeid) () const<br>_Gets the cape id._  |
|  const [**ImageData**](structendstone_1_1Skin_1_1ImageData.md) & | [**getSkinData**](#function-getskindata) () const<br>_Gets the skin data._  |
|  const std::string & | [**getSkinId**](#function-getskinid) () const<br>_Gets the skin id._  |




























## Public Functions Documentation




### function Skin [1/2]

```C++
endstone::Skin::Skin () = default
```




<hr>



### function Skin [2/2]

```C++
inline endstone::Skin::Skin (
    std::string skin_id,
    ImageData skin_data,
    std::optional< std::string > cape_id=std::nullopt,
    std::optional< ImageData > cape_data=std::nullopt
) 
```




<hr>



### function getCapeData 

_Gets the cape data._ 
```C++
inline const std::optional< ImageData > & endstone::Skin::getCapeData () const
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
inline const ImageData & endstone::Skin::getSkinData () const
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

