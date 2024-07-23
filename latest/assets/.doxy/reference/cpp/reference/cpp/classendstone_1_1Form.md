

# Class endstone::Form

**template &lt;typename T&gt;**



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Form**](classendstone_1_1Form.md)



_Represents a generic form._ [More...](#detailed-description)

* `#include <endstone/form/form.h>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::variant&lt; std::string, [**Translatable**](classendstone_1_1Translatable.md) &gt; | [**TextType**](#typedef-texttype)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Form**](#function-form) () <br> |
|  TextType | [**getTitle**](#function-gettitle) () const<br>_Gets the title of the form._  |
|  T & | [**setTitle**](#function-settitle) (TextType title) <br>_Sets the title of the form._  |








## Protected Attributes

| Type | Name |
| ---: | :--- |
|  TextType | [**title\_**](#variable-title_)  <br> |




















# Detailed Description


The [**Form**](classendstone_1_1Form.md) class provides a base abstraction for different types of forms. It supports three types of forms: Action, Message, and Modal. Each form has a title, which can be a string or a translatable object. 


    
## Public Types Documentation




### typedef TextType 

```C++
using endstone::Form< T >::TextType =  std::variant<std::string, Translatable>;
```




<hr>
## Public Functions Documentation




### function Form 

```C++
inline explicit endstone::Form::Form () 
```




<hr>



### function getTitle 

_Gets the title of the form._ 
```C++
inline TextType endstone::Form::getTitle () const
```





**Returns:**

The title of the form. 





        

<hr>



### function setTitle 

_Sets the title of the form._ 
```C++
inline T & endstone::Form::setTitle (
    TextType title
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
TextType endstone::Form< T >::title_;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/form/form.h`

