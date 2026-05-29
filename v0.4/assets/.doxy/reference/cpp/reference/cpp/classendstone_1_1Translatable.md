

# Class endstone::Translatable



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Translatable**](classendstone_1_1Translatable.md)



_Represents an object with a text representation that can be translated by the Minecraft client._ 

* `#include <endstone/translatable.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Translatable**](#function-translatable) (std::string translate, std::vector&lt; std::string &gt; with={}) <br> |
|  const std::vector&lt; std::string &gt; & | [**getParameters**](#function-getparameters) () const<br>_Get the translation parameters._  |
|  const std::string & | [**getTranslationKey**](#function-gettranslationkey) () const<br>_Get the translation key for use in a translation component._  |




























## Public Functions Documentation




### function Translatable 

```C++
inline endstone::Translatable::Translatable (
    std::string translate,
    std::vector< std::string > with={}
) 
```




<hr>



### function getParameters 

_Get the translation parameters._ 
```C++
inline const std::vector< std::string > & endstone::Translatable::getParameters () const
```





**Returns:**

the translation parameters 





        

<hr>



### function getTranslationKey 

_Get the translation key for use in a translation component._ 
```C++
inline const std::string & endstone::Translatable::getTranslationKey () const
```





**Returns:**

the translation key 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/translatable.h`

