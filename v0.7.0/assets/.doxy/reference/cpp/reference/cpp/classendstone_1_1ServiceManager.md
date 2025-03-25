

# Class endstone::ServiceManager



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ServiceManager**](classendstone_1_1ServiceManager.md)



_Represent a service manager that manages services and service providers._ [More...](#detailed-description)

* `#include <endstone/plugin/service_manager.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual std::shared\_ptr&lt; [**Service**](classendstone_1_1Service.md) &gt; | [**get**](#function-get) (std::string name) const = 0<br>_Queries for a provider. This may return null if no provider has been registered for a service. The highest priority provider is returned._  |
|  std::shared\_ptr&lt; [**T**](classendstone_1_1Vector.md) &gt; | [**load**](#function-load) (std::string name) const<br> |
| virtual [**void**](classendstone_1_1Vector.md) | [**registerService**](#function-registerservice) (std::string name, std::shared\_ptr&lt; [**Service**](classendstone_1_1Service.md) &gt; provider, [**const**](classendstone_1_1Vector.md) [**Plugin**](classendstone_1_1Plugin.md) & plugin, ServicePriority priority) = 0<br>_Register a provider of a service._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**unregister**](#function-unregister-12) (std::string name, [**const**](classendstone_1_1Vector.md) [**Service**](classendstone_1_1Service.md) & provider) = 0<br>_Unregister a particular provider for a particular service._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**unregister**](#function-unregister-22) ([**const**](classendstone_1_1Vector.md) [**Service**](classendstone_1_1Service.md) & provider) = 0<br>_Unregister a particular provider._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**unregisterAll**](#function-unregisterall) ([**const**](classendstone_1_1Vector.md) [**Plugin**](classendstone_1_1Plugin.md) & plugin) = 0<br>_Unregister all the services registered by a particular plugin._  |
| virtual  | [**~ServiceManager**](#function-servicemanager) () = default<br> |




























## Detailed Description


Services are an interface specifying a list of methods that a provider must implement. Providers are implementations of these services. 


    
## Public Functions Documentation




### function get 

_Queries for a provider. This may return null if no provider has been registered for a service. The highest priority provider is returned._ 
```C++
virtual std::shared_ptr< Service > endstone::ServiceManager::get (
    std::string name
) const = 0
```





**Parameters:**


* `name` The service name 



**Returns:**

provider or null 





        

<hr>



### function load 

```C++
template<typename  T>
inline std::shared_ptr< T > endstone::ServiceManager::load (
    std::string name
) const
```




<hr>



### function registerService 

_Register a provider of a service._ 
```C++
virtual void endstone::ServiceManager::registerService (
    std::string name,
    std::shared_ptr< Service > provider,
    const  Plugin & plugin,
    ServicePriority priority
) = 0
```





**Parameters:**


* `name` service name 
* `provider` service provider to register 
* `plugin` plugin associated with the service 
* `priority` priority of the provider 




        

<hr>



### function unregister [1/2]

_Unregister a particular provider for a particular service._ 
```C++
virtual void endstone::ServiceManager::unregister (
    std::string name,
    const  Service & provider
) = 0
```





**Parameters:**


* `name` The service name 
* `provider` The service provider implementation 




        

<hr>



### function unregister [2/2]

_Unregister a particular provider._ 
```C++
virtual void endstone::ServiceManager::unregister (
    const  Service & provider
) = 0
```





**Parameters:**


* `provider` The service provider implementation 




        

<hr>



### function unregisterAll 

_Unregister all the services registered by a particular plugin._ 
```C++
virtual void endstone::ServiceManager::unregisterAll (
    const  Plugin & plugin
) = 0
```





**Parameters:**


* `plugin` The plugin 




        

<hr>



### function ~ServiceManager 

```C++
virtual endstone::ServiceManager::~ServiceManager () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/plugin/service_manager.h`

