

# Namespace endstone::detail



[**Namespace List**](namespaces.md) **>** [**endstone**](namespaceendstone.md) **>** [**detail**](namespaceendstone_1_1detail.md)




















## Classes

| Type | Name |
| ---: | :--- |
| class | [**CommandBuilder**](classendstone_1_1detail_1_1CommandBuilder.md) <br> |
| struct | [**JoinView**](structendstone_1_1detail_1_1JoinView.md) &lt;typename It, typename Sentinel&gt;<br> |
| class | [**PermissionBuilder**](classendstone_1_1detail_1_1PermissionBuilder.md) <br> |
| struct | [**PluginDescriptionBuilder**](structendstone_1_1detail_1_1PluginDescriptionBuilder.md) <br> |






## Public Attributes

| Type | Name |
| ---: | :--- |
|  Return(\*)(Arg...) | [**fp\_cast**](#variable-fp_cast)  <br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|  void \* | [**fp\_cast**](#function-fp_cast) (Return(\*)(Args...) fp) <br>_Cast a function pointer to void pointer._  |
|  void \* | [**fp\_cast**](#function-fp_cast) (Return(Class::\*)(Args...) fp) <br>_Cast a member function pointer to void pointer._  |
|  auto | [**fp\_cast**](#function-fp_cast) (Return(Class::\*)(Arg...), void \* address) <br>_Gets the original member function pointer from a detour member function pointer (const, no ref-qualifier)._  |
|  [**Server**](classendstone_1_1Server.md) & | [**getServer**](#function-getserver) () <br> |
|  [**JoinView**](structendstone_1_1detail_1_1JoinView.md)&lt; It, Sentinel &gt; | [**join**](#function-join) (It begin, Sentinel end, std::string\_view sep) <br> |
|  auto | [**join**](#function-join) (const Range & range, std::string\_view sep) <br> |
|  std::string | [**quoted**](#function-quoted) (std::string\_view value) <br> |




























## Public Attributes Documentation




### variable fp\_cast 

```C++
Return(*)(Arg...) endstone::detail::fp_cast(Return(*fp)(Arg...), void *func);
```




<hr>
## Public Functions Documentation




### function fp\_cast 

_Cast a function pointer to void pointer._ 
```C++
template<typename Return, typename... Args>
void * endstone::detail::fp_cast (
    Return(*)(Args...) fp
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
    Return(Class::*)(Args...) fp
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



### function fp\_cast 

_Gets the original member function pointer from a detour member function pointer (const, no ref-qualifier)._ 
```C++
template<typename Return, typename Class, typename... Arg>
auto endstone::detail::fp_cast (
    Return(Class::*)(Arg...),
    void * address
) 
```




<hr>



### function getServer 

```C++
inline Server & endstone::detail::getServer () 
```



Gets the current [**Server**](classendstone_1_1Server.md) singleton




**Returns:**

[**Server**](classendstone_1_1Server.md) instance being run 





        

<hr>



### function join 

```C++
template<typename It, typename Sentinel>
JoinView < It, Sentinel > endstone::detail::join (
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
    const Range & range,
    std::string_view sep
) 
```




<hr>



### function quoted 

```C++
inline std::string endstone::detail::quoted (
    std::string_view value
) 
```



Quotes and escapes a string the way the `{:?}` debug presentation does.


`std::format` only gained the `?` (debug) specifier in C++23, so this provides equivalent behaviour under C++20. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/detail.h`

