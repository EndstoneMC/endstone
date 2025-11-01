

# Class endstone::detail::CommandBuilder



[**ClassList**](annotated.md) **>** [**CommandBuilder**](classendstone_1_1detail_1_1CommandBuilder.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**CommandBuilder**](#function-commandbuilder) (std::string name) <br> |
|  CommandBuilder & | [**aliases**](#function-aliases) (Alias... aliases) <br> |
|  [**Command**](classendstone_1_1Command.md) | [**build**](#function-build) () const<br> |
|  CommandBuilder & | [**description**](#function-description) (std::string description) <br> |
|  CommandBuilder & | [**permissions**](#function-permissions) (Permissions... permissions) <br> |
|  CommandBuilder & | [**usages**](#function-usages) (Usage... usages) <br> |




























## Public Functions Documentation




### function CommandBuilder 

```C++
inline explicit CommandBuilder::CommandBuilder (
    std::string name
) 
```




<hr>



### function aliases 

```C++
template<typename... Alias>
inline CommandBuilder & CommandBuilder::aliases (
    Alias... aliases
) 
```




<hr>



### function build 

```C++
inline Command CommandBuilder::build () const
```




<hr>



### function description 

```C++
inline CommandBuilder & CommandBuilder::description (
    std::string description
) 
```




<hr>



### function permissions 

```C++
template<typename... Permissions>
inline CommandBuilder & CommandBuilder::permissions (
    Permissions... permissions
) 
```




<hr>



### function usages 

```C++
template<typename... Usage>
inline CommandBuilder & CommandBuilder::usages (
    Usage... usages
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/plugin/plugin.h`

