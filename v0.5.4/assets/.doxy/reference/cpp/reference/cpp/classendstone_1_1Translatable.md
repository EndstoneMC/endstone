

# Class endstone::Translatable



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Translatable**](classendstone_1_1Translatable.md)



_Represents an object with a text representation that can be translated by the Minecraft client._ 

* `#include <endstone/translatable.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Translatable**](#function-translatable) (std::string translate, std::vector&lt; std::string &gt; with={}) <br> |
|  bool | [**empty**](#function-empty) () const<br>_Check if the message to be translated is empty._  |
|  const std::string & | [**getTranslate**](#function-gettranslate) () const<br>_Get the message to be translated._  |
|  const std::vector&lt; std::string &gt; & | [**getWith**](#function-getwith) () const<br>_Get the translation parameters._  |




























## Public Functions Documentation




### function Translatable 

```C++
inline explicit endstone::Translatable::Translatable (
    std::string translate,
    std::vector< std::string > with={}
) 
```




<hr>



### function empty 

_Check if the message to be translated is empty._ 
```C++
inline bool endstone::Translatable::empty () const
```





**Returns:**

true if the message to be translated is empty, false otherwise 





        

<hr>



### function getTranslate 

_Get the message to be translated._ 
```C++
inline const std::string & endstone::Translatable::getTranslate () const
```





**Returns:**

the message to be translated 





        

<hr>



### function getWith 

_Get the translation parameters._ 
```C++
inline const std::vector< std::string > & endstone::Translatable::getWith () const
```





**Returns:**

the translation parameters 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/translatable.h`

