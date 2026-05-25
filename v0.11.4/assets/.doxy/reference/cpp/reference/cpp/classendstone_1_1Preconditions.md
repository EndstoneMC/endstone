

# Class endstone::Preconditions



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Preconditions**](classendstone_1_1Preconditions.md)












































## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**void**](classendstone_1_1Identifier.md) | [**checkArgument**](#function-checkargument) ([**bool**](classendstone_1_1Identifier.md) condition, fmt::format\_string&lt; Args... &gt; format, [**Args**](classendstone_1_1Identifier.md) &&... args) <br> |
|  [**void**](classendstone_1_1Identifier.md) | [**checkState**](#function-checkstate) ([**bool**](classendstone_1_1Identifier.md) condition, fmt::format\_string&lt; Args... &gt; format, [**Args**](classendstone_1_1Identifier.md) &&... args) <br> |


























## Public Static Functions Documentation




### function checkArgument 

```C++
template<typename... Args>
static inline void endstone::Preconditions::checkArgument (
    bool condition,
    fmt::format_string< Args... > format,
    Args &&... args
) 
```




<hr>



### function checkState 

```C++
template<typename... Args>
static inline void endstone::Preconditions::checkState (
    bool condition,
    fmt::format_string< Args... > format,
    Args &&... args
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/check.h`

