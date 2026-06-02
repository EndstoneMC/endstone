

# Class endstone::Nullable

**template &lt;[**class**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md)&gt;**



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Nullable**](classendstone_1_1Nullable.md)



_A wrapper around a std::shared\_ptr that may be null._ [More...](#detailed-description)

* `#include <endstone/util/pointers.h>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef [**T**](classendstone_1_1Identifier.md) | [**element\_type**](#typedef-element_type)  <br> |
| typedef std::shared\_ptr&lt; [**T**](classendstone_1_1Identifier.md) &gt; | [**pointer\_type**](#typedef-pointer_type)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  [**constexpr**](classendstone_1_1Identifier.md) | [**Nullable**](#function-nullable-17) () noexcept<br> |
|  [**constexpr**](classendstone_1_1Identifier.md) | [**Nullable**](#function-nullable-27) (std::nullptr\_t) noexcept<br> |
|   | [**Nullable**](#function-nullable-37) (std::shared\_ptr&lt; [**T**](classendstone_1_1Identifier.md) &gt; ptr) <br> |
|   | [**Nullable**](#function-nullable-47) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**T**](classendstone_1_1Identifier.md) &gt; & other) <br> |
|   | [**Nullable**](#function-nullable-57) (std::shared\_ptr&lt; [**U**](classendstone_1_1Identifier.md) &gt; ptr) <br>_Covariant conversion from a shared\_ptr of a convertible (e.g._  |
|   | [**Nullable**](#function-nullable-67) ([**const**](classendstone_1_1Identifier.md) [**Nullable**](classendstone_1_1Nullable.md)&lt; [**U**](classendstone_1_1Identifier.md) &gt; & other) <br>_Covariant conversion from a_ [_**Nullable**_](classendstone_1_1Nullable.md) _of a convertible (e.g._ |
|   | [**Nullable**](#function-nullable-77) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**U**](classendstone_1_1Identifier.md) &gt; & other) <br>_Covariant conversion from a_ [_**NotNull**_](classendstone_1_1NotNull.md) _of a convertible (e.g._ |
|  [**const**](classendstone_1_1Identifier.md) pointer\_type & | [**get**](#function-get) () noexcept const<br> |
|   | [**operator bool**](#function-operator-bool) () noexcept const<br> |
|  [**T**](classendstone_1_1Identifier.md) & | [**operator\***](#function-operator) () noexcept const<br> |
|  [**T**](classendstone_1_1Identifier.md) \* | [**operator-&gt;**](#function-operator-) () noexcept const<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**operator==**](#function-operator_1) (std::nullptr\_t) noexcept const<br> |
|  [**T**](classendstone_1_1Identifier.md) & | [**value**](#function-value) () const<br>_Returns a reference to the contained value, throwing_ `std::runtime_error` _if this is null._ |
|  pointer\_type | [**value\_or**](#function-value_or) (pointer\_type default\_value) const<br>_Returns the contained pointer if present, otherwise the supplied default._  |




























## Detailed Description


Convertible from a [**NotNull**](classendstone_1_1NotNull.md), and contextually convertible to bool to test for a value. 


    
## Public Types Documentation




### typedef element\_type 

```C++
using endstone::Nullable< T >::element_type =  T;
```




<hr>



### typedef pointer\_type 

```C++
using endstone::Nullable< T >::pointer_type =  std::shared_ptr<T>;
```




<hr>
## Public Functions Documentation




### function Nullable [1/7]

```C++
constexpr endstone::Nullable::Nullable () noexcept
```




<hr>



### function Nullable [2/7]

```C++
inline constexpr endstone::Nullable::Nullable (
    std::nullptr_t
) noexcept
```




<hr>



### function Nullable [3/7]

```C++
inline endstone::Nullable::Nullable (
    std::shared_ptr< T > ptr
) 
```




<hr>



### function Nullable [4/7]

```C++
inline endstone::Nullable::Nullable (
    const  NotNull < T > & other
) 
```




<hr>



### function Nullable [5/7]

_Covariant conversion from a shared\_ptr of a convertible (e.g._ 
```C++
template<class  U>
inline endstone::Nullable::Nullable (
    std::shared_ptr< U > ptr
) 
```



derived) type. 


        

<hr>



### function Nullable [6/7]

_Covariant conversion from a_ [_**Nullable**_](classendstone_1_1Nullable.md) _of a convertible (e.g._
```C++
template<class  U>
inline endstone::Nullable::Nullable (
    const  Nullable < U > & other
) 
```



derived) type. 


        

<hr>



### function Nullable [7/7]

_Covariant conversion from a_ [_**NotNull**_](classendstone_1_1NotNull.md) _of a convertible (e.g._
```C++
template<class  U>
inline endstone::Nullable::Nullable (
    const  NotNull < U > & other
) 
```



derived) type. 


        

<hr>



### function get 

```C++
inline const pointer_type & endstone::Nullable::get () noexcept const
```




<hr>



### function operator bool 

```C++
inline explicit endstone::Nullable::operator bool () noexcept const
```




<hr>



### function operator\* 

```C++
inline T & endstone::Nullable::operator* () noexcept const
```




<hr>



### function operator-&gt; 

```C++
inline T * endstone::Nullable::operator-> () noexcept const
```




<hr>



### function operator== 

```C++
inline bool endstone::Nullable::operator== (
    std::nullptr_t
) noexcept const
```




<hr>



### function value 

_Returns a reference to the contained value, throwing_ `std::runtime_error` _if this is null._
```C++
inline T & endstone::Nullable::value () const
```




<hr>



### function value\_or 

_Returns the contained pointer if present, otherwise the supplied default._ 
```C++
inline pointer_type endstone::Nullable::value_or (
    pointer_type default_value
) const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/util/pointers.h`

