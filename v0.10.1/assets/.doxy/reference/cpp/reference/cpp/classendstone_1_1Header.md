

# Class endstone::Header



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Header**](classendstone_1_1Header.md)



_Represents a header with a label._ 

* `#include <endstone/form/controls/header.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Header**](#function-header-12) () = default<br> |
|   | [**Header**](#function-header-22) (Message label) <br> |
|  Message | [**getLabel**](#function-getlabel) () const<br>_Gets the label of the header._  |
|  [**Header**](classendstone_1_1Header.md) & | [**setLabel**](#function-setlabel) (Message label) <br>_Sets the label of the header._  |




























## Public Functions Documentation




### function Header [1/2]

```C++
endstone::Header::Header () = default
```




<hr>



### function Header [2/2]

```C++
inline explicit endstone::Header::Header (
    Message label
) 
```




<hr>



### function getLabel 

_Gets the label of the header._ 
```C++
inline Message endstone::Header::getLabel () const
```





**Returns:**

The label of the header. 





        

<hr>



### function setLabel 

_Sets the label of the header._ 
```C++
inline Header & endstone::Header::setLabel (
    Message label
) 
```





**Parameters:**


* `label` The new label for the header. 



**Returns:**

A reference to the current header. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/form/controls/header.h`

