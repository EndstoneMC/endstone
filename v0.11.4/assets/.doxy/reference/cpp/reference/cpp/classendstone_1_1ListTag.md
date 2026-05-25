

# Class endstone::ListTag



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ListTag**](classendstone_1_1ListTag.md)








Inherits the following classes: [endstone::nbt::TagBase](structendstone_1_1nbt_1_1TagBase.md)














## Public Types

| Type | Name |
| ---: | :--- |
| typedef container\_type::const\_iterator | [**const\_iterator**](#typedef-const_iterator)  <br> |
| typedef std::vector&lt; [**value\_type**](classendstone_1_1nbt_1_1Tag.md) &gt; | [**container\_type**](#typedef-container_type)  <br> |
| typedef container\_type::iterator | [**iterator**](#typedef-iterator)  <br> |
| typedef std::size\_t | [**size\_type**](#typedef-size_type)  <br> |
| typedef [**nbt::Tag**](classendstone_1_1nbt_1_1Tag.md) | [**value\_type**](#typedef-value_type)  <br> |








































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ListTag**](#function-listtag-12) () = default<br> |
|   | [**ListTag**](#function-listtag-22) (std::initializer\_list&lt; [**T**](classendstone_1_1Identifier.md) &gt; init) <br> |
|  [**value\_type**](classendstone_1_1nbt_1_1Tag.md) & | [**at**](#function-at-12) (size\_type i) <br> |
|  [**const**](classendstone_1_1Identifier.md) [**value\_type**](classendstone_1_1nbt_1_1Tag.md) & | [**at**](#function-at-22) (size\_type i) const<br> |
|  iterator | [**begin**](#function-begin-12) () noexcept<br> |
|  const\_iterator | [**begin**](#function-begin-22) () noexcept const<br> |
|  const\_iterator | [**cbegin**](#function-cbegin) () noexcept const<br> |
|  const\_iterator | [**cend**](#function-cend) () noexcept const<br> |
|  [**void**](classendstone_1_1Identifier.md) | [**clear**](#function-clear) () noexcept<br> |
|  [**value\_type**](classendstone_1_1nbt_1_1Tag.md) & | [**emplace\_back**](#function-emplace_back) ([**Args**](classendstone_1_1Identifier.md) &&... args) <br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**empty**](#function-empty) () noexcept const<br> |
|  iterator | [**end**](#function-end-12) () noexcept<br> |
|  const\_iterator | [**end**](#function-end-22) () noexcept const<br> |
|  iterator | [**erase**](#function-erase-12) (const\_iterator pos) <br> |
|  iterator | [**erase**](#function-erase-22) (const\_iterator first, const\_iterator last) <br> |
|  [**value\_type**](classendstone_1_1nbt_1_1Tag.md) & | [**operator[]**](#function-operator) (size\_type i) <br> |
|  [**const**](classendstone_1_1Identifier.md) [**value\_type**](classendstone_1_1nbt_1_1Tag.md) & | [**operator[]**](#function-operator_1) (size\_type i) const<br> |
|  [**void**](classendstone_1_1Identifier.md) | [**push\_back**](#function-push_back-12) ([**const**](classendstone_1_1Identifier.md) [**value\_type**](classendstone_1_1nbt_1_1Tag.md) & v) <br> |
|  [**void**](classendstone_1_1Identifier.md) | [**push\_back**](#function-push_back-22) ([**value\_type**](classendstone_1_1nbt_1_1Tag.md) && v) <br> |
|  size\_type | [**size**](#function-size) () noexcept const<br> |
|  nbt::Type | [**type**](#function-type) () noexcept const<br> |
























































## Public Types Documentation




### typedef const\_iterator 

```C++
using endstone::ListTag::const_iterator =  container_type::const_iterator;
```




<hr>



### typedef container\_type 

```C++
using endstone::ListTag::container_type =  std::vector<value_type>;
```




<hr>



### typedef iterator 

```C++
using endstone::ListTag::iterator =  container_type::iterator;
```




<hr>



### typedef size\_type 

```C++
using endstone::ListTag::size_type =  std::size_t;
```




<hr>



### typedef value\_type 

```C++
using endstone::ListTag::value_type =  nbt::Tag;
```




<hr>
## Public Functions Documentation




### function ListTag [1/2]

```C++
endstone::ListTag::ListTag () = default
```




<hr>



### function ListTag [2/2]

```C++
template<typename  T>
endstone::ListTag::ListTag (
    std::initializer_list< T > init
) 
```




<hr>



### function at [1/2]

```C++
inline value_type & endstone::ListTag::at (
    size_type i
) 
```




<hr>



### function at [2/2]

```C++
inline const  value_type & endstone::ListTag::at (
    size_type i
) const
```




<hr>



### function begin [1/2]

```C++
inline iterator endstone::ListTag::begin () noexcept
```




<hr>



### function begin [2/2]

```C++
inline const_iterator endstone::ListTag::begin () noexcept const
```




<hr>



### function cbegin 

```C++
inline const_iterator endstone::ListTag::cbegin () noexcept const
```




<hr>



### function cend 

```C++
inline const_iterator endstone::ListTag::cend () noexcept const
```




<hr>



### function clear 

```C++
inline void endstone::ListTag::clear () noexcept
```




<hr>



### function emplace\_back 

```C++
template<class... Args>
value_type & endstone::ListTag::emplace_back (
    Args &&... args
) 
```




<hr>



### function empty 

```C++
inline bool endstone::ListTag::empty () noexcept const
```




<hr>



### function end [1/2]

```C++
inline iterator endstone::ListTag::end () noexcept
```




<hr>



### function end [2/2]

```C++
inline const_iterator endstone::ListTag::end () noexcept const
```




<hr>



### function erase [1/2]

```C++
inline iterator endstone::ListTag::erase (
    const_iterator pos
) 
```




<hr>



### function erase [2/2]

```C++
inline iterator endstone::ListTag::erase (
    const_iterator first,
    const_iterator last
) 
```




<hr>



### function operator[] 

```C++
inline value_type & endstone::ListTag::operator[] (
    size_type i
) 
```




<hr>



### function operator[] 

```C++
inline const  value_type & endstone::ListTag::operator[] (
    size_type i
) const
```




<hr>



### function push\_back [1/2]

```C++
void endstone::ListTag::push_back (
    const  value_type & v
) 
```




<hr>



### function push\_back [2/2]

```C++
void endstone::ListTag::push_back (
    value_type && v
) 
```




<hr>



### function size 

```C++
inline size_type endstone::ListTag::size () noexcept const
```




<hr>



### function type 

```C++
inline nbt::Type endstone::ListTag::type () noexcept const
```




<hr>## Friends Documentation





### friend operator!= 

```C++
bool endstone::ListTag::operator!= (
    const  ListTag & a,
    const  ListTag & b
) noexcept
```




<hr>



### friend operator== 

```C++
bool endstone::ListTag::operator== (
    const  ListTag & a,
    const  ListTag & b
) noexcept
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/nbt/list.h`

