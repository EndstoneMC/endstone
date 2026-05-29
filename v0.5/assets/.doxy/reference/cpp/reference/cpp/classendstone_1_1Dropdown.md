

# Class endstone::Dropdown



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Dropdown**](classendstone_1_1Dropdown.md)



_Represents a dropdown with a set of predefined options._ 

* `#include <endstone/form/controls/dropdown.h>`





Inherited by the following classes: [endstone::StepSlider](classendstone_1_1StepSlider.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Dropdown**](#function-dropdown-12) () = default<br> |
|   | [**Dropdown**](#function-dropdown-22) (Message label, std::vector&lt; std::string &gt; options, std::optional&lt; int &gt; default\_index=std::nullopt) <br> |
|  [**Dropdown**](classendstone_1_1Dropdown.md) & | [**addOption**](#function-addoption) (const std::string & option) <br>_Adds a new option to the dropdown._  |
|  std::optional&lt; int &gt; | [**getDefaultIndex**](#function-getdefaultindex) () const<br>_Gets the default index of the dropdown._  |
|  Message | [**getLabel**](#function-getlabel) () const<br>_Gets the label of the dropdown._  |
|  std::vector&lt; std::string &gt; | [**getOptions**](#function-getoptions) () const<br>_Gets the options of the dropdown._  |
|  [**Dropdown**](classendstone_1_1Dropdown.md) & | [**setDefaultIndex**](#function-setdefaultindex) (std::optional&lt; int &gt; default\_index) <br>_Sets the default index of the dropdown._  |
|  [**Dropdown**](classendstone_1_1Dropdown.md) & | [**setLabel**](#function-setlabel) (Message label) <br>_Sets the label of the dropdown._  |
|  [**Dropdown**](classendstone_1_1Dropdown.md) & | [**setOptions**](#function-setoptions) (std::vector&lt; std::string &gt; options) <br>_Sets the options of the dropdown._  |




























## Public Functions Documentation




### function Dropdown [1/2]

```C++
endstone::Dropdown::Dropdown () = default
```




<hr>



### function Dropdown [2/2]

```C++
inline explicit endstone::Dropdown::Dropdown (
    Message label,
    std::vector< std::string > options,
    std::optional< int > default_index=std::nullopt
) 
```




<hr>



### function addOption 

_Adds a new option to the dropdown._ 
```C++
inline Dropdown & endstone::Dropdown::addOption (
    const std::string & option
) 
```





**Parameters:**


* `option` The new option for the dropdown. 



**Returns:**

A reference to the dropdown itself. 





        

<hr>



### function getDefaultIndex 

_Gets the default index of the dropdown._ 
```C++
inline std::optional< int > endstone::Dropdown::getDefaultIndex () const
```





**Returns:**

The default index of the dropdown. 





        

<hr>



### function getLabel 

_Gets the label of the dropdown._ 
```C++
inline Message endstone::Dropdown::getLabel () const
```





**Returns:**

The label of the dropdown. 





        

<hr>



### function getOptions 

_Gets the options of the dropdown._ 
```C++
inline std::vector< std::string > endstone::Dropdown::getOptions () const
```





**Returns:**

The options of the dropdown. 





        

<hr>



### function setDefaultIndex 

_Sets the default index of the dropdown._ 
```C++
inline Dropdown & endstone::Dropdown::setDefaultIndex (
    std::optional< int > default_index
) 
```





**Parameters:**


* `default_index` The new default index for the dropdown. 



**Returns:**

A reference to the dropdown itself. 





        

<hr>



### function setLabel 

_Sets the label of the dropdown._ 
```C++
inline Dropdown & endstone::Dropdown::setLabel (
    Message label
) 
```





**Parameters:**


* `label` The new label for the dropdown. 



**Returns:**

A reference to the dropdown itself. 





        

<hr>



### function setOptions 

_Sets the options of the dropdown._ 
```C++
inline Dropdown & endstone::Dropdown::setOptions (
    std::vector< std::string > options
) 
```





**Parameters:**


* `options` The new options for the dropdown. 



**Returns:**

A reference to the dropdown itself. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/form/controls/dropdown.h`

