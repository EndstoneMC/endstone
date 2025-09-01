

# Class endstone::Location



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Location**](classendstone_1_1Location.md)



_Represents a 3-dimensional location in a dimension within a level._ 

* `#include <endstone/level/location.h>`



Inherits the following classes: [endstone::Position](classendstone_1_1Position.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Location**](#function-location-13) ([**T**](classendstone_1_1Vector.md) x, [**T**](classendstone_1_1Vector.md) y, [**T**](classendstone_1_1Vector.md) z, [**float**](classendstone_1_1Vector.md) pitch=0.0, [**float**](classendstone_1_1Vector.md) yaw=0.0) <br> |
|   | [**Location**](#function-location-23) ([**T**](classendstone_1_1Vector.md) x, [**T**](classendstone_1_1Vector.md) y, [**T**](classendstone_1_1Vector.md) z, [**float**](classendstone_1_1Vector.md) pitch, [**Dimension**](classendstone_1_1Dimension.md) & dimension) <br> |
|   | [**Location**](#function-location-33) ([**T**](classendstone_1_1Vector.md) x, [**T**](classendstone_1_1Vector.md) y, [**T**](classendstone_1_1Vector.md) z, [**float**](classendstone_1_1Vector.md) pitch, [**float**](classendstone_1_1Vector.md) yaw, [**Dimension**](classendstone_1_1Dimension.md) & dimension) <br> |
|   | [**Position**](#function-position-12) ([**T**](classendstone_1_1Vector.md) x, [**T**](classendstone_1_1Vector.md) y, [**T**](classendstone_1_1Vector.md) z) <br> |
|   | [**Position**](#function-position-22) ([**T**](classendstone_1_1Vector.md) x, [**T**](classendstone_1_1Vector.md) y, [**T**](classendstone_1_1Vector.md) z, [**Dimension**](classendstone_1_1Dimension.md) & dimension) <br> |
|  [**Vector**](classendstone_1_1Vector.md) | [**getDirection**](#function-getdirection) () const<br> |
|  [**float**](classendstone_1_1Vector.md) | [**getPitch**](#function-getpitch) () const<br> |
|  [**float**](classendstone_1_1Vector.md) | [**getYaw**](#function-getyaw) () const<br> |
|  [**void**](classendstone_1_1Vector.md) | [**setPitch**](#function-setpitch) ([**float**](classendstone_1_1Vector.md) pitch) <br> |
|  [**void**](classendstone_1_1Vector.md) | [**setYaw**](#function-setyaw) ([**float**](classendstone_1_1Vector.md) yaw) <br> |


## Public Functions inherited from endstone::Position

See [endstone::Position](classendstone_1_1Position.md)

| Type | Name |
| ---: | :--- |
|   | [**Position**](classendstone_1_1Position.md#function-position-12) ([**T**](classendstone_1_1Vector.md) x, [**T**](classendstone_1_1Vector.md) y, [**T**](classendstone_1_1Vector.md) z) <br> |
|   | [**Position**](classendstone_1_1Position.md#function-position-22) ([**T**](classendstone_1_1Vector.md) x, [**T**](classendstone_1_1Vector.md) y, [**T**](classendstone_1_1Vector.md) z, [**Dimension**](classendstone_1_1Dimension.md) & dimension) <br> |
|  [**int**](classendstone_1_1Vector.md) | [**getBlockX**](classendstone_1_1Position.md#function-getblockx) () const<br>_Gets the floored value of the X component, indicating the block that this location is contained with._  |
|  [**int**](classendstone_1_1Vector.md) | [**getBlockY**](classendstone_1_1Position.md#function-getblocky) () const<br>_Gets the floored value of the Y component, indicating the block that this location is contained with._  |
|  [**int**](classendstone_1_1Vector.md) | [**getBlockZ**](classendstone_1_1Position.md#function-getblockz) () const<br>_Gets the floored value of the Z component, indicating the block that this location is contained with._  |
|  [**Dimension**](classendstone_1_1Dimension.md) \* | [**getDimension**](classendstone_1_1Position.md#function-getdimension) () const<br> |
|  [**void**](classendstone_1_1Vector.md) | [**setDimension**](classendstone_1_1Position.md#function-setdimension) ([**Dimension**](classendstone_1_1Dimension.md) & dimension) <br> |


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
|  [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & | [**normalize**](classendstone_1_1Vector.md#function-normalize) () <br> |
|  [**constexpr**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; | [**operator\***](classendstone_1_1Vector.md#function-operator) ([**const**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & other) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; | [**operator\***](classendstone_1_1Vector.md#function-operator_1) ([**T**](classendstone_1_1Vector.md) scalar) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & | [**operator\*=**](classendstone_1_1Vector.md#function-operator_2) ([**const**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & other) <br> |
|  [**constexpr**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; | [**operator+**](classendstone_1_1Vector.md#function-operator_3) ([**const**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & other) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; | [**operator+**](classendstone_1_1Vector.md#function-operator_4) ([**T**](classendstone_1_1Vector.md) scalar) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & | [**operator+=**](classendstone_1_1Vector.md#function-operator_5) ([**const**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & other) <br> |
|  [**constexpr**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; | [**operator-**](classendstone_1_1Vector.md#function-operator-) ([**const**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & other) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; | [**operator-**](classendstone_1_1Vector.md#function-operator-_1) ([**T**](classendstone_1_1Vector.md) scalar) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & | [**operator-=**](classendstone_1_1Vector.md#function-operator-_2) ([**const**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & other) <br> |
|  [**constexpr**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; | [**operator/**](classendstone_1_1Vector.md#function-operator_6) ([**const**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & other) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; | [**operator/**](classendstone_1_1Vector.md#function-operator_7) ([**T**](classendstone_1_1Vector.md) scalar) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & | [**operator/=**](classendstone_1_1Vector.md#function-operator_8) ([**const**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & other) <br> |
|  [**constexpr**](classendstone_1_1Vector.md) [**bool**](classendstone_1_1Vector.md) | [**operator==**](classendstone_1_1Vector.md#function-operator_9) ([**const**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & other) const<br> |
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




### function Location [1/3]

```C++
template<typename  T, typename>
inline endstone::Location::Location (
    T x,
    T y,
    T z,
    float pitch=0.0,
    float yaw=0.0
) 
```




<hr>



### function Location [2/3]

```C++
template<typename  T, typename>
inline endstone::Location::Location (
    T x,
    T y,
    T z,
    float pitch,
    Dimension & dimension
) 
```




<hr>



### function Location [3/3]

```C++
template<typename  T, typename>
inline endstone::Location::Location (
    T x,
    T y,
    T z,
    float pitch,
    float yaw,
    Dimension & dimension
) 
```




<hr>



### function Position [1/2]

```C++
template<typename  T, typename>
inline endstone::Location::Position (
    T x,
    T y,
    T z
) 
```




<hr>



### function Position [2/2]

```C++
template<typename  T, typename>
inline endstone::Location::Position (
    T x,
    T y,
    T z,
    Dimension & dimension
) 
```




<hr>



### function getDirection 

```C++
inline Vector endstone::Location::getDirection () const
```



Gets a unit-vector pointing in the direction that this [**Location**](classendstone_1_1Location.md) is facing.




**Returns:**

a vector pointing the direction of this location's pitch and yaw 





        

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

