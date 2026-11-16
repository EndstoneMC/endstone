

# Class endstone::nbt::Tag



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**nbt**](namespaceendstone_1_1nbt.md) **>** [**Tag**](classendstone_1_1nbt_1_1Tag.md)






















## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::variant&lt; std::monostate, ByteTag, ShortTag, IntTag, LongTag, FloatTag, DoubleTag, StringTag, ByteArrayTag, IntArrayTag, [**ListTag**](classendstone_1_1ListTag.md), [**CompoundTag**](classendstone_1_1CompoundTag.md) &gt; | [**Storage**](#typedef-storage)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Tag**](#function-tag-123) () noexcept<br> |
|   | [**Tag**](#function-tag-223) (const ByteTag & v) <br> |
|   | [**Tag**](#function-tag-323) (ByteTag && v) <br> |
|   | [**Tag**](#function-tag-423) (const ShortTag & v) <br> |
|   | [**Tag**](#function-tag-523) (ShortTag && v) <br> |
|   | [**Tag**](#function-tag-623) (const IntTag & v) <br> |
|   | [**Tag**](#function-tag-723) (IntTag && v) <br> |
|   | [**Tag**](#function-tag-823) (const LongTag & v) <br> |
|   | [**Tag**](#function-tag-923) (LongTag && v) <br> |
|   | [**Tag**](#function-tag-1023) (const FloatTag & v) <br> |
|   | [**Tag**](#function-tag-1123) (FloatTag && v) <br> |
|   | [**Tag**](#function-tag-1223) (const DoubleTag & v) <br> |
|   | [**Tag**](#function-tag-1323) (DoubleTag && v) <br> |
|   | [**Tag**](#function-tag-1423) (const StringTag & v) <br> |
|   | [**Tag**](#function-tag-1523) (StringTag && v) <br> |
|   | [**Tag**](#function-tag-1623) (const ByteArrayTag & v) <br> |
|   | [**Tag**](#function-tag-1723) (ByteArrayTag && v) <br> |
|   | [**Tag**](#function-tag-1823) (const IntArrayTag & v) <br> |
|   | [**Tag**](#function-tag-1923) (IntArrayTag && v) <br> |
|   | [**Tag**](#function-tag-2023) (const [**ListTag**](classendstone_1_1ListTag.md) & v) <br> |
|   | [**Tag**](#function-tag-2123) ([**ListTag**](classendstone_1_1ListTag.md) && v) <br> |
|   | [**Tag**](#function-tag-2223) (const [**CompoundTag**](classendstone_1_1CompoundTag.md) & v) <br> |
|   | [**Tag**](#function-tag-2323) ([**CompoundTag**](classendstone_1_1CompoundTag.md) && v) <br> |
|  Tag & | [**at**](#function-at-14) (const std::string & key) <br> |
|  const Tag & | [**at**](#function-at-24) (const std::string & key) const<br> |
|  Tag & | [**at**](#function-at-34) (std::size\_t index) <br> |
|  const Tag & | [**at**](#function-at-44) (std::size\_t index) const<br> |
|  bool | [**contains**](#function-contains) (const std::string & key) noexcept const<br> |
|  std::pair&lt; CompoundTag::iterator, bool &gt; | [**emplace**](#function-emplace) (Args &&... args) <br> |
|  [**ListTag**](classendstone_1_1ListTag.md) & | [**emplace\_back**](#function-emplace_back) (Args &&... args) <br> |
|  bool | [**empty**](#function-empty) () noexcept const<br> |
|  T & | [**get**](#function-get-12) () <br> |
|  const T & | [**get**](#function-get-22) () const<br> |
|  T \* | [**get\_if**](#function-get_if-12) () noexcept<br> |
|  const T \* | [**get\_if**](#function-get_if-22) () noexcept const<br> |
|  Tag & | [**operator[]**](#function-operator) (const std::string & key) <br> |
|  Tag & | [**operator[]**](#function-operator_1) (std::size\_t index) <br> |
|  std::size\_t | [**size**](#function-size) () noexcept const<br> |
|  Type | [**type**](#function-type) () noexcept const<br> |
|  decltype(auto) | [**visit**](#function-visit-12) (Fn && visitor) const<br> |
|  decltype(auto) | [**visit**](#function-visit-22) (Fn && visitor) <br> |




























## Public Types Documentation




### typedef Storage 

```C++
using endstone::nbt::Tag::Storage =  std::variant<std::monostate, ByteTag, ShortTag, IntTag, LongTag, FloatTag, DoubleTag, StringTag,
                                 ByteArrayTag, IntArrayTag, ListTag, CompoundTag>;
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
    const ByteTag & v
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
    const ShortTag & v
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
    const IntTag & v
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
    const LongTag & v
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
    const FloatTag & v
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
    const DoubleTag & v
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
    const StringTag & v
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
    const ByteArrayTag & v
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
    const IntArrayTag & v
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
    const ListTag & v
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
    const CompoundTag & v
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
inline const Tag & endstone::nbt::Tag::at (
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
inline const Tag & endstone::nbt::Tag::at (
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
template<typename T>
inline T & endstone::nbt::Tag::get () 
```




<hr>



### function get [2/2]

```C++
template<typename T>
inline const T & endstone::nbt::Tag::get () const
```




<hr>



### function get\_if [1/2]

```C++
template<typename T>
inline T * endstone::nbt::Tag::get_if () noexcept
```




<hr>



### function get\_if [2/2]

```C++
template<typename T>
inline const T * endstone::nbt::Tag::get_if () noexcept const
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
template<typename Fn>
inline decltype(auto) endstone::nbt::Tag::visit (
    Fn && visitor
) const
```




<hr>



### function visit [2/2]

```C++
template<typename Fn>
inline decltype(auto) endstone::nbt::Tag::visit (
    Fn && visitor
) 
```




<hr>## Friends Documentation





### friend operator!= 

```C++
inline bool endstone::nbt::Tag::operator!= (
    const Tag & a,
    const Tag & b
) noexcept
```




<hr>



### friend operator== 

```C++
inline bool endstone::nbt::Tag::operator== (
    const Tag & a,
    const Tag & b
) noexcept
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/nbt/tag.h`

