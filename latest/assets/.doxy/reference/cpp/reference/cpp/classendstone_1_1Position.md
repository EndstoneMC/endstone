

# Class endstone::Position



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Position**](classendstone_1_1Position.md)



_Represents a 3-dimensional position in a dimension within a level._ 

* `#include <endstone/level/position.h>`



Inherits the following classes: [endstone::Vector](classendstone_1_1Vector.md)


Inherited by the following classes: [endstone::Location](classendstone_1_1Location.md)




















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Position**](#function-position) ([**Dimension**](classendstone_1_1Dimension.md) \* dimension, [**float**](classendstone_1_1Vector.md) x, [**float**](classendstone_1_1Vector.md) y, [**float**](classendstone_1_1Vector.md) z) <br> |
|  [**int**](classendstone_1_1Vector.md) | [**getBlockX**](#function-getblockx) () const<br>_Gets the floored value of the X component, indicating the block that this location is contained with._  |
|  [**int**](classendstone_1_1Vector.md) | [**getBlockY**](#function-getblocky) () const<br>_Gets the floored value of the Y component, indicating the block that this location is contained with._  |
|  [**int**](classendstone_1_1Vector.md) | [**getBlockZ**](#function-getblockz) () const<br>_Gets the floored value of the Z component, indicating the block that this location is contained with._  |
|  [**Dimension**](classendstone_1_1Dimension.md) \* | [**getDimension**](#function-getdimension) () const<br> |
|  [**void**](classendstone_1_1Vector.md) | [**setDimension**](#function-setdimension) ([**Dimension**](classendstone_1_1Dimension.md) & dimension) <br> |


## Public Functions inherited from endstone::Vector

See [endstone::Vector](classendstone_1_1Vector.md)

| Type | Name |
| ---: | :--- |
|  [**constexpr**](classendstone_1_1Vector.md) | [**Vector**](classendstone_1_1Vector.md#function-vector-12) () <br> |
|  [**constexpr**](classendstone_1_1Vector.md) | [**Vector**](classendstone_1_1Vector.md#function-vector-22) ([**T**](classendstone_1_1Vector.md) x, [**T**](classendstone_1_1Vector.md) y, [**T**](classendstone_1_1Vector.md) z) <br> |
|  [**constexpr**](classendstone_1_1Vector.md) [**T**](classendstone_1_1Vector.md) | [**distance**](classendstone_1_1Vector.md#function-distance) ([**const**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & other) const<br> |
|  [**constexpr**](classendstone_1_1Vector.md) [**T**](classendstone_1_1Vector.md) | [**distanceSquared**](classendstone_1_1Vector.md#function-distancesquared) ([**const**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & other) const<br> |
|  [**constexpr**](classendstone_1_1Vector.md) [**T**](classendstone_1_1Vector.md) | [**getX**](classendstone_1_1Vector.md#function-getx) () const<br>_Gets the X component._  |
|  [**constexpr**](classendstone_1_1Vector.md) [**T**](classendstone_1_1Vector.md) | [**getY**](classendstone_1_1Vector.md#function-gety) () const<br>_Gets the Y component._  |
|  [**constexpr**](classendstone_1_1Vector.md) [**T**](classendstone_1_1Vector.md) | [**getZ**](classendstone_1_1Vector.md#function-getz) () const<br>_Gets the Z component._  |
|  [**constexpr**](classendstone_1_1Vector.md) [**T**](classendstone_1_1Vector.md) | [**length**](classendstone_1_1Vector.md#function-length) () const<br>_Gets the magnitude of the vector, defined as sqrt(x^2+y^2+z^2)._  |
|  [**constexpr**](classendstone_1_1Vector.md) [**T**](classendstone_1_1Vector.md) | [**lengthSquared**](classendstone_1_1Vector.md#function-lengthsquared) () const<br>_Gets the magnitude of the vector squared._  |
|  [**constexpr**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; | [**operator\***](classendstone_1_1Vector.md#function-operator) ([**const**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & other) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; | [**operator\***](classendstone_1_1Vector.md#function-operator_1) ([**T**](classendstone_1_1Vector.md) scalar) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & | [**operator\*=**](classendstone_1_1Vector.md#function-operator_2) ([**const**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & other) <br> |
|  [**constexpr**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; | [**operator+**](classendstone_1_1Vector.md#function-operator_3) ([**const**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & other) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; | [**operator+**](classendstone_1_1Vector.md#function-operator_4) ([**T**](classendstone_1_1Vector.md) scalar) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & | [**operator+=**](classendstone_1_1Vector.md#function-operator_5) ([**const**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & other) <br> |
|  [**constexpr**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; | [**operator-**](classendstone_1_1Vector.md#function-operator_6) ([**const**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & other) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; | [**operator-**](classendstone_1_1Vector.md#function-operator_7) ([**T**](classendstone_1_1Vector.md) scalar) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & | [**operator-=**](classendstone_1_1Vector.md#function-operator_8) ([**const**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & other) <br> |
|  [**constexpr**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; | [**operator/**](classendstone_1_1Vector.md#function-operator_9) ([**const**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & other) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; | [**operator/**](classendstone_1_1Vector.md#function-operator_10) ([**T**](classendstone_1_1Vector.md) scalar) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & | [**operator/=**](classendstone_1_1Vector.md#function-operator_11) ([**const**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & other) <br> |
|  [**constexpr**](classendstone_1_1Vector.md) [**bool**](classendstone_1_1Vector.md) | [**operator==**](classendstone_1_1Vector.md#function-operator_12) ([**const**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & other) const<br> |
|  [**constexpr**](classendstone_1_1Vector.md) [**void**](classendstone_1_1Vector.md) | [**setX**](classendstone_1_1Vector.md#function-setx) ([**T**](classendstone_1_1Vector.md) x) <br>_Set the X component._  |
|  [**constexpr**](classendstone_1_1Vector.md) [**void**](classendstone_1_1Vector.md) | [**setY**](classendstone_1_1Vector.md#function-sety) ([**T**](classendstone_1_1Vector.md) y) <br>_Set the Y component._  |
|  [**constexpr**](classendstone_1_1Vector.md) [**void**](classendstone_1_1Vector.md) | [**setZ**](classendstone_1_1Vector.md#function-setz) ([**T**](classendstone_1_1Vector.md) z) <br>_Set the Z component._  |
















## Protected Attributes inherited from endstone::Vector

See [endstone::Vector](classendstone_1_1Vector.md)

| Type | Name |
| ---: | :--- |
|  [**T**](classendstone_1_1Vector.md) | [**x\_**](classendstone_1_1Vector.md#variable-x_)  <br> |
|  [**T**](classendstone_1_1Vector.md) | [**y\_**](classendstone_1_1Vector.md#variable-y_)  <br> |
|  [**T**](classendstone_1_1Vector.md) | [**z\_**](classendstone_1_1Vector.md#variable-z_)  <br> |






































## Public Functions Documentation




### function Position 

```C++
inline endstone::Position::Position (
    Dimension * dimension,
    float x,
    float y,
    float z
) 
```




<hr>



### function getBlockX 

_Gets the floored value of the X component, indicating the block that this location is contained with._ 
```C++
inline int endstone::Position::getBlockX () const
```





**Returns:**

block X 





        

<hr>



### function getBlockY 

_Gets the floored value of the Y component, indicating the block that this location is contained with._ 
```C++
inline int endstone::Position::getBlockY () const
```





**Returns:**

block Y 





        

<hr>



### function getBlockZ 

_Gets the floored value of the Z component, indicating the block that this location is contained with._ 
```C++
inline int endstone::Position::getBlockZ () const
```





**Returns:**

block Z 





        

<hr>



### function getDimension 

```C++
inline Dimension * endstone::Position::getDimension () const
```



Gets the dimension that this position resides in




**Returns:**

[**Dimension**](classendstone_1_1Dimension.md) that contains this position, or nullptr if the dimension is not set. 





        

<hr>



### function setDimension 

```C++
inline void endstone::Position::setDimension (
    Dimension & dimension
) 
```



Sets the dimension that this position resides in




**Parameters:**


* `dimension` New dimension that this position resides in 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/level/position.h`

