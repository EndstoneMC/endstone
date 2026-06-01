

# Class endstone::nbt::Tag



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**nbt**](namespaceendstone_1_1nbt.md) **>** [**Tag**](classendstone_1_1nbt_1_1Tag.md)



_A single NBT tag that may hold any of the supported payload types._ [More...](#detailed-description)

* `#include <endstone/nbt/tag.h>`

















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
|  [**Tag**](classendstone_1_1nbt_1_1Tag.md) & | [**at**](#function-at-14) ([**const**](classendstone_1_1Identifier.md) std::string & key) <br>_Access the child tag with the given key, treating this tag as a compound, with bounds checking._  |
|  [**const**](classendstone_1_1Identifier.md) [**Tag**](classendstone_1_1nbt_1_1Tag.md) & | [**at**](#function-at-24) ([**const**](classendstone_1_1Identifier.md) std::string & key) const<br>_Access the child tag with the given key, treating this tag as a compound, with bounds checking._  |
|  [**Tag**](classendstone_1_1nbt_1_1Tag.md) & | [**at**](#function-at-34) (std::size\_t index) <br>_Access the child tag at the given index, treating this tag as a list, with bounds checking._  |
|  [**const**](classendstone_1_1Identifier.md) [**Tag**](classendstone_1_1nbt_1_1Tag.md) & | [**at**](#function-at-44) (std::size\_t index) const<br>_Access the child tag at the given index, treating this tag as a list, with bounds checking._  |
|  [**bool**](classendstone_1_1Identifier.md) | [**contains**](#function-contains) ([**const**](classendstone_1_1Identifier.md) std::string & key) noexcept const<br>_Check whether this tag is a compound containing the given key._  |
|  std::string | [**dump**](#function-dump-12) (std::endian byte\_order=std::endian::little, [**bool**](classendstone_1_1Identifier.md) network=[**false**](classendstone_1_1Identifier.md)) const<br>_Serialize this tag to binary NBT with an empty root name._  |
|  std::string | [**dump**](#function-dump-22) ([**const**](classendstone_1_1Identifier.md) std::string & name, std::endian byte\_order=std::endian::little, [**bool**](classendstone_1_1Identifier.md) network=[**false**](classendstone_1_1Identifier.md)) const<br>_Serialize this tag to binary NBT with a root name._  |
|  std::pair&lt; CompoundTag::iterator, [**bool**](classendstone_1_1Identifier.md) &gt; | [**emplace**](#function-emplace) ([**Args**](classendstone_1_1Identifier.md) &&... args) <br>_Construct an entry in place in this compound._  |
|  [**ListTag**](classendstone_1_1ListTag.md) & | [**emplace\_back**](#function-emplace_back) ([**Args**](classendstone_1_1Identifier.md) &&... args) <br>_Construct a child tag in place at the end of this list._  |
|  [**bool**](classendstone_1_1Identifier.md) | [**empty**](#function-empty) () noexcept const<br>_Check whether this tag holds no elements._  |
|  [**T**](classendstone_1_1Identifier.md) & | [**get**](#function-get-12) () <br>_Get the payload as the given tag type._  |
|  [**const**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md) & | [**get**](#function-get-22) () const<br>_Get the payload as the given tag type._  |
|  [**T**](classendstone_1_1Identifier.md) \* | [**get\_if**](#function-get_if-12) () noexcept<br>_Get a pointer to the payload if this tag holds the given type._  |
|  [**const**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md) \* | [**get\_if**](#function-get_if-22) () noexcept const<br>_Get a pointer to the payload if this tag holds the given type._  |
|  [**Tag**](classendstone_1_1nbt_1_1Tag.md) & | [**operator[]**](#function-operator) ([**const**](classendstone_1_1Identifier.md) std::string & key) <br>_Access the child tag with the given key, treating this tag as a compound._  |
|  [**Tag**](classendstone_1_1nbt_1_1Tag.md) & | [**operator[]**](#function-operator_1) (std::size\_t index) <br>_Access the child tag at the given index, treating this tag as a list._  |
|  std::size\_t | [**size**](#function-size) () noexcept const<br>_Get the number of elements held by this tag._  |
|  Type | [**type**](#function-type) () noexcept const<br>_Get the type of payload currently held by this tag._  |
|  [**decltype**](classendstone_1_1Identifier.md)([**auto**](classendstone_1_1Identifier.md)) | [**visit**](#function-visit-12) ([**Fn**](classendstone_1_1Identifier.md) && visitor) const<br>_Apply a visitor to the payload currently held by this tag._  |
|  [**decltype**](classendstone_1_1Identifier.md)([**auto**](classendstone_1_1Identifier.md)) | [**visit**](#function-visit-22) ([**Fn**](classendstone_1_1Identifier.md) && visitor) <br>_Apply a visitor to the payload currently held by this tag._  |




























## Detailed Description


A [**Tag**](classendstone_1_1nbt_1_1Tag.md) behaves as a tagged union over every NBT type (byte, short, int, long, float, double, string, byte array, int array, list and compound). A default-constructed [**Tag**](classendstone_1_1nbt_1_1Tag.md) is empty (the End type). Indexing or inserting into an empty [**Tag**](classendstone_1_1nbt_1_1Tag.md) promotes it to a [**ListTag**](classendstone_1_1ListTag.md) or [**CompoundTag**](classendstone_1_1CompoundTag.md) as appropriate. 


    
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

_Access the child tag with the given key, treating this tag as a compound, with bounds checking._ 
```C++
inline Tag & endstone::nbt::Tag::at (
    const std::string & key
) 
```



An empty tag is promoted to a [**CompoundTag**](classendstone_1_1CompoundTag.md).




**Parameters:**


* `key` the key to access 



**Returns:**

a reference to the child tag 




**Exception:**


* `std::runtime_error` if this tag is not a compound 
* `std::out_of_range` if the key is not present 




        

<hr>



### function at [2/4]

_Access the child tag with the given key, treating this tag as a compound, with bounds checking._ 
```C++
inline const  Tag & endstone::nbt::Tag::at (
    const std::string & key
) const
```





**Parameters:**


* `key` the key to access 



**Returns:**

a const reference to the child tag 




**Exception:**


* `std::runtime_error` if this tag is not a compound 
* `std::out_of_range` if the key is not present 




        

<hr>



### function at [3/4]

_Access the child tag at the given index, treating this tag as a list, with bounds checking._ 
```C++
inline Tag & endstone::nbt::Tag::at (
    std::size_t index
) 
```



An empty tag is promoted to a [**ListTag**](classendstone_1_1ListTag.md).




**Parameters:**


* `index` the index to access 



**Returns:**

a reference to the child tag 




**Exception:**


* `std::runtime_error` if this tag is not a list 
* `std::out_of_range` if the index is out of range 




        

<hr>



### function at [4/4]

_Access the child tag at the given index, treating this tag as a list, with bounds checking._ 
```C++
inline const  Tag & endstone::nbt::Tag::at (
    std::size_t index
) const
```





**Parameters:**


* `index` the index to access 



**Returns:**

a const reference to the child tag 




**Exception:**


* `std::runtime_error` if this tag is not a list 
* `std::out_of_range` if the index is out of range 




        

<hr>



### function contains 

_Check whether this tag is a compound containing the given key._ 
```C++
inline bool endstone::nbt::Tag::contains (
    const std::string & key
) noexcept const
```





**Parameters:**


* `key` the key to look up 



**Returns:**

true if this tag is a compound and contains the key, false otherwise 





        

<hr>



### function dump [1/2]

_Serialize this tag to binary NBT with an empty root name._ 
```C++
inline std::string endstone::nbt::Tag::dump (
    std::endian byte_order=std::endian::little,
    bool network=false
) const
```





**Parameters:**


* `byte_order` std::endian::little (Bedrock) or std::endian::big (Java). 
* `network` If true, use Bedrock network varint encoding. 



**Returns:**

Binary NBT data. 




**See also:** endstone::nbt::dump(), endstone::nbt::load() 



        

<hr>



### function dump [2/2]

_Serialize this tag to binary NBT with a root name._ 
```C++
inline std::string endstone::nbt::Tag::dump (
    const std::string & name,
    std::endian byte_order=std::endian::little,
    bool network=false
) const
```





**Parameters:**


* `name` The root tag name. 
* `byte_order` std::endian::little (Bedrock) or std::endian::big (Java). 
* `network` If true, use Bedrock network varint encoding. 



**Returns:**

Binary NBT data. 





        

<hr>



### function emplace 

_Construct an entry in place in this compound._ 
```C++
template<class... Args>
inline std::pair< CompoundTag::iterator, bool > endstone::nbt::Tag::emplace (
    Args &&... args
) 
```



An empty tag is promoted to a [**CompoundTag**](classendstone_1_1CompoundTag.md).




**Parameters:**


* `args` the arguments forwarded to construct the key/value entry 



**Returns:**

a pair of an iterator to the entry and a bool that is true if a new entry was inserted 




**Exception:**


* `std::runtime_error` if this tag is not a compound 




        

<hr>



### function emplace\_back 

_Construct a child tag in place at the end of this list._ 
```C++
template<class... Args>
inline ListTag & endstone::nbt::Tag::emplace_back (
    Args &&... args
) 
```



An empty tag is promoted to a [**ListTag**](classendstone_1_1ListTag.md).




**Parameters:**


* `args` the arguments forwarded to the element constructor 



**Returns:**

a reference to this tag's underlying list 




**Exception:**


* `std::runtime_error` if this tag is not a list 




        

<hr>



### function empty 

_Check whether this tag holds no elements._ 
```C++
inline bool endstone::nbt::Tag::empty () noexcept const
```





**Returns:**

true if [**size()**](classendstone_1_1nbt_1_1Tag.md#function-size) is 0 





        

<hr>



### function get [1/2]

_Get the payload as the given tag type._ 
```C++
template<typename  T>
inline T & endstone::nbt::Tag::get () 
```





**Template parameters:**


* `T` the tag type to retrieve 



**Returns:**

a reference to the held value 




**Exception:**


* `std::runtime_error` if this tag does not currently hold a T 




        

<hr>



### function get [2/2]

_Get the payload as the given tag type._ 
```C++
template<typename  T>
inline const  T & endstone::nbt::Tag::get () const
```





**Template parameters:**


* `T` the tag type to retrieve 



**Returns:**

a const reference to the held value 




**Exception:**


* `std::runtime_error` if this tag does not currently hold a T 




        

<hr>



### function get\_if [1/2]

_Get a pointer to the payload if this tag holds the given type._ 
```C++
template<typename  T>
inline T * endstone::nbt::Tag::get_if () noexcept
```





**Template parameters:**


* `T` the tag type to retrieve 



**Returns:**

a pointer to the held value, or nullptr if this tag does not hold a T 





        

<hr>



### function get\_if [2/2]

_Get a pointer to the payload if this tag holds the given type._ 
```C++
template<typename  T>
inline const  T * endstone::nbt::Tag::get_if () noexcept const
```





**Template parameters:**


* `T` the tag type to retrieve 



**Returns:**

a const pointer to the held value, or nullptr if this tag does not hold a T 





        

<hr>



### function operator[] 

_Access the child tag with the given key, treating this tag as a compound._ 
```C++
inline Tag & endstone::nbt::Tag::operator[] (
    const std::string & key
) 
```



An empty tag is promoted to a [**CompoundTag**](classendstone_1_1CompoundTag.md), and a missing key is inserted with an empty value.




**Parameters:**


* `key` the key to access 



**Returns:**

a reference to the child tag 




**Exception:**


* `std::runtime_error` if this tag is not a compound 




        

<hr>



### function operator[] 

_Access the child tag at the given index, treating this tag as a list._ 
```C++
inline Tag & endstone::nbt::Tag::operator[] (
    std::size_t index
) 
```



An empty tag is promoted to a [**ListTag**](classendstone_1_1ListTag.md).




**Parameters:**


* `index` the index to access 



**Returns:**

a reference to the child tag 




**Exception:**


* `std::runtime_error` if this tag is not a list 




        

<hr>



### function size 

_Get the number of elements held by this tag._ 
```C++
inline std::size_t endstone::nbt::Tag::size () noexcept const
```



For a list, compound or array this is the element count; for a string it is the character count; scalar tags report 1 and an empty tag reports 0.




**Returns:**

the element count 





        

<hr>



### function type 

_Get the type of payload currently held by this tag._ 
```C++
inline Type endstone::nbt::Tag::type () noexcept const
```





**Returns:**

the tag type, or Type::End if the tag is empty 





        

<hr>



### function visit [1/2]

_Apply a visitor to the payload currently held by this tag._ 
```C++
template<typename  Fn>
inline decltype ( auto ) endstone::nbt::Tag::visit (
    Fn && visitor
) const
```





**Template parameters:**


* `Fn` the visitor type, callable with any of the tag payload types (including std::monostate) 



**Parameters:**


* `visitor` the visitor to invoke 



**Returns:**

whatever the visitor returns 





        

<hr>



### function visit [2/2]

_Apply a visitor to the payload currently held by this tag._ 
```C++
template<typename  Fn>
inline decltype ( auto ) endstone::nbt::Tag::visit (
    Fn && visitor
) 
```





**Template parameters:**


* `Fn` the visitor type, callable with any of the tag payload types (including std::monostate) 



**Parameters:**


* `visitor` the visitor to invoke 



**Returns:**

whatever the visitor returns 





        

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

