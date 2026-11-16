

# Namespace endstone::nbt



[**Namespace List**](namespaces.md) **>** [**endstone**](namespaceendstone.md) **>** [**nbt**](namespaceendstone_1_1nbt.md)




















## Classes

| Type | Name |
| ---: | :--- |
| class | [**ArrayTag**](classendstone_1_1nbt_1_1ArrayTag.md) &lt;typename T&gt;<br> |
| class | [**Tag**](classendstone_1_1nbt_1_1Tag.md) <br> |
| struct | [**TagBase**](structendstone_1_1nbt_1_1TagBase.md) <br> |
| class | [**ValueTag**](classendstone_1_1nbt_1_1ValueTag.md) &lt;typename T&gt;<br> |
| struct | [**escape\_view**](structendstone_1_1nbt_1_1escape__view.md) <br> |


## Public Types

| Type | Name |
| ---: | :--- |
| enum std::uint8\_t | [**Type**](#enum-type)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  [**escape\_view**](structendstone_1_1nbt_1_1escape__view.md) | [**escaped**](#function-escaped) (std::string\_view value) <br> |




























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




### function escaped 

```C++
inline escape_view endstone::nbt::escaped (
    std::string_view value
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/nbt/array.h`

