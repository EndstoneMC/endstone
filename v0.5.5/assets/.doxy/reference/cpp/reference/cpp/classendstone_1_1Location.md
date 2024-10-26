

# Class endstone::Location



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Location**](classendstone_1_1Location.md)



_Represents a 3-dimensional location in a dimension within a level._ 

* `#include <endstone/level/location.h>`



Inherits the following classes: [endstone::Position](classendstone_1_1Position.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Location**](#function-location-12) ([**Dimension**](classendstone_1_1Dimension.md) \* dimension, int x, int y, int z, float pitch=0.0, float yaw=0.0) <br> |
|   | [**Location**](#function-location-22) ([**Dimension**](classendstone_1_1Dimension.md) \* dimension, float x, float y, float z, float pitch=0.0, float yaw=0.0) <br> |
|  float | [**getPitch**](#function-getpitch) () const<br> |
|  float | [**getYaw**](#function-getyaw) () const<br> |
|  void | [**setPitch**](#function-setpitch) (float pitch) <br> |
|  void | [**setYaw**](#function-setyaw) (float yaw) <br> |


## Public Functions inherited from endstone::Position

See [endstone::Position](classendstone_1_1Position.md)

| Type | Name |
| ---: | :--- |
|   | [**Position**](classendstone_1_1Position.md#function-position) ([**Dimension**](classendstone_1_1Dimension.md) \* dimension, float x, float y, float z) <br> |
|  int | [**getBlockX**](classendstone_1_1Position.md#function-getblockx) () const<br>_Gets the floored value of the X component, indicating the block that this location is contained with._  |
|  int | [**getBlockY**](classendstone_1_1Position.md#function-getblocky) () const<br>_Gets the floored value of the Y component, indicating the block that this location is contained with._  |
|  int | [**getBlockZ**](classendstone_1_1Position.md#function-getblockz) () const<br>_Gets the floored value of the Z component, indicating the block that this location is contained with._  |
|  [**Dimension**](classendstone_1_1Dimension.md) \* | [**getDimension**](classendstone_1_1Position.md#function-getdimension) () const<br> |
|  void | [**setDimension**](classendstone_1_1Position.md#function-setdimension) ([**Dimension**](classendstone_1_1Dimension.md) & dimension) <br> |


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




### function Location [1/2]

```C++
inline endstone::Location::Location (
    Dimension * dimension,
    int x,
    int y,
    int z,
    float pitch=0.0,
    float yaw=0.0
) 
```




<hr>



### function Location [2/2]

```C++
inline endstone::Location::Location (
    Dimension * dimension,
    float x,
    float y,
    float z,
    float pitch=0.0,
    float yaw=0.0
) 
```




<hr>



### function getPitch 


```C++
inline float endstone::Location::getPitch () const
```



Gets the pitch of this location, measured in degrees.




**Returns:**

the incline's pitch 





        

<hr>



### function getYaw 


```C++
inline float endstone::Location::getYaw () const
```



Gets the yaw of this location, measured in degrees.




**Returns:**

the rotation's yaw 





        

<hr>



### function setPitch 


```C++
inline void endstone::Location::setPitch (
    float pitch
) 
```



Sets the pitch of this location, measured in degrees. 
* A pitch of 0 represents level forward facing. 
* A pitch of 90 represents downward facing, or negative y direction. 
* A pitch of -90 represents upward facing, or positive y direction. 

Increasing pitch values the equivalent of looking down.




**Parameters:**


* `pitch` new incline's pitch 




        

<hr>



### function setYaw 


```C++
inline void endstone::Location::setYaw (
    float yaw
) 
```



Sets the yaw of this location, measured in degrees. 
* A yaw of 0 or 360 represents the positive z direction. 
* A yaw of 180 represents the negative z direction. 
* A yaw of 90 represents the negative x direction. 
* A yaw of 270 represents the positive x direction. 

Increasing yaw values are the equivalent of turning to your right-facing, increasing the scale of the next respective axis, and decreasing the scale of the previous axis.




**Parameters:**


* `yaw` new rotation's yaw 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/level/location.h`

