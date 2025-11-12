

# Class endstone::CompoundTag



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**CompoundTag**](classendstone_1_1CompoundTag.md)








Inherits the following classes: [endstone::nbt::TagBase](structendstone_1_1nbt_1_1TagBase.md)














## Public Types

| Type | Name |
| ---: | :--- |
| typedef map\_type::const\_iterator | [**const\_iterator**](#typedef-const_iterator)  <br> |
| typedef map\_type::iterator | [**iterator**](#typedef-iterator)  <br> |
| typedef std::string | [**key\_type**](#typedef-key_type)  <br> |
| typedef std::map&lt; key\_type, [**value\_type**](classendstone_1_1nbt_1_1Tag.md) &gt; | [**map\_type**](#typedef-map_type)  <br> |
| typedef std::size\_t | [**size\_type**](#typedef-size_type)  <br> |
| typedef [**nbt::Tag**](classendstone_1_1nbt_1_1Tag.md) | [**value\_type**](#typedef-value_type)  <br> |








































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**CompoundTag**](#function-compoundtag-12) () = default<br> |
|   | [**CompoundTag**](#function-compoundtag-22) (std::initializer\_list&lt; std::pair&lt; key\_type, [**value\_type**](classendstone_1_1nbt_1_1Tag.md) &gt; &gt; init) <br> |
|  [**value\_type**](classendstone_1_1nbt_1_1Tag.md) & | [**at**](#function-at-12) ([**const**](classendstone_1_1Identifier.md) key\_type & key) <br> |
|  [**const**](classendstone_1_1Identifier.md) [**value\_type**](classendstone_1_1nbt_1_1Tag.md) & | [**at**](#function-at-22) (key\_type key) const<br> |
|  iterator | [**begin**](#function-begin-12) () noexcept<br> |
|  const\_iterator | [**begin**](#function-begin-22) () noexcept const<br> |
|  const\_iterator | [**cbegin**](#function-cbegin) () noexcept const<br> |
|  const\_iterator | [**cend**](#function-cend) () noexcept const<br> |
|  [**void**](classendstone_1_1Identifier.md) | [**clear**](#function-clear) () noexcept<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**contains**](#function-contains) ([**const**](classendstone_1_1Identifier.md) key\_type & key) noexcept const<br> |
|  std::pair&lt; iterator, [**bool**](classendstone_1_1Identifier.md) &gt; | [**emplace**](#function-emplace) ([**Args**](classendstone_1_1Identifier.md) &&... args) <br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**empty**](#function-empty) () noexcept const<br> |
|  iterator | [**end**](#function-end-12) () noexcept<br> |
|  const\_iterator | [**end**](#function-end-22) () noexcept const<br> |
|  iterator | [**erase**](#function-erase-13) (const\_iterator pos) <br> |
|  size\_type | [**erase**](#function-erase-23) ([**const**](classendstone_1_1Identifier.md) key\_type & key) <br> |
|  iterator | [**erase**](#function-erase-33) (const\_iterator first, const\_iterator last) <br> |
|  std::pair&lt; iterator, [**bool**](classendstone_1_1Identifier.md) &gt; | [**insert**](#function-insert) ([**P**](classendstone_1_1Identifier.md) && v) <br> |
|  std::pair&lt; iterator, [**bool**](classendstone_1_1Identifier.md) &gt; | [**insert\_or\_assign**](#function-insert_or_assign) ([**const**](classendstone_1_1Identifier.md) key\_type & key, [**M**](classendstone_1_1Identifier.md) && obj) <br> |
|  [**void**](classendstone_1_1Identifier.md) | [**merge**](#function-merge-12) ([**CompoundTag**](classendstone_1_1CompoundTag.md) & source) <br> |
|  [**void**](classendstone_1_1Identifier.md) | [**merge**](#function-merge-22) ([**CompoundTag**](classendstone_1_1CompoundTag.md) && source) <br> |
|  [**value\_type**](classendstone_1_1nbt_1_1Tag.md) & | [**operator[]**](#function-operator) ([**const**](classendstone_1_1Identifier.md) key\_type & key) <br> |
|  size\_type | [**size**](#function-size) () noexcept const<br> |
|  [**void**](classendstone_1_1Identifier.md) | [**swap**](#function-swap) ([**CompoundTag**](classendstone_1_1CompoundTag.md) & other) noexcept<br> |
|  std::pair&lt; iterator, [**bool**](classendstone_1_1Identifier.md) &gt; | [**try\_emplace**](#function-try_emplace) ([**const**](classendstone_1_1Identifier.md) key\_type & key, [**Args**](classendstone_1_1Identifier.md) &&... args) <br> |
























































## Public Types Documentation




### typedef const\_iterator 

```C++
using endstone::CompoundTag::const_iterator =  map_type::const_iterator;
```




<hr>



### typedef iterator 

```C++
using endstone::CompoundTag::iterator =  map_type::iterator;
```




<hr>



### typedef key\_type 

```C++
using endstone::CompoundTag::key_type =  std::string;
```




<hr>



### typedef map\_type 

```C++
using endstone::CompoundTag::map_type =  std::map<key_type, value_type>;
```




<hr>



### typedef size\_type 

```C++
using endstone::CompoundTag::size_type =  std::size_t;
```




<hr>



### typedef value\_type 

```C++
using endstone::CompoundTag::value_type =  nbt::Tag;
```




<hr>
## Public Functions Documentation




### function CompoundTag [1/2]

```C++
endstone::CompoundTag::CompoundTag () = default
```




<hr>



### function CompoundTag [2/2]

```C++
endstone::CompoundTag::CompoundTag (
    std::initializer_list< std::pair< key_type, value_type > > init
) 
```




<hr>



### function at [1/2]

```C++
inline value_type & endstone::CompoundTag::at (
    const key_type & key
) 
```




<hr>



### function at [2/2]

```C++
inline const  value_type & endstone::CompoundTag::at (
    key_type key
) const
```




<hr>



### function begin [1/2]

```C++
inline iterator endstone::CompoundTag::begin () noexcept
```




<hr>



### function begin [2/2]

```C++
inline const_iterator endstone::CompoundTag::begin () noexcept const
```




<hr>



### function cbegin 

```C++
inline const_iterator endstone::CompoundTag::cbegin () noexcept const
```




<hr>



### function cend 

```C++
inline const_iterator endstone::CompoundTag::cend () noexcept const
```




<hr>



### function clear 

```C++
inline void endstone::CompoundTag::clear () noexcept
```




<hr>



### function contains 

```C++
inline bool endstone::CompoundTag::contains (
    const key_type & key
) noexcept const
```




<hr>



### function emplace 

```C++
template<class... Args>
std::pair< iterator, bool > endstone::CompoundTag::emplace (
    Args &&... args
) 
```




<hr>



### function empty 

```C++
inline bool endstone::CompoundTag::empty () noexcept const
```




<hr>



### function end [1/2]

```C++
inline iterator endstone::CompoundTag::end () noexcept
```




<hr>



### function end [2/2]

```C++
inline const_iterator endstone::CompoundTag::end () noexcept const
```




<hr>



### function erase [1/3]

```C++
inline iterator endstone::CompoundTag::erase (
    const_iterator pos
) 
```




<hr>



### function erase [2/3]

```C++
inline size_type endstone::CompoundTag::erase (
    const key_type & key
) 
```




<hr>



### function erase [3/3]

```C++
inline iterator endstone::CompoundTag::erase (
    const_iterator first,
    const_iterator last
) 
```




<hr>



### function insert 

```C++
template<class  P>
std::pair< iterator, bool > endstone::CompoundTag::insert (
    P && v
) 
```




<hr>



### function insert\_or\_assign 

```C++
template<class  M>
std::pair< iterator, bool > endstone::CompoundTag::insert_or_assign (
    const key_type & key,
    M && obj
) 
```




<hr>



### function merge [1/2]

```C++
inline void endstone::CompoundTag::merge (
    CompoundTag & source
) 
```




<hr>



### function merge [2/2]

```C++
inline void endstone::CompoundTag::merge (
    CompoundTag && source
) 
```




<hr>



### function operator[] 

```C++
inline value_type & endstone::CompoundTag::operator[] (
    const key_type & key
) 
```




<hr>



### function size 

```C++
inline size_type endstone::CompoundTag::size () noexcept const
```




<hr>



### function swap 

```C++
inline void endstone::CompoundTag::swap (
    CompoundTag & other
) noexcept
```




<hr>



### function try\_emplace 

```C++
template<class... Args>
std::pair< iterator, bool > endstone::CompoundTag::try_emplace (
    const key_type & key,
    Args &&... args
) 
```




<hr>## Friends Documentation





### friend operator!= 

```C++
bool endstone::CompoundTag::operator!= (
    const  CompoundTag & a,
    const  CompoundTag & b
) noexcept
```




<hr>



### friend operator== 

```C++
bool endstone::CompoundTag::operator== (
    const  CompoundTag & a,
    const  CompoundTag & b
) noexcept
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/nbt/compound.h`

