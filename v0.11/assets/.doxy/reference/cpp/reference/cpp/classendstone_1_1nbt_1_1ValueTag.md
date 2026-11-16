

# Class endstone::nbt::ValueTag

**template &lt;typename T&gt;**



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**nbt**](namespaceendstone_1_1nbt.md) **>** [**ValueTag**](classendstone_1_1nbt_1_1ValueTag.md)








Inherits the following classes: [endstone::nbt::TagBase](structendstone_1_1nbt_1_1TagBase.md)














## Public Types

| Type | Name |
| ---: | :--- |
| typedef T | [**value\_type**](#typedef-value_type)  <br> |








































## Public Functions

| Type | Name |
| ---: | :--- |
|  constexpr | [**ValueTag**](#function-valuetag-13) () = default<br> |
|  constexpr | [**ValueTag**](#function-valuetag-23) (const T & v) <br> |
|  constexpr | [**ValueTag**](#function-valuetag-33) (T && v) <br> |
|   | [**operator const T &**](#function-operator-const-t-&) () noexcept const<br> |
|  const T & | [**value**](#function-value) () noexcept const<br> |
























































## Public Types Documentation




### typedef value\_type 

```C++
using endstone::nbt::ValueTag< T >::value_type =  T;
```




<hr>
## Public Functions Documentation




### function ValueTag [1/3]

```C++
constexpr endstone::nbt::ValueTag::ValueTag () = default
```




<hr>



### function ValueTag [2/3]

```C++
inline explicit constexpr endstone::nbt::ValueTag::ValueTag (
    const T & v
) 
```




<hr>



### function ValueTag [3/3]

```C++
inline explicit constexpr endstone::nbt::ValueTag::ValueTag (
    T && v
) 
```




<hr>



### function operator const T & 

```C++
inline endstone::nbt::ValueTag::operator const T & () noexcept const
```




<hr>



### function value 

```C++
inline const T & endstone::nbt::ValueTag::value () noexcept const
```




<hr>## Friends Documentation





### friend operator!= 

```C++
inline bool endstone::nbt::ValueTag::operator!= (
    const ValueTag & a,
    const ValueTag & b
) noexcept
```




<hr>



### friend operator!= 

```C++
inline bool endstone::nbt::ValueTag::operator!= (
    const ValueTag & a,
    const T & b
) noexcept
```




<hr>



### friend operator!= 

```C++
inline bool endstone::nbt::ValueTag::operator!= (
    const T & a,
    const ValueTag & b
) noexcept
```




<hr>



### friend operator== 

```C++
inline bool endstone::nbt::ValueTag::operator== (
    const ValueTag & a,
    const ValueTag & b
) noexcept
```




<hr>



### friend operator== 

```C++
inline bool endstone::nbt::ValueTag::operator== (
    const ValueTag & a,
    const T & b
) noexcept
```




<hr>



### friend operator== 

```C++
inline bool endstone::nbt::ValueTag::operator== (
    const T & a,
    const ValueTag & b
) noexcept
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/nbt/value.h`

