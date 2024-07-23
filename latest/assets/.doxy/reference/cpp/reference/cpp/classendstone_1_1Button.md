

# Class endstone::Button



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Button**](classendstone_1_1Button.md)



_Represents a button with text and an optional icon._ 

* `#include <endstone/form/controls/button.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Button**](#function-button-12) () = default<br> |
|   | [**Button**](#function-button-22) (Message text, std::optional&lt; std::string &gt; icon=std::nullopt) <br> |
|  std::optional&lt; std::string &gt; | [**getIcon**](#function-geticon) () const<br>_Get the icon of the button._  |
|  Message | [**getText**](#function-gettext) () const<br>_Gets the text of the button._  |
|  void | [**setIcon**](#function-seticon) (std::string icon) <br>_Sets the icon for the button._  |
|  void | [**setText**](#function-settext) (Message text) <br>_Sets the text of the button._  |




























## Public Functions Documentation




### function Button [1/2]

```C++
endstone::Button::Button () = default
```




<hr>



### function Button [2/2]

```C++
inline explicit endstone::Button::Button (
    Message text,
    std::optional< std::string > icon=std::nullopt
) 
```




<hr>



### function getIcon 

_Get the icon of the button._ 
```C++
inline std::optional< std::string > endstone::Button::getIcon () const
```





**Returns:**

The path or URL to the icon file 





        

<hr>



### function getText 

_Gets the text of the button._ 
```C++
inline Message endstone::Button::getText () const
```





**Returns:**

The text. 





        

<hr>



### function setIcon 

_Sets the icon for the button._ 
```C++
inline void endstone::Button::setIcon (
    std::string icon
) 
```





**Parameters:**


* `icon` The path or URL to the icon file. 




        

<hr>



### function setText 

_Sets the text of the button._ 
```C++
inline void endstone::Button::setText (
    Message text
) 
```





**Parameters:**


* `text` The new text for the button. 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/form/controls/button.h`

