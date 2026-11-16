

# Class endstone::Nullable

**template &lt;class T&gt;**



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Nullable**](classendstone_1_1Nullable.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|  constexpr | [**Nullable**](#function-nullable-14) () noexcept<br> |
|  constexpr | [**Nullable**](#function-nullable-24) (std::nullptr\_t) noexcept<br> |
|   | [**Nullable**](#function-nullable-34) (std::shared\_ptr&lt; T &gt; ptr) <br> |
|   | [**Nullable**](#function-nullable-44) (const [**NotNull**](classendstone_1_1NotNull.md)&lt; T &gt; & other) <br> |
|  const std::shared\_ptr&lt; T &gt; & | [**get**](#function-get) () noexcept const<br> |
|   | [**operator bool**](#function-operator-bool) () noexcept const<br> |
|  bool | [**operator!=**](#function-operator) (const Nullable &) const<br> |
|  bool | [**operator!=**](#function-operator_1) (std::nullptr\_t) noexcept const<br> |
|  T & | [**operator\***](#function-operator_2) () noexcept const<br> |
|  T \* | [**operator-&gt;**](#function-operator-) () noexcept const<br> |
|  bool | [**operator==**](#function-operator_3) (const Nullable &) const<br> |
|  bool | [**operator==**](#function-operator_4) (std::nullptr\_t) noexcept const<br> |




























## Public Functions Documentation




### function Nullable [1/4]

```C++
constexpr endstone::Nullable::Nullable () noexcept
```




<hr>



### function Nullable [2/4]

```C++
inline constexpr endstone::Nullable::Nullable (
    std::nullptr_t
) noexcept
```




<hr>



### function Nullable [3/4]

```C++
inline endstone::Nullable::Nullable (
    std::shared_ptr< T > ptr
) 
```




<hr>



### function Nullable [4/4]

```C++
inline endstone::Nullable::Nullable (
    const NotNull < T > & other
) 
```




<hr>



### function get 

```C++
inline const std::shared_ptr< T > & endstone::Nullable::get () noexcept const
```




<hr>



### function operator bool 

```C++
inline explicit endstone::Nullable::operator bool () noexcept const
```




<hr>



### function operator!= 

```C++
bool endstone::Nullable::operator!= (
    const Nullable &
) const
```




<hr>



### function operator!= 

```C++
inline bool endstone::Nullable::operator!= (
    std::nullptr_t
) noexcept const
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
bool endstone::Nullable::operator== (
    const Nullable &
) const
```




<hr>



### function operator== 

```C++
inline bool endstone::Nullable::operator== (
    std::nullptr_t
) noexcept const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/util/pointers.h`

