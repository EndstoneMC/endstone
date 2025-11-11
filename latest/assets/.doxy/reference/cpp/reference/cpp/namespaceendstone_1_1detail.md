

# Namespace endstone::detail



[**Namespace List**](namespaces.md) **>** [**endstone**](namespaceendstone.md) **>** [**detail**](namespaceendstone_1_1detail.md)


























## Public Attributes

| Type | Name |
| ---: | :--- |
|  [**Return**](classendstone_1_1Identifier.md)(\*)(Arg...) | [**fp\_cast**](#variable-fp_cast)  <br>_Gets the original member function pointer from a detour member function pointer (const, no ref-qualifier)_  |
















## Public Functions

| Type | Name |
| ---: | :--- |
|  [**void**](classendstone_1_1Identifier.md) \* | [**fp\_cast**](#function-fp_cast) ([**Return**](classendstone_1_1Identifier.md)(\*)(Args...) fp) <br>_Cast a function pointer to void pointer._  |
|  [**void**](classendstone_1_1Identifier.md) \* | [**fp\_cast**](#function-fp_cast) ([**Return**](classendstone_1_1Identifier.md)(Class::\*)(Args...) fp) <br>_Cast a member function pointer to void pointer._  |
|  [**Server**](classendstone_1_1Server.md) & | [**getServer**](#function-getserver) () <br> |




























## Public Attributes Documentation




### variable fp\_cast 

_Gets the original member function pointer from a detour member function pointer (const, no ref-qualifier)_ 
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

```C++
inline Server & endstone::detail::getServer () 
```



Gets the current [**Server**](classendstone_1_1Server.md) singleton




**Returns:**

[**Server**](classendstone_1_1Server.md) instance being run 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/detail.h`

