

# Class endstone::UUID



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**UUID**](classendstone_1_1UUID.md)



_Implementation of Universally Unique Identifier (_ [_**UUID**_](classendstone_1_1UUID.md) _)_[More...](#detailed-description)

* `#include <endstone/util/uuid.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  std::uint8\_t | [**data**](#variable-data)  <br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|  std::uint8\_t \* | [**begin**](#function-begin-12) () noexcept<br> |
|  const uint8\_t \* | [**begin**](#function-begin-22) () noexcept const<br> |
|  std::uint8\_t \* | [**end**](#function-end-12) () noexcept<br> |
|  const uint8\_t \* | [**end**](#function-end-22) () noexcept const<br> |
|  bool | [**isNil**](#function-isnil) () noexcept const<br> |
|  std::string | [**str**](#function-str) () const<br> |
|  void | [**swap**](#function-swap) ([**UUID**](classendstone_1_1UUID.md) & rhs) noexcept<br> |
|  int | [**version**](#function-version) () noexcept const<br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  constexpr std::size\_t | [**size**](#function-size) () noexcept<br> |


























# Detailed Description


Adapted from [https://github.com/boostorg/uuid/blob/develop/include/boost/uuid/uuid.hpp](https://github.com/boostorg/uuid/blob/develop/include/boost/uuid/uuid.hpp) 


    
## Public Attributes Documentation




### variable data 

```C++
std::uint8_t endstone::UUID::data[16];
```




<hr>
## Public Functions Documentation




### function begin [1/2]

```C++
inline std::uint8_t * endstone::UUID::begin () noexcept
```




<hr>



### function begin [2/2]

```C++
inline const uint8_t * endstone::UUID::begin () noexcept const
```




<hr>



### function end [1/2]

```C++
inline std::uint8_t * endstone::UUID::end () noexcept
```




<hr>



### function end [2/2]

```C++
inline const uint8_t * endstone::UUID::end () noexcept const
```




<hr>



### function isNil 

```C++
inline bool endstone::UUID::isNil () noexcept const
```




<hr>



### function str 

```C++
inline std::string endstone::UUID::str () const
```




<hr>



### function swap 

```C++
inline void endstone::UUID::swap (
    UUID & rhs
) noexcept
```




<hr>



### function version 

```C++
inline int endstone::UUID::version () noexcept const
```




<hr>
## Public Static Functions Documentation




### function size 

```C++
static inline constexpr std::size_t endstone::UUID::size () noexcept
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/util/uuid.h`

