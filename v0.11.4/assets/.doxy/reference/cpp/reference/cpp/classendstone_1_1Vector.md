

# Class endstone::Vector



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Vector**](classendstone_1_1Vector.md)



_Represents a 3-dimensional vector._ 

* `#include <endstone/util/vector.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|  [**constexpr**](classendstone_1_1Identifier.md) | [**Vector**](#function-vector-14) () = default<br>_Construct the vector with all components as 0._  |
|  [**constexpr**](classendstone_1_1Identifier.md) | [**Vector**](#function-vector-24) ([**T**](classendstone_1_1Identifier.md) x, [**T**](classendstone_1_1Identifier.md) y, [**T**](classendstone_1_1Identifier.md) z) <br>_Construct the vector with provided components._  |
|   | [**Vector**](#function-vector-34) ([**const**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) & other) = default<br> |
|   | [**Vector**](#function-vector-44) ([**Vector**](classendstone_1_1Vector.md) && other) noexcept<br> |
|  [**float**](classendstone_1_1Identifier.md) | [**angle**](#function-angle) ([**const**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) & other) const<br>_Gets the angle between this vector and another in radians._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) & | [**crossProduct**](#function-crossproduct) ([**const**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) & other) <br>_Calculates the cross-product of this vector with another._  |
|  [**float**](classendstone_1_1Identifier.md) | [**distance**](#function-distance) ([**const**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) & other) const<br>_Get the distance between this vector and another._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**float**](classendstone_1_1Identifier.md) | [**distanceSquared**](#function-distancesquared) ([**const**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) & other) const<br>_Get the squared distance between this vector and another._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**float**](classendstone_1_1Identifier.md) | [**dot**](#function-dot) ([**const**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) & other) const<br>_Calculates the dot product of this vector with another. The dot product is defined as x1\*x2+y1\*y2+z1\*z2. The returned value is a scalar._  |
|  [**int**](classendstone_1_1Identifier.md) | [**getBlockX**](#function-getblockx) () const<br>_Gets the floored value of the X component, indicating the block that this vector is contained with._  |
|  [**int**](classendstone_1_1Identifier.md) | [**getBlockY**](#function-getblocky) () const<br>_Gets the floored value of the Y component, indicating the block that this vector is contained with._  |
|  [**int**](classendstone_1_1Identifier.md) | [**getBlockZ**](#function-getblockz) () const<br>_Gets the floored value of the Z component, indicating the block that this vector is contained with._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) | [**getCrossProduct**](#function-getcrossproduct) ([**const**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) & other) const<br>_Calculates the cross-product of this vector with another without mutating the original._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) | [**getMidpoint**](#function-getmidpoint) ([**const**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) & other) const<br>_Gets a new midpoint vector between this vector and another._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**float**](classendstone_1_1Identifier.md) | [**getX**](#function-getx) () const<br>_Gets the X component._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**float**](classendstone_1_1Identifier.md) | [**getY**](#function-gety) () const<br>_Gets the Y component._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**float**](classendstone_1_1Identifier.md) | [**getZ**](#function-getz) () const<br>_Gets the Z component._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**bool**](classendstone_1_1Identifier.md) | [**isInAABB**](#function-isinaabb) ([**const**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) & min, [**const**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) & max) const<br>_Returns whether this vector is in an axis-aligned bounding box._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**bool**](classendstone_1_1Identifier.md) | [**isInSphere**](#function-isinsphere) ([**const**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) & origin, [**float**](classendstone_1_1Identifier.md) radius) const<br>_Returns whether this vector is within a sphere._  |
|  [**bool**](classendstone_1_1Identifier.md) | [**isNormalized**](#function-isnormalized) () const<br>_Returns if a vector is normalized._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**bool**](classendstone_1_1Identifier.md) | [**isZero**](#function-iszero) () const<br>_Check whether or not each component of this vector is equal to 0._  |
|  [**float**](classendstone_1_1Identifier.md) | [**length**](#function-length) () const<br>_Gets the magnitude of the vector, defined as sqrt(x^2+y^2+z^2)._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**float**](classendstone_1_1Identifier.md) | [**lengthSquared**](#function-lengthsquared) () const<br>_Gets the magnitude of the vector squared._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) & | [**midpoint**](#function-midpoint) ([**const**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) & other) <br>_Sets this vector to the midpoint between this vector and another._  |
|  [**Vector**](classendstone_1_1Vector.md) & | [**normalize**](#function-normalize) () <br>_Converts this vector to a unit vector (a vector with length of 1)._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) & | [**normalizeZeros**](#function-normalizezeros) () <br>_Converts each component of value_ `-0.0` _to_`0.0` _._ |
|  [**bool**](classendstone_1_1Identifier.md) | [**operator!=**](#function-operator) ([**const**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) & other) noexcept const<br> |
|  [**constexpr**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) | [**operator\***](#function-operator_1) ([**const**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) & other) const<br> |
|  [**constexpr**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) | [**operator\***](#function-operator_2) ([**T**](classendstone_1_1Identifier.md) scalar) const<br> |
|  [**Vector**](classendstone_1_1Vector.md) & | [**operator\*=**](#function-operator_3) ([**const**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) & other) <br> |
|  [**constexpr**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) | [**operator+**](#function-operator_4) ([**const**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) & other) const<br> |
|  [**constexpr**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) | [**operator+**](#function-operator_5) ([**T**](classendstone_1_1Identifier.md) scalar) const<br> |
|  [**Vector**](classendstone_1_1Vector.md) & | [**operator+=**](#function-operator_6) ([**const**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) & other) <br> |
|  [**constexpr**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) | [**operator-**](#function-operator-) ([**const**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) & other) const<br> |
|  [**constexpr**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) | [**operator-**](#function-operator-_1) ([**T**](classendstone_1_1Identifier.md) scalar) const<br> |
|  [**Vector**](classendstone_1_1Vector.md) & | [**operator-=**](#function-operator-_2) ([**const**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) & other) <br> |
|  [**constexpr**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) | [**operator/**](#function-operator_7) ([**const**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) & other) const<br> |
|  [**constexpr**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) | [**operator/**](#function-operator_8) ([**T**](classendstone_1_1Identifier.md) scalar) const<br> |
|  [**Vector**](classendstone_1_1Vector.md) & | [**operator/=**](#function-operator_9) ([**const**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) & other) <br> |
|  [**Vector**](classendstone_1_1Vector.md) & | [**operator=**](#function-operator_10) ([**const**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) & other) = default<br> |
|  [**Vector**](classendstone_1_1Vector.md) & | [**operator=**](#function-operator_11) ([**Vector**](classendstone_1_1Vector.md) && other) noexcept<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**operator==**](#function-operator_12) ([**const**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) & other) noexcept const<br> |
|  [**Vector**](classendstone_1_1Vector.md) & | [**rotateAroundAxis**](#function-rotatearoundaxis) ([**const**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) & axis, [**float**](classendstone_1_1Identifier.md) angle) <br>_Rotates the vector around a given arbitrary axis in 3-dimensional space._  |
|  [**Vector**](classendstone_1_1Vector.md) & | [**rotateAroundNonUnitAxis**](#function-rotatearoundnonunitaxis) ([**const**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) & axis, [**float**](classendstone_1_1Identifier.md) angle) <br>_Rotates the vector around a given arbitrary axis in 3-dimensional space._  |
|  [**Vector**](classendstone_1_1Vector.md) & | [**rotateAroundX**](#function-rotatearoundx) ([**float**](classendstone_1_1Identifier.md) angle) <br>_Rotates the vector around the x-axis._  |
|  [**Vector**](classendstone_1_1Vector.md) & | [**rotateAroundY**](#function-rotatearoundy) ([**float**](classendstone_1_1Identifier.md) angle) <br>_Rotates the vector around the y-axis._  |
|  [**Vector**](classendstone_1_1Vector.md) & | [**rotateAroundZ**](#function-rotatearoundz) ([**float**](classendstone_1_1Identifier.md) angle) <br>_Rotates the vector around the z-axis._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) & | [**setX**](#function-setx) ([**T**](classendstone_1_1Identifier.md) x) <br>_Set the X component._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) & | [**setY**](#function-sety) ([**T**](classendstone_1_1Identifier.md) y) <br>_Set the Y component._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) & | [**setZ**](#function-setz) ([**T**](classendstone_1_1Identifier.md) z) <br>_Set the Z component._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**Vector**](classendstone_1_1Vector.md) & | [**zero**](#function-zero) () <br>_Zero this vector's components._  |








## Protected Attributes

| Type | Name |
| ---: | :--- |
|  [**float**](classendstone_1_1Identifier.md) | [**x\_**](#variable-x_)   = `0.0`<br> |
|  [**float**](classendstone_1_1Identifier.md) | [**y\_**](#variable-y_)   = `0.0`<br> |
|  [**float**](classendstone_1_1Identifier.md) | [**z\_**](#variable-z_)   = `0.0`<br> |




















## Public Functions Documentation




### function Vector [1/4]

_Construct the vector with all components as 0._ 
```C++
constexpr endstone::Vector::Vector () = default
```




<hr>



### function Vector [2/4]

_Construct the vector with provided components._ 
```C++
template<std::convertible_to< float > T>
inline constexpr endstone::Vector::Vector (
    T x,
    T y,
    T z
) 
```





**Parameters:**


* `x` X component 
* `y` Y component 
* `z` Z component 




        

<hr>



### function Vector [3/4]

```C++
endstone::Vector::Vector (
    const  Vector & other
) = default
```




<hr>



### function Vector [4/4]

```C++
endstone::Vector::Vector (
    Vector && other
) noexcept
```




<hr>



### function angle 

_Gets the angle between this vector and another in radians._ 
```C++
inline float endstone::Vector::angle (
    const  Vector & other
) const
```





**Parameters:**


* `other` The other vector 



**Returns:**

angle in radians 





        

<hr>



### function crossProduct 

_Calculates the cross-product of this vector with another._ 
```C++
inline constexpr  Vector & endstone::Vector::crossProduct (
    const  Vector & other
) 
```



The cross-product is defined as:
* x = y1 \* z2 - y2 \* z1
* y = z1 \* x2 - z2 \* x1
* z = x1 \* y2 - x2 \* y1






**Parameters:**


* `other` The other vector 



**Returns:**

the same vector 





        

<hr>



### function distance 

_Get the distance between this vector and another._ 
```C++
inline float endstone::Vector::distance (
    const  Vector & other
) const
```





**Parameters:**


* `other` The other vector 



**Returns:**

the distance 





        

<hr>



### function distanceSquared 

_Get the squared distance between this vector and another._ 
```C++
inline constexpr  float endstone::Vector::distanceSquared (
    const  Vector & other
) const
```





**Parameters:**


* `other` The other vector 



**Returns:**

the distance 





        

<hr>



### function dot 

_Calculates the dot product of this vector with another. The dot product is defined as x1\*x2+y1\*y2+z1\*z2. The returned value is a scalar._ 
```C++
inline constexpr  float endstone::Vector::dot (
    const  Vector & other
) const
```





**Parameters:**


* `other` The other vector 



**Returns:**

dot product 





        

<hr>



### function getBlockX 

_Gets the floored value of the X component, indicating the block that this vector is contained with._ 
```C++
inline int endstone::Vector::getBlockX () const
```





**Returns:**

block X 





        

<hr>



### function getBlockY 

_Gets the floored value of the Y component, indicating the block that this vector is contained with._ 
```C++
inline int endstone::Vector::getBlockY () const
```





**Returns:**

block y 





        

<hr>



### function getBlockZ 

_Gets the floored value of the Z component, indicating the block that this vector is contained with._ 
```C++
inline int endstone::Vector::getBlockZ () const
```





**Returns:**

block z 





        

<hr>



### function getCrossProduct 

_Calculates the cross-product of this vector with another without mutating the original._ 
```C++
inline constexpr  Vector endstone::Vector::getCrossProduct (
    const  Vector & other
) const
```



The cross-product is defined as:
* x = y1 \* z2 - y2 \* z1
* y = z1 \* x2 - z2 \* x1
* z = x1 \* y2 - x2 \* y1






**Parameters:**


* `other` The other vector 



**Returns:**

a new vector 





        

<hr>



### function getMidpoint 

_Gets a new midpoint vector between this vector and another._ 
```C++
inline constexpr  Vector endstone::Vector::getMidpoint (
    const  Vector & other
) const
```





**Parameters:**


* `other` The other vector 



**Returns:**

a new midpoint vector 





        

<hr>



### function getX 

_Gets the X component._ 
```C++
inline constexpr  float endstone::Vector::getX () const
```





**Returns:**

The X component. 





        

<hr>



### function getY 

_Gets the Y component._ 
```C++
inline constexpr  float endstone::Vector::getY () const
```





**Returns:**

The Y component. 





        

<hr>



### function getZ 

_Gets the Z component._ 
```C++
inline constexpr  float endstone::Vector::getZ () const
```





**Returns:**

The Z component. 





        

<hr>



### function isInAABB 

_Returns whether this vector is in an axis-aligned bounding box._ 
```C++
inline constexpr  bool endstone::Vector::isInAABB (
    const  Vector & min,
    const  Vector & max
) const
```



The minimum and maximum vectors given must be truly the minimum and maximum X, Y and Z components.




**Parameters:**


* `min` Minimum vector 
* `max` Maximum vector



**Returns:**

whether this vector is in the AABB 





        

<hr>



### function isInSphere 

_Returns whether this vector is within a sphere._ 
```C++
inline constexpr  bool endstone::Vector::isInSphere (
    const  Vector & origin,
    float radius
) const
```





**Parameters:**


* `origin` Sphere origin. 
* `radius` Sphere radius



**Returns:**

whether this vector is in the sphere 





        

<hr>



### function isNormalized 

_Returns if a vector is normalized._ 
```C++
inline bool endstone::Vector::isNormalized () const
```





**Returns:**

whether the vector is normalized 





        

<hr>



### function isZero 

_Check whether or not each component of this vector is equal to 0._ 
```C++
inline constexpr  bool endstone::Vector::isZero () const
```





**Returns:**

true if equal to zero, false if at least one component is non-zero 





        

<hr>



### function length 

_Gets the magnitude of the vector, defined as sqrt(x^2+y^2+z^2)._ 
```C++
inline float endstone::Vector::length () const
```





**Returns:**

the magnitude 





        

<hr>



### function lengthSquared 

_Gets the magnitude of the vector squared._ 
```C++
inline constexpr  float endstone::Vector::lengthSquared () const
```





**Returns:**

the magnitude 





        

<hr>



### function midpoint 

_Sets this vector to the midpoint between this vector and another._ 
```C++
inline constexpr  Vector & endstone::Vector::midpoint (
    const  Vector & other
) 
```





**Parameters:**


* `other` The other vector 



**Returns:**

this same vector (now a midpoint) 





        

<hr>



### function normalize 

_Converts this vector to a unit vector (a vector with length of 1)._ 
```C++
inline Vector & endstone::Vector::normalize () 
```





**Returns:**

the same vector 





        

<hr>



### function normalizeZeros 

_Converts each component of value_ `-0.0` _to_`0.0` _._
```C++
inline constexpr  Vector & endstone::Vector::normalizeZeros () 
```





**Returns:**

This vector. 





        

<hr>



### function operator!= 

```C++
inline bool endstone::Vector::operator!= (
    const  Vector & other
) noexcept const
```




<hr>



### function operator\* 

```C++
inline constexpr  Vector endstone::Vector::operator* (
    const  Vector & other
) const
```




<hr>



### function operator\* 

```C++
template<std::convertible_to< float > T>
inline constexpr  Vector endstone::Vector::operator* (
    T scalar
) const
```




<hr>



### function operator\*= 

```C++
inline Vector & endstone::Vector::operator*= (
    const  Vector & other
) 
```




<hr>



### function operator+ 

```C++
inline constexpr  Vector endstone::Vector::operator+ (
    const  Vector & other
) const
```




<hr>



### function operator+ 

```C++
template<std::convertible_to< float > T>
inline constexpr  Vector endstone::Vector::operator+ (
    T scalar
) const
```




<hr>



### function operator+= 

```C++
inline Vector & endstone::Vector::operator+= (
    const  Vector & other
) 
```




<hr>



### function operator- 

```C++
inline constexpr  Vector endstone::Vector::operator- (
    const  Vector & other
) const
```




<hr>



### function operator- 

```C++
template<std::convertible_to< float > T>
inline constexpr  Vector endstone::Vector::operator- (
    T scalar
) const
```




<hr>



### function operator-= 

```C++
inline Vector & endstone::Vector::operator-= (
    const  Vector & other
) 
```




<hr>



### function operator/ 

```C++
inline constexpr  Vector endstone::Vector::operator/ (
    const  Vector & other
) const
```




<hr>



### function operator/ 

```C++
template<std::convertible_to< float > T>
inline constexpr  Vector endstone::Vector::operator/ (
    T scalar
) const
```




<hr>



### function operator/= 

```C++
inline Vector & endstone::Vector::operator/= (
    const  Vector & other
) 
```




<hr>



### function operator= 

```C++
Vector & endstone::Vector::operator= (
    const  Vector & other
) = default
```




<hr>



### function operator= 

```C++
Vector & endstone::Vector::operator= (
    Vector && other
) noexcept
```




<hr>



### function operator== 

```C++
inline bool endstone::Vector::operator== (
    const  Vector & other
) noexcept const
```




<hr>



### function rotateAroundAxis 

_Rotates the vector around a given arbitrary axis in 3-dimensional space._ 
```C++
inline Vector & endstone::Vector::rotateAroundAxis (
    const  Vector & axis,
    float angle
) 
```



Rotation will follow the general Right-Hand-Rule, which means rotation will be counterclockwise when the axis is pointing towards the observer.


This method will always make sure the provided axis is a unit vector, to not modify the length of the vector when rotating. If you are experienced with the scaling of a non-unit axis vector, you can use `rotateAroundNonUnitAxis(Vector, float)`.




**Parameters:**


* `axis` the axis to rotate the vector around. If the passed vector is not of length 1, it gets normalized before using it for the rotation.
* `angle` the angle to rotate the vector around the axis



**Returns:**

the same vector 





        

<hr>



### function rotateAroundNonUnitAxis 

_Rotates the vector around a given arbitrary axis in 3-dimensional space._ 
```C++
inline Vector & endstone::Vector::rotateAroundNonUnitAxis (
    const  Vector & axis,
    float angle
) 
```



Rotation will follow the general Right-Hand-Rule, which means rotation will be counterclockwise when the axis is pointing towards the observer.


Note that the vector length will change accordingly to the axis vector length. If the provided axis is not a unit vector, the rotated vector will not have its previous length. The scaled length of the resulting vector will be related to the axis vector. If you are not sure about the scaling of the vector, use `rotateAroundAxis(Vector, float)`.




**Parameters:**


* `axis` the axis to rotate the vector around. 
* `angle` the angle to rotate the vector around the axis



**Returns:**

the same vector 





        

<hr>



### function rotateAroundX 

_Rotates the vector around the x-axis._ 
```C++
inline Vector & endstone::Vector::rotateAroundX (
    float angle
) 
```





**Parameters:**


* `angle` the angle to rotate the vector about. This angle is passed in radians



**Returns:**

the same vector 





        

<hr>



### function rotateAroundY 

_Rotates the vector around the y-axis._ 
```C++
inline Vector & endstone::Vector::rotateAroundY (
    float angle
) 
```





**Parameters:**


* `angle` the angle to rotate the vector about. This angle is passed in radians



**Returns:**

the same vector 





        

<hr>



### function rotateAroundZ 

_Rotates the vector around the z-axis._ 
```C++
inline Vector & endstone::Vector::rotateAroundZ (
    float angle
) 
```





**Parameters:**


* `angle` the angle to rotate the vector about. This angle is passed in radians



**Returns:**

the same vector 





        

<hr>



### function setX 

_Set the X component._ 
```C++
template<std::convertible_to< float > T>
inline constexpr  Vector & endstone::Vector::setX (
    T x
) 
```





**Parameters:**


* `x` The new X component. 



**Returns:**

This vector. 





        

<hr>



### function setY 

_Set the Y component._ 
```C++
template<std::convertible_to< float > T>
inline constexpr  Vector & endstone::Vector::setY (
    T y
) 
```





**Parameters:**


* `y` The new Y component. 



**Returns:**

This vector. 





        

<hr>



### function setZ 

_Set the Z component._ 
```C++
template<std::convertible_to< float > T>
inline constexpr  Vector & endstone::Vector::setZ (
    T z
) 
```





**Parameters:**


* `z` The new Z component. 



**Returns:**

This vector. 





        

<hr>



### function zero 

_Zero this vector's components._ 
```C++
inline constexpr  Vector & endstone::Vector::zero () 
```





**Returns:**

the same vector 





        

<hr>
## Protected Attributes Documentation




### variable x\_ 

```C++
float endstone::Vector::x_;
```




<hr>



### variable y\_ 

```C++
float endstone::Vector::y_;
```




<hr>



### variable z\_ 

```C++
float endstone::Vector::z_;
```




<hr>## Friends Documentation





### friend operator\* 

```C++
template<typename  T>
inline Vector endstone::Vector::operator* (
    T scalar,
    const  Vector & v
) 
```




<hr>



### friend operator+ 

```C++
template<typename  T>
inline Vector endstone::Vector::operator+ (
    T scalar,
    const  Vector & v
) 
```




<hr>



### friend operator- 

```C++
template<typename  T>
inline Vector endstone::Vector::operator- (
    T scalar,
    const  Vector & v
) 
```




<hr>



### friend operator/ 

```C++
template<typename  T>
inline Vector endstone::Vector::operator/ (
    T scalar,
    const  Vector & v
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/util/vector.h`

