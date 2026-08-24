

# Class endstone::ClassInfo



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ClassInfo**](classendstone_1_1ClassInfo.md)



_A type identity that stays comparable across shared library boundaries._ [More...](#detailed-description)

* `#include <endstone/object.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ClassInfo**](#function-classinfo) ([**const**](classendstone_1_1Identifier.md) std::type\_info & info) noexcept<br> |
|  [**const**](classendstone_1_1Identifier.md) std::type\_info & | [**info**](#function-info) () noexcept const<br>_The underlying_ `std::type_info` _._ |
|  [**const**](classendstone_1_1Identifier.md) [**char**](classendstone_1_1Identifier.md) \* | [**name**](#function-name) () noexcept const<br>_The mangled name of the type._  |
|  [**bool**](classendstone_1_1Identifier.md) | [**operator==**](#function-operator) ([**const**](classendstone_1_1Identifier.md) [**ClassInfo**](classendstone_1_1ClassInfo.md) & other) noexcept const<br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**ClassInfo**](classendstone_1_1ClassInfo.md) | [**of**](#function-of) () noexcept<br>_The identity of T._  |


























## Detailed Description


libc++ compares `std::type_info` by address, and a type named from more than one shared library has one typeinfo per module once those modules are built with hidden visibility, as endstone and its plugins are. Comparing the mangled name instead lets a type named in a plugin match the same type named in the runtime. 


    
## Public Functions Documentation




### function ClassInfo 

```C++
inline explicit endstone::ClassInfo::ClassInfo (
    const std::type_info & info
) noexcept
```




<hr>



### function info 

_The underlying_ `std::type_info` _._
```C++
inline const std::type_info & endstone::ClassInfo::info () noexcept const
```




<hr>



### function name 

_The mangled name of the type._ 
```C++
inline const  char * endstone::ClassInfo::name () noexcept const
```




<hr>



### function operator== 

```C++
inline bool endstone::ClassInfo::operator== (
    const  ClassInfo & other
) noexcept const
```




<hr>
## Public Static Functions Documentation




### function of 

_The identity of T._ 
```C++
template<typename  T>
static inline ClassInfo endstone::ClassInfo::of () noexcept
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/object.h`

