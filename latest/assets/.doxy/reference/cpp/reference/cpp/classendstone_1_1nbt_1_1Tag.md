

# Class endstone::nbt::Tag



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**nbt**](namespaceendstone_1_1nbt.md) **>** [**Tag**](classendstone_1_1nbt_1_1Tag.md)






















## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::variant&lt; std::monostate, [**ByteTag**](classendstone_1_1nbt_1_1ValueTag.md), [**ShortTag**](classendstone_1_1nbt_1_1ValueTag.md), [**IntTag**](classendstone_1_1nbt_1_1ValueTag.md), [**LongTag**](classendstone_1_1nbt_1_1ValueTag.md), [**FloatTag**](classendstone_1_1nbt_1_1ValueTag.md), [**DoubleTag**](classendstone_1_1nbt_1_1ValueTag.md), [**StringTag**](classendstone_1_1nbt_1_1ValueTag.md), [**ByteArrayTag**](classendstone_1_1nbt_1_1ArrayTag.md), [**IntArrayTag**](classendstone_1_1nbt_1_1ArrayTag.md), [**ListTag**](classendstone_1_1ListTag.md), [**CompoundTag**](classendstone_1_1CompoundTag.md) &gt; | [**Storage**](#typedef-storage)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Tag**](#function-tag-112) () noexcept<br> |
|   | [**Tag**](#function-tag-212) ([**ByteTag**](classendstone_1_1nbt_1_1ValueTag.md) v) <br> |
|   | [**Tag**](#function-tag-312) ([**ShortTag**](classendstone_1_1nbt_1_1ValueTag.md) v) <br> |
|   | [**Tag**](#function-tag-412) ([**IntTag**](classendstone_1_1nbt_1_1ValueTag.md) v) <br> |
|   | [**Tag**](#function-tag-512) ([**LongTag**](classendstone_1_1nbt_1_1ValueTag.md) v) <br> |
|   | [**Tag**](#function-tag-612) ([**FloatTag**](classendstone_1_1nbt_1_1ValueTag.md) v) <br> |
|   | [**Tag**](#function-tag-712) ([**DoubleTag**](classendstone_1_1nbt_1_1ValueTag.md) v) <br> |
|   | [**Tag**](#function-tag-812) ([**StringTag**](classendstone_1_1nbt_1_1ValueTag.md) v) <br> |
|   | [**Tag**](#function-tag-912) ([**ByteArrayTag**](classendstone_1_1nbt_1_1ArrayTag.md) v) <br> |
|   | [**Tag**](#function-tag-1012) ([**IntArrayTag**](classendstone_1_1nbt_1_1ArrayTag.md) v) <br> |
|   | [**Tag**](#function-tag-1112) ([**ListTag**](classendstone_1_1ListTag.md) v) <br> |
|   | [**Tag**](#function-tag-1212) ([**CompoundTag**](classendstone_1_1CompoundTag.md) v) <br> |
|  [**Tag**](classendstone_1_1nbt_1_1Tag.md) & | [**at**](#function-at-14) (std::string\_view key) <br> |
|  [**const**](classendstone_1_1Identifier.md) [**Tag**](classendstone_1_1nbt_1_1Tag.md) & | [**at**](#function-at-24) (std::string\_view key) const<br> |
|  [**Tag**](classendstone_1_1nbt_1_1Tag.md) & | [**at**](#function-at-34) (std::size\_t index) <br> |
|  [**const**](classendstone_1_1Identifier.md) [**Tag**](classendstone_1_1nbt_1_1Tag.md) & | [**at**](#function-at-44) (std::size\_t index) const<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**contains**](#function-contains) (std::string\_view key) noexcept const<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**empty**](#function-empty) () noexcept const<br> |
|  [**decltype**](classendstone_1_1Identifier.md)([**auto**](classendstone_1_1Identifier.md)) | [**get**](#function-get-12) () <br> |
|  [**decltype**](classendstone_1_1Identifier.md)([**auto**](classendstone_1_1Identifier.md)) | [**get**](#function-get-22) () const<br> |
|  [**auto**](classendstone_1_1Identifier.md) | [**get\_if**](#function-get_if-12) () noexcept<br> |
|  [**const**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md) \* | [**get\_if**](#function-get_if-22) () noexcept const<br> |
|  [**Tag**](classendstone_1_1nbt_1_1Tag.md) & | [**operator[]**](#function-operator) (std::string\_view key) <br> |
|  [**Tag**](classendstone_1_1nbt_1_1Tag.md) & | [**operator[]**](#function-operator_1) (std::size\_t index) <br> |
|  std::size\_t | [**size**](#function-size) () noexcept const<br> |
|  Type | [**type**](#function-type) () noexcept const<br> |




























## Public Types Documentation




### typedef Storage 

```C++
using endstone::nbt::Tag::Storage =  std::variant<std::monostate, ByteTag, ShortTag, IntTag, LongTag, FloatTag, DoubleTag, StringTag, ByteArrayTag, IntArrayTag, ListTag, CompoundTag>;
```




<hr>
## Public Functions Documentation




### function Tag [1/12]

```C++
inline endstone::nbt::Tag::Tag () noexcept
```




<hr>



### function Tag [2/12]

```C++
inline explicit endstone::nbt::Tag::Tag (
    ByteTag v
) 
```




<hr>



### function Tag [3/12]

```C++
inline explicit endstone::nbt::Tag::Tag (
    ShortTag v
) 
```




<hr>



### function Tag [4/12]

```C++
inline explicit endstone::nbt::Tag::Tag (
    IntTag v
) 
```




<hr>



### function Tag [5/12]

```C++
inline explicit endstone::nbt::Tag::Tag (
    LongTag v
) 
```




<hr>



### function Tag [6/12]

```C++
inline explicit endstone::nbt::Tag::Tag (
    FloatTag v
) 
```




<hr>



### function Tag [7/12]

```C++
inline explicit endstone::nbt::Tag::Tag (
    DoubleTag v
) 
```




<hr>



### function Tag [8/12]

```C++
inline explicit endstone::nbt::Tag::Tag (
    StringTag v
) 
```




<hr>



### function Tag [9/12]

```C++
inline explicit endstone::nbt::Tag::Tag (
    ByteArrayTag v
) 
```




<hr>



### function Tag [10/12]

```C++
inline explicit endstone::nbt::Tag::Tag (
    IntArrayTag v
) 
```




<hr>



### function Tag [11/12]

```C++
inline explicit endstone::nbt::Tag::Tag (
    ListTag v
) 
```




<hr>



### function Tag [12/12]

```C++
inline explicit endstone::nbt::Tag::Tag (
    CompoundTag v
) 
```




<hr>



### function at [1/4]

```C++
Tag & endstone::nbt::Tag::at (
    std::string_view key
) 
```




<hr>



### function at [2/4]

```C++
const  Tag & endstone::nbt::Tag::at (
    std::string_view key
) const
```




<hr>



### function at [3/4]

```C++
Tag & endstone::nbt::Tag::at (
    std::size_t index
) 
```




<hr>



### function at [4/4]

```C++
const  Tag & endstone::nbt::Tag::at (
    std::size_t index
) const
```




<hr>



### function contains 

```C++
bool endstone::nbt::Tag::contains (
    std::string_view key
) noexcept const
```




<hr>



### function empty 

```C++
bool endstone::nbt::Tag::empty () noexcept const
```




<hr>



### function get [1/2]

```C++
template<typename  T>
inline decltype ( auto ) endstone::nbt::Tag::get () 
```




<hr>



### function get [2/2]

```C++
template<typename  T>
inline decltype ( auto ) endstone::nbt::Tag::get () const
```




<hr>



### function get\_if [1/2]

```C++
template<typename  T>
inline auto endstone::nbt::Tag::get_if () noexcept
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
Tag & endstone::nbt::Tag::operator[] (
    std::string_view key
) 
```




<hr>



### function operator[] 

```C++
Tag & endstone::nbt::Tag::operator[] (
    std::size_t index
) 
```




<hr>



### function size 

```C++
std::size_t endstone::nbt::Tag::size () noexcept const
```




<hr>



### function type 

```C++
Type endstone::nbt::Tag::type () noexcept const
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

