

# Class endstone::Vector



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Vector**](classendstone_1_1Vector.md)



_Represents a 3-dimensional vector._ 

* `#include <endstone/util/vector.h>`





Inherited by the following classes: [endstone::Location](classendstone_1_1Location.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
|  constexpr | [**Vector**](#function-vector-12) () = default<br>_Construct the vector with all components as 0._  |
|  constexpr | [**Vector**](#function-vector-22) (T x, T y, T z) <br>_Construct the vector with provided components._  |
|  float | [**distance**](#function-distance) (const [**Vector**](classendstone_1_1Vector.md) & other) const<br> |
|  constexpr float | [**distanceSquared**](#function-distancesquared) (const [**Vector**](classendstone_1_1Vector.md) & other) const<br> |
|  constexpr float | [**getX**](#function-getx) () const<br>_Gets the X component._  |
|  constexpr float | [**getY**](#function-gety) () const<br>_Gets the Y component._  |
|  constexpr float | [**getZ**](#function-getz) () const<br>_Gets the Z component._  |
|  float | [**length**](#function-length) () const<br>_Gets the magnitude of the vector, defined as sqrt(x^2+y^2+z^2)._  |
|  constexpr float | [**lengthSquared**](#function-lengthsquared) () const<br>_Gets the magnitude of the vector squared._  |
|  [**Vector**](classendstone_1_1Vector.md) & | [**normalize**](#function-normalize) () <br> |
|  constexpr [**Vector**](classendstone_1_1Vector.md) | [**operator\***](#function-operator) (const [**Vector**](classendstone_1_1Vector.md) & other) const<br> |
|  [**Vector**](classendstone_1_1Vector.md) | [**operator\***](#function-operator_1) (float scalar) const<br> |
|  [**Vector**](classendstone_1_1Vector.md) & | [**operator\*=**](#function-operator_2) (const [**Vector**](classendstone_1_1Vector.md) & other) <br> |
|  constexpr [**Vector**](classendstone_1_1Vector.md) | [**operator+**](#function-operator_3) (const [**Vector**](classendstone_1_1Vector.md) & other) const<br> |
|  [**Vector**](classendstone_1_1Vector.md) | [**operator+**](#function-operator_4) (float scalar) const<br> |
|  [**Vector**](classendstone_1_1Vector.md) & | [**operator+=**](#function-operator_5) (const [**Vector**](classendstone_1_1Vector.md) & other) <br> |
|  constexpr [**Vector**](classendstone_1_1Vector.md) | [**operator-**](#function-operator-) (const [**Vector**](classendstone_1_1Vector.md) & other) const<br> |
|  [**Vector**](classendstone_1_1Vector.md) | [**operator-**](#function-operator-_1) (float scalar) const<br> |
|  [**Vector**](classendstone_1_1Vector.md) & | [**operator-=**](#function-operator-_2) (const [**Vector**](classendstone_1_1Vector.md) & other) <br> |
|  constexpr [**Vector**](classendstone_1_1Vector.md) | [**operator/**](#function-operator_6) (const [**Vector**](classendstone_1_1Vector.md) & other) const<br> |
|  [**Vector**](classendstone_1_1Vector.md) | [**operator/**](#function-operator_7) (float scalar) const<br> |
|  [**Vector**](classendstone_1_1Vector.md) & | [**operator/=**](#function-operator_8) (const [**Vector**](classendstone_1_1Vector.md) & other) <br> |
|  constexpr bool | [**operator==**](#function-operator_9) (const [**Vector**](classendstone_1_1Vector.md) & other) const<br> |
|  constexpr void | [**setX**](#function-setx) (float x) <br>_Set the X component._  |
|  constexpr void | [**setY**](#function-sety) (float y) <br>_Set the Y component._  |
|  constexpr void | [**setZ**](#function-setz) (float z) <br>_Set the Z component._  |








## Protected Attributes

| Type | Name |
| ---: | :--- |
|  float | [**x\_**](#variable-x_)   = `0.0`<br> |
|  float | [**y\_**](#variable-y_)   = `0.0`<br> |
|  float | [**z\_**](#variable-z_)   = `0.0`<br> |




















## Public Functions Documentation




### function Vector [1/2]

_Construct the vector with all components as 0._ 
```C++
constexpr endstone::Vector::Vector () = default
```




<hr>



### function Vector [2/2]

_Construct the vector with provided components._ 
```C++
template<typename T>
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



### function distance 

```C++
inline float endstone::Vector::distance (
    const Vector & other
) const
```



Get the distance between this vector and another.




**Parameters:**


* `other` The other vector 



**Returns:**

the distance 





        

<hr>



### function distanceSquared 

```C++
inline constexpr float endstone::Vector::distanceSquared (
    const Vector & other
) const
```



Get the squared distance between this vector and another.




**Parameters:**


* `other` The other vector 



**Returns:**

the distance 





        

<hr>



### function getX 

_Gets the X component._ 
```C++
inline constexpr float endstone::Vector::getX () const
```





**Returns:**

The X component. 





        

<hr>



### function getY 

_Gets the Y component._ 
```C++
inline constexpr float endstone::Vector::getY () const
```





**Returns:**

The Y component. 





        

<hr>



### function getZ 

_Gets the Z component._ 
```C++
inline constexpr float endstone::Vector::getZ () const
```





**Returns:**

The Z component. 





        

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
inline constexpr float endstone::Vector::lengthSquared () const
```





**Returns:**

the magnitude 





        

<hr>



### function normalize 

```C++
inline Vector & endstone::Vector::normalize () 
```



Converts this vector to a unit vector (a vector with length of 1).




**Returns:**

the same vector 





        

<hr>



### function operator\* 

```C++
inline constexpr Vector endstone::Vector::operator* (
    const Vector & other
) const
```




<hr>



### function operator\* 

```C++
inline Vector endstone::Vector::operator* (
    float scalar
) const
```




<hr>



### function operator\*= 

```C++
inline Vector & endstone::Vector::operator*= (
    const Vector & other
) 
```




<hr>



### function operator+ 

```C++
inline constexpr Vector endstone::Vector::operator+ (
    const Vector & other
) const
```




<hr>



### function operator+ 

```C++
inline Vector endstone::Vector::operator+ (
    float scalar
) const
```




<hr>



### function operator+= 

```C++
inline Vector & endstone::Vector::operator+= (
    const Vector & other
) 
```




<hr>



### function operator- 

```C++
inline constexpr Vector endstone::Vector::operator- (
    const Vector & other
) const
```




<hr>



### function operator- 

```C++
inline Vector endstone::Vector::operator- (
    float scalar
) const
```




<hr>



### function operator-= 

```C++
inline Vector & endstone::Vector::operator-= (
    const Vector & other
) 
```




<hr>



### function operator/ 

```C++
inline constexpr Vector endstone::Vector::operator/ (
    const Vector & other
) const
```




<hr>



### function operator/ 

```C++
inline Vector endstone::Vector::operator/ (
    float scalar
) const
```




<hr>



### function operator/= 

```C++
inline Vector & endstone::Vector::operator/= (
    const Vector & other
) 
```




<hr>



### function operator== 

```C++
inline constexpr bool endstone::Vector::operator== (
    const Vector & other
) const
```




<hr>



### function setX 

_Set the X component._ 
```C++
inline constexpr void endstone::Vector::setX (
    float x
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
inline constexpr void endstone::Vector::setY (
    float y
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
inline constexpr void endstone::Vector::setZ (
    float z
) 
```





**Parameters:**


* `z` The new Z component. 



**Returns:**

This vector. 





        

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
inline Vector endstone::Vector::operator* (
    float scalar,
    const Vector & v
) 
```




<hr>



### friend operator+ 

```C++
inline Vector endstone::Vector::operator+ (
    float scalar,
    const Vector & v
) 
```




<hr>



### friend operator- 

```C++
inline Vector endstone::Vector::operator- (
    float scalar,
    const Vector & v
) 
```




<hr>



### friend operator/ 

```C++
inline Vector endstone::Vector::operator/ (
    float scalar,
    const Vector & v
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/util/vector.h`

