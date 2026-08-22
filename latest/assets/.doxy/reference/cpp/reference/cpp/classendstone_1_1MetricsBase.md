

# Class endstone::MetricsBase



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**MetricsBase**](classendstone_1_1MetricsBase.md)



_Collects and submits the data behind a_ [_**Metrics**_](classendstone_1_1Metrics.md) _instance._[More...](#detailed-description)

* `#include <endstone/metrics/base.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**MetricsBase**](#function-metricsbase-12) () = default<br> |
|   | [**MetricsBase**](#function-metricsbase-22) ([**const**](classendstone_1_1Identifier.md) [**MetricsBase**](classendstone_1_1MetricsBase.md) &) = delete<br> |
| virtual [**void**](classendstone_1_1Identifier.md) | [**addCustomChart**](#function-addcustomchart) (std::unique\_ptr&lt; [**CustomChart**](classendstone_1_1CustomChart.md) &gt; chart) = 0<br>_Adds a custom chart._  |
|  [**MetricsBase**](classendstone_1_1MetricsBase.md) & | [**operator=**](#function-operator) ([**const**](classendstone_1_1Identifier.md) [**MetricsBase**](classendstone_1_1MetricsBase.md) &) = delete<br> |
| virtual [**void**](classendstone_1_1Identifier.md) | [**shutdown**](#function-shutdown) () noexcept = 0<br>_Stops collecting and submitting data._  |
| virtual  | [**~MetricsBase**](#function-metricsbase) () = default<br> |




























## Detailed Description


Plugins hold a [**Metrics**](classendstone_1_1Metrics.md) rather than implementing this interface. 


    
## Public Functions Documentation




### function MetricsBase [1/2]

```C++
endstone::MetricsBase::MetricsBase () = default
```




<hr>



### function MetricsBase [2/2]

```C++
endstone::MetricsBase::MetricsBase (
    const  MetricsBase &
) = delete
```




<hr>



### function addCustomChart 

_Adds a custom chart._ 
```C++
virtual void endstone::MetricsBase::addCustomChart (
    std::unique_ptr< CustomChart > chart
) = 0
```





**Parameters:**


* `chart` the chart to add 




        

<hr>



### function operator= 

```C++
MetricsBase & endstone::MetricsBase::operator= (
    const  MetricsBase &
) = delete
```




<hr>



### function shutdown 

_Stops collecting and submitting data._ 
```C++
virtual void endstone::MetricsBase::shutdown () noexcept = 0
```




<hr>



### function ~MetricsBase 

```C++
virtual endstone::MetricsBase::~MetricsBase () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/metrics/base.h`

