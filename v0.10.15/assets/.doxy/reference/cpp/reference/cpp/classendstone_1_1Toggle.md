

# Class endstone::Toggle



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Toggle**](classendstone_1_1Toggle.md)



_Represents a toggle button with a label._ 

* `#include <endstone/form/controls/toggle.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Toggle**](#function-toggle-12) () = default<br> |
|   | [**Toggle**](#function-toggle-22) (Message label, bool default\_value=false) <br> |
|  bool | [**getDefaultValue**](#function-getdefaultvalue) () const<br>_Gets the default value of the toggle._  |
|  Message | [**getLabel**](#function-getlabel) () const<br>_Gets the label of the toggle._  |
|  [**Toggle**](classendstone_1_1Toggle.md) & | [**setDefaultValue**](#function-setdefaultvalue) (bool value) <br>_Sets the default value of the toggle._  |
|  [**Toggle**](classendstone_1_1Toggle.md) & | [**setLabel**](#function-setlabel) (Message label) <br>_Sets the label of the toggle._  |




























## Public Functions Documentation




### function Toggle [1/2]

```C++
endstone::Toggle::Toggle () = default
```




<hr>



### function Toggle [2/2]

```C++
inline explicit endstone::Toggle::Toggle (
    Message label,
    bool default_value=false
) 
```




<hr>



### function getDefaultValue 

_Gets the default value of the toggle._ 
```C++
inline bool endstone::Toggle::getDefaultValue () const
```





**Returns:**

The default value of the toggle. 





        

<hr>



### function getLabel 

_Gets the label of the toggle._ 
```C++
inline Message endstone::Toggle::getLabel () const
```





**Returns:**

The label of the toggle. 





        

<hr>



### function setDefaultValue 

_Sets the default value of the toggle._ 
```C++
inline Toggle & endstone::Toggle::setDefaultValue (
    bool value
) 
```





**Parameters:**


* `value` The new default value for the toggle. 



**Returns:**

A reference to the current toggle. 





        

<hr>



### function setLabel 

_Sets the label of the toggle._ 
```C++
inline Toggle & endstone::Toggle::setLabel (
    Message label
) 
```





**Parameters:**


* `label` The new label for the toggle. 



**Returns:**

A reference to the current toggle. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/form/controls/toggle.h`

