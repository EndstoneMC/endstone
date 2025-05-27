

# Class endstone::Slider



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Slider**](classendstone_1_1Slider.md)



_Represents a slider with a label._ 

* `#include <endstone/form/controls/slider.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Slider**](#function-slider-12) () = default<br> |
|   | [**Slider**](#function-slider-22) (Message label, float min, float max, float step, std::optional&lt; float &gt; default\_value=std::nullopt) <br> |
|  std::optional&lt; float &gt; | [**getDefaultValue**](#function-getdefaultvalue) () const<br>_Gets the default value of the slider._  |
|  Message | [**getLabel**](#function-getlabel) () const<br>_Gets the label of the toggle._  |
|  float | [**getMax**](#function-getmax) () const<br>_Gets the maximum value of the slider._  |
|  float | [**getMin**](#function-getmin) () const<br>_Gets the minimum value of the slider._  |
|  float | [**getStep**](#function-getstep) () const<br>_Gets the step size of the slider._  |
|  [**Slider**](classendstone_1_1Slider.md) & | [**setDefaultValue**](#function-setdefaultvalue) (std::optional&lt; float &gt; default\_value) <br>_Sets the default value of the slider._  |
|  [**Slider**](classendstone_1_1Slider.md) & | [**setLabel**](#function-setlabel) (Message label) <br>_Sets the label of the toggle._  |
|  [**Slider**](classendstone_1_1Slider.md) & | [**setMax**](#function-setmax) (float max) <br>_Sets the maximum value of the slider._  |
|  [**Slider**](classendstone_1_1Slider.md) & | [**setMin**](#function-setmin) (float min) <br>_Sets the minimum value of the slider._  |
|  [**Slider**](classendstone_1_1Slider.md) & | [**setStep**](#function-setstep) (float step) <br>_Sets the step size of the slider._  |




























## Public Functions Documentation




### function Slider [1/2]

```C++
endstone::Slider::Slider () = default
```




<hr>



### function Slider [2/2]

```C++
inline explicit endstone::Slider::Slider (
    Message label,
    float min,
    float max,
    float step,
    std::optional< float > default_value=std::nullopt
) 
```




<hr>



### function getDefaultValue 

_Gets the default value of the slider._ 
```C++
inline std::optional< float > endstone::Slider::getDefaultValue () const
```





**Returns:**

The default value of the slider. 





        

<hr>



### function getLabel 

_Gets the label of the toggle._ 
```C++
inline Message endstone::Slider::getLabel () const
```





**Returns:**

The label of the toggle. 





        

<hr>



### function getMax 

_Gets the maximum value of the slider._ 
```C++
inline float endstone::Slider::getMax () const
```





**Returns:**

The maximum value of the slider. 





        

<hr>



### function getMin 

_Gets the minimum value of the slider._ 
```C++
inline float endstone::Slider::getMin () const
```





**Returns:**

The minimum value of the slider. 





        

<hr>



### function getStep 

_Gets the step size of the slider._ 
```C++
inline float endstone::Slider::getStep () const
```





**Returns:**

The step size of the slider. 





        

<hr>



### function setDefaultValue 

_Sets the default value of the slider._ 
```C++
inline Slider & endstone::Slider::setDefaultValue (
    std::optional< float > default_value
) 
```





**Parameters:**


* `default_value` The new default value for the slider. 



**Returns:**

A reference to the current slider, for function chaining. 





        

<hr>



### function setLabel 

_Sets the label of the toggle._ 
```C++
inline Slider & endstone::Slider::setLabel (
    Message label
) 
```





**Parameters:**


* `label` The new label for the toggle. 



**Returns:**

A reference to the current toggle. 





        

<hr>



### function setMax 

_Sets the maximum value of the slider._ 
```C++
inline Slider & endstone::Slider::setMax (
    float max
) 
```





**Parameters:**


* `max` The new maximum value for the slider. 



**Returns:**

A reference to the current slider, for function chaining. 





        

<hr>



### function setMin 

_Sets the minimum value of the slider._ 
```C++
inline Slider & endstone::Slider::setMin (
    float min
) 
```





**Parameters:**


* `min` The new minimum value for the slider. 



**Returns:**

A reference to the current slider, for function chaining. 





        

<hr>



### function setStep 

_Sets the step size of the slider._ 
```C++
inline Slider & endstone::Slider::setStep (
    float step
) 
```





**Parameters:**


* `step` The new step size for the slider. 



**Returns:**

A reference to the current slider, for function chaining. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/form/controls/slider.h`

