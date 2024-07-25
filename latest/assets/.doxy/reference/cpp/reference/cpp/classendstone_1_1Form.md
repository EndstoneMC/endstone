

# Class endstone::Form

**template &lt;typename T&gt;**



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Form**](classendstone_1_1Form.md)



_Represents a generic form._ [More...](#detailed-description)

* `#include <endstone/form/form.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Form**](#function-form) () = default<br> |
|  std::function&lt; void([**Player**](classendstone_1_1Player.md) \*)&gt; | [**getOnClose**](#function-getonclose) () const<br>_Gets the on close callback of the form._  |
|  Message | [**getTitle**](#function-gettitle) () const<br>_Gets the title of the form._  |
|  T & | [**setOnClose**](#function-setonclose) (std::function&lt; void([**Player**](classendstone_1_1Player.md) \*)&gt; on\_close) <br>_Sets the on close callback of the form._  |
|  T & | [**setTitle**](#function-settitle) (Message title) <br>_Sets the title of the form._  |








## Protected Attributes

| Type | Name |
| ---: | :--- |
|  std::function&lt; void([**Player**](classendstone_1_1Player.md) \*)&gt; | [**on\_close\_**](#variable-on_close_)  <br> |
|  Message | [**title\_**](#variable-title_)  <br> |




















# Detailed Description


The [**Form**](classendstone_1_1Form.md) class provides a base abstraction for different types of forms. It supports three types of forms: Action, Message, and Modal. Each form has a title, which can be a string or a translatable object. 


    
## Public Functions Documentation




### function Form 

```C++
explicit endstone::Form::Form () = default
```




<hr>



### function getOnClose 

_Gets the on close callback of the form._ 
```C++
inline std::function< void( Player *)> endstone::Form::getOnClose () const
```





**Returns:**

The on close callback of the form. 





        

<hr>



### function getTitle 

_Gets the title of the form._ 
```C++
inline Message endstone::Form::getTitle () const
```





**Returns:**

The title of the form. 





        

<hr>



### function setOnClose 

_Sets the on close callback of the form._ 
```C++
inline T & endstone::Form::setOnClose (
    std::function< void( Player *)> on_close
) 
```





**Parameters:**


* `on_close` The callback to be set. 



**Returns:**

A reference to the current form. 





        

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




### variable on\_close\_ 

```C++
std::function<void(Player *)> endstone::Form< T >::on_close_;
```




<hr>



### variable title\_ 

```C++
Message endstone::Form< T >::title_;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/form/form.h`

