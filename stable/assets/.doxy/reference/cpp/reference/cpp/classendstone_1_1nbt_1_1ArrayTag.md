

# Class endstone::nbt::ArrayTag

**template &lt;[**typename**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md)&gt;**



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**nbt**](namespaceendstone_1_1nbt.md) **>** [**ArrayTag**](classendstone_1_1nbt_1_1ArrayTag.md)








Inherits the following classes: [endstone::nbt::TagBase](structendstone_1_1nbt_1_1TagBase.md)














## Public Types

| Type | Name |
| ---: | :--- |
| typedef [**typename**](classendstone_1_1Identifier.md) storage\_type::const\_iterator | [**const\_iterator**](#typedef-const_iterator)  <br> |
| typedef [**typename**](classendstone_1_1Identifier.md) storage\_type::difference\_type | [**difference\_type**](#typedef-difference_type)  <br> |
| typedef [**typename**](classendstone_1_1Identifier.md) storage\_type::iterator | [**iterator**](#typedef-iterator)  <br> |
| typedef std::size\_t | [**size\_type**](#typedef-size_type)  <br> |
| typedef std::vector&lt; [**value\_type**](classendstone_1_1Identifier.md) &gt; | [**storage\_type**](#typedef-storage_type)  <br> |
| typedef [**T**](classendstone_1_1Identifier.md) | [**value\_type**](#typedef-value_type)  <br> |








































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ArrayTag**](#function-arraytag-14) () = default<br> |
|   | [**ArrayTag**](#function-arraytag-24) (storage\_type v) <br> |
|   | [**ArrayTag**](#function-arraytag-34) ([**It**](classendstone_1_1Identifier.md) first, [**It**](classendstone_1_1Identifier.md) last) <br> |
|   | [**ArrayTag**](#function-arraytag-44) (std::initializer\_list&lt; [**value\_type**](classendstone_1_1Identifier.md) &gt; init) <br> |
|  [**void**](classendstone_1_1Identifier.md) | [**assign**](#function-assign) ([**It**](classendstone_1_1Identifier.md) first, [**It**](classendstone_1_1Identifier.md) last) <br> |
|  [**value\_type**](classendstone_1_1Identifier.md) & | [**at**](#function-at-12) (size\_type i) <br> |
|  [**const**](classendstone_1_1Identifier.md) [**value\_type**](classendstone_1_1Identifier.md) & | [**at**](#function-at-22) (size\_type i) const<br> |
|  iterator | [**begin**](#function-begin-12) () noexcept<br> |
|  const\_iterator | [**begin**](#function-begin-22) () noexcept const<br> |
|  const\_iterator | [**cbegin**](#function-cbegin) () noexcept const<br> |
|  const\_iterator | [**cend**](#function-cend) () noexcept const<br> |
|  [**void**](classendstone_1_1Identifier.md) | [**clear**](#function-clear) () noexcept<br> |
|  [**value\_type**](classendstone_1_1Identifier.md) \* | [**data**](#function-data-12) () noexcept<br> |
|  [**const**](classendstone_1_1Identifier.md) [**value\_type**](classendstone_1_1Identifier.md) \* | [**data**](#function-data-22) () noexcept const<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**empty**](#function-empty) () noexcept const<br> |
|  iterator | [**end**](#function-end-12) () noexcept<br> |
|  const\_iterator | [**end**](#function-end-22) () noexcept const<br> |
|  iterator | [**erase**](#function-erase) (const\_iterator pos) <br> |
|  iterator | [**insert**](#function-insert) (const\_iterator pos, [**value\_type**](classendstone_1_1Identifier.md) v) <br> |
|  [**value\_type**](classendstone_1_1Identifier.md) & | [**operator[]**](#function-operator) (size\_type i) <br> |
|  [**const**](classendstone_1_1Identifier.md) [**value\_type**](classendstone_1_1Identifier.md) & | [**operator[]**](#function-operator_1) (size\_type i) const<br> |
|  [**void**](classendstone_1_1Identifier.md) | [**push\_back**](#function-push_back) ([**value\_type**](classendstone_1_1Identifier.md) v) <br> |
|  size\_type | [**size**](#function-size) () noexcept const<br> |
























































## Public Types Documentation




### typedef const\_iterator 

```C++
using endstone::nbt::ArrayTag< T >::const_iterator =  typename storage_type::const_iterator;
```




<hr>



### typedef difference\_type 

```C++
using endstone::nbt::ArrayTag< T >::difference_type =  typename storage_type::difference_type;
```




<hr>



### typedef iterator 

```C++
using endstone::nbt::ArrayTag< T >::iterator =  typename storage_type::iterator;
```




<hr>



### typedef size\_type 

```C++
using endstone::nbt::ArrayTag< T >::size_type =  std::size_t;
```




<hr>



### typedef storage\_type 

```C++
using endstone::nbt::ArrayTag< T >::storage_type =  std::vector<value_type>;
```




<hr>



### typedef value\_type 

```C++
using endstone::nbt::ArrayTag< T >::value_type =  T;
```




<hr>
## Public Functions Documentation




### function ArrayTag [1/4]

```C++
endstone::nbt::ArrayTag::ArrayTag () = default
```




<hr>



### function ArrayTag [2/4]

```C++
inline explicit endstone::nbt::ArrayTag::ArrayTag (
    storage_type v
) 
```




<hr>



### function ArrayTag [3/4]

```C++
template<class  It>
inline endstone::nbt::ArrayTag::ArrayTag (
    It first,
    It last
) 
```




<hr>



### function ArrayTag [4/4]

```C++
inline endstone::nbt::ArrayTag::ArrayTag (
    std::initializer_list< value_type > init
) 
```




<hr>



### function assign 

```C++
template<class  It>
inline void endstone::nbt::ArrayTag::assign (
    It first,
    It last
) 
```




<hr>



### function at [1/2]

```C++
inline value_type & endstone::nbt::ArrayTag::at (
    size_type i
) 
```




<hr>



### function at [2/2]

```C++
inline const  value_type & endstone::nbt::ArrayTag::at (
    size_type i
) const
```




<hr>



### function begin [1/2]

```C++
inline iterator endstone::nbt::ArrayTag::begin () noexcept
```




<hr>



### function begin [2/2]

```C++
inline const_iterator endstone::nbt::ArrayTag::begin () noexcept const
```




<hr>



### function cbegin 

```C++
inline const_iterator endstone::nbt::ArrayTag::cbegin () noexcept const
```




<hr>



### function cend 

```C++
inline const_iterator endstone::nbt::ArrayTag::cend () noexcept const
```




<hr>



### function clear 

```C++
inline void endstone::nbt::ArrayTag::clear () noexcept
```




<hr>



### function data [1/2]

```C++
inline value_type * endstone::nbt::ArrayTag::data () noexcept
```




<hr>



### function data [2/2]

```C++
inline const  value_type * endstone::nbt::ArrayTag::data () noexcept const
```




<hr>



### function empty 

```C++
inline bool endstone::nbt::ArrayTag::empty () noexcept const
```




<hr>



### function end [1/2]

```C++
inline iterator endstone::nbt::ArrayTag::end () noexcept
```




<hr>



### function end [2/2]

```C++
inline const_iterator endstone::nbt::ArrayTag::end () noexcept const
```




<hr>



### function erase 

```C++
inline iterator endstone::nbt::ArrayTag::erase (
    const_iterator pos
) 
```




<hr>



### function insert 

```C++
inline iterator endstone::nbt::ArrayTag::insert (
    const_iterator pos,
    value_type v
) 
```




<hr>



### function operator[] 

```C++
inline value_type & endstone::nbt::ArrayTag::operator[] (
    size_type i
) 
```




<hr>



### function operator[] 

```C++
inline const  value_type & endstone::nbt::ArrayTag::operator[] (
    size_type i
) const
```




<hr>



### function push\_back 

```C++
inline void endstone::nbt::ArrayTag::push_back (
    value_type v
) 
```




<hr>



### function size 

```C++
inline size_type endstone::nbt::ArrayTag::size () noexcept const
```




<hr>## Friends Documentation





### friend operator!= 

```C++
inline bool endstone::nbt::ArrayTag::operator!= (
    const  ArrayTag & a,
    const  ArrayTag & b
) noexcept
```




<hr>



### friend operator!= 

```C++
inline bool endstone::nbt::ArrayTag::operator!= (
    const  ArrayTag & a,
    const storage_type & b
) noexcept
```




<hr>



### friend operator!= 

```C++
inline bool endstone::nbt::ArrayTag::operator!= (
    const storage_type & a,
    const  ArrayTag & b
) noexcept
```




<hr>



### friend operator== 

```C++
inline bool endstone::nbt::ArrayTag::operator== (
    const  ArrayTag & a,
    const  ArrayTag & b
) noexcept
```




<hr>



### friend operator== 

```C++
inline bool endstone::nbt::ArrayTag::operator== (
    const  ArrayTag & a,
    const storage_type & b
) noexcept
```




<hr>



### friend operator== 

```C++
inline bool endstone::nbt::ArrayTag::operator== (
    const storage_type & a,
    const  ArrayTag & b
) noexcept
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/nbt/array.h`

