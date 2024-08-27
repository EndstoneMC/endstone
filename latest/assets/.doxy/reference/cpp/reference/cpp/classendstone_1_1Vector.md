

# Class endstone::Vector

**template &lt;typename T&gt;**



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Vector**](classendstone_1_1Vector.md)



_Represents a 3-dimensional vector._ 

* `#include <endstone/util/vector.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|  constexpr | [**Vector**](#function-vector-12) () <br> |
|  constexpr | [**Vector**](#function-vector-22) (T x, T y, T z) <br> |
|  constexpr T | [**distance**](#function-distance) (const [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; & other) const<br> |
|  constexpr T | [**distanceSquared**](#function-distancesquared) (const [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; & other) const<br> |
|  constexpr T | [**getX**](#function-getx) () const<br>_Gets the X component._  |
|  constexpr T | [**getY**](#function-gety) () const<br>_Gets the Y component._  |
|  constexpr T | [**getZ**](#function-getz) () const<br>_Gets the Z component._  |
|  constexpr T | [**length**](#function-length) () const<br>_Gets the magnitude of the vector, defined as sqrt(x^2+y^2+z^2)._  |
|  constexpr T | [**lengthSquared**](#function-lengthsquared) () const<br>_Gets the magnitude of the vector squared._  |
|  constexpr [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; | [**operator\***](#function-operator) (const [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; & other) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; | [**operator\***](#function-operator_1) (T scalar) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; & | [**operator\*=**](#function-operator_2) (const [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; & other) <br> |
|  constexpr [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; | [**operator+**](#function-operator_3) (const [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; & other) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; | [**operator+**](#function-operator_4) (T scalar) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; & | [**operator+=**](#function-operator_5) (const [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; & other) <br> |
|  constexpr [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; | [**operator-**](#function-operator_6) (const [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; & other) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; | [**operator-**](#function-operator_7) (T scalar) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; & | [**operator-=**](#function-operator_8) (const [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; & other) <br> |
|  constexpr [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; | [**operator/**](#function-operator_9) (const [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; & other) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; | [**operator/**](#function-operator_10) (T scalar) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; & | [**operator/=**](#function-operator_11) (const [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; & other) <br> |
|  constexpr bool | [**operator==**](#function-operator_12) (const [**Vector**](classendstone_1_1Vector.md)&lt; T &gt; & other) const<br> |
|  constexpr void | [**setX**](#function-setx) (T x) <br>_Set the X component._  |
|  constexpr void | [**setY**](#function-sety) (T y) <br>_Set the Y component._  |
|  constexpr void | [**setZ**](#function-setz) (T z) <br>_Set the Z component._  |








## Protected Attributes

| Type | Name |
| ---: | :--- |
|  T | [**x\_**](#variable-x_)  <br> |
|  T | [**y\_**](#variable-y_)  <br> |
|  T | [**z\_**](#variable-z_)  <br> |




















## Public Functions Documentation




### function Vector [1/2]

```C++
inline constexpr endstone::Vector::Vector () 
```




<hr>



### function Vector [2/2]

```C++
inline constexpr endstone::Vector::Vector (
    T x,
    T y,
    T z
) 
```




<hr>



### function distance 


```C++
inline constexpr T endstone::Vector::distance (
    const Vector < T > & other
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
inline constexpr T endstone::Vector::distanceSquared (
    const Vector < T > & other
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
inline constexpr T endstone::Vector::getX () const
```





**Returns:**

The X component. 





        

<hr>



### function getY 

_Gets the Y component._ 
```C++
inline constexpr T endstone::Vector::getY () const
```





**Returns:**

The Y component. 





        

<hr>



### function getZ 

_Gets the Z component._ 
```C++
inline constexpr T endstone::Vector::getZ () const
```





**Returns:**

The Z component. 





        

<hr>



### function length 

_Gets the magnitude of the vector, defined as sqrt(x^2+y^2+z^2)._ 
```C++
inline constexpr T endstone::Vector::length () const
```





**Returns:**

the magnitude 





        

<hr>



### function lengthSquared 

_Gets the magnitude of the vector squared._ 
```C++
inline constexpr T endstone::Vector::lengthSquared () const
```





**Returns:**

the magnitude 





        

<hr>



### function operator\* 

```C++
inline constexpr Vector < T > endstone::Vector::operator* (
    const Vector < T > & other
) const
```




<hr>



### function operator\* 

```C++
inline Vector < T > endstone::Vector::operator* (
    T scalar
) const
```




<hr>



### function operator\*= 

```C++
inline Vector < T > & endstone::Vector::operator*= (
    const Vector < T > & other
) 
```




<hr>



### function operator+ 

```C++
inline constexpr Vector < T > endstone::Vector::operator+ (
    const Vector < T > & other
) const
```




<hr>



### function operator+ 

```C++
inline Vector < T > endstone::Vector::operator+ (
    T scalar
) const
```




<hr>



### function operator+= 

```C++
inline Vector < T > & endstone::Vector::operator+= (
    const Vector < T > & other
) 
```




<hr>



### function operator- 

```C++
inline constexpr Vector < T > endstone::Vector::operator- (
    const Vector < T > & other
) const
```




<hr>



### function operator- 

```C++
inline Vector < T > endstone::Vector::operator- (
    T scalar
) const
```




<hr>



### function operator-= 

```C++
inline Vector < T > & endstone::Vector::operator-= (
    const Vector < T > & other
) 
```




<hr>



### function operator/ 

```C++
inline constexpr Vector < T > endstone::Vector::operator/ (
    const Vector < T > & other
) const
```




<hr>



### function operator/ 

```C++
inline Vector < T > endstone::Vector::operator/ (
    T scalar
) const
```




<hr>



### function operator/= 

```C++
inline Vector < T > & endstone::Vector::operator/= (
    const Vector < T > & other
) 
```




<hr>



### function operator== 

```C++
inline constexpr bool endstone::Vector::operator== (
    const Vector < T > & other
) const
```




<hr>



### function setX 

_Set the X component._ 
```C++
inline constexpr void endstone::Vector::setX (
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
inline constexpr void endstone::Vector::setY (
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
inline constexpr void endstone::Vector::setZ (
    T z
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
T endstone::Vector< T >::x_;
```




<hr>



### variable y\_ 

```C++
T endstone::Vector< T >::y_;
```




<hr>



### variable z\_ 

```C++
T endstone::Vector< T >::z_;
```




<hr>## Friends Documentation





### friend operator\* 

```C++
inline Vector < T > endstone::Vector::operator* (
    T scalar,
    const Vector < T > & v
) 
```




<hr>



### friend operator+ 

```C++
inline Vector < T > endstone::Vector::operator+ (
    T scalar,
    const Vector < T > & v
) 
```




<hr>



### friend operator- 

```C++
inline Vector < T > endstone::Vector::operator- (
    T scalar,
    const Vector < T > & v
) 
```




<hr>



### friend operator/ 

```C++
inline Vector < T > endstone::Vector::operator/ (
    T scalar,
    const Vector < T > & v
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/util/vector.h`

