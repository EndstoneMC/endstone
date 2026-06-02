

# Class endstone::AttributeModifier



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**AttributeModifier**](classendstone_1_1AttributeModifier.md)



_Concrete implementation of an attribute modifier._ 

* `#include <endstone/attribute/attribute_modifier.h>`

















## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Operation**](#enum-operation)  <br>_Operation to be applied._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**AttributeModifier**](#function-attributemodifier) ([**AttributeModifierId**](classendstone_1_1Identifier.md) id, [**float**](classendstone_1_1Identifier.md) amount, [**Operation**](classendstone_1_1AttributeModifier.md#enum-operation) operation) <br>_Construct a new attribute modifier._  |
|  [**float**](classendstone_1_1Identifier.md) | [**getAmount**](#function-getamount) () const<br>_Get the amount by which this modifier will apply its operation._  |
|  [**AttributeModifierId**](classendstone_1_1Identifier.md) | [**getId**](#function-getid) () const<br>_Get the id of this modifier._  |
|  [**Operation**](classendstone_1_1AttributeModifier.md#enum-operation) | [**getOperation**](#function-getoperation) () const<br>_Get the operation this modifier will apply._  |




























## Public Types Documentation




### enum Operation 

_Operation to be applied._ 
```C++
enum endstone::AttributeModifier::Operation {
    Add,
    MultiplyBase,
    Multiply,
    Cap
};
```




<hr>
## Public Functions Documentation




### function AttributeModifier 

_Construct a new attribute modifier._ 
```C++
inline endstone::AttributeModifier::AttributeModifier (
    AttributeModifierId id,
    float amount,
    Operation operation
) 
```





**Parameters:**


* `id` the id of the modifier 
* `amount` the amount by which the modifier applies its operation 
* `operation` the operation to apply 




        

<hr>



### function getAmount 

_Get the amount by which this modifier will apply its operation._ 
```C++
inline float endstone::AttributeModifier::getAmount () const
```





**Returns:**

modification amount 





        

<hr>



### function getId 

_Get the id of this modifier._ 
```C++
inline AttributeModifierId endstone::AttributeModifier::getId () const
```





**Returns:**

id 





        

<hr>



### function getOperation 

_Get the operation this modifier will apply._ 
```C++
inline Operation endstone::AttributeModifier::getOperation () const
```





**Returns:**

operation 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/attribute/attribute_modifier.h`

