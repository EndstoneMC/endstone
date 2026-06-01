

# Class endstone::UUID



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**UUID**](classendstone_1_1UUID.md)



_Implementation of Universally Unique_ [_**Identifier**_](classendstone_1_1Identifier.md) _(_[_**UUID**_](classendstone_1_1UUID.md) _)._[More...](#detailed-description)

* `#include <endstone/util/uuid.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  std::uint8\_t | [**data**](#variable-data)   = `{0}`<br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|  std::uint8\_t \* | [**begin**](#function-begin-12) () noexcept<br>_Returns an iterator to the first byte._  |
|  [**const**](classendstone_1_1Identifier.md) [**uint8\_t**](classendstone_1_1Identifier.md) \* | [**begin**](#function-begin-22) () noexcept const<br>_Returns an iterator to the first byte._  |
|  std::uint8\_t \* | [**end**](#function-end-12) () noexcept<br>_Returns an iterator past the last byte._  |
|  [**const**](classendstone_1_1Identifier.md) [**uint8\_t**](classendstone_1_1Identifier.md) \* | [**end**](#function-end-22) () noexcept const<br>_Returns an iterator past the last byte._  |
|  [**bool**](classendstone_1_1Identifier.md) | [**isNil**](#function-isnil) () noexcept const<br>_Checks whether this_ [_**UUID**_](classendstone_1_1UUID.md) _is nil (all bytes are zero)._ |
|  std::string | [**str**](#function-str) () const<br>_Returns the canonical hyphenated string representation of this_ [_**UUID**_](classendstone_1_1UUID.md) _._ |
|  [**void**](classendstone_1_1Identifier.md) | [**swap**](#function-swap) ([**UUID**](classendstone_1_1UUID.md) & rhs) noexcept<br>_Swaps the contents of this_ [_**UUID**_](classendstone_1_1UUID.md) _with another._ |
|  [**int**](classendstone_1_1Identifier.md) | [**version**](#function-version) () noexcept const<br>_Returns the version number encoded in this_ [_**UUID**_](classendstone_1_1UUID.md) _._ |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**constexpr**](classendstone_1_1Identifier.md) std::size\_t | [**size**](#function-size) () noexcept<br>_Returns the number of bytes in a_ [_**UUID**_](classendstone_1_1UUID.md) _._ |


























## Detailed Description


Adapted from [https://github.com/boostorg/uuid/blob/develop/include/boost/uuid/uuid.hpp](https://github.com/boostorg/uuid/blob/develop/include/boost/uuid/uuid.hpp) 


    
## Public Attributes Documentation




### variable data 

```C++
std::uint8_t endstone::UUID::data[16];
```




<hr>
## Public Functions Documentation




### function begin [1/2]

_Returns an iterator to the first byte._ 
```C++
inline std::uint8_t * endstone::UUID::begin () noexcept
```





**Returns:**

Pointer to the first byte 





        

<hr>



### function begin [2/2]

_Returns an iterator to the first byte._ 
```C++
inline const  uint8_t * endstone::UUID::begin () noexcept const
```





**Returns:**

Pointer to the first byte   





        

<hr>



### function end [1/2]

_Returns an iterator past the last byte._ 
```C++
inline std::uint8_t * endstone::UUID::end () noexcept
```





**Returns:**

Pointer past the last byte 





        

<hr>



### function end [2/2]

_Returns an iterator past the last byte._ 
```C++
inline const  uint8_t * endstone::UUID::end () noexcept const
```





**Returns:**

Pointer past the last byte   





        

<hr>



### function isNil 

_Checks whether this_ [_**UUID**_](classendstone_1_1UUID.md) _is nil (all bytes are zero)._
```C++
inline bool endstone::UUID::isNil () noexcept const
```





**Returns:**

true if every byte is zero, false otherwise 





        

<hr>



### function str 

_Returns the canonical hyphenated string representation of this_ [_**UUID**_](classendstone_1_1UUID.md) _._
```C++
inline std::string endstone::UUID::str () const
```





**Returns:**

The [**UUID**](classendstone_1_1UUID.md) formatted as a 36-character string 





        

<hr>



### function swap 

_Swaps the contents of this_ [_**UUID**_](classendstone_1_1UUID.md) _with another._
```C++
inline void endstone::UUID::swap (
    UUID & rhs
) noexcept
```





**Parameters:**


* `rhs` The [**UUID**](classendstone_1_1UUID.md) to swap with 




        

<hr>



### function version 

_Returns the version number encoded in this_ [_**UUID**_](classendstone_1_1UUID.md) _._
```C++
inline int endstone::UUID::version () noexcept const
```





**Returns:**

The [**UUID**](classendstone_1_1UUID.md) version (1-5), or -1 if it is not a recognized version 





        

<hr>
## Public Static Functions Documentation




### function size 

_Returns the number of bytes in a_ [_**UUID**_](classendstone_1_1UUID.md) _._
```C++
static inline constexpr std::size_t endstone::UUID::size () noexcept
```





**Returns:**

The size in bytes (always 16) 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/util/uuid.h`

