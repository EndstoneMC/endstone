

# Class endstone::Label



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Label**](classendstone_1_1Label.md)



_Represents a text label._ 

* `#include <endstone/form/controls/label.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Label**](#function-label-12) () = default<br> |
|   | [**Label**](#function-label-22) (Message text) <br> |
|  Message | [**getText**](#function-gettext) () const<br>_Gets the text of the label._  |
|  [**Label**](classendstone_1_1Label.md) & | [**setText**](#function-settext) (Message text) <br>_Sets the text of the label._  |




























## Public Functions Documentation




### function Label [1/2]

```C++
endstone::Label::Label () = default
```




<hr>



### function Label [2/2]

```C++
inline explicit endstone::Label::Label (
    Message text
) 
```




<hr>



### function getText 

_Gets the text of the label._ 
```C++
inline Message endstone::Label::getText () const
```





**Returns:**

The text of the label. 





        

<hr>



### function setText 

_Sets the text of the label._ 
```C++
inline Label & endstone::Label::setText (
    Message text
) 
```





**Parameters:**


* `text` The new text for the label. 



**Returns:**

A reference to the current label. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/form/controls/label.h`

