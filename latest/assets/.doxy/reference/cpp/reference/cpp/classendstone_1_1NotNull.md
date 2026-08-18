

# Class endstone::NotNull

**template &lt;[**class**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md)&gt;**



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**NotNull**](classendstone_1_1NotNull.md)



_A wrapper around a std::shared\_ptr that documents that it is never null._ [More...](#detailed-description)

* `#include <endstone/util/pointers.h>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef [**T**](classendstone_1_1Identifier.md) | [**element\_type**](#typedef-element_type)  <br> |
| typedef std::shared\_ptr&lt; [**T**](classendstone_1_1Identifier.md) &gt; | [**pointer\_type**](#typedef-pointer_type)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**NotNull**](#function-notnull-17) () = delete<br> |
|   | [**NotNull**](#function-notnull-27) (std::shared\_ptr&lt; [**T**](classendstone_1_1Identifier.md) &gt; ptr) <br> |
|   | [**NotNull**](#function-notnull-37) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md) & other) = default<br> |
|   | [**NotNull**](#function-notnull-47) (std::shared\_ptr&lt; [**U**](classendstone_1_1Identifier.md) &gt; ptr) <br>_Covariant conversion from a shared\_ptr of a convertible (e.g._  |
|   | [**NotNull**](#function-notnull-57) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md)&lt; [**U**](classendstone_1_1Identifier.md) &gt; & other) <br>_Covariant conversion from a_ [_**NotNull**_](classendstone_1_1NotNull.md) _of a convertible (e.g._ |
|   | [**NotNull**](#function-notnull-67) ([**const**](classendstone_1_1Identifier.md) [**Nullable**](classendstone_1_1Nullable.md)&lt; [**T**](classendstone_1_1Identifier.md) &gt; & other) <br>_Narrowing conversion from a_ [_**Nullable**_](classendstone_1_1Nullable.md) _._ |
|   | [**NotNull**](#function-notnull-77) (std::nullptr\_t) = delete<br> |
|  [**NotNull**](classendstone_1_1NotNull.md)&lt; [**U**](classendstone_1_1Identifier.md) &gt; | [**cast**](#function-cast) () const<br>_Statically downcasts to a related type, keeping the shared ownership._  |
|  [**const**](classendstone_1_1Identifier.md) pointer\_type & | [**get**](#function-get) () noexcept const<br> |
|  [**T**](classendstone_1_1Identifier.md) & | [**operator\***](#function-operator) () noexcept const<br> |
|  [**NotNull**](classendstone_1_1NotNull.md) & | [**operator++**](#function-operator_1) () = delete<br> |
|  [**NotNull**](classendstone_1_1NotNull.md) | [**operator++**](#function-operator_2) ([**int**](classendstone_1_1Identifier.md)) = delete<br> |
|  [**NotNull**](classendstone_1_1NotNull.md) & | [**operator+=**](#function-operator_3) (std::ptrdiff\_t) = delete<br> |
|  [**NotNull**](classendstone_1_1NotNull.md) & | [**operator--**](#function-operator-) () = delete<br> |
|  [**NotNull**](classendstone_1_1NotNull.md) | [**operator--**](#function-operator-_1) ([**int**](classendstone_1_1Identifier.md)) = delete<br> |
|  [**NotNull**](classendstone_1_1NotNull.md) & | [**operator-=**](#function-operator-) (std::ptrdiff\_t) = delete<br> |
|  [**T**](classendstone_1_1Identifier.md) \* | [**operator-&gt;**](#function-operator-_1) () noexcept const<br> |
|  [**NotNull**](classendstone_1_1NotNull.md) & | [**operator=**](#function-operator_4) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md) & other) = default<br> |
|  [**NotNull**](classendstone_1_1NotNull.md) & | [**operator=**](#function-operator_5) (std::nullptr\_t) = delete<br> |
|  [**void**](classendstone_1_1Identifier.md) | [**operator[]**](#function-operator_6) (std::ptrdiff\_t) const<br> |
|  [**void**](classendstone_1_1Identifier.md) | [**swap**](#function-swap) ([**NotNull**](classendstone_1_1NotNull.md) & other) noexcept<br> |




























## Detailed Description


This carries the same weight as Java's `@ NotNull`: it states the contract and costs nothing at run time. Passing `nullptr` is a compile error, as are the pointer-mutating operators, but a null `shared_ptr` handed over at run time is not diagnosed. Validate at the boundary where the pointer enters the API, not here. 


    
## Public Types Documentation




### typedef element\_type 

```C++
using endstone::NotNull< T >::element_type =  T;
```




<hr>



### typedef pointer\_type 

```C++
using endstone::NotNull< T >::pointer_type =  std::shared_ptr<T>;
```




<hr>
## Public Functions Documentation




### function NotNull [1/7]

```C++
endstone::NotNull::NotNull () = delete
```




<hr>



### function NotNull [2/7]

```C++
inline endstone::NotNull::NotNull (
    std::shared_ptr< T > ptr
) 
```




<hr>



### function NotNull [3/7]

```C++
endstone::NotNull::NotNull (
    const  NotNull & other
) = default
```




<hr>



### function NotNull [4/7]

_Covariant conversion from a shared\_ptr of a convertible (e.g._ 
```C++
template<class  U>
inline endstone::NotNull::NotNull (
    std::shared_ptr< U > ptr
) 
```



derived) type. 


        

<hr>



### function NotNull [5/7]

_Covariant conversion from a_ [_**NotNull**_](classendstone_1_1NotNull.md) _of a convertible (e.g._
```C++
template<class  U>
inline endstone::NotNull::NotNull (
    const  NotNull < U > & other
) 
```



derived) type. 


        

<hr>



### function NotNull [6/7]

_Narrowing conversion from a_ [_**Nullable**_](classendstone_1_1Nullable.md) _._
```C++
endstone::NotNull::NotNull (
    const  Nullable < T > & other
) 
```




<hr>



### function NotNull [7/7]

```C++
endstone::NotNull::NotNull (
    std::nullptr_t
) = delete
```




<hr>



### function cast 

_Statically downcasts to a related type, keeping the shared ownership._ 
```C++
template<class  U>
inline NotNull < U > endstone::NotNull::cast () const
```




<hr>



### function get 

```C++
inline const pointer_type & endstone::NotNull::get () noexcept const
```




<hr>



### function operator\* 

```C++
inline T & endstone::NotNull::operator* () noexcept const
```




<hr>



### function operator++ 

```C++
NotNull & endstone::NotNull::operator++ () = delete
```




<hr>



### function operator++ 

```C++
NotNull endstone::NotNull::operator++ (
    int
) = delete
```




<hr>



### function operator+= 

```C++
NotNull & endstone::NotNull::operator+= (
    std::ptrdiff_t
) = delete
```




<hr>



### function operator-- 

```C++
NotNull & endstone::NotNull::operator-- () = delete
```




<hr>



### function operator-- 

```C++
NotNull endstone::NotNull::operator-- (
    int
) = delete
```




<hr>



### function operator-= 

```C++
NotNull & endstone::NotNull::operator-= (
    std::ptrdiff_t
) = delete
```




<hr>



### function operator-&gt; 

```C++
inline T * endstone::NotNull::operator-> () noexcept const
```




<hr>



### function operator= 

```C++
NotNull & endstone::NotNull::operator= (
    const  NotNull & other
) = default
```




<hr>



### function operator= 

```C++
NotNull & endstone::NotNull::operator= (
    std::nullptr_t
) = delete
```




<hr>



### function operator[] 

```C++
void endstone::NotNull::operator[] (
    std::ptrdiff_t
) const
```




<hr>



### function swap 

```C++
inline void endstone::NotNull::swap (
    NotNull & other
) noexcept
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/util/pointers.h`

