

# Class endstone::Error



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Error**](classendstone_1_1Error.md)



_Represents an error with a message and the stack trace._ 

* `#include <endstone/util/error.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Error**](#function-error) (std::string\_view message, std::string\_view stack\_trace) <br> |
|  std::string\_view | [**getMessage**](#function-getmessage) () noexcept const<br>_Returns the error message._  |
|  std::string\_view | [**getStackTrace**](#function-getstacktrace) () noexcept const<br>_Returns the stack trace associated with the error._  |




























## Public Functions Documentation




### function Error 

```C++
inline endstone::Error::Error (
    std::string_view message,
    std::string_view stack_trace
) 
```




<hr>



### function getMessage 

_Returns the error message._ 
```C++
inline std::string_view endstone::Error::getMessage () noexcept const
```





**Returns:**

The error message. 





        

<hr>



### function getStackTrace 

_Returns the stack trace associated with the error._ 
```C++
inline std::string_view endstone::Error::getStackTrace () noexcept const
```





**Returns:**

The stack trace associated with the error. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/util/error.h`

