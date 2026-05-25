

# Class endstone::Location



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Location**](classendstone_1_1Location.md)



_Represents a 3-dimensional location in a dimension within a level._ 

* `#include <endstone/level/location.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Location**](#function-location) ([**Dimension**](classendstone_1_1Dimension.md) & dimension, [**T**](classendstone_1_1Identifier.md) x, [**T**](classendstone_1_1Identifier.md) y, [**T**](classendstone_1_1Identifier.md) z, [**const**](classendstone_1_1Identifier.md) [**float**](classendstone_1_1Identifier.md) pitch=0.0, [**const**](classendstone_1_1Identifier.md) [**float**](classendstone_1_1Identifier.md) yaw=0.0) <br> |
|  [**float**](classendstone_1_1Identifier.md) | [**distance**](#function-distance) ([**const**](classendstone_1_1Identifier.md) [**Location**](classendstone_1_1Location.md) & other) const<br>_Get the distance between this location and another._  |
|  [**float**](classendstone_1_1Identifier.md) | [**distanceSquared**](#function-distancesquared) ([**const**](classendstone_1_1Identifier.md) [**Location**](classendstone_1_1Location.md) & other) const<br>_Get the squared distance between this location and another._  |
|  std::unique\_ptr&lt; [**Block**](classendstone_1_1Block.md) &gt; | [**getBlock**](#function-getblock) () const<br>_Gets the block at the represented location._  |
|  [**int**](classendstone_1_1Identifier.md) | [**getBlockX**](#function-getblockx) () const<br>_Gets the floored value of the X component, indicating the block that this location is contained with._  |
|  [**int**](classendstone_1_1Identifier.md) | [**getBlockY**](#function-getblocky) () const<br>_Gets the floored value of the Y component, indicating the block that this location is contained with._  |
|  [**int**](classendstone_1_1Identifier.md) | [**getBlockZ**](#function-getblockz) () const<br>_Gets the floored value of the Z component, indicating the block that this location is contained with._  |
|  [**Dimension**](classendstone_1_1Dimension.md) & | [**getDimension**](#function-getdimension) () const<br>_Gets the dimension that this location resides in._  |
|  [**Vector**](classendstone_1_1Vector.md) | [**getDirection**](#function-getdirection) () const<br>_Gets a unit-vector pointing in the direction that this_ [_**Location**_](classendstone_1_1Location.md) _is facing._ |
|  [**float**](classendstone_1_1Identifier.md) | [**getPitch**](#function-getpitch) () const<br>_Gets the pitch of this location, measured in degrees._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**float**](classendstone_1_1Identifier.md) | [**getX**](#function-getx) () const<br>_Gets the x-coordinate of this location._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**float**](classendstone_1_1Identifier.md) | [**getY**](#function-gety) () const<br>_Gets the y-coordinate of this location._  |
|  [**float**](classendstone_1_1Identifier.md) | [**getYaw**](#function-getyaw) () const<br>_Gets the yaw of this location, measured in degrees._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**float**](classendstone_1_1Identifier.md) | [**getZ**](#function-getz) () const<br>_Gets the z-coordinate of this location._  |
|  [**float**](classendstone_1_1Identifier.md) | [**length**](#function-length) () const<br>_Gets the magnitude of the location, defined as sqrt(x^2+y^2+z^2)._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**float**](classendstone_1_1Identifier.md) | [**lengthSquared**](#function-lengthsquared) () const<br>_Gets the magnitude of the location squared._  |
|   | [**operator Vector**](#function-operator-vector) () noexcept const<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**operator!=**](#function-operator) ([**const**](classendstone_1_1Identifier.md) [**Location**](classendstone_1_1Location.md) & other) noexcept const<br> |
|  [**Location**](classendstone_1_1Location.md) & | [**operator\*=**](#function-operator_1) ([**T**](classendstone_1_1Identifier.md) scalar) <br> |
|  [**Location**](classendstone_1_1Location.md) & | [**operator+=**](#function-operator_2) ([**const**](classendstone_1_1Identifier.md) [**Location**](classendstone_1_1Location.md) & other) <br> |
|  [**Location**](classendstone_1_1Location.md) & | [**operator+=**](#function-operator_3) ([**const**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) & other) <br> |
|  [**Location**](classendstone_1_1Location.md) & | [**operator-=**](#function-operator-) ([**const**](classendstone_1_1Identifier.md) [**Location**](classendstone_1_1Location.md) & other) <br> |
|  [**Location**](classendstone_1_1Location.md) & | [**operator-=**](#function-operator-_1) ([**const**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) & other) <br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**operator==**](#function-operator_4) ([**const**](classendstone_1_1Identifier.md) [**Location**](classendstone_1_1Location.md) & other) noexcept const<br> |
|  [**void**](classendstone_1_1Identifier.md) | [**setDimension**](#function-setdimension) ([**Dimension**](classendstone_1_1Dimension.md) & dimension) <br>_Sets the dimension that this position resides in._  |
|  [**Location**](classendstone_1_1Location.md) & | [**setDirection**](#function-setdirection) ([**const**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) & vector) <br>_Sets the yaw and pitch to point in the direction of the vector._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setPitch**](#function-setpitch) ([**float**](classendstone_1_1Identifier.md) pitch) <br>_Sets the pitch of this location, measured in degrees._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**void**](classendstone_1_1Identifier.md) | [**setX**](#function-setx) ([**T**](classendstone_1_1Identifier.md) x) <br>_Sets the x-coordinate of this location._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**void**](classendstone_1_1Identifier.md) | [**setY**](#function-sety) ([**T**](classendstone_1_1Identifier.md) y) <br>_Sets the y-coordinate of this location._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setYaw**](#function-setyaw) ([**float**](classendstone_1_1Identifier.md) yaw) <br>_Sets the yaw of this location, measured in degrees._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**void**](classendstone_1_1Identifier.md) | [**setZ**](#function-setz) ([**T**](classendstone_1_1Identifier.md) z) <br>_Sets the z-coordinate of this location._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**Location**](classendstone_1_1Location.md) & | [**zero**](#function-zero) () <br>_Zero this vector's components. Not world-aware._  |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**float**](classendstone_1_1Identifier.md) | [**normalizePitch**](#function-normalizepitch) ([**float**](classendstone_1_1Identifier.md) pitch) <br>_Normalizes the given pitch angle to a value between_ `+/-90` _degrees._ |
|  [**float**](classendstone_1_1Identifier.md) | [**normalizeYaw**](#function-normalizeyaw) ([**float**](classendstone_1_1Identifier.md) yaw) <br>_Normalizes the given yaw angle to a value between_ `+/-180` _degrees._ |


























## Public Functions Documentation




### function Location 

```C++
template<std::convertible_to< float > T>
inline endstone::Location::Location (
    Dimension & dimension,
    T x,
    T y,
    T z,
    const  float pitch=0.0,
    const  float yaw=0.0
) 
```




<hr>



### function distance 

_Get the distance between this location and another._ 
```C++
inline float endstone::Location::distance (
    const  Location & other
) const
```





**Parameters:**


* `other` The other location 



**Returns:**

the distance 





        

<hr>



### function distanceSquared 

_Get the squared distance between this location and another._ 
```C++
inline float endstone::Location::distanceSquared (
    const  Location & other
) const
```





**Parameters:**


* `other` The other location 



**Returns:**

the distance 





        

<hr>



### function getBlock 

_Gets the block at the represented location._ 
```C++
inline std::unique_ptr< Block > endstone::Location::getBlock () const
```





**Returns:**

[**Block**](classendstone_1_1Block.md) at the represented location 





        

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

_Gets the dimension that this location resides in._ 
```C++
inline Dimension & endstone::Location::getDimension () const
```





**Returns:**

[**Dimension**](classendstone_1_1Dimension.md) that contains this location 





        

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



### function getX 

_Gets the x-coordinate of this location._ 
```C++
inline constexpr  float endstone::Location::getX () const
```





**Returns:**

x-coordinate 





        

<hr>



### function getY 

_Gets the y-coordinate of this location._ 
```C++
inline constexpr  float endstone::Location::getY () const
```





**Returns:**

y-coordinate 





        

<hr>



### function getYaw 

_Gets the yaw of this location, measured in degrees._ 
```C++
inline float endstone::Location::getYaw () const
```





**Returns:**

the rotation's yaw 





        

<hr>



### function getZ 

_Gets the z-coordinate of this location._ 
```C++
inline constexpr  float endstone::Location::getZ () const
```





**Returns:**

z-coordinate 





        

<hr>



### function length 

_Gets the magnitude of the location, defined as sqrt(x^2+y^2+z^2)._ 
```C++
inline float endstone::Location::length () const
```



Not world-aware and orientation independent.




**Returns:**

the magnitude 





        

<hr>



### function lengthSquared 

_Gets the magnitude of the location squared._ 
```C++
inline constexpr  float endstone::Location::lengthSquared () const
```



Not world-aware and orientation independent.




**Returns:**

the magnitude 





        

<hr>



### function operator Vector 

```C++
inline endstone::Location::operator Vector () noexcept const
```




<hr>



### function operator!= 

```C++
inline bool endstone::Location::operator!= (
    const  Location & other
) noexcept const
```




<hr>



### function operator\*= 

```C++
template<std::convertible_to< float > T>
inline Location & endstone::Location::operator*= (
    T scalar
) 
```




<hr>



### function operator+= 

```C++
inline Location & endstone::Location::operator+= (
    const  Location & other
) 
```




<hr>



### function operator+= 

```C++
inline Location & endstone::Location::operator+= (
    const  Vector & other
) 
```




<hr>



### function operator-= 

```C++
inline Location & endstone::Location::operator-= (
    const  Location & other
) 
```




<hr>



### function operator-= 

```C++
inline Location & endstone::Location::operator-= (
    const  Vector & other
) 
```




<hr>



### function operator== 

```C++
inline bool endstone::Location::operator== (
    const  Location & other
) noexcept const
```




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



### function setDirection 

_Sets the yaw and pitch to point in the direction of the vector._ 
```C++
inline Location & endstone::Location::setDirection (
    const  Vector & vector
) 
```





**Parameters:**


* `vector` the direction vector 



**Returns:**

the same location 





        

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



### function setX 

_Sets the x-coordinate of this location._ 
```C++
template<std::convertible_to< float > T>
inline constexpr  void endstone::Location::setX (
    T x
) 
```





**Parameters:**


* `x` X-coordinate 




        

<hr>



### function setY 

_Sets the y-coordinate of this location._ 
```C++
template<std::convertible_to< float > T>
inline constexpr  void endstone::Location::setY (
    T y
) 
```





**Parameters:**


* `y` y-coordinate 




        

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



### function setZ 

_Sets the z-coordinate of this location._ 
```C++
template<std::convertible_to< float > T>
inline constexpr  void endstone::Location::setZ (
    T z
) 
```





**Parameters:**


* `z` z-coordinate 




        

<hr>



### function zero 

_Zero this vector's components. Not world-aware._ 
```C++
inline constexpr  Location & endstone::Location::zero () 
```





**Returns:**

the same location 





        

<hr>
## Public Static Functions Documentation




### function normalizePitch 

_Normalizes the given pitch angle to a value between_ `+/-90` _degrees._
```C++
static inline float endstone::Location::normalizePitch (
    float pitch
) 
```





**Parameters:**


* `pitch` the pitch in degrees 



**Returns:**

the normalized pitch in degrees 





        

<hr>



### function normalizeYaw 

_Normalizes the given yaw angle to a value between_ `+/-180` _degrees._
```C++
static inline float endstone::Location::normalizeYaw (
    float yaw
) 
```





**Parameters:**


* `yaw` the yaw in degrees 



**Returns:**

the normalized yaw in degrees 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/level/location.h`

