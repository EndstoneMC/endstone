

# Class endstone::Location



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Location**](classendstone_1_1Location.md)



_Represents a 3-dimensional location in a dimension within a level._ 

* `#include <endstone/level/location.h>`



Inherits the following classes: [endstone::Vector](classendstone_1_1Vector.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Location**](#function-location-14) (T x, T y, T z, const float pitch=0.0, const float yaw=0.0) <br> |
|   | [**Location**](#function-location-24) (T x, T y, T z, [**Dimension**](classendstone_1_1Dimension.md) & dimension) <br> |
|   | [**Location**](#function-location-34) (T x, T y, T z, const float pitch, [**Dimension**](classendstone_1_1Dimension.md) & dimension) <br> |
|   | [**Location**](#function-location-44) (T x, T y, T z, const float pitch, const float yaw, [**Dimension**](classendstone_1_1Dimension.md) & dimension) <br> |
|  int | [**getBlockX**](#function-getblockx) () const<br>_Gets the floored value of the X component, indicating the block that this location is contained with._  |
|  int | [**getBlockY**](#function-getblocky) () const<br>_Gets the floored value of the Y component, indicating the block that this location is contained with._  |
|  int | [**getBlockZ**](#function-getblockz) () const<br>_Gets the floored value of the Z component, indicating the block that this location is contained with._  |
|  [**Dimension**](classendstone_1_1Dimension.md) \* | [**getDimension**](#function-getdimension) () const<br>_Gets the dimension that this position resides in._  |
|  [**Vector**](classendstone_1_1Vector.md) | [**getDirection**](#function-getdirection) () const<br>_Gets a unit-vector pointing in the direction that this_ [_**Location**_](classendstone_1_1Location.md) _is facing._ |
|  float | [**getPitch**](#function-getpitch) () const<br>_Gets the pitch of this location, measured in degrees._  |
|  float | [**getYaw**](#function-getyaw) () const<br>_Gets the yaw of this location, measured in degrees._  |
|  void | [**setDimension**](#function-setdimension) ([**Dimension**](classendstone_1_1Dimension.md) & dimension) <br>_Sets the dimension that this position resides in._  |
|  void | [**setPitch**](#function-setpitch) (float pitch) <br>_Sets the pitch of this location, measured in degrees._  |
|  void | [**setYaw**](#function-setyaw) (float yaw) <br>_Sets the yaw of this location, measured in degrees._  |


## Public Functions inherited from endstone::Vector

See [endstone::Vector](classendstone_1_1Vector.md)

| Type | Name |
| ---: | :--- |
|  constexpr | [**Vector**](classendstone_1_1Vector.md#function-vector-12) () = default<br>_Construct the vector with all components as 0._  |
|  constexpr | [**Vector**](classendstone_1_1Vector.md#function-vector-22) (T x, T y, T z) <br>_Construct the vector with provided components._  |
|  float | [**distance**](classendstone_1_1Vector.md#function-distance) (const [**Vector**](classendstone_1_1Vector.md) & other) const<br> |
|  constexpr float | [**distanceSquared**](classendstone_1_1Vector.md#function-distancesquared) (const [**Vector**](classendstone_1_1Vector.md) & other) const<br> |
|  constexpr float | [**getX**](classendstone_1_1Vector.md#function-getx) () const<br>_Gets the X component._  |
|  constexpr float | [**getY**](classendstone_1_1Vector.md#function-gety) () const<br>_Gets the Y component._  |
|  constexpr float | [**getZ**](classendstone_1_1Vector.md#function-getz) () const<br>_Gets the Z component._  |
|  float | [**length**](classendstone_1_1Vector.md#function-length) () const<br>_Gets the magnitude of the vector, defined as sqrt(x^2+y^2+z^2)._  |
|  constexpr float | [**lengthSquared**](classendstone_1_1Vector.md#function-lengthsquared) () const<br>_Gets the magnitude of the vector squared._  |
|  [**Vector**](classendstone_1_1Vector.md) & | [**normalize**](classendstone_1_1Vector.md#function-normalize) () <br> |
|  constexpr [**Vector**](classendstone_1_1Vector.md) | [**operator\***](classendstone_1_1Vector.md#function-operator) (const [**Vector**](classendstone_1_1Vector.md) & other) const<br> |
|  [**Vector**](classendstone_1_1Vector.md) | [**operator\***](classendstone_1_1Vector.md#function-operator_1) (float scalar) const<br> |
|  [**Vector**](classendstone_1_1Vector.md) & | [**operator\*=**](classendstone_1_1Vector.md#function-operator_2) (const [**Vector**](classendstone_1_1Vector.md) & other) <br> |
|  constexpr [**Vector**](classendstone_1_1Vector.md) | [**operator+**](classendstone_1_1Vector.md#function-operator_3) (const [**Vector**](classendstone_1_1Vector.md) & other) const<br> |
|  [**Vector**](classendstone_1_1Vector.md) | [**operator+**](classendstone_1_1Vector.md#function-operator_4) (float scalar) const<br> |
|  [**Vector**](classendstone_1_1Vector.md) & | [**operator+=**](classendstone_1_1Vector.md#function-operator_5) (const [**Vector**](classendstone_1_1Vector.md) & other) <br> |
|  constexpr [**Vector**](classendstone_1_1Vector.md) | [**operator-**](classendstone_1_1Vector.md#function-operator-) (const [**Vector**](classendstone_1_1Vector.md) & other) const<br> |
|  [**Vector**](classendstone_1_1Vector.md) | [**operator-**](classendstone_1_1Vector.md#function-operator-_1) (float scalar) const<br> |
|  [**Vector**](classendstone_1_1Vector.md) & | [**operator-=**](classendstone_1_1Vector.md#function-operator-_2) (const [**Vector**](classendstone_1_1Vector.md) & other) <br> |
|  constexpr [**Vector**](classendstone_1_1Vector.md) | [**operator/**](classendstone_1_1Vector.md#function-operator_6) (const [**Vector**](classendstone_1_1Vector.md) & other) const<br> |
|  [**Vector**](classendstone_1_1Vector.md) | [**operator/**](classendstone_1_1Vector.md#function-operator_7) (float scalar) const<br> |
|  [**Vector**](classendstone_1_1Vector.md) & | [**operator/=**](classendstone_1_1Vector.md#function-operator_8) (const [**Vector**](classendstone_1_1Vector.md) & other) <br> |
|  constexpr bool | [**operator==**](classendstone_1_1Vector.md#function-operator_9) (const [**Vector**](classendstone_1_1Vector.md) & other) const<br> |
|  constexpr void | [**setX**](classendstone_1_1Vector.md#function-setx) (float x) <br>_Set the X component._  |
|  constexpr void | [**setY**](classendstone_1_1Vector.md#function-sety) (float y) <br>_Set the Y component._  |
|  constexpr void | [**setZ**](classendstone_1_1Vector.md#function-setz) (float z) <br>_Set the Z component._  |
















## Protected Attributes inherited from endstone::Vector

See [endstone::Vector](classendstone_1_1Vector.md)

| Type | Name |
| ---: | :--- |
|  float | [**x\_**](classendstone_1_1Vector.md#variable-x_)   = `0.0`<br> |
|  float | [**y\_**](classendstone_1_1Vector.md#variable-y_)   = `0.0`<br> |
|  float | [**z\_**](classendstone_1_1Vector.md#variable-z_)   = `0.0`<br> |






































## Public Functions Documentation




### function Location [1/4]

```C++
template<typename T, typename>
inline endstone::Location::Location (
    T x,
    T y,
    T z,
    const float pitch=0.0,
    const float yaw=0.0
) 
```




<hr>



### function Location [2/4]

```C++
template<typename T, typename>
inline endstone::Location::Location (
    T x,
    T y,
    T z,
    Dimension & dimension
) 
```




<hr>



### function Location [3/4]

```C++
template<typename T, typename>
inline endstone::Location::Location (
    T x,
    T y,
    T z,
    const float pitch,
    Dimension & dimension
) 
```




<hr>



### function Location [4/4]

```C++
template<typename T, typename>
inline endstone::Location::Location (
    T x,
    T y,
    T z,
    const float pitch,
    const float yaw,
    Dimension & dimension
) 
```




<hr>



### function getBlockX 

_Gets the floored value of the X component, indicating the block that this location is contained with._ 
```C++
inline int endstone::Location::getBlockX () const
```





**Returns:**

block X 





        

<hr>



### function getBlockY 

_Gets the floored value of the Y component, indicating the block that this location is contained with._ 
```C++
inline int endstone::Location::getBlockY () const
```





**Returns:**

block Y 





        

<hr>



### function getBlockZ 

_Gets the floored value of the Z component, indicating the block that this location is contained with._ 
```C++
inline int endstone::Location::getBlockZ () const
```





**Returns:**

block Z 





        

<hr>



### function getDimension 

_Gets the dimension that this position resides in._ 
```C++
inline Dimension * endstone::Location::getDimension () const
```





**Returns:**

[**Dimension**](classendstone_1_1Dimension.md) that contains this position, or nullptr if the dimension is not set. 





        

<hr>



### function getDirection 

_Gets a unit-vector pointing in the direction that this_ [_**Location**_](classendstone_1_1Location.md) _is facing._
```C++
inline Vector endstone::Location::getDirection () const
```





**Returns:**

a vector pointing the direction of this location's pitch and yaw 





        

<hr>



### function getPitch 

_Gets the pitch of this location, measured in degrees._ 
```C++
inline float endstone::Location::getPitch () const
```





**Returns:**

the incline's pitch 





        

<hr>



### function getYaw 

_Gets the yaw of this location, measured in degrees._ 
```C++
inline float endstone::Location::getYaw () const
```





**Returns:**

the rotation's yaw 





        

<hr>



### function setDimension 

_Sets the dimension that this position resides in._ 
```C++
inline void endstone::Location::setDimension (
    Dimension & dimension
) 
```





**Parameters:**


* `dimension` New dimension that this position resides in 




        

<hr>



### function setPitch 

_Sets the pitch of this location, measured in degrees._ 
```C++
inline void endstone::Location::setPitch (
    float pitch
) 
```




* A pitch of 0 represents level forward facing.
* A pitch of 90 represents downward facing, or negative y direction.
* A pitch of -90 represents upward facing, or positive y direction.




Increasing pitch values the equivalent of looking down.




**Parameters:**


* `pitch` new incline's pitch 




        

<hr>



### function setYaw 

_Sets the yaw of this location, measured in degrees._ 
```C++
inline void endstone::Location::setYaw (
    float yaw
) 
```




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

