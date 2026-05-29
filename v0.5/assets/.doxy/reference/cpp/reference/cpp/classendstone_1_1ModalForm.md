

# Class endstone::ModalForm



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ModalForm**](classendstone_1_1ModalForm.md)



_Represents a modal form with controls._ 

* `#include <endstone/form/modal_form.h>`



Inherits the following classes: [endstone::Form](classendstone_1_1Form.md)














## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::variant&lt; [**Dropdown**](classendstone_1_1Dropdown.md), [**Label**](classendstone_1_1Label.md), [**Slider**](classendstone_1_1Slider.md), [**StepSlider**](classendstone_1_1StepSlider.md), [**TextInput**](classendstone_1_1TextInput.md), [**Toggle**](classendstone_1_1Toggle.md) &gt; | [**Control**](#typedef-control)  <br> |
| typedef std::function&lt; void([**Player**](classendstone_1_1Player.md) \*, std::string)&gt; | [**OnSubmitCallback**](#typedef-onsubmitcallback)  <br> |


## Public Types inherited from endstone::Form

See [endstone::Form](classendstone_1_1Form.md)

| Type | Name |
| ---: | :--- |
| typedef std::function&lt; void([**Player**](classendstone_1_1Player.md) \*)&gt; | [**OnCloseCallback**](classendstone_1_1Form.md#typedef-onclosecallback)  <br> |






































## Public Functions

| Type | Name |
| ---: | :--- |
|  [**ModalForm**](classendstone_1_1ModalForm.md) & | [**addControl**](#function-addcontrol) (const Control & control) <br>_Adds a control to the form._  |
|  std::vector&lt; Control &gt; | [**getControls**](#function-getcontrols) () const<br>_Gets the controls of the modal form._  |
|  std::optional&lt; std::string &gt; | [**getIcon**](#function-geticon) () const<br>_Get the icon of the form._  |
|  OnSubmitCallback | [**getOnSubmit**](#function-getonsubmit) () const<br>_Gets the on submit callback of the form._  |
|  std::optional&lt; Message &gt; | [**getSubmitButton**](#function-getsubmitbutton) () const<br>_Gets the submit button text of the form._  |
|  [**ModalForm**](classendstone_1_1ModalForm.md) & | [**setControls**](#function-setcontrols) (std::vector&lt; Control &gt; controls) <br>_Sets the controls of the modal form._  |
|  [**ModalForm**](classendstone_1_1ModalForm.md) & | [**setIcon**](#function-seticon) (std::optional&lt; std::string &gt; icon) <br>_Sets the icon of the form._  |
|  [**ModalForm**](classendstone_1_1ModalForm.md) & | [**setOnSubmit**](#function-setonsubmit) (OnSubmitCallback on\_submit) <br>_Sets the on submit callback of the form._  |
|  [**ModalForm**](classendstone_1_1ModalForm.md) & | [**setSubmitButton**](#function-setsubmitbutton) (std::optional&lt; Message &gt; text) <br>_Sets the submit button text of the form._  |


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




### typedef Control 

```C++
using endstone::ModalForm::Control =  std::variant<Dropdown, Label, Slider, StepSlider, TextInput, Toggle>;
```




<hr>



### typedef OnSubmitCallback 

```C++
using endstone::ModalForm::OnSubmitCallback =  std::function<void(Player *, std::string)>;
```




<hr>
## Public Functions Documentation




### function addControl 

_Adds a control to the form._ 
```C++
inline ModalForm & endstone::ModalForm::addControl (
    const Control & control
) 
```





**Parameters:**


* `control` The control to add to the form. 



**Returns:**

A reference to the current form. 





        

<hr>



### function getControls 

_Gets the controls of the modal form._ 
```C++
inline std::vector< Control > endstone::ModalForm::getControls () const
```





**Returns:**

A list of controls in the modal form. 





        

<hr>



### function getIcon 

_Get the icon of the form._ 
```C++
inline std::optional< std::string > endstone::ModalForm::getIcon () const
```





**Returns:**

The path or URL to the icon file 





        

<hr>



### function getOnSubmit 

_Gets the on submit callback of the form._ 
```C++
inline OnSubmitCallback endstone::ModalForm::getOnSubmit () const
```





**Returns:**

The on submit callback of the form. 





        

<hr>



### function getSubmitButton 

_Gets the submit button text of the form._ 
```C++
inline std::optional< Message > endstone::ModalForm::getSubmitButton () const
```





**Returns:**

The submit button text of the form. 





        

<hr>



### function setControls 

_Sets the controls of the modal form._ 
```C++
inline ModalForm & endstone::ModalForm::setControls (
    std::vector< Control > controls
) 
```





**Parameters:**


* `controls` The list of controls to set. 



**Returns:**

A reference to the current form. 





        

<hr>



### function setIcon 

_Sets the icon of the form._ 
```C++
inline ModalForm & endstone::ModalForm::setIcon (
    std::optional< std::string > icon
) 
```





**Parameters:**


* `icon` The path or URL to the icon file. 



**Returns:**

A reference to the current form. 





        

<hr>



### function setOnSubmit 

_Sets the on submit callback of the form._ 
```C++
inline ModalForm & endstone::ModalForm::setOnSubmit (
    OnSubmitCallback on_submit
) 
```





**Parameters:**


* `on_submit` The callback to be set. 



**Returns:**

A reference to the current form. 





        

<hr>



### function setSubmitButton 

_Sets the submit button text of the form._ 
```C++
inline ModalForm & endstone::ModalForm::setSubmitButton (
    std::optional< Message > text
) 
```





**Parameters:**


* `text` The submit button text to set. 



**Returns:**

A reference to the current form. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/form/modal_form.h`

