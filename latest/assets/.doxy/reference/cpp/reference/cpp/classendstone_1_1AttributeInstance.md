

# Class endstone::AttributeInstance



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**AttributeInstance**](classendstone_1_1AttributeInstance.md)



_Represents a mutable instance of an attribute and its associated modifiers and values._ 

* `#include <endstone/attribute/attribute_instance.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual float | [**getBaseMaxValue**](#function-getbasemaxvalue) () const = 0<br>_Base max value of this instance before modifiers are applied._  |
| virtual float | [**getBaseMinValue**](#function-getbaseminvalue) () const = 0<br>_Base min value of this instance before modifiers are applied._  |
| virtual float | [**getBaseValue**](#function-getbasevalue) () const = 0<br>_Base value of this instance before modifiers are applied._  |
| virtual float | [**getMaxValue**](#function-getmaxvalue) () const = 0<br> |
| virtual float | [**getMinValue**](#function-getminvalue) () const = 0<br> |
| virtual std::string | [**getType**](#function-gettype) () const = 0<br>_The attribute type pertaining to this instance._  |
| virtual float | [**getValue**](#function-getvalue) () const = 0<br> |
| virtual void | [**setBaseMaxValue**](#function-setbasemaxvalue) (float value) = 0<br>_Set the base max value of this instance._  |
| virtual void | [**setBaseMinValue**](#function-setbaseminvalue) (float value) = 0<br>_Set the base min value of this instance._  |
| virtual void | [**setBaseValue**](#function-setbasevalue) (float value) = 0<br>_Set the base value of this instance._  |
| virtual  | [**~AttributeInstance**](#function-attributeinstance) () = default<br> |




























## Public Functions Documentation




### function getBaseMaxValue 

_Base max value of this instance before modifiers are applied._ 
```C++
virtual float endstone::AttributeInstance::getBaseMaxValue () const = 0
```





**Returns:**

base max value 





        

<hr>



### function getBaseMinValue 

_Base min value of this instance before modifiers are applied._ 
```C++
virtual float endstone::AttributeInstance::getBaseMinValue () const = 0
```





**Returns:**

base max value 





        

<hr>



### function getBaseValue 

_Base value of this instance before modifiers are applied._ 
```C++
virtual float endstone::AttributeInstance::getBaseValue () const = 0
```





**Returns:**

base value 





        

<hr>



### function getMaxValue 

```C++
virtual float endstone::AttributeInstance::getMaxValue () const = 0
```



Get the max value of this instance after all associated modifiers have been applied.




**Returns:**

the max attribute value 





        

<hr>



### function getMinValue 

```C++
virtual float endstone::AttributeInstance::getMinValue () const = 0
```



Get the min value of this instance after all associated modifiers have been applied.




**Returns:**

the min attribute value 





        

<hr>



### function getType 

_The attribute type pertaining to this instance._ 
```C++
virtual std::string endstone::AttributeInstance::getType () const = 0
```





**Returns:**

the attribute type 





        

<hr>



### function getValue 

```C++
virtual float endstone::AttributeInstance::getValue () const = 0
```



Get the value of this instance after all associated modifiers have been applied.




**Returns:**

the total attribute value 





        

<hr>



### function setBaseMaxValue 

_Set the base max value of this instance._ 
```C++
virtual void endstone::AttributeInstance::setBaseMaxValue (
    float value
) = 0
```





**Parameters:**


* `value` new base max value 




        

<hr>



### function setBaseMinValue 

_Set the base min value of this instance._ 
```C++
virtual void endstone::AttributeInstance::setBaseMinValue (
    float value
) = 0
```





**Parameters:**


* `value` new base min value 




        

<hr>



### function setBaseValue 

_Set the base value of this instance._ 
```C++
virtual void endstone::AttributeInstance::setBaseValue (
    float value
) = 0
```





**Parameters:**


* `value` new base value 




        

<hr>



### function ~AttributeInstance 

```C++
virtual endstone::AttributeInstance::~AttributeInstance () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/attribute/attribute_instance.h`

