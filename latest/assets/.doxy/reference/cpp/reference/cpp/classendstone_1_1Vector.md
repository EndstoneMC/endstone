

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
|  constexpr T | [**getX**](#function-getx) () const<br> |
|  constexpr T | [**getY**](#function-gety) () const<br> |
|  constexpr T | [**getZ**](#function-getz) () const<br> |
|  constexpr T | [**length**](#function-length) () const<br> |
|  constexpr T | [**lengthSquared**](#function-lengthsquared) () const<br> |
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
|  constexpr void | [**setX**](#function-setx) (T x) <br> |
|  constexpr void | [**setY**](#function-sety) (T y) <br> |
|  constexpr void | [**setZ**](#function-setz) (T z) <br> |




























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


```C++
inline constexpr T endstone::Vector::getX () const
```



Gets the X component.




**Returns:**

The X component. 





        

<hr>



### function getY 


```C++
inline constexpr T endstone::Vector::getY () const
```



Gets the Y component.




**Returns:**

The Y component. 





        

<hr>



### function getZ 


```C++
inline constexpr T endstone::Vector::getZ () const
```



Gets the Z component.




**Returns:**

The Z component. 





        

<hr>



### function length 


```C++
inline constexpr T endstone::Vector::length () const
```



Gets the magnitude of the vector, defined as sqrt(x^2+y^2+z^2).




**Returns:**

the magnitude 





        

<hr>



### function lengthSquared 


```C++
inline constexpr T endstone::Vector::lengthSquared () const
```



Gets the magnitude of the vector squared.




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



### function setX 


```C++
inline constexpr void endstone::Vector::setX (
    T x
) 
```



Set the X component.




**Parameters:**


* `x` The new X component. 



**Returns:**

This vector. 





        

<hr>



### function setY 


```C++
inline constexpr void endstone::Vector::setY (
    T y
) 
```



Set the Y component.




**Parameters:**


* `y` The new Y component. 



**Returns:**

This vector. 





        

<hr>



### function setZ 


```C++
inline constexpr void endstone::Vector::setZ (
    T z
) 
```



Set the Z component.




**Parameters:**


* `z` The new Z component. 



**Returns:**

This vector. 





        

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

