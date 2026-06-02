

# Class endstone::AttributeInstance



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**AttributeInstance**](classendstone_1_1AttributeInstance.md)



_Represents a mutable instance of an attribute and its associated modifiers and values._ 

* `#include <endstone/attribute/attribute_instance.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**void**](classendstone_1_1Identifier.md) | [**addModifier**](#function-addmodifier) ([**const**](classendstone_1_1Identifier.md) [**AttributeModifier**](classendstone_1_1AttributeModifier.md) & modifier) = 0<br>_Add a modifier to this instance._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**addTransientModifier**](#function-addtransientmodifier) ([**const**](classendstone_1_1Identifier.md) [**AttributeModifier**](classendstone_1_1AttributeModifier.md) & modifier) = 0<br>_Add a transient modifier to this instance._  |
| virtual [**float**](classendstone_1_1Identifier.md) | [**getBaseValue**](#function-getbasevalue) () const = 0<br>_Base value of this instance before modifiers are applied._  |
| virtual [**float**](classendstone_1_1Identifier.md) | [**getMaxValue**](#function-getmaxvalue) () const = 0<br>_The maximum value this instance is allowed to take._  |
| virtual [**float**](classendstone_1_1Identifier.md) | [**getMinValue**](#function-getminvalue) () const = 0<br>_The minimum value this instance is allowed to take._  |
| virtual std::optional&lt; [**AttributeModifier**](classendstone_1_1AttributeModifier.md) &gt; | [**getModifier**](#function-getmodifier) ([**AttributeModifierId**](classendstone_1_1Identifier.md) id) const = 0<br>_Gets the modifier with the corresponding id._  |
| virtual std::vector&lt; [**AttributeModifier**](classendstone_1_1AttributeModifier.md) &gt; | [**getModifiers**](#function-getmodifiers) () const = 0<br>_Get all modifiers present on this instance._  |
| virtual [**AttributeId**](classendstone_1_1Identifier.md) | [**getType**](#function-gettype) () const = 0<br>_The attribute type pertaining to this instance._  |
| virtual [**float**](classendstone_1_1Identifier.md) | [**getValue**](#function-getvalue) () const = 0<br>_Get the value of this instance after all associated modifiers have been applied._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**removeModifier**](#function-removemodifier-12) ([**const**](classendstone_1_1Identifier.md) [**AttributeModifier**](classendstone_1_1AttributeModifier.md) & modifier) = 0<br>_Remove a modifier from this instance._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**removeModifier**](#function-removemodifier-22) ([**AttributeModifierId**](classendstone_1_1Identifier.md) id) = 0<br>_Remove a modifier with the corresponding id from this instance._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setBaseValue**](#function-setbasevalue) ([**float**](classendstone_1_1Identifier.md) value) = 0<br>_Set the base value of this instance._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setMaxValue**](#function-setmaxvalue) ([**float**](classendstone_1_1Identifier.md) value) = 0<br>_Set the maximum value this instance is allowed to take._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setMinValue**](#function-setminvalue) ([**float**](classendstone_1_1Identifier.md) value) = 0<br>_Set the minimum value this instance is allowed to take._  |
| virtual  | [**~AttributeInstance**](#function-attributeinstance) () = default<br> |




























## Public Functions Documentation




### function addModifier 

_Add a modifier to this instance._ 
```C++
virtual void endstone::AttributeInstance::addModifier (
    const  AttributeModifier & modifier
) = 0
```





**Parameters:**


* `modifier` to add 




        

<hr>



### function addTransientModifier 

_Add a transient modifier to this instance._ 
```C++
virtual void endstone::AttributeInstance::addTransientModifier (
    const  AttributeModifier & modifier
) = 0
```



Transient modifiers are not persisted (saved with the NBT data).




**Parameters:**


* `modifier` to add 




        

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

_The maximum value this instance is allowed to take._ 
```C++
virtual float endstone::AttributeInstance::getMaxValue () const = 0
```





**Note:**

Bedrock-specific. The value of this instance is clamped to the range [min, max].




**Returns:**

maximum value 





        

<hr>



### function getMinValue 

_The minimum value this instance is allowed to take._ 
```C++
virtual float endstone::AttributeInstance::getMinValue () const = 0
```





**Note:**

Bedrock-specific. The value of this instance is clamped to the range [min, max].




**Returns:**

minimum value 





        

<hr>



### function getModifier 

_Gets the modifier with the corresponding id._ 
```C++
virtual std::optional< AttributeModifier > endstone::AttributeInstance::getModifier (
    AttributeModifierId id
) const = 0
```





**Parameters:**


* `id` the id of the modifier 



**Returns:**

the modifier, or std::nullopt if no modifier with the given id is present 





        

<hr>



### function getModifiers 

_Get all modifiers present on this instance._ 
```C++
virtual std::vector< AttributeModifier > endstone::AttributeInstance::getModifiers () const = 0
```





**Returns:**

a copied collection of all modifiers 





        

<hr>



### function getType 

_The attribute type pertaining to this instance._ 
```C++
virtual AttributeId endstone::AttributeInstance::getType () const = 0
```





**Returns:**

the attribute type 





        

<hr>



### function getValue 

_Get the value of this instance after all associated modifiers have been applied._ 
```C++
virtual float endstone::AttributeInstance::getValue () const = 0
```





**Returns:**

the total attribute value 





        

<hr>



### function removeModifier [1/2]

_Remove a modifier from this instance._ 
```C++
virtual void endstone::AttributeInstance::removeModifier (
    const  AttributeModifier & modifier
) = 0
```





**Parameters:**


* `modifier` to remove 




        

<hr>



### function removeModifier [2/2]

_Remove a modifier with the corresponding id from this instance._ 
```C++
virtual void endstone::AttributeInstance::removeModifier (
    AttributeModifierId id
) = 0
```





**Parameters:**


* `id` the id of the modifier to remove 




        

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



### function setMaxValue 

_Set the maximum value this instance is allowed to take._ 
```C++
virtual void endstone::AttributeInstance::setMaxValue (
    float value
) = 0
```



For example, raising the maximum of an entity's `minecraft:health` attribute increases its maximum health.




**Note:**

Bedrock-specific. The value is re-clamped to the new range on the next recalculation.




**Parameters:**


* `value` new maximum value 




        

<hr>



### function setMinValue 

_Set the minimum value this instance is allowed to take._ 
```C++
virtual void endstone::AttributeInstance::setMinValue (
    float value
) = 0
```





**Note:**

Bedrock-specific. The value is re-clamped to the new range on the next recalculation.




**Parameters:**


* `value` new minimum value 




        

<hr>



### function ~AttributeInstance 

```C++
virtual endstone::AttributeInstance::~AttributeInstance () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/attribute/attribute_instance.h`

