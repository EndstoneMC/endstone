

# Class endstone::Preconditions



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Preconditions**](classendstone_1_1Preconditions.md)



_Static helpers for validating method arguments and object state._ 

* `#include <endstone/check.h>`







































## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**void**](classendstone_1_1Identifier.md) | [**checkArgument**](#function-checkargument) ([**bool**](classendstone_1_1Identifier.md) condition, std::format\_string&lt; Args... &gt; format, [**Args**](classendstone_1_1Identifier.md) &&... args) <br>_Ensures the truth of an expression involving one or more parameters to the calling method._  |
|  [**void**](classendstone_1_1Identifier.md) | [**checkState**](#function-checkstate) ([**bool**](classendstone_1_1Identifier.md) condition, std::format\_string&lt; Args... &gt; format, [**Args**](classendstone_1_1Identifier.md) &&... args) <br>_Ensures the truth of an expression involving the state of the calling instance._  |


























## Public Static Functions Documentation




### function checkArgument 

_Ensures the truth of an expression involving one or more parameters to the calling method._ 
```C++
template<typename... Args>
static inline void endstone::Preconditions::checkArgument (
    bool condition,
    std::format_string< Args... > format,
    Args &&... args
) 
```





**Parameters:**


* `condition` The expression that must hold. 
* `format` A format string describing the failure, used when the condition is false. 
* `args` Arguments to substitute into the format string. 



**Exception:**


* `std::invalid_argument` if condition is false. 




        

<hr>



### function checkState 

_Ensures the truth of an expression involving the state of the calling instance._ 
```C++
template<typename... Args>
static inline void endstone::Preconditions::checkState (
    bool condition,
    std::format_string< Args... > format,
    Args &&... args
) 
```





**Parameters:**


* `condition` The expression that must hold. 
* `format` A format string describing the failure, used when the condition is false. 
* `args` Arguments to substitute into the format string. 



**Exception:**


* `std::runtime_error` if condition is false. 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/check.h`

