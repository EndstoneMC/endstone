

# Class endstone::NotNull

**template &lt;[**class**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md)&gt;**



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**NotNull**](classendstone_1_1NotNull.md)






















## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::shared\_ptr&lt; [**T**](classendstone_1_1Identifier.md) &gt; | [**pointer\_type**](#typedef-pointer_type)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**NotNull**](#function-notnull-14) () = delete<br> |
|   | [**NotNull**](#function-notnull-24) (std::shared\_ptr&lt; [**T**](classendstone_1_1Identifier.md) &gt; ptr) <br> |
|   | [**NotNull**](#function-notnull-34) ([**const**](classendstone_1_1Identifier.md) [**NotNull**](classendstone_1_1NotNull.md) & other) = default<br> |
|   | [**NotNull**](#function-notnull-44) (std::nullptr\_t) = delete<br> |
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




























## Public Types Documentation




### typedef pointer\_type 

```C++
using endstone::NotNull< T >::pointer_type =  std::shared_ptr<T>;
```




<hr>
## Public Functions Documentation




### function NotNull [1/4]

```C++
endstone::NotNull::NotNull () = delete
```




<hr>



### function NotNull [2/4]

```C++
inline endstone::NotNull::NotNull (
    std::shared_ptr< T > ptr
) 
```




<hr>



### function NotNull [3/4]

```C++
endstone::NotNull::NotNull (
    const  NotNull & other
) = default
```




<hr>



### function NotNull [4/4]

```C++
endstone::NotNull::NotNull (
    std::nullptr_t
) = delete
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

