

# Namespace endstone::nbt



[**Namespace List**](namespaces.md) **>** [**endstone**](namespaceendstone.md) **>** [**nbt**](namespaceendstone_1_1nbt.md)


















## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**detail**](namespaceendstone_1_1nbt_1_1detail.md) <br> |


## Classes

| Type | Name |
| ---: | :--- |
| class | [**ArrayTag**](classendstone_1_1nbt_1_1ArrayTag.md) &lt;[**typename**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md)&gt;<br> |
| class | [**Tag**](classendstone_1_1nbt_1_1Tag.md) <br> |
| struct | [**TagBase**](structendstone_1_1nbt_1_1TagBase.md) <br> |
| class | [**ValueTag**](classendstone_1_1nbt_1_1ValueTag.md) &lt;[**typename**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md)&gt;<br> |


## Public Types

| Type | Name |
| ---: | :--- |
| enum std::uint8\_t | [**Type**](#enum-type)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  std::string | [**dump**](#function-dump) ([**const**](classendstone_1_1Identifier.md) [**Tag**](classendstone_1_1nbt_1_1Tag.md) & tag, std::endian byte\_order=std::endian::little, [**bool**](classendstone_1_1Identifier.md) network=[**false**](classendstone_1_1Identifier.md)) <br>_Serialize an NBT tag to binary format with an empty name._  |
|  std::string | [**dump**](#function-dump) ([**const**](classendstone_1_1Identifier.md) [**Tag**](classendstone_1_1nbt_1_1Tag.md) & tag, [**const**](classendstone_1_1Identifier.md) std::string & name, std::endian byte\_order=std::endian::little, [**bool**](classendstone_1_1Identifier.md) network=[**false**](classendstone_1_1Identifier.md)) <br>_Serialize an NBT tag to binary format with a name._  |
|  [**Tag**](classendstone_1_1nbt_1_1Tag.md) | [**load**](#function-load) (std::string\_view data, std::string & name, std::endian byte\_order=std::endian::little, [**bool**](classendstone_1_1Identifier.md) network=[**false**](classendstone_1_1Identifier.md)) <br>_Deserialize binary NBT data into a_ [_**Tag**_](classendstone_1_1nbt_1_1Tag.md) _, populating the root tag name._ |
|  [**Tag**](classendstone_1_1nbt_1_1Tag.md) | [**load**](#function-load) (std::string\_view data, std::endian byte\_order=std::endian::little, [**bool**](classendstone_1_1Identifier.md) network=[**false**](classendstone_1_1Identifier.md)) <br>_Deserialize binary NBT data into a_ [_**Tag**_](classendstone_1_1nbt_1_1Tag.md) _, discarding the root tag name._ |




























## Public Types Documentation




### enum Type 

```C++
enum endstone::nbt::Type {
    End = 0,
    Byte = 1,
    Short = 2,
    Int = 3,
    Long = 4,
    Float = 5,
    Double = 6,
    ByteArray = 7,
    String = 8,
    List = 9,
    Compound = 10,
    IntArray = 11
};
```




<hr>
## Public Functions Documentation




### function dump 

_Serialize an NBT tag to binary format with an empty name._ 
```C++
inline std::string endstone::nbt::dump (
    const  Tag & tag,
    std::endian byte_order=std::endian::little,
    bool network=false
) 
```





**Parameters:**


* `tag` The tag to serialize. 
* `byte_order` Byte order: std::endian::little (Bedrock) or std::endian::big (Java). 
* `network` If true, use Bedrock network varint encoding for lengths and Int/Long values. 



**Returns:**

Binary NBT data as [type:1][name:""][payload]. 





        

<hr>



### function dump 

_Serialize an NBT tag to binary format with a name._ 
```C++
inline std::string endstone::nbt::dump (
    const  Tag & tag,
    const std::string & name,
    std::endian byte_order=std::endian::little,
    bool network=false
) 
```





**Parameters:**


* `tag` The tag to serialize. 
* `name` The root tag name (e.g. "" for Bedrock level.dat, or a descriptive name). 
* `byte_order` Byte order: std::endian::little (Bedrock) or std::endian::big (Java). 
* `network` If true, use Bedrock network varint encoding for lengths and Int/Long values. 



**Returns:**

Binary NBT data as [type:1][name:string][payload]. 





        

<hr>



### function load 

_Deserialize binary NBT data into a_ [_**Tag**_](classendstone_1_1nbt_1_1Tag.md) _, populating the root tag name._
```C++
inline Tag endstone::nbt::load (
    std::string_view data,
    std::string & name,
    std::endian byte_order=std::endian::little,
    bool network=false
) 
```





**Parameters:**


* `data` Binary NBT data. Consumed from front via string\_view. 
* `name` The deserialized root tag name. 
* `byte_order` Byte order used in the binary data. 
* `network` If true, expect Bedrock network varint encoding. 



**Returns:**

The deserialized tag. 




**Exception:**


* `std::runtime_error` If the data is truncated or malformed. 




        

<hr>



### function load 

_Deserialize binary NBT data into a_ [_**Tag**_](classendstone_1_1nbt_1_1Tag.md) _, discarding the root tag name._
```C++
inline Tag endstone::nbt::load (
    std::string_view data,
    std::endian byte_order=std::endian::little,
    bool network=false
) 
```





**Parameters:**


* `data` Binary NBT data. 
* `byte_order` Byte order used in the binary data. 
* `network` If true, expect Bedrock network varint encoding. 



**Returns:**

The deserialized tag. 




**Exception:**


* `std::runtime_error` If the data is truncated or malformed. 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/nbt/array.h`

