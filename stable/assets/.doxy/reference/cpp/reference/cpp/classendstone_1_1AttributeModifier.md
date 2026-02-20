

# Class endstone::AttributeModifier



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**AttributeModifier**](classendstone_1_1AttributeModifier.md)



_Represents an attribute modifier._ 

* `#include <endstone/attribute/attribute_modifier.h>`

















## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Operation**](#enum-operation)  <br>_Operation to be applied._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**AttributeModifier**](#function-attributemodifier-12) (std::string name, [**float**](classendstone_1_1Identifier.md) amount, [**Operation**](classendstone_1_1AttributeModifier.md#enum-operation) operation) <br> |
|   | [**AttributeModifier**](#function-attributemodifier-22) (std::string name, [**UUID**](classendstone_1_1UUID.md) uuid, [**float**](classendstone_1_1Identifier.md) amount, [**Operation**](classendstone_1_1AttributeModifier.md#enum-operation) operation) <br> |
|  [**float**](classendstone_1_1Identifier.md) | [**getAmount**](#function-getamount) () const<br>_Get the amount by which this modifier will apply the operation._  |
|  std::string | [**getName**](#function-getname) () const<br>_Get the name of this modifier._  |
|  [**Operation**](classendstone_1_1AttributeModifier.md#enum-operation) | [**getOperation**](#function-getoperation) () const<br>_Get the operation this modifier will apply._  |
|  [**UUID**](classendstone_1_1UUID.md) | [**getUniqueId**](#function-getuniqueid) () const<br>_Get the unique ID for this modifier._  |




























## Public Types Documentation




### enum Operation 

_Operation to be applied._ 
```C++
enum endstone::AttributeModifier::Operation {
    Add,
    MultiplyBase,
    Multiply
};
```




<hr>
## Public Functions Documentation




### function AttributeModifier [1/2]

```C++
inline endstone::AttributeModifier::AttributeModifier (
    std::string name,
    float amount,
    Operation operation
) 
```




<hr>



### function AttributeModifier [2/2]

```C++
inline endstone::AttributeModifier::AttributeModifier (
    std::string name,
    UUID uuid,
    float amount,
    Operation operation
) 
```




<hr>



### function getAmount 

_Get the amount by which this modifier will apply the operation._ 
```C++
inline float endstone::AttributeModifier::getAmount () const
```





**Returns:**

modification amount 





        

<hr>



### function getName 

_Get the name of this modifier._ 
```C++
inline std::string endstone::AttributeModifier::getName () const
```





**Returns:**

name 





        

<hr>



### function getOperation 

_Get the operation this modifier will apply._ 
```C++
inline Operation endstone::AttributeModifier::getOperation () const
```





**Returns:**

operation 





        

<hr>



### function getUniqueId 

_Get the unique ID for this modifier._ 
```C++
inline UUID endstone::AttributeModifier::getUniqueId () const
```





**Returns:**

unique id 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/attribute/attribute_modifier.h`

