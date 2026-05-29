

# Namespace endstone::detail



[**Namespace List**](namespaces.md) **>** [**endstone**](namespaceendstone.md) **>** [**detail**](namespaceendstone_1_1detail.md)




















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**JoinView**](structendstone_1_1detail_1_1JoinView.md) &lt;[**typename**](classendstone_1_1Identifier.md) [**It**](classendstone_1_1Identifier.md), [**typename**](classendstone_1_1Identifier.md) [**Sentinel**](classendstone_1_1Identifier.md)&gt;<br>_A lightweight, formattable view over a range that renders its elements separated by a delimiter._  |






## Public Attributes

| Type | Name |
| ---: | :--- |
|  [**Return**](classendstone_1_1Identifier.md)(\*)(Arg...) | [**fp\_cast**](#variable-fp_cast)  <br>_Gets the original member function pointer from a detour member function pointer (const, no ref-qualifier)._  |
















## Public Functions

| Type | Name |
| ---: | :--- |
|  [**void**](classendstone_1_1Identifier.md) \* | [**fp\_cast**](#function-fp_cast) ([**Return**](classendstone_1_1Identifier.md)(\*)(Args...) fp) <br>_Cast a function pointer to void pointer._  |
|  [**void**](classendstone_1_1Identifier.md) \* | [**fp\_cast**](#function-fp_cast) ([**Return**](classendstone_1_1Identifier.md)(Class::\*)(Args...) fp) <br>_Cast a member function pointer to void pointer._  |
|  [**Server**](classendstone_1_1Server.md) & | [**getServer**](#function-getserver) () <br>_Gets the current_ [_**Server**_](classendstone_1_1Server.md) _singleton._ |
|  [**JoinView**](structendstone_1_1detail_1_1JoinView.md)&lt; [**It**](classendstone_1_1Identifier.md), [**Sentinel**](classendstone_1_1Identifier.md) &gt; | [**join**](#function-join) ([**It**](classendstone_1_1Identifier.md) begin, [**Sentinel**](classendstone_1_1Identifier.md) end, std::string\_view sep) <br> |
|  [**auto**](classendstone_1_1Identifier.md) | [**join**](#function-join) ([**const**](classendstone_1_1Identifier.md) [**Range**](classendstone_1_1Identifier.md) & range, std::string\_view sep) <br> |
|  std::string | [**quoted**](#function-quoted) (std::string\_view value) <br>_Quotes and escapes a string the way the_ `{:?}` _debug presentation does._ |




























## Public Attributes Documentation




### variable fp\_cast 

_Gets the original member function pointer from a detour member function pointer (const, no ref-qualifier)._ 
```C++
Return(Class::*)(Arg...) endstone::detail::fp_cast(Return(Class::*fp)(Arg...) const, void *address);
```




<hr>
## Public Functions Documentation




### function fp\_cast 

_Cast a function pointer to void pointer._ 
```C++
template<typename Return, typename... Args>
void * endstone::detail::fp_cast (
    Return (*)(Args...) fp
) 
```





**Template parameters:**


* `Return` The return type of the function. 
* `Args` The argument types of the function.



**Parameters:**


* `fp` The function pointer to be casted.



**Returns:**

A void pointer to the function. 





        

<hr>



### function fp\_cast 

_Cast a member function pointer to void pointer._ 
```C++
template<typename Return, typename Class, typename... Args>
void * endstone::detail::fp_cast (
    Return (Class::*)(Args...) fp
) 
```



Cast a constant member function pointer to void pointer.




**Template parameters:**


* `Return` The return type of the member function. 
* `Class` The class type that the member function belongs to. 
* `Args` The argument types of the member function.



**Parameters:**


* `fp` Pointer to the member function to be casted.



**Returns:**

A void pointer to the member function.




**Template parameters:**


* `Return` The return type of the constant member function. 
* `Class` The class type that the constant member function belongs to. 
* `Args` The argument types of the constant member function.



**Parameters:**


* `fp` Pointer to the constant member function to be casted.



**Returns:**

A void pointer to the constant member function. 





        

<hr>



### function getServer 

_Gets the current_ [_**Server**_](classendstone_1_1Server.md) _singleton._
```C++
inline Server & endstone::detail::getServer () 
```





**Returns:**

[**Server**](classendstone_1_1Server.md) instance being run 





        

<hr>



### function join 

```C++
template<typename It, typename Sentinel>
JoinView < It , Sentinel > endstone::detail::join (
    It begin,
    Sentinel end,
    std::string_view sep
) 
```




<hr>



### function join 

```C++
template<typename Range>
auto endstone::detail::join (
    const  Range & range,
    std::string_view sep
) 
```




<hr>



### function quoted 

_Quotes and escapes a string the way the_ `{:?}` _debug presentation does._
```C++
inline std::string endstone::detail::quoted (
    std::string_view value
) 
```



`std::format` only gained the `?` (debug) specifier in C++23, so this provides equivalent behaviour under C++20. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/detail.h`

