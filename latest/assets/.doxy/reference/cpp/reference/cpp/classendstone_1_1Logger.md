

# Class endstone::Logger



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Logger**](classendstone_1_1Logger.md)



[_**Logger**_](classendstone_1_1Logger.md) _class which can format and output varies levels of logs._

* `#include <endstone/logger.h>`

















## Public Types

| Type | Name |
| ---: | :--- |
| enum std::uint8\_t | [**Level**](#enum-level)  <br>_Specifies the log level._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Logger**](#function-logger) () = default<br> |
|  void | [**critical**](#function-critical-12) (const std::string\_view message) const<br> |
|  void | [**critical**](#function-critical-22) (const fmt::format\_string&lt; Args... &gt; format, Args &&... args) const<br> |
|  void | [**debug**](#function-debug-12) (const std::string\_view message) const<br> |
|  void | [**debug**](#function-debug-22) (const fmt::format\_string&lt; Args... &gt; format, Args &&... args) const<br> |
|  void | [**error**](#function-error-12) (const std::string\_view message) const<br> |
|  void | [**error**](#function-error-22) (const fmt::format\_string&lt; Args... &gt; format, Args &&... args) const<br> |
| virtual std::string\_view | [**getName**](#function-getname) () const = 0<br>_Get the name of this_ [_**Logger**_](classendstone_1_1Logger.md) _instance._ |
|  void | [**info**](#function-info-12) (const std::string\_view message) const<br> |
|  void | [**info**](#function-info-22) (const fmt::format\_string&lt; Args... &gt; format, Args &&... args) const<br> |
| virtual bool | [**isEnabledFor**](#function-isenabledfor) ([**Level**](classendstone_1_1Logger.md#enum-level) level) const = 0<br>_Check if the_ [_**Logger**_](classendstone_1_1Logger.md) _instance is enabled for the given log_[_**Level**_](classendstone_1_1Level.md) _._ |
| virtual void | [**log**](#function-log-12) ([**Level**](classendstone_1_1Logger.md#enum-level) level, std::string\_view message) const = 0<br>_Log a message at the given level._  |
|  void | [**log**](#function-log-22) ([**Level**](classendstone_1_1Logger.md#enum-level) level, const fmt::format\_string&lt; Args... &gt; format, Args &&... args) const<br> |
| virtual void | [**setLevel**](#function-setlevel) ([**Level**](classendstone_1_1Logger.md#enum-level) level) = 0<br>_Set the logging level for this_ [_**Logger**_](classendstone_1_1Logger.md) _instance._ |
|  void | [**trace**](#function-trace-12) (const std::string\_view message) const<br> |
|  void | [**trace**](#function-trace-22) (const fmt::format\_string&lt; Args... &gt; format, Args &&... args) const<br> |
|  void | [**warning**](#function-warning-12) (const std::string\_view message) const<br> |
|  void | [**warning**](#function-warning-22) (const fmt::format\_string&lt; Args... &gt; format, Args &&... args) const<br> |
| virtual  | [**~Logger**](#function-logger) () = default<br> |




























## Public Types Documentation




### enum Level 

_Specifies the log level._ 
```C++
enum endstone::Logger::Level {
    Trace = 0,
    Debug = 1,
    Info = 2,
    Warning = 3,
    Error = 4,
    Critical = 5,
    Off = 6
};
```




<hr>
## Public Functions Documentation




### function Logger 

```C++
endstone::Logger::Logger () = default
```




<hr>



### function critical [1/2]

```C++
inline void endstone::Logger::critical (
    const std::string_view message
) const
```




<hr>



### function critical [2/2]

```C++
template<typename... Args, std::enable_if_t<(sizeof...(Args) > 0), int >>
inline void endstone::Logger::critical (
    const fmt::format_string< Args... > format,
    Args &&... args
) const
```




<hr>



### function debug [1/2]

```C++
inline void endstone::Logger::debug (
    const std::string_view message
) const
```




<hr>



### function debug [2/2]

```C++
template<typename... Args, std::enable_if_t<(sizeof...(Args) > 0), int >>
inline void endstone::Logger::debug (
    const fmt::format_string< Args... > format,
    Args &&... args
) const
```




<hr>



### function error [1/2]

```C++
inline void endstone::Logger::error (
    const std::string_view message
) const
```




<hr>



### function error [2/2]

```C++
template<typename... Args, std::enable_if_t<(sizeof...(Args) > 0), int >>
inline void endstone::Logger::error (
    const fmt::format_string< Args... > format,
    Args &&... args
) const
```




<hr>



### function getName 

_Get the name of this_ [_**Logger**_](classendstone_1_1Logger.md) _instance._
```C++
virtual std::string_view endstone::Logger::getName () const = 0
```





**Returns:**

The name of this [**Logger**](classendstone_1_1Logger.md) instance. 





        

<hr>



### function info [1/2]

```C++
inline void endstone::Logger::info (
    const std::string_view message
) const
```




<hr>



### function info [2/2]

```C++
template<typename... Args, std::enable_if_t<(sizeof...(Args) > 0), int >>
inline void endstone::Logger::info (
    const fmt::format_string< Args... > format,
    Args &&... args
) const
```




<hr>



### function isEnabledFor 

_Check if the_ [_**Logger**_](classendstone_1_1Logger.md) _instance is enabled for the given log_[_**Level**_](classendstone_1_1Level.md) _._
```C++
virtual bool endstone::Logger::isEnabledFor (
    Level level
) const = 0
```





**Parameters:**


* `level` The log level to check 



**Returns:**

true if the logger is enabled for the [**Level**](classendstone_1_1Level.md), false otherwise. 





        

<hr>



### function log [1/2]

_Log a message at the given level._ 
```C++
virtual void endstone::Logger::log (
    Level level,
    std::string_view message
) const = 0
```





**Parameters:**


* `level` The level at which the message should be logged. 
* `message` The message to log. 




        

<hr>



### function log [2/2]

```C++
template<typename... Args, std::enable_if_t<(sizeof...(Args) > 0), int >>
inline void endstone::Logger::log (
    Level level,
    const fmt::format_string< Args... > format,
    Args &&... args
) const
```




<hr>



### function setLevel 

_Set the logging level for this_ [_**Logger**_](classendstone_1_1Logger.md) _instance._
```C++
virtual void endstone::Logger::setLevel (
    Level level
) = 0
```





**Parameters:**


* `level` The desired log level 




        

<hr>



### function trace [1/2]

```C++
inline void endstone::Logger::trace (
    const std::string_view message
) const
```




<hr>



### function trace [2/2]

```C++
template<typename... Args, std::enable_if_t<(sizeof...(Args) > 0), int >>
inline void endstone::Logger::trace (
    const fmt::format_string< Args... > format,
    Args &&... args
) const
```




<hr>



### function warning [1/2]

```C++
inline void endstone::Logger::warning (
    const std::string_view message
) const
```




<hr>



### function warning [2/2]

```C++
template<typename... Args, std::enable_if_t<(sizeof...(Args) > 0), int >>
inline void endstone::Logger::warning (
    const fmt::format_string< Args... > format,
    Args &&... args
) const
```




<hr>



### function ~Logger 

```C++
virtual endstone::Logger::~Logger () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/logger.h`

