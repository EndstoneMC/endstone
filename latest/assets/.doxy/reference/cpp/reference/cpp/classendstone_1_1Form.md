

# Class endstone::Form

**template &lt;typename T&gt;**



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Form**](classendstone_1_1Form.md)



_Represents a generic form._ [More...](#detailed-description)

* `#include <endstone/form/form.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Form**](#function-form) () <br> |
|  Message | [**getTitle**](#function-gettitle) () const<br>_Gets the title of the form._  |
|  T & | [**setTitle**](#function-settitle) (Message title) <br>_Sets the title of the form._  |








## Protected Attributes

| Type | Name |
| ---: | :--- |
|  Message | [**title\_**](#variable-title_)  <br> |




















# Detailed Description


The [**Form**](classendstone_1_1Form.md) class provides a base abstraction for different types of forms. It supports three types of forms: Action, Message, and Modal. Each form has a title, which can be a string or a translatable object. 


    
## Public Functions Documentation




### function Form 

```C++
inline explicit endstone::Form::Form () 
```




<hr>



### function getTitle 

_Gets the title of the form._ 
```C++
inline Message endstone::Form::getTitle () const
```





**Returns:**

The title of the form. 





        

<hr>



### function setTitle 

_Sets the title of the form._ 
```C++
inline T & endstone::Form::setTitle (
    Message title
) 
```





**Parameters:**


* `title` The title of the form to be set. 



**Returns:**

A reference to the current form. 





        

<hr>
## Protected Attributes Documentation




### variable title\_ 

```C++
Message endstone::Form< T >::title_;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/form/form.h`

