

# Class endstone::Translatable



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Translatable**](classendstone_1_1Translatable.md)



_Represents an object with a text representation that can be translated by the Minecraft client._ 

* `#include <endstone/lang/translatable.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Translatable**](#function-translatable) (std::string text, std::vector&lt; std::string &gt; params={}) <br> |
|  [**bool**](classendstone_1_1Vector.md) | [**empty**](#function-empty) () const<br>_Check if the message to be translated is empty._  |
|  [**const**](classendstone_1_1Vector.md) std::vector&lt; std::string &gt; & | [**getParameters**](#function-getparameters) () const<br>_Get the translation parameters._  |
|  [**const**](classendstone_1_1Vector.md) std::string & | [**getText**](#function-gettext) () const<br>_Get the text to be translated._  |




























## Public Functions Documentation




### function Translatable 

```C++
inline explicit endstone::Translatable::Translatable (
    std::string text,
    std::vector< std::string > params={}
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



### function getParameters 

_Get the translation parameters._ 
```C++
inline const std::vector< std::string > & endstone::Translatable::getParameters () const
```





**Returns:**

the translation parameters 





        

<hr>



### function getText 

_Get the text to be translated._ 
```C++
inline const std::string & endstone::Translatable::getText () const
```





**Returns:**

the text to be translated 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/lang/translatable.h`

