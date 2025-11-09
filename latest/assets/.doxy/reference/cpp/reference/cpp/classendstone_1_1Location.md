

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
|  constexpr | [**Vector**](classendstone_1_1Vector.md#function-vector-14) () = default<br>_Construct the vector with all components as 0._  |
|  constexpr | [**Vector**](classendstone_1_1Vector.md#function-vector-24) (T x, T y, T z) <br>_Construct the vector with provided components._  |
|   | [**Vector**](classendstone_1_1Vector.md#function-vector-34) (const [**Vector**](classendstone_1_1Vector.md) & other) = default<br> |
|   | [**Vector**](classendstone_1_1Vector.md#function-vector-44) ([**Vector**](classendstone_1_1Vector.md) && other) noexcept<br> |
|  float | [**angle**](classendstone_1_1Vector.md#function-angle) (const [**Vector**](classendstone_1_1Vector.md) & other) const<br>_Gets the angle between this vector and another in radians._  |
|  constexpr [**Vector**](classendstone_1_1Vector.md) & | [**crossProduct**](classendstone_1_1Vector.md#function-crossproduct) (const [**Vector**](classendstone_1_1Vector.md) & other) <br>_Calculates the cross-product of this vector with another._  |
|  float | [**distance**](classendstone_1_1Vector.md#function-distance) (const [**Vector**](classendstone_1_1Vector.md) & other) const<br>_Get the distance between this vector and another._  |
|  constexpr float | [**distanceSquared**](classendstone_1_1Vector.md#function-distancesquared) (const [**Vector**](classendstone_1_1Vector.md) & other) const<br>_Get the squared distance between this vector and another._  |
|  constexpr float | [**dot**](classendstone_1_1Vector.md#function-dot) (const [**Vector**](classendstone_1_1Vector.md) & other) const<br>_Calculates the dot product of this vector with another. The dot product is defined as x1\*x2+y1\*y2+z1\*z2. The returned value is a scalar._  |
|  int | [**getBlockX**](classendstone_1_1Vector.md#function-getblockx) () const<br>_Gets the floored value of the X component, indicating the block that this vector is contained with._  |
|  int | [**getBlockY**](classendstone_1_1Vector.md#function-getblocky) () const<br>_Gets the floored value of the Y component, indicating the block that this vector is contained with._  |
|  int | [**getBlockZ**](classendstone_1_1Vector.md#function-getblockz) () const<br>_Gets the floored value of the Z component, indicating the block that this vector is contained with._  |
|  constexpr [**Vector**](classendstone_1_1Vector.md) | [**getCrossProduct**](classendstone_1_1Vector.md#function-getcrossproduct) (const [**Vector**](classendstone_1_1Vector.md) & other) const<br>_Calculates the cross-product of this vector with another without mutating the original._  |
|  constexpr [**Vector**](classendstone_1_1Vector.md) | [**getMidpoint**](classendstone_1_1Vector.md#function-getmidpoint) (const [**Vector**](classendstone_1_1Vector.md) & other) const<br>_Gets a new midpoint vector between this vector and another._  |
|  constexpr float | [**getX**](classendstone_1_1Vector.md#function-getx) () const<br>_Gets the X component._  |
|  constexpr float | [**getY**](classendstone_1_1Vector.md#function-gety) () const<br>_Gets the Y component._  |
|  constexpr float | [**getZ**](classendstone_1_1Vector.md#function-getz) () const<br>_Gets the Z component._  |
|  constexpr bool | [**isInAABB**](classendstone_1_1Vector.md#function-isinaabb) (const [**Vector**](classendstone_1_1Vector.md) & min, const [**Vector**](classendstone_1_1Vector.md) & max) const<br>_Returns whether this vector is in an axis-aligned bounding box._  |
|  constexpr bool | [**isInSphere**](classendstone_1_1Vector.md#function-isinsphere) (const [**Vector**](classendstone_1_1Vector.md) & origin, float radius) const<br>_Returns whether this vector is within a sphere._  |
|  bool | [**isNormalized**](classendstone_1_1Vector.md#function-isnormalized) () const<br>_Returns if a vector is normalized._  |
|  constexpr bool | [**isZero**](classendstone_1_1Vector.md#function-iszero) () const<br>_Check whether or not each component of this vector is equal to 0._  |
|  float | [**length**](classendstone_1_1Vector.md#function-length) () const<br>_Gets the magnitude of the vector, defined as sqrt(x^2+y^2+z^2)._  |
|  constexpr float | [**lengthSquared**](classendstone_1_1Vector.md#function-lengthsquared) () const<br>_Gets the magnitude of the vector squared._  |
|  constexpr [**Vector**](classendstone_1_1Vector.md) & | [**midpoint**](classendstone_1_1Vector.md#function-midpoint) (const [**Vector**](classendstone_1_1Vector.md) & other) <br>_Sets this vector to the midpoint between this vector and another._  |
|  [**Vector**](classendstone_1_1Vector.md) & | [**normalize**](classendstone_1_1Vector.md#function-normalize) () <br>_Converts this vector to a unit vector (a vector with length of 1)._  |
|  constexpr [**Vector**](classendstone_1_1Vector.md) & | [**normalizeZeros**](classendstone_1_1Vector.md#function-normalizezeros) () <br>_Converts each component of value_ `-0.0` _to_`0.0` _._ |
|  bool | [**operator!=**](classendstone_1_1Vector.md#function-operator) (const [**Vector**](classendstone_1_1Vector.md) & other) noexcept const<br> |
|  constexpr [**Vector**](classendstone_1_1Vector.md) | [**operator\***](classendstone_1_1Vector.md#function-operator_1) (const [**Vector**](classendstone_1_1Vector.md) & other) const<br> |
|  constexpr [**Vector**](classendstone_1_1Vector.md) | [**operator\***](classendstone_1_1Vector.md#function-operator_2) (T scalar) const<br> |
|  [**Vector**](classendstone_1_1Vector.md) & | [**operator\*=**](classendstone_1_1Vector.md#function-operator_3) (const [**Vector**](classendstone_1_1Vector.md) & other) <br> |
|  constexpr [**Vector**](classendstone_1_1Vector.md) | [**operator+**](classendstone_1_1Vector.md#function-operator_4) (const [**Vector**](classendstone_1_1Vector.md) & other) const<br> |
|  constexpr [**Vector**](classendstone_1_1Vector.md) | [**operator+**](classendstone_1_1Vector.md#function-operator_5) (T scalar) const<br> |
|  [**Vector**](classendstone_1_1Vector.md) & | [**operator+=**](classendstone_1_1Vector.md#function-operator_6) (const [**Vector**](classendstone_1_1Vector.md) & other) <br> |
|  constexpr [**Vector**](classendstone_1_1Vector.md) | [**operator-**](classendstone_1_1Vector.md#function-operator-) (const [**Vector**](classendstone_1_1Vector.md) & other) const<br> |
|  constexpr [**Vector**](classendstone_1_1Vector.md) | [**operator-**](classendstone_1_1Vector.md#function-operator-_1) (T scalar) const<br> |
|  [**Vector**](classendstone_1_1Vector.md) & | [**operator-=**](classendstone_1_1Vector.md#function-operator-_2) (const [**Vector**](classendstone_1_1Vector.md) & other) <br> |
|  constexpr [**Vector**](classendstone_1_1Vector.md) | [**operator/**](classendstone_1_1Vector.md#function-operator_7) (const [**Vector**](classendstone_1_1Vector.md) & other) const<br> |
|  constexpr [**Vector**](classendstone_1_1Vector.md) | [**operator/**](classendstone_1_1Vector.md#function-operator_8) (T scalar) const<br> |
|  [**Vector**](classendstone_1_1Vector.md) & | [**operator/=**](classendstone_1_1Vector.md#function-operator_9) (const [**Vector**](classendstone_1_1Vector.md) & other) <br> |
|  [**Vector**](classendstone_1_1Vector.md) & | [**operator=**](classendstone_1_1Vector.md#function-operator_10) (const [**Vector**](classendstone_1_1Vector.md) & other) = default<br> |
|  [**Vector**](classendstone_1_1Vector.md) & | [**operator=**](classendstone_1_1Vector.md#function-operator_11) ([**Vector**](classendstone_1_1Vector.md) && other) noexcept<br> |
|  bool | [**operator==**](classendstone_1_1Vector.md#function-operator_12) (const [**Vector**](classendstone_1_1Vector.md) & other) noexcept const<br> |
|  [**Vector**](classendstone_1_1Vector.md) & | [**rotateAroundAxis**](classendstone_1_1Vector.md#function-rotatearoundaxis) (const [**Vector**](classendstone_1_1Vector.md) & axis, float angle) <br>_Rotates the vector around a given arbitrary axis in 3-dimensional space._  |
|  [**Vector**](classendstone_1_1Vector.md) & | [**rotateAroundNonUnitAxis**](classendstone_1_1Vector.md#function-rotatearoundnonunitaxis) (const [**Vector**](classendstone_1_1Vector.md) & axis, float angle) <br>_Rotates the vector around a given arbitrary axis in 3-dimensional space._  |
|  [**Vector**](classendstone_1_1Vector.md) & | [**rotateAroundX**](classendstone_1_1Vector.md#function-rotatearoundx) (float angle) <br>_Rotates the vector around the x-axis._  |
|  [**Vector**](classendstone_1_1Vector.md) & | [**rotateAroundY**](classendstone_1_1Vector.md#function-rotatearoundy) (float angle) <br>_Rotates the vector around the y-axis._  |
|  [**Vector**](classendstone_1_1Vector.md) & | [**rotateAroundZ**](classendstone_1_1Vector.md#function-rotatearoundz) (float angle) <br>_Rotates the vector around the z-axis._  |
|  constexpr [**Vector**](classendstone_1_1Vector.md) & | [**setX**](classendstone_1_1Vector.md#function-setx) (float x) <br>_Set the X component._  |
|  constexpr [**Vector**](classendstone_1_1Vector.md) & | [**setY**](classendstone_1_1Vector.md#function-sety) (float y) <br>_Set the Y component._  |
|  constexpr [**Vector**](classendstone_1_1Vector.md) & | [**setZ**](classendstone_1_1Vector.md#function-setz) (float z) <br>_Set the Z component._  |
|  constexpr [**Vector**](classendstone_1_1Vector.md) & | [**zero**](classendstone_1_1Vector.md#function-zero) () <br>_Zero this vector's components._  |
















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

