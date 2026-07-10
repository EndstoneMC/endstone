

# Struct endstone::detail::JoinView

**template &lt;[**typename**](classendstone_1_1Identifier.md) [**It**](classendstone_1_1Identifier.md), [**typename**](classendstone_1_1Identifier.md) [**Sentinel**](classendstone_1_1Identifier.md)&gt;**



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**detail**](namespaceendstone_1_1detail.md) **>** [**JoinView**](structendstone_1_1detail_1_1JoinView.md)



[More...](#detailed-description)

* `#include <endstone/util/format.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  [**It**](classendstone_1_1Identifier.md) | [**begin**](#variable-begin)  <br> |
|  [**Sentinel**](classendstone_1_1Identifier.md) | [**end**](#variable-end)  <br> |
|  std::string\_view | [**sep**](#variable-sep)  <br> |












































## Detailed Description


A lightweight, formattable view over a range that renders its elements separated by a delimiter.


This is a drop-in replacement for `fmt::join`, which has no equivalent in `std::format` (C++20). It allocates nothing of its own: elements are written straight to the output iterator. 


    
## Public Attributes Documentation




### variable begin 

```C++
It endstone::detail::JoinView< It, Sentinel >::begin;
```




<hr>



### variable end 

```C++
Sentinel endstone::detail::JoinView< It, Sentinel >::end;
```




<hr>



### variable sep 

```C++
std::string_view endstone::detail::JoinView< It, Sentinel >::sep;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/util/format.h`

