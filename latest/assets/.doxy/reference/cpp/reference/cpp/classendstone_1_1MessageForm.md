

# Class endstone::MessageForm



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**MessageForm**](classendstone_1_1MessageForm.md)



_Represents a form with two buttons._ 

* `#include <endstone/form/message_form.h>`



Inherits the following classes: [endstone::Form](classendstone_1_1Form.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
|  Message | [**getButton1**](#function-getbutton1) () const<br>_Get the text of button1._  |
|  Message | [**getButton2**](#function-getbutton2) () const<br>_Get the text of button2._  |
|  Message | [**getContent**](#function-getcontent) () const<br>_Get the content of the form._  |
|  [**MessageForm**](classendstone_1_1MessageForm.md) & | [**setButton1**](#function-setbutton1) (Message text) <br>_Set the text of button1._  |
|  [**MessageForm**](classendstone_1_1MessageForm.md) & | [**setButton2**](#function-setbutton2) (Message text) <br>_Set the text of button2._  |
|  [**MessageForm**](classendstone_1_1MessageForm.md) & | [**setContent**](#function-setcontent) (Message text) <br>_Set the content of the form._  |


## Public Functions inherited from endstone::Form

See [endstone::Form](classendstone_1_1Form.md)

| Type | Name |
| ---: | :--- |
|   | [**Form**](classendstone_1_1Form.md#function-form) () = default<br> |
|  Message | [**getTitle**](classendstone_1_1Form.md#function-gettitle) () const<br>_Gets the title of the form._  |
|  T & | [**setTitle**](classendstone_1_1Form.md#function-settitle) (Message title) <br>_Sets the title of the form._  |
















## Protected Attributes inherited from endstone::Form

See [endstone::Form](classendstone_1_1Form.md)

| Type | Name |
| ---: | :--- |
|  Message | [**title\_**](classendstone_1_1Form.md#variable-title_)  <br> |






































## Public Functions Documentation




### function getButton1 

_Get the text of button1._ 
```C++
inline Message endstone::MessageForm::getButton1 () const
```





**Returns:**

The text of button1. 





        

<hr>



### function getButton2 

_Get the text of button2._ 
```C++
inline Message endstone::MessageForm::getButton2 () const
```





**Returns:**

The text of button2. 





        

<hr>



### function getContent 

_Get the content of the form._ 
```C++
inline Message endstone::MessageForm::getContent () const
```





**Returns:**

The content of the form. 





        

<hr>



### function setButton1 

_Set the text of button1._ 
```C++
inline MessageForm & endstone::MessageForm::setButton1 (
    Message text
) 
```





**Parameters:**


* `text` The text to set as the button1 text. 



**Returns:**

A reference to the current form. 





        

<hr>



### function setButton2 

_Set the text of button2._ 
```C++
inline MessageForm & endstone::MessageForm::setButton2 (
    Message text
) 
```





**Parameters:**


* `text` The text to set as the button2 text. 



**Returns:**

A reference to the current form. 





        

<hr>



### function setContent 

_Set the content of the form._ 
```C++
inline MessageForm & endstone::MessageForm::setContent (
    Message text
) 
```





**Parameters:**


* `text` The text to set as the content. 



**Returns:**

A reference to the current form. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/form/message_form.h`

