

# Class endstone::Vector

**template &lt;[**typename**](classendstone_1_1Vector.md) [**T**](classendstone_1_1Vector.md)&gt;**



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Vector**](classendstone_1_1Vector.md)



_Represents a 3-dimensional vector._ 

* `#include <endstone/util/vector.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|  [**constexpr**](classendstone_1_1Vector.md) | [**Vector**](#function-vector-12) () <br> |
|  [**constexpr**](classendstone_1_1Vector.md) | [**Vector**](#function-vector-22) ([**T**](classendstone_1_1Vector.md) x, [**T**](classendstone_1_1Vector.md) y, [**T**](classendstone_1_1Vector.md) z) <br> |
|  [**constexpr**](classendstone_1_1Vector.md) [**T**](classendstone_1_1Vector.md) | [**distance**](#function-distance) ([**const**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & other) const<br> |
|  [**constexpr**](classendstone_1_1Vector.md) [**T**](classendstone_1_1Vector.md) | [**distanceSquared**](#function-distancesquared) ([**const**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & other) const<br> |
|  [**constexpr**](classendstone_1_1Vector.md) [**T**](classendstone_1_1Vector.md) | [**getX**](#function-getx) () const<br>_Gets the X component._  |
|  [**constexpr**](classendstone_1_1Vector.md) [**T**](classendstone_1_1Vector.md) | [**getY**](#function-gety) () const<br>_Gets the Y component._  |
|  [**constexpr**](classendstone_1_1Vector.md) [**T**](classendstone_1_1Vector.md) | [**getZ**](#function-getz) () const<br>_Gets the Z component._  |
|  [**constexpr**](classendstone_1_1Vector.md) [**T**](classendstone_1_1Vector.md) | [**length**](#function-length) () const<br>_Gets the magnitude of the vector, defined as sqrt(x^2+y^2+z^2)._  |
|  [**constexpr**](classendstone_1_1Vector.md) [**T**](classendstone_1_1Vector.md) | [**lengthSquared**](#function-lengthsquared) () const<br>_Gets the magnitude of the vector squared._  |
|  [**constexpr**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; | [**operator\***](#function-operator) ([**const**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & other) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; | [**operator\***](#function-operator_1) ([**T**](classendstone_1_1Vector.md) scalar) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & | [**operator\*=**](#function-operator_2) ([**const**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & other) <br> |
|  [**constexpr**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; | [**operator+**](#function-operator_3) ([**const**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & other) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; | [**operator+**](#function-operator_4) ([**T**](classendstone_1_1Vector.md) scalar) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & | [**operator+=**](#function-operator_5) ([**const**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & other) <br> |
|  [**constexpr**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; | [**operator-**](#function-operator_6) ([**const**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & other) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; | [**operator-**](#function-operator_7) ([**T**](classendstone_1_1Vector.md) scalar) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & | [**operator-=**](#function-operator_8) ([**const**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & other) <br> |
|  [**constexpr**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; | [**operator/**](#function-operator_9) ([**const**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & other) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; | [**operator/**](#function-operator_10) ([**T**](classendstone_1_1Vector.md) scalar) const<br> |
|  [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & | [**operator/=**](#function-operator_11) ([**const**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & other) <br> |
|  [**constexpr**](classendstone_1_1Vector.md) [**bool**](classendstone_1_1Vector.md) | [**operator==**](#function-operator_12) ([**const**](classendstone_1_1Vector.md) [**Vector**](classendstone_1_1Vector.md)&lt; [**T**](classendstone_1_1Vector.md) &gt; & other) const<br> |
|  [**constexpr**](classendstone_1_1Vector.md) [**void**](classendstone_1_1Vector.md) | [**setX**](#function-setx) ([**T**](classendstone_1_1Vector.md) x) <br>_Set the X component._  |
|  [**constexpr**](classendstone_1_1Vector.md) [**void**](classendstone_1_1Vector.md) | [**setY**](#function-sety) ([**T**](classendstone_1_1Vector.md) y) <br>_Set the Y component._  |
|  [**constexpr**](classendstone_1_1Vector.md) [**void**](classendstone_1_1Vector.md) | [**setZ**](#function-setz) ([**T**](classendstone_1_1Vector.md) z) <br>_Set the Z component._  |








## Protected Attributes

| Type | Name |
| ---: | :--- |
|  [**T**](classendstone_1_1Vector.md) | [**x\_**](#variable-x_)  <br> |
|  [**T**](classendstone_1_1Vector.md) | [**y\_**](#variable-y_)  <br> |
|  [**T**](classendstone_1_1Vector.md) | [**z\_**](#variable-z_)  <br> |




















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
inline constexpr  T endstone::Vector::distance (
    const  Vector < T > & other
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
inline constexpr  T endstone::Vector::distanceSquared (
    const  Vector < T > & other
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
inline constexpr  T endstone::Vector::getX () const
```





**Returns:**

The X component. 





        

<hr>



### function getY 

_Gets the Y component._ 
```C++
inline constexpr  T endstone::Vector::getY () const
```





**Returns:**

The Y component. 





        

<hr>



### function getZ 

_Gets the Z component._ 
```C++
inline constexpr  T endstone::Vector::getZ () const
```





**Returns:**

The Z component. 





        

<hr>



### function length 

_Gets the magnitude of the vector, defined as sqrt(x^2+y^2+z^2)._ 
```C++
inline constexpr  T endstone::Vector::length () const
```





**Returns:**

the magnitude 





        

<hr>



### function lengthSquared 

_Gets the magnitude of the vector squared._ 
```C++
inline constexpr  T endstone::Vector::lengthSquared () const
```





**Returns:**

the magnitude 





        

<hr>



### function operator\* 

```C++
inline constexpr  Vector < T > endstone::Vector::operator* (
    const  Vector < T > & other
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
    const  Vector < T > & other
) 
```




<hr>



### function operator+ 

```C++
inline constexpr  Vector < T > endstone::Vector::operator+ (
    const  Vector < T > & other
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
    const  Vector < T > & other
) 
```




<hr>



### function operator- 

```C++
inline constexpr  Vector < T > endstone::Vector::operator- (
    const  Vector < T > & other
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
    const  Vector < T > & other
) 
```




<hr>



### function operator/ 

```C++
inline constexpr  Vector < T > endstone::Vector::operator/ (
    const  Vector < T > & other
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
    const  Vector < T > & other
) 
```




<hr>



### function operator== 

```C++
inline constexpr  bool endstone::Vector::operator== (
    const  Vector < T > & other
) const
```




<hr>



### function setX 

_Set the X component._ 
```C++
inline constexpr  void endstone::Vector::setX (
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
inline constexpr  void endstone::Vector::setY (
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
inline constexpr  void endstone::Vector::setZ (
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
    const  Vector < T > & v
) 
```




<hr>



### friend operator+ 

```C++
inline Vector < T > endstone::Vector::operator+ (
    T scalar,
    const  Vector < T > & v
) 
```




<hr>



### friend operator- 

```C++
inline Vector < T > endstone::Vector::operator- (
    T scalar,
    const  Vector < T > & v
) 
```




<hr>



### friend operator/ 

```C++
inline Vector < T > endstone::Vector::operator/ (
    T scalar,
    const  Vector < T > & v
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/util/vector.h`

