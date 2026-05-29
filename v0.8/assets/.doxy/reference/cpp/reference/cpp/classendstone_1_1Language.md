

# Class endstone::Language



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Language**](classendstone_1_1Language.md)



_Represents the interface for translating text into different languages._ 

* `#include <endstone/lang/language.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual std::string | [**getLocale**](#function-getlocale) () const = 0<br>_Gets the current locale._  |
| virtual std::string | [**translate**](#function-translate-16) (std::string text) const = 0<br>_Translates a given text into the current locale._  |
| virtual std::string | [**translate**](#function-translate-26) (std::string text, std::string locale) const = 0<br>_Translates a given text into a specific locale._  |
| virtual std::string | [**translate**](#function-translate-36) (std::string text, std::vector&lt; std::string &gt; params) const = 0<br>_Translates a given text using a set of parameters for the current locale._  |
| virtual std::string | [**translate**](#function-translate-46) (std::string text, std::vector&lt; std::string &gt; params, std::string locale) const = 0<br>_Translates a given text using a set of parameters for a specific locale._  |
| virtual std::string | [**translate**](#function-translate-56) ([**Translatable**](classendstone_1_1Translatable.md) translatable) const = 0<br>_Translates a_ [_**Translatable**_](classendstone_1_1Translatable.md) _object into the current locale._ |
| virtual std::string | [**translate**](#function-translate-66) ([**Translatable**](classendstone_1_1Translatable.md) translatable, std::string locale) const = 0<br>_Translates a_ [_**Translatable**_](classendstone_1_1Translatable.md) _object into a specific locale._ |
| virtual  | [**~Language**](#function-language) () = default<br> |




























## Public Functions Documentation




### function getLocale 

_Gets the current locale._ 
```C++
virtual std::string endstone::Language::getLocale () const = 0
```





**Returns:**

A string representing the current locale (e.g., "en\_US"). 





        

<hr>



### function translate [1/6]

_Translates a given text into the current locale._ 
```C++
virtual std::string endstone::Language::translate (
    std::string text
) const = 0
```





**Parameters:**


* `text` The text to be translated. 



**Returns:**

The translated text in the current locale. 





        

<hr>



### function translate [2/6]

_Translates a given text into a specific locale._ 
```C++
virtual std::string endstone::Language::translate (
    std::string text,
    std::string locale
) const = 0
```





**Parameters:**


* `text` The text to be translated. 
* `locale` The locale identifier for the desired translation (e.g., "en\_US"). 



**Returns:**

The translated text in the specified locale. 





        

<hr>



### function translate [3/6]

_Translates a given text using a set of parameters for the current locale._ 
```C++
virtual std::string endstone::Language::translate (
    std::string text,
    std::vector< std::string > params
) const = 0
```





**Parameters:**


* `text` The text to be translated. 
* `params` A list of parameters to be used in the translation. 



**Returns:**

The translated text in the current locale, with parameters applied. 





        

<hr>



### function translate [4/6]

_Translates a given text using a set of parameters for a specific locale._ 
```C++
virtual std::string endstone::Language::translate (
    std::string text,
    std::vector< std::string > params,
    std::string locale
) const = 0
```





**Parameters:**


* `text` The text to be translated. 
* `params` A list of parameters to be used in the translation. 
* `locale` The locale identifier for the desired translation (e.g., "en\_US"). 



**Returns:**

The translated text in the specified locale, with parameters applied. 





        

<hr>



### function translate [5/6]

_Translates a_ [_**Translatable**_](classendstone_1_1Translatable.md) _object into the current locale._
```C++
virtual std::string endstone::Language::translate (
    Translatable translatable
) const = 0
```





**Parameters:**


* `translatable` A [**Translatable**](classendstone_1_1Translatable.md) object containing text and parameters. 



**Returns:**

The translated text in the current locale. 





        

<hr>



### function translate [6/6]

_Translates a_ [_**Translatable**_](classendstone_1_1Translatable.md) _object into a specific locale._
```C++
virtual std::string endstone::Language::translate (
    Translatable translatable,
    std::string locale
) const = 0
```





**Parameters:**


* `translatable` A [**Translatable**](classendstone_1_1Translatable.md) object containing text and parameters. 
* `locale` The locale identifier for the desired translation (e.g., "en\_US"). 



**Returns:**

The translated text in the specified locale. 





        

<hr>



### function ~Language 

```C++
virtual endstone::Language::~Language () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/lang/language.h`

