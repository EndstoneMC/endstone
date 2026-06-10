

# Class endstone::DimensionCreator



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**DimensionCreator**](classendstone_1_1DimensionCreator.md)



_Represents the options that may be used to create a custom_ [_**Dimension**_](classendstone_1_1Dimension.md) _._

* `#include <endstone/level/dimension_creator.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**DimensionCreator**](#function-dimensioncreator) ([**DimensionId**](classendstone_1_1Identifier.md) id) <br>_Creates an instance of a_ [_**DimensionCreator**_](classendstone_1_1DimensionCreator.md) _for the given dimension id._ |
|  [**DimensionId**](classendstone_1_1Identifier.md) | [**getId**](#function-getid) () const<br>_Gets the identifier of the dimension that is being created._  |
|  [**DimensionCreator**](classendstone_1_1DimensionCreator.md) & | [**id**](#function-id) ([**DimensionId**](classendstone_1_1Identifier.md) id) <br>_Sets the identifier of the dimension that is being created._  |




























## Public Functions Documentation




### function DimensionCreator 

_Creates an instance of a_ [_**DimensionCreator**_](classendstone_1_1DimensionCreator.md) _for the given dimension id._
```C++
inline explicit endstone::DimensionCreator::DimensionCreator (
    DimensionId id
) 
```





**Parameters:**


* `id` the identifier of the dimension to create, e.g. `myplugin:void_realm` 




        

<hr>



### function getId 

_Gets the identifier of the dimension that is being created._ 
```C++
inline DimensionId endstone::DimensionCreator::getId () const
```





**Returns:**

the dimension identifier 





        

<hr>



### function id 

_Sets the identifier of the dimension that is being created._ 
```C++
inline DimensionCreator & endstone::DimensionCreator::id (
    DimensionId id
) 
```





**Parameters:**


* `id` the dimension identifier 



**Returns:**

this [**DimensionCreator**](classendstone_1_1DimensionCreator.md), for chaining 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/level/dimension_creator.h`

