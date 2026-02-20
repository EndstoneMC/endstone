

# Class endstone::nbt::Tag



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**nbt**](namespaceendstone_1_1nbt.md) **>** [**Tag**](classendstone_1_1nbt_1_1Tag.md)






















## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::variant&lt; std::monostate, [**ByteTag**](classendstone_1_1nbt_1_1ValueTag.md), [**ShortTag**](classendstone_1_1nbt_1_1ValueTag.md), [**IntTag**](classendstone_1_1nbt_1_1ValueTag.md), [**LongTag**](classendstone_1_1nbt_1_1ValueTag.md), [**FloatTag**](classendstone_1_1nbt_1_1ValueTag.md), [**DoubleTag**](classendstone_1_1nbt_1_1ValueTag.md), [**StringTag**](classendstone_1_1nbt_1_1ValueTag.md), [**ByteArrayTag**](classendstone_1_1nbt_1_1ArrayTag.md), [**IntArrayTag**](classendstone_1_1nbt_1_1ArrayTag.md), [**ListTag**](classendstone_1_1ListTag.md), [**CompoundTag**](classendstone_1_1CompoundTag.md) &gt; | [**Storage**](#typedef-storage)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Tag**](#function-tag-123) () noexcept<br> |
|   | [**Tag**](#function-tag-223) ([**const**](classendstone_1_1Identifier.md) [**ByteTag**](classendstone_1_1nbt_1_1ValueTag.md) & v) <br> |
|   | [**Tag**](#function-tag-323) ([**ByteTag**](classendstone_1_1nbt_1_1ValueTag.md) && v) <br> |
|   | [**Tag**](#function-tag-423) ([**const**](classendstone_1_1Identifier.md) [**ShortTag**](classendstone_1_1nbt_1_1ValueTag.md) & v) <br> |
|   | [**Tag**](#function-tag-523) ([**ShortTag**](classendstone_1_1nbt_1_1ValueTag.md) && v) <br> |
|   | [**Tag**](#function-tag-623) ([**const**](classendstone_1_1Identifier.md) [**IntTag**](classendstone_1_1nbt_1_1ValueTag.md) & v) <br> |
|   | [**Tag**](#function-tag-723) ([**IntTag**](classendstone_1_1nbt_1_1ValueTag.md) && v) <br> |
|   | [**Tag**](#function-tag-823) ([**const**](classendstone_1_1Identifier.md) [**LongTag**](classendstone_1_1nbt_1_1ValueTag.md) & v) <br> |
|   | [**Tag**](#function-tag-923) ([**LongTag**](classendstone_1_1nbt_1_1ValueTag.md) && v) <br> |
|   | [**Tag**](#function-tag-1023) ([**const**](classendstone_1_1Identifier.md) [**FloatTag**](classendstone_1_1nbt_1_1ValueTag.md) & v) <br> |
|   | [**Tag**](#function-tag-1123) ([**FloatTag**](classendstone_1_1nbt_1_1ValueTag.md) && v) <br> |
|   | [**Tag**](#function-tag-1223) ([**const**](classendstone_1_1Identifier.md) [**DoubleTag**](classendstone_1_1nbt_1_1ValueTag.md) & v) <br> |
|   | [**Tag**](#function-tag-1323) ([**DoubleTag**](classendstone_1_1nbt_1_1ValueTag.md) && v) <br> |
|   | [**Tag**](#function-tag-1423) ([**const**](classendstone_1_1Identifier.md) [**StringTag**](classendstone_1_1nbt_1_1ValueTag.md) & v) <br> |
|   | [**Tag**](#function-tag-1523) ([**StringTag**](classendstone_1_1nbt_1_1ValueTag.md) && v) <br> |
|   | [**Tag**](#function-tag-1623) ([**const**](classendstone_1_1Identifier.md) [**ByteArrayTag**](classendstone_1_1nbt_1_1ArrayTag.md) & v) <br> |
|   | [**Tag**](#function-tag-1723) ([**ByteArrayTag**](classendstone_1_1nbt_1_1ArrayTag.md) && v) <br> |
|   | [**Tag**](#function-tag-1823) ([**const**](classendstone_1_1Identifier.md) [**IntArrayTag**](classendstone_1_1nbt_1_1ArrayTag.md) & v) <br> |
|   | [**Tag**](#function-tag-1923) ([**IntArrayTag**](classendstone_1_1nbt_1_1ArrayTag.md) && v) <br> |
|   | [**Tag**](#function-tag-2023) ([**const**](classendstone_1_1Identifier.md) [**ListTag**](classendstone_1_1ListTag.md) & v) <br> |
|   | [**Tag**](#function-tag-2123) ([**ListTag**](classendstone_1_1ListTag.md) && v) <br> |
|   | [**Tag**](#function-tag-2223) ([**const**](classendstone_1_1Identifier.md) [**CompoundTag**](classendstone_1_1CompoundTag.md) & v) <br> |
|   | [**Tag**](#function-tag-2323) ([**CompoundTag**](classendstone_1_1CompoundTag.md) && v) <br> |
|  [**Tag**](classendstone_1_1nbt_1_1Tag.md) & | [**at**](#function-at-14) ([**const**](classendstone_1_1Identifier.md) std::string & key) <br> |
|  [**const**](classendstone_1_1Identifier.md) [**Tag**](classendstone_1_1nbt_1_1Tag.md) & | [**at**](#function-at-24) ([**const**](classendstone_1_1Identifier.md) std::string & key) const<br> |
|  [**Tag**](classendstone_1_1nbt_1_1Tag.md) & | [**at**](#function-at-34) (std::size\_t index) <br> |
|  [**const**](classendstone_1_1Identifier.md) [**Tag**](classendstone_1_1nbt_1_1Tag.md) & | [**at**](#function-at-44) (std::size\_t index) const<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**contains**](#function-contains) ([**const**](classendstone_1_1Identifier.md) std::string & key) noexcept const<br> |
|  std::pair&lt; CompoundTag::iterator, [**bool**](classendstone_1_1Identifier.md) &gt; | [**emplace**](#function-emplace) ([**Args**](classendstone_1_1Identifier.md) &&... args) <br> |
|  [**ListTag**](classendstone_1_1ListTag.md) & | [**emplace\_back**](#function-emplace_back) ([**Args**](classendstone_1_1Identifier.md) &&... args) <br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**empty**](#function-empty) () noexcept const<br> |
|  [**T**](classendstone_1_1Identifier.md) & | [**get**](#function-get-12) () <br> |
|  [**const**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md) & | [**get**](#function-get-22) () const<br> |
|  [**T**](classendstone_1_1Identifier.md) \* | [**get\_if**](#function-get_if-12) () noexcept<br> |
|  [**const**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md) \* | [**get\_if**](#function-get_if-22) () noexcept const<br> |
|  [**Tag**](classendstone_1_1nbt_1_1Tag.md) & | [**operator[]**](#function-operator) ([**const**](classendstone_1_1Identifier.md) std::string & key) <br> |
|  [**Tag**](classendstone_1_1nbt_1_1Tag.md) & | [**operator[]**](#function-operator_1) (std::size\_t index) <br> |
|  std::size\_t | [**size**](#function-size) () noexcept const<br> |
|  Type | [**type**](#function-type) () noexcept const<br> |
|  [**decltype**](classendstone_1_1Identifier.md)([**auto**](classendstone_1_1Identifier.md)) | [**visit**](#function-visit-12) ([**Fn**](classendstone_1_1Identifier.md) && visitor) const<br> |
|  [**decltype**](classendstone_1_1Identifier.md)([**auto**](classendstone_1_1Identifier.md)) | [**visit**](#function-visit-22) ([**Fn**](classendstone_1_1Identifier.md) && visitor) <br> |




























## Public Types Documentation




### typedef Storage 

```C++
using endstone::nbt::Tag::Storage =  std::variant<std::monostate, ByteTag, ShortTag, IntTag, LongTag, FloatTag, DoubleTag, StringTag, ByteArrayTag, IntArrayTag, ListTag, CompoundTag>;
```




<hr>
## Public Functions Documentation




### function Tag [1/23]

```C++
inline endstone::nbt::Tag::Tag () noexcept
```




<hr>



### function Tag [2/23]

```C++
inline endstone::nbt::Tag::Tag (
    const  ByteTag & v
) 
```




<hr>



### function Tag [3/23]

```C++
inline endstone::nbt::Tag::Tag (
    ByteTag && v
) 
```




<hr>



### function Tag [4/23]

```C++
inline endstone::nbt::Tag::Tag (
    const  ShortTag & v
) 
```




<hr>



### function Tag [5/23]

```C++
inline endstone::nbt::Tag::Tag (
    ShortTag && v
) 
```




<hr>



### function Tag [6/23]

```C++
inline endstone::nbt::Tag::Tag (
    const  IntTag & v
) 
```




<hr>



### function Tag [7/23]

```C++
inline endstone::nbt::Tag::Tag (
    IntTag && v
) 
```




<hr>



### function Tag [8/23]

```C++
inline endstone::nbt::Tag::Tag (
    const  LongTag & v
) 
```




<hr>



### function Tag [9/23]

```C++
inline endstone::nbt::Tag::Tag (
    LongTag && v
) 
```




<hr>



### function Tag [10/23]

```C++
inline endstone::nbt::Tag::Tag (
    const  FloatTag & v
) 
```




<hr>



### function Tag [11/23]

```C++
inline endstone::nbt::Tag::Tag (
    FloatTag && v
) 
```




<hr>



### function Tag [12/23]

```C++
inline endstone::nbt::Tag::Tag (
    const  DoubleTag & v
) 
```




<hr>



### function Tag [13/23]

```C++
inline endstone::nbt::Tag::Tag (
    DoubleTag && v
) 
```




<hr>



### function Tag [14/23]

```C++
inline endstone::nbt::Tag::Tag (
    const  StringTag & v
) 
```




<hr>



### function Tag [15/23]

```C++
inline endstone::nbt::Tag::Tag (
    StringTag && v
) 
```




<hr>



### function Tag [16/23]

```C++
inline endstone::nbt::Tag::Tag (
    const  ByteArrayTag & v
) 
```




<hr>



### function Tag [17/23]

```C++
inline endstone::nbt::Tag::Tag (
    ByteArrayTag && v
) 
```




<hr>



### function Tag [18/23]

```C++
inline endstone::nbt::Tag::Tag (
    const  IntArrayTag & v
) 
```




<hr>



### function Tag [19/23]

```C++
inline endstone::nbt::Tag::Tag (
    IntArrayTag && v
) 
```




<hr>



### function Tag [20/23]

```C++
inline endstone::nbt::Tag::Tag (
    const  ListTag & v
) 
```




<hr>



### function Tag [21/23]

```C++
inline endstone::nbt::Tag::Tag (
    ListTag && v
) 
```




<hr>



### function Tag [22/23]

```C++
inline endstone::nbt::Tag::Tag (
    const  CompoundTag & v
) 
```




<hr>



### function Tag [23/23]

```C++
inline endstone::nbt::Tag::Tag (
    CompoundTag && v
) 
```




<hr>



### function at [1/4]

```C++
inline Tag & endstone::nbt::Tag::at (
    const std::string & key
) 
```




<hr>



### function at [2/4]

```C++
inline const  Tag & endstone::nbt::Tag::at (
    const std::string & key
) const
```




<hr>



### function at [3/4]

```C++
inline Tag & endstone::nbt::Tag::at (
    std::size_t index
) 
```




<hr>



### function at [4/4]

```C++
inline const  Tag & endstone::nbt::Tag::at (
    std::size_t index
) const
```




<hr>



### function contains 

```C++
inline bool endstone::nbt::Tag::contains (
    const std::string & key
) noexcept const
```




<hr>



### function emplace 

```C++
template<class... Args>
inline std::pair< CompoundTag::iterator, bool > endstone::nbt::Tag::emplace (
    Args &&... args
) 
```




<hr>



### function emplace\_back 

```C++
template<class... Args>
inline ListTag & endstone::nbt::Tag::emplace_back (
    Args &&... args
) 
```




<hr>



### function empty 

```C++
inline bool endstone::nbt::Tag::empty () noexcept const
```




<hr>



### function get [1/2]

```C++
template<typename  T>
inline T & endstone::nbt::Tag::get () 
```




<hr>



### function get [2/2]

```C++
template<typename  T>
inline const  T & endstone::nbt::Tag::get () const
```




<hr>



### function get\_if [1/2]

```C++
template<typename  T>
inline T * endstone::nbt::Tag::get_if () noexcept
```




<hr>



### function get\_if [2/2]

```C++
template<typename  T>
inline const  T * endstone::nbt::Tag::get_if () noexcept const
```




<hr>



### function operator[] 

```C++
inline Tag & endstone::nbt::Tag::operator[] (
    const std::string & key
) 
```




<hr>



### function operator[] 

```C++
inline Tag & endstone::nbt::Tag::operator[] (
    std::size_t index
) 
```




<hr>



### function size 

```C++
inline std::size_t endstone::nbt::Tag::size () noexcept const
```




<hr>



### function type 

```C++
inline Type endstone::nbt::Tag::type () noexcept const
```




<hr>



### function visit [1/2]

```C++
template<typename  Fn>
inline decltype ( auto ) endstone::nbt::Tag::visit (
    Fn && visitor
) const
```




<hr>



### function visit [2/2]

```C++
template<typename  Fn>
inline decltype ( auto ) endstone::nbt::Tag::visit (
    Fn && visitor
) 
```




<hr>## Friends Documentation





### friend operator!= 

```C++
inline bool endstone::nbt::Tag::operator!= (
    const  Tag & a,
    const  Tag & b
) noexcept
```




<hr>



### friend operator== 

```C++
inline bool endstone::nbt::Tag::operator== (
    const  Tag & a,
    const  Tag & b
) noexcept
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/nbt/tag.h`

