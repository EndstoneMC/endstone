

# Class endstone::MessageForm



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**MessageForm**](classendstone_1_1MessageForm.md)



_Represents a form with two buttons._ 

* `#include <endstone/form/message_form.h>`



Inherits the following classes: [endstone::Form](classendstone_1_1Form.md)














## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::function&lt; void([**Player**](classendstone_1_1Player.md) \*, int)&gt; | [**OnSubmitCallback**](#typedef-onsubmitcallback)  <br> |


## Public Types inherited from endstone::Form

See [endstone::Form](classendstone_1_1Form.md)

| Type | Name |
| ---: | :--- |
| typedef std::function&lt; void([**Player**](classendstone_1_1Player.md) \*)&gt; | [**OnCloseCallback**](classendstone_1_1Form.md#typedef-onclosecallback)  <br> |






































## Public Functions

| Type | Name |
| ---: | :--- |
|  Message | [**getButton1**](#function-getbutton1) () const<br>_Get the text of button1._  |
|  Message | [**getButton2**](#function-getbutton2) () const<br>_Get the text of button2._  |
|  Message | [**getContent**](#function-getcontent) () const<br>_Get the content of the form._  |
|  OnSubmitCallback | [**getOnSubmit**](#function-getonsubmit) () const<br>_Gets the on submit callback of the form._  |
|  [**MessageForm**](classendstone_1_1MessageForm.md) & | [**setButton1**](#function-setbutton1) (Message text) <br>_Set the text of button1._  |
|  [**MessageForm**](classendstone_1_1MessageForm.md) & | [**setButton2**](#function-setbutton2) (Message text) <br>_Set the text of button2._  |
|  [**MessageForm**](classendstone_1_1MessageForm.md) & | [**setContent**](#function-setcontent) (Message text) <br>_Set the content of the form._  |
|  [**MessageForm**](classendstone_1_1MessageForm.md) & | [**setOnSubmit**](#function-setonsubmit) (OnSubmitCallback on\_submit) <br>_Sets the on submit callback of the form._  |


## Public Functions inherited from endstone::Form

See [endstone::Form](classendstone_1_1Form.md)

| Type | Name |
| ---: | :--- |
|   | [**Form**](classendstone_1_1Form.md#function-form) () = default<br> |
|  OnCloseCallback | [**getOnClose**](classendstone_1_1Form.md#function-getonclose) () const<br>_Gets the on close callback of the form._  |
|  Message | [**getTitle**](classendstone_1_1Form.md#function-gettitle) () const<br>_Gets the title of the form._  |
|  T & | [**setOnClose**](classendstone_1_1Form.md#function-setonclose) (OnCloseCallback on\_close) <br>_Sets the on close callback of the form._  |
|  T & | [**setTitle**](classendstone_1_1Form.md#function-settitle) (Message title) <br>_Sets the title of the form._  |
















## Protected Attributes inherited from endstone::Form

See [endstone::Form](classendstone_1_1Form.md)

| Type | Name |
| ---: | :--- |
|  OnCloseCallback | [**on\_close\_**](classendstone_1_1Form.md#variable-on_close_)  <br> |
|  Message | [**title\_**](classendstone_1_1Form.md#variable-title_)  <br> |






































## Public Types Documentation




### typedef OnSubmitCallback 

```C++
using endstone::MessageForm::OnSubmitCallback =  std::function<void(Player *, int)>;
```




<hr>
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



### function getOnSubmit 

_Gets the on submit callback of the form._ 
```C++
inline OnSubmitCallback endstone::MessageForm::getOnSubmit () const
```





**Returns:**

The on submit callback of the form. 





        

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



### function setOnSubmit 

_Sets the on submit callback of the form._ 
```C++
inline MessageForm & endstone::MessageForm::setOnSubmit (
    OnSubmitCallback on_submit
) 
```





**Parameters:**


* `on_submit` The callback to be set. 



**Returns:**

A reference to the current form. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/form/message_form.h`

