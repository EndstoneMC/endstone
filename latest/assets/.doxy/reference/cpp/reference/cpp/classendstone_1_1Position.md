

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
|  constexpr T | [**getX**](classendstone_1_1Vector.md#function-getx) () const<br> |
|  constexpr T | [**getY**](classendstone_1_1Vector.md#function-gety) () const<br> |
|  constexpr T | [**getZ**](classendstone_1_1Vector.md#function-getz) () const<br> |
|  constexpr T | [**length**](classendstone_1_1Vector.md#function-length) () const<br> |
|  constexpr T | [**lengthSquared**](classendstone_1_1Vector.md#function-lengthsquared) () const<br> |
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
|  constexpr void | [**setX**](classendstone_1_1Vector.md#function-setx) (T x) <br> |
|  constexpr void | [**setY**](classendstone_1_1Vector.md#function-sety) (T y) <br> |
|  constexpr void | [**setZ**](classendstone_1_1Vector.md#function-setz) (T z) <br> |






















































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

