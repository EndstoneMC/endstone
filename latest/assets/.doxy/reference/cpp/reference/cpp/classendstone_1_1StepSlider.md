

# Class endstone::StepSlider



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**StepSlider**](classendstone_1_1StepSlider.md)



_Represents a step slider with a set of predefined options._ 

* `#include <endstone/form/controls/step_slider.h>`



Inherits the following classes: [endstone::Dropdown](classendstone_1_1Dropdown.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Dropdown**](#function-dropdown-12) () = default<br> |
|   | [**Dropdown**](#function-dropdown-22) (Message label, std::vector&lt; std::string &gt; options, std::optional&lt; int &gt; default\_index=std::nullopt) <br> |


## Public Functions inherited from endstone::Dropdown

See [endstone::Dropdown](classendstone_1_1Dropdown.md)

| Type | Name |
| ---: | :--- |
|   | [**Dropdown**](classendstone_1_1Dropdown.md#function-dropdown-12) () = default<br> |
|   | [**Dropdown**](classendstone_1_1Dropdown.md#function-dropdown-22) (Message label, std::vector&lt; std::string &gt; options, std::optional&lt; int &gt; default\_index=std::nullopt) <br> |
|  [**Dropdown**](classendstone_1_1Dropdown.md) & | [**addOption**](classendstone_1_1Dropdown.md#function-addoption) (const std::string & option) <br>_Adds a new option to the dropdown._  |
|  std::optional&lt; int &gt; | [**getDefaultIndex**](classendstone_1_1Dropdown.md#function-getdefaultindex) () const<br>_Gets the default index of the dropdown._  |
|  Message | [**getLabel**](classendstone_1_1Dropdown.md#function-getlabel) () const<br>_Gets the label of the dropdown._  |
|  std::vector&lt; std::string &gt; | [**getOptions**](classendstone_1_1Dropdown.md#function-getoptions) () const<br>_Gets the options of the dropdown._  |
|  [**Dropdown**](classendstone_1_1Dropdown.md) & | [**setDefaultIndex**](classendstone_1_1Dropdown.md#function-setdefaultindex) (std::optional&lt; int &gt; default\_index) <br>_Sets the default index of the dropdown._  |
|  [**Dropdown**](classendstone_1_1Dropdown.md) & | [**setLabel**](classendstone_1_1Dropdown.md#function-setlabel) (Message label) <br>_Sets the label of the dropdown._  |
|  [**Dropdown**](classendstone_1_1Dropdown.md) & | [**setOptions**](classendstone_1_1Dropdown.md#function-setoptions) (std::vector&lt; std::string &gt; options) <br>_Sets the options of the dropdown._  |






















































## Public Functions Documentation




### function Dropdown [1/2]

```C++
endstone::StepSlider::Dropdown () = default
```




<hr>



### function Dropdown [2/2]

```C++
inline explicit endstone::StepSlider::Dropdown (
    Message label,
    std::vector< std::string > options,
    std::optional< int > default_index=std::nullopt
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/form/controls/step_slider.h`

