

# Class endstone::ActionForm



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ActionForm**](classendstone_1_1ActionForm.md)



_Represents a form with buttons that let the player take action._ 

* `#include <endstone/form/action_form.h>`



Inherits the following classes: [endstone::Form](classendstone_1_1Form.md)












## Classes

| Type | Name |
| ---: | :--- |
| class | [**Button**](classendstone_1_1ActionForm_1_1Button.md) <br>_Represents a button with text and an optional icon._  |


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
|   | [**ActionForm**](#function-actionform) () = default<br> |
|  [**ActionForm**](classendstone_1_1ActionForm.md) & | [**addButton**](#function-addbutton) (const Message & text, const std::optional&lt; std::string &gt; & icon=std::nullopt, Button::OnClickCallback on\_click={}) <br>_Adds a button to the form._  |
|  const std::vector&lt; [**Button**](classendstone_1_1ActionForm_1_1Button.md) &gt; & | [**getButtons**](#function-getbuttons) () const<br>_Get the buttons of the action form._  |
|  Message | [**getContent**](#function-getcontent) () const<br>_Get the content of the form._  |
|  OnSubmitCallback | [**getOnSubmit**](#function-getonsubmit) () const<br>_Gets the on submit callback of the form._  |
|  [**ActionForm**](classendstone_1_1ActionForm.md) & | [**setButtons**](#function-setbuttons) (const std::vector&lt; [**Button**](classendstone_1_1ActionForm_1_1Button.md) &gt; & buttons) <br>_Set the buttons of the action form._  |
|  [**ActionForm**](classendstone_1_1ActionForm.md) & | [**setContent**](#function-setcontent) (Message text) <br>_Set the content of the form._  |
|  [**ActionForm**](classendstone_1_1ActionForm.md) & | [**setOnSubmit**](#function-setonsubmit) (OnSubmitCallback on\_submit) <br>_Sets the on submit callback of the form._  |


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
using endstone::ActionForm::OnSubmitCallback =  std::function<void(Player *, int)>;
```




<hr>
## Public Functions Documentation




### function ActionForm 

```C++
explicit endstone::ActionForm::ActionForm () = default
```




<hr>



### function addButton 

_Adds a button to the form._ 
```C++
inline ActionForm & endstone::ActionForm::addButton (
    const Message & text,
    const std::optional< std::string > & icon=std::nullopt,
    Button::OnClickCallback on_click={}
) 
```





**Parameters:**


* `text` The text of the button 
* `icon` The path or URL to the icon image 
* `on_click` The on click callback of the button 



**Returns:**

A reference to the current form. 





        

<hr>



### function getButtons 

_Get the buttons of the action form._ 
```C++
inline const std::vector< Button > & endstone::ActionForm::getButtons () const
```





**Returns:**

A list of buttons in the action form. 





        

<hr>



### function getContent 

_Get the content of the form._ 
```C++
inline Message endstone::ActionForm::getContent () const
```





**Returns:**

The content of the form. 





        

<hr>



### function getOnSubmit 

_Gets the on submit callback of the form._ 
```C++
inline OnSubmitCallback endstone::ActionForm::getOnSubmit () const
```





**Returns:**

The on submit callback of the form. 





        

<hr>



### function setButtons 

_Set the buttons of the action form._ 
```C++
inline ActionForm & endstone::ActionForm::setButtons (
    const std::vector< Button > & buttons
) 
```





**Parameters:**


* `buttons` The list of buttons to set. 



**Returns:**

A reference to the current form. 





        

<hr>



### function setContent 

_Set the content of the form._ 
```C++
inline ActionForm & endstone::ActionForm::setContent (
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
inline ActionForm & endstone::ActionForm::setOnSubmit (
    OnSubmitCallback on_submit
) 
```





**Parameters:**


* `on_submit` The callback to be set. 



**Returns:**

A reference to the current form. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/form/action_form.h`

