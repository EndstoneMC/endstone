

# Class endstone::Preconditions



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Preconditions**](classendstone_1_1Preconditions.md)












































## Public Static Functions

| Type | Name |
| ---: | :--- |
|  void | [**checkArgument**](#function-checkargument) (bool condition, std::format\_string&lt; Args... &gt; format, Args &&... args) <br> |
|  void | [**checkState**](#function-checkstate) (bool condition, std::format\_string&lt; Args... &gt; format, Args &&... args) <br> |


























## Public Static Functions Documentation




### function checkArgument 

```C++
template<typename... Args>
static inline void endstone::Preconditions::checkArgument (
    bool condition,
    std::format_string< Args... > format,
    Args &&... args
) 
```




<hr>



### function checkState 

```C++
template<typename... Args>
static inline void endstone::Preconditions::checkState (
    bool condition,
    std::format_string< Args... > format,
    Args &&... args
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/check.h`

