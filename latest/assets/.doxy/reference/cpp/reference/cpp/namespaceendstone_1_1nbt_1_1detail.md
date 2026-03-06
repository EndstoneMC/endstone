

# Namespace endstone::nbt::detail



[**Namespace List**](namespaces.md) **>** [**endstone**](namespaceendstone.md) **>** [**nbt**](namespaceendstone_1_1nbt.md) **>** [**detail**](namespaceendstone_1_1nbt_1_1detail.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|  [**void**](classendstone_1_1Identifier.md) | [**read\_bytes**](#function-read_bytes) (std::string\_view & in, [**void**](classendstone_1_1Identifier.md) \* dst, std::size\_t n) <br> |
|  std::int32\_t | [**read\_length**](#function-read_length) (std::string\_view & in, std::endian byte\_order, [**bool**](classendstone_1_1Identifier.md) network) <br> |
|  std::string | [**read\_string**](#function-read_string) (std::string\_view & in, std::endian byte\_order, [**bool**](classendstone_1_1Identifier.md) network) <br> |
|  [**Tag**](classendstone_1_1nbt_1_1Tag.md) | [**read\_tag\_payload**](#function-read_tag_payload) (std::string\_view & in, Type type, std::endian byte\_order, [**bool**](classendstone_1_1Identifier.md) network) <br> |
|  [**T**](classendstone_1_1Identifier.md) | [**read\_value**](#function-read_value) (std::string\_view & in, std::endian byte\_order) <br> |
|  std::uint32\_t | [**read\_varint**](#function-read_varint) (std::string\_view & in) <br> |
|  std::uint64\_t | [**read\_varint64**](#function-read_varint64) (std::string\_view & in) <br> |
|  std::int32\_t | [**read\_zigzag32**](#function-read_zigzag32) (std::string\_view & in) <br> |
|  std::int64\_t | [**read\_zigzag64**](#function-read_zigzag64) (std::string\_view & in) <br> |
|  [**void**](classendstone_1_1Identifier.md) | [**write\_bytes**](#function-write_bytes) (std::string & out, [**const**](classendstone_1_1Identifier.md) [**void**](classendstone_1_1Identifier.md) \* data, std::size\_t n) <br> |
|  [**void**](classendstone_1_1Identifier.md) | [**write\_length**](#function-write_length) (std::string & out, std::int32\_t len, std::endian byte\_order, [**bool**](classendstone_1_1Identifier.md) network) <br> |
|  [**void**](classendstone_1_1Identifier.md) | [**write\_string**](#function-write_string) (std::string & out, [**const**](classendstone_1_1Identifier.md) std::string & str, std::endian byte\_order, [**bool**](classendstone_1_1Identifier.md) network) <br> |
|  [**void**](classendstone_1_1Identifier.md) | [**write\_tag\_payload**](#function-write_tag_payload) (std::string & out, [**const**](classendstone_1_1Identifier.md) [**Tag**](classendstone_1_1nbt_1_1Tag.md) & tag, std::endian byte\_order, [**bool**](classendstone_1_1Identifier.md) network) <br> |
|  [**void**](classendstone_1_1Identifier.md) | [**write\_value**](#function-write_value) (std::string & out, [**T**](classendstone_1_1Identifier.md) value, std::endian byte\_order) <br> |
|  [**void**](classendstone_1_1Identifier.md) | [**write\_varint**](#function-write_varint) (std::string & out, std::uint32\_t value) <br> |
|  [**void**](classendstone_1_1Identifier.md) | [**write\_varint64**](#function-write_varint64) (std::string & out, std::uint64\_t value) <br> |
|  [**void**](classendstone_1_1Identifier.md) | [**write\_zigzag32**](#function-write_zigzag32) (std::string & out, std::int32\_t value) <br> |
|  [**void**](classendstone_1_1Identifier.md) | [**write\_zigzag64**](#function-write_zigzag64) (std::string & out, std::int64\_t value) <br> |
|  std::int32\_t | [**zigzag\_decode32**](#function-zigzag_decode32) (std::uint32\_t value) <br> |
|  std::int64\_t | [**zigzag\_decode64**](#function-zigzag_decode64) (std::uint64\_t value) <br> |
|  std::uint32\_t | [**zigzag\_encode32**](#function-zigzag_encode32) (std::int32\_t value) <br> |
|  std::uint64\_t | [**zigzag\_encode64**](#function-zigzag_encode64) (std::int64\_t value) <br> |




























## Public Functions Documentation




### function read\_bytes 

```C++
inline void endstone::nbt::detail::read_bytes (
    std::string_view & in,
    void * dst,
    std::size_t n
) 
```




<hr>



### function read\_length 

```C++
inline std::int32_t endstone::nbt::detail::read_length (
    std::string_view & in,
    std::endian byte_order,
    bool network
) 
```




<hr>



### function read\_string 

```C++
inline std::string endstone::nbt::detail::read_string (
    std::string_view & in,
    std::endian byte_order,
    bool network
) 
```




<hr>



### function read\_tag\_payload 

```C++
inline Tag endstone::nbt::detail::read_tag_payload (
    std::string_view & in,
    Type type,
    std::endian byte_order,
    bool network
) 
```




<hr>



### function read\_value 

```C++
template<typename T>
T endstone::nbt::detail::read_value (
    std::string_view & in,
    std::endian byte_order
) 
```




<hr>



### function read\_varint 

```C++
inline std::uint32_t endstone::nbt::detail::read_varint (
    std::string_view & in
) 
```




<hr>



### function read\_varint64 

```C++
inline std::uint64_t endstone::nbt::detail::read_varint64 (
    std::string_view & in
) 
```




<hr>



### function read\_zigzag32 

```C++
inline std::int32_t endstone::nbt::detail::read_zigzag32 (
    std::string_view & in
) 
```




<hr>



### function read\_zigzag64 

```C++
inline std::int64_t endstone::nbt::detail::read_zigzag64 (
    std::string_view & in
) 
```




<hr>



### function write\_bytes 

```C++
inline void endstone::nbt::detail::write_bytes (
    std::string & out,
    const  void * data,
    std::size_t n
) 
```




<hr>



### function write\_length 

```C++
inline void endstone::nbt::detail::write_length (
    std::string & out,
    std::int32_t len,
    std::endian byte_order,
    bool network
) 
```




<hr>



### function write\_string 

```C++
inline void endstone::nbt::detail::write_string (
    std::string & out,
    const std::string & str,
    std::endian byte_order,
    bool network
) 
```




<hr>



### function write\_tag\_payload 

```C++
inline void endstone::nbt::detail::write_tag_payload (
    std::string & out,
    const  Tag & tag,
    std::endian byte_order,
    bool network
) 
```




<hr>



### function write\_value 

```C++
template<typename T>
void endstone::nbt::detail::write_value (
    std::string & out,
    T value,
    std::endian byte_order
) 
```




<hr>



### function write\_varint 

```C++
inline void endstone::nbt::detail::write_varint (
    std::string & out,
    std::uint32_t value
) 
```




<hr>



### function write\_varint64 

```C++
inline void endstone::nbt::detail::write_varint64 (
    std::string & out,
    std::uint64_t value
) 
```




<hr>



### function write\_zigzag32 

```C++
inline void endstone::nbt::detail::write_zigzag32 (
    std::string & out,
    std::int32_t value
) 
```




<hr>



### function write\_zigzag64 

```C++
inline void endstone::nbt::detail::write_zigzag64 (
    std::string & out,
    std::int64_t value
) 
```




<hr>



### function zigzag\_decode32 

```C++
inline std::int32_t endstone::nbt::detail::zigzag_decode32 (
    std::uint32_t value
) 
```




<hr>



### function zigzag\_decode64 

```C++
inline std::int64_t endstone::nbt::detail::zigzag_decode64 (
    std::uint64_t value
) 
```




<hr>



### function zigzag\_encode32 

```C++
inline std::uint32_t endstone::nbt::detail::zigzag_encode32 (
    std::int32_t value
) 
```




<hr>



### function zigzag\_encode64 

```C++
inline std::uint64_t endstone::nbt::detail::zigzag_encode64 (
    std::int64_t value
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/nbt/io.h`

