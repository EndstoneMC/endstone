

# Class endstone::Metrics



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Metrics**](classendstone_1_1Metrics.md)



_Collects and submits anonymous usage statistics for a plugin to bStats._ [More...](#detailed-description)

* `#include <endstone/metrics/metrics.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Metrics**](#function-metrics) ([**Plugin**](classendstone_1_1Plugin.md) & plugin, [**int**](classendstone_1_1Identifier.md) service\_id) <br>_Creates a new_ [_**Metrics**_](classendstone_1_1Metrics.md) _instance._ |
|  [**void**](classendstone_1_1Identifier.md) | [**addCustomChart**](#function-addcustomchart) (std::unique\_ptr&lt; [**CustomChart**](classendstone_1_1CustomChart.md) &gt; chart) <br>_Adds a custom chart._  |
|  [**void**](classendstone_1_1Identifier.md) | [**shutdown**](#function-shutdown) () noexcept<br>_Stops collecting and submitting data._  |




























## Detailed Description


The server owns the metrics behind this handle and retires them when it reloads or shuts down, so a plugin may add its charts in [**Plugin::onEnable**](classendstone_1_1Plugin.md#function-onenable) and drop the handle. Asking twice for the same service id gives back the same metrics. 


    
## Public Functions Documentation




### function Metrics 

_Creates a new_ [_**Metrics**_](classendstone_1_1Metrics.md) _instance._
```C++
inline endstone::Metrics::Metrics (
    Plugin & plugin,
    int service_id
) 
```





**Parameters:**


* `plugin` the plugin these metrics belong to 
* `service_id` the id of the service, found at [https://bstats.org/what-is-my-plugin-id](https://bstats.org/what-is-my-plugin-id) 




        

<hr>



### function addCustomChart 

_Adds a custom chart._ 
```C++
inline void endstone::Metrics::addCustomChart (
    std::unique_ptr< CustomChart > chart
) 
```





**Parameters:**


* `chart` the chart to add 




        

<hr>



### function shutdown 

_Stops collecting and submitting data._ 
```C++
inline void endstone::Metrics::shutdown () noexcept
```



Repeated calls have no effect. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/metrics/metrics.h`

