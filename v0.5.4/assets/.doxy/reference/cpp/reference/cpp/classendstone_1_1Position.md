

# Class endstone::Position



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Position**](classendstone_1_1Position.md)



_Represents a 3-dimensional position in a dimension within a level._ 

* `#include <endstone/level/position.h>`



Inherits the following classes: [endstone::Vector](classendstone_1_1Vector.md)


Inherited by the following classes: [endstone::Location](classendstone_1_1Location.md)




















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Position**](#function-position) ([**Dimension**](classendstone_1_1Dimension.md) \* dimension, float x, float y, float z) <br> |
|  int | [**getBlockX**](#function-getblockx) () const<br>_Gets the floored value of the X component, indicating the block that this location is contained with._  |
|  int | [**getBlockY**](#function-getblocky) () const<br>_Gets the floored value of the Y component, indicating the block that this location is contained with._  |
|  int | [**getBlockZ**](#function-getblockz) () const<br>_Gets the floored value of the Z component, indicating the block that this location is contained with._  |
|  [**Dimension**](classendstone_1_1Dimension.md) \* | [**getDimension**](#function-getdimension) () const<br> |
|  void | [**setDimension**](#function-setdimension) ([**Dimension**](classendstone_1_1Dimension.md) & dimension) <br> |


## Public Functions inherited from endstone::Vector

See [endstone::Vector](classendstone_1_1Vector.md)

| Type | Name |
| ---: | :--- |
|  constexpr | [**Vector**](classendstone_1_1Vector.md#function-vector-12) () <br> |
|  constexpr | [**Vector**](classendstone_1_1Vector.md#function-vector-22) (T x, T y, T z) <br> |
|  constexpr T | [**distance**](classendstone_1_1Vector.md#function-distance) (const [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; & other) const<br> |
|  constexpr T | [**distanceSquared**](classendstone_1_1Vector.md#function-distancesquared) (const [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; & other) const<br> |
|  constexpr T | [**getX**](classendstone_1_1Vector.md#function-getx) () const<br>_Gets the X component._  |
|  constexpr T | [**getY**](classendstone_1_1Vector.md#function-gety) () const<br>_Gets the Y component._  |
|  constexpr T | [**getZ**](classendstone_1_1Vector.md#function-getz) () const<br>_Gets the Z component._  |
|  constexpr T | [**length**](classendstone_1_1Vector.md#function-length) () const<br>_Gets the magnitude of the vector, defined as sqrt(x^2+y^2+z^2)._  |
|  constexpr T | [**lengthSquared**](classendstone_1_1Vector.md#function-lengthsquared) () const<br>_Gets the magnitude of the vector squared._  |
|  constexpr [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; | [**operator\***](classendstone_1_1Vector.md#function-operator) (const [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; & other) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; | [**operator\***](classendstone_1_1Vector.md#function-operator_1) (T scalar) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; & | [**operator\*=**](classendstone_1_1Vector.md#function-operator_2) (const [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; & other) <br> |
|  constexpr [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; | [**operator+**](classendstone_1_1Vector.md#function-operator_3) (const [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; & other) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; | [**operator+**](classendstone_1_1Vector.md#function-operator_4) (T scalar) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; & | [**operator+=**](classendstone_1_1Vector.md#function-operator_5) (const [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; & other) <br> |
|  constexpr [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; | [**operator-**](classendstone_1_1Vector.md#function-operator_6) (const [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; & other) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; | [**operator-**](classendstone_1_1Vector.md#function-operator_7) (T scalar) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; & | [**operator-=**](classendstone_1_1Vector.md#function-operator_8) (const [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; & other) <br> |
|  constexpr [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; | [**operator/**](classendstone_1_1Vector.md#function-operator_9) (const [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; & other) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; | [**operator/**](classendstone_1_1Vector.md#function-operator_10) (T scalar) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; & | [**operator/=**](classendstone_1_1Vector.md#function-operator_11) (const [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; & other) <br> |
|  constexpr bool | [**operator==**](classendstone_1_1Vector.md#function-operator_12) (const [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; & other) const<br> |
|  constexpr void | [**setX**](classendstone_1_1Vector.md#function-setx) (T x) <br>_Set the X component._  |
|  constexpr void | [**setY**](classendstone_1_1Vector.md#function-sety) (T y) <br>_Set the Y component._  |
|  constexpr void | [**setZ**](classendstone_1_1Vector.md#function-setz) (T z) <br>_Set the Z component._  |
















## Protected Attributes inherited from endstone::Vector

See [endstone::Vector](classendstone_1_1Vector.md)

| Type | Name |
| ---: | :--- |
|  T | [**x\_**](classendstone_1_1Vector.md#variable-x_)  <br> |
|  T | [**y\_**](classendstone_1_1Vector.md#variable-y_)  <br> |
|  T | [**z\_**](classendstone_1_1Vector.md#variable-z_)  <br> |






































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

