

# Class endstone::detail::CommandBuilder



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**detail**](namespaceendstone_1_1detail.md) **>** [**CommandBuilder**](classendstone_1_1detail_1_1CommandBuilder.md)










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**CommandBuilder**](#function-commandbuilder) (std::string name) <br> |
|  [**CommandBuilder**](classendstone_1_1detail_1_1CommandBuilder.md) & | [**aliases**](#function-aliases) (Alias... aliases) <br> |
|  [**Command**](classendstone_1_1Command.md) | [**build**](#function-build) () const<br> |
|  [**CommandBuilder**](classendstone_1_1detail_1_1CommandBuilder.md) & | [**description**](#function-description) (std::string description) <br> |
|  [**CommandBuilder**](classendstone_1_1detail_1_1CommandBuilder.md) & | [**permissions**](#function-permissions) (Permissions... permissions) <br> |
|  [**CommandBuilder**](classendstone_1_1detail_1_1CommandBuilder.md) & | [**usages**](#function-usages) (Usage... usages) <br> |




























## Public Functions Documentation




### function CommandBuilder 

```C++
inline explicit endstone::detail::CommandBuilder::CommandBuilder (
    std::string name
) 
```




<hr>



### function aliases 

```C++
template<typename... Alias>
inline CommandBuilder & endstone::detail::CommandBuilder::aliases (
    Alias... aliases
) 
```




<hr>



### function build 

```C++
inline Command endstone::detail::CommandBuilder::build () const
```




<hr>



### function description 

```C++
inline CommandBuilder & endstone::detail::CommandBuilder::description (
    std::string description
) 
```




<hr>



### function permissions 

```C++
template<typename... Permissions>
inline CommandBuilder & endstone::detail::CommandBuilder::permissions (
    Permissions... permissions
) 
```




<hr>



### function usages 

```C++
template<typename... Usage>
inline CommandBuilder & endstone::detail::CommandBuilder::usages (
    Usage... usages
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/plugin/plugin.h`

