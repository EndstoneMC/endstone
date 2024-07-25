

# Class endstone::ActionForm::Button



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ActionForm**](classendstone_1_1ActionForm.md) **>** [**Button**](classendstone_1_1ActionForm_1_1Button.md)



_Represents a button with text and an optional icon._ 

* `#include <endstone/form/action_form.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Button**](#function-button-12) () = default<br> |
|   | [**Button**](#function-button-22) (Message text, std::optional&lt; std::string &gt; icon=std::nullopt) <br> |
|  std::optional&lt; std::string &gt; | [**getIcon**](#function-geticon) () const<br>_Get the icon of the button._  |
|  Message | [**getText**](#function-gettext) () const<br>_Gets the text of the button._  |
|  [**Button**](classendstone_1_1ActionForm_1_1Button.md) & | [**setIcon**](#function-seticon) (std::string icon) <br>_Sets the icon for the button._  |
|  [**Button**](classendstone_1_1ActionForm_1_1Button.md) & | [**setText**](#function-settext) (Message text) <br>_Sets the text of the button._  |




























## Public Functions Documentation




### function Button [1/2]

```C++
endstone::ActionForm::Button::Button () = default
```




<hr>



### function Button [2/2]

```C++
inline explicit endstone::ActionForm::Button::Button (
    Message text,
    std::optional< std::string > icon=std::nullopt
) 
```




<hr>



### function getIcon 

_Get the icon of the button._ 
```C++
inline std::optional< std::string > endstone::ActionForm::Button::getIcon () const
```





**Returns:**

The path or URL to the icon file 





        

<hr>



### function getText 

_Gets the text of the button._ 
```C++
inline Message endstone::ActionForm::Button::getText () const
```





**Returns:**

The text. 





        

<hr>



### function setIcon 

_Sets the icon for the button._ 
```C++
inline Button & endstone::ActionForm::Button::setIcon (
    std::string icon
) 
```





**Parameters:**


* `icon` The path or URL to the icon file. 



**Returns:**

A reference to the current button. 





        

<hr>



### function setText 

_Sets the text of the button._ 
```C++
inline Button & endstone::ActionForm::Button::setText (
    Message text
) 
```





**Parameters:**


* `text` The new text for the button. 



**Returns:**

A reference to the current button. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/form/action_form.h`

