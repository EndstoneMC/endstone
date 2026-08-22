

# Class endstone::SingleLineChart



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**SingleLineChart**](classendstone_1_1SingleLineChart.md)



_A bStats line chart with a single line._ 

* `#include <endstone/metrics/single_line_chart.h>`



Inherits the following classes: [endstone::CustomChart](classendstone_1_1CustomChart.md)














## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::function&lt; [**int**](classendstone_1_1Identifier.md)()&gt; | [**Callback**](#typedef-callback)  <br> |








































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**SingleLineChart**](#function-singlelinechart) (std::string chart\_id, Callback get\_value) <br> |
| virtual std::optional&lt; JsonObject &gt; | [**getChartData**](#function-getchartdata) () override<br>_Gets the data for this chart._  |


## Public Functions inherited from endstone::CustomChart

See [endstone::CustomChart](classendstone_1_1CustomChart.md)

| Type | Name |
| ---: | :--- |
|   | [**CustomChart**](classendstone_1_1CustomChart.md#function-customchart-13) (std::string chart\_id) <br>_Creates a chart with the given bStats chart id._  |
|   | [**CustomChart**](classendstone_1_1CustomChart.md#function-customchart-23) ([**const**](classendstone_1_1Identifier.md) [**CustomChart**](classendstone_1_1CustomChart.md) &) = delete<br> |
|   | [**CustomChart**](classendstone_1_1CustomChart.md#function-customchart-33) ([**CustomChart**](classendstone_1_1CustomChart.md) &&) = delete<br> |
| virtual std::optional&lt; JsonObject &gt; | [**getChartData**](classendstone_1_1CustomChart.md#function-getchartdata) () = 0<br>_Gets the data for this chart._  |
|  [**const**](classendstone_1_1Identifier.md) std::string & | [**getChartId**](classendstone_1_1CustomChart.md#function-getchartid) () noexcept const<br>_Gets the id of this chart._  |
|  [**CustomChart**](classendstone_1_1CustomChart.md) & | [**operator=**](classendstone_1_1CustomChart.md#function-operator) ([**const**](classendstone_1_1Identifier.md) [**CustomChart**](classendstone_1_1CustomChart.md) &) = delete<br> |
|  [**CustomChart**](classendstone_1_1CustomChart.md) & | [**operator=**](classendstone_1_1CustomChart.md#function-operator_1) ([**CustomChart**](classendstone_1_1CustomChart.md) &&) = delete<br> |
| virtual  | [**~CustomChart**](classendstone_1_1CustomChart.md#function-customchart) () = default<br> |






















































## Public Types Documentation




### typedef Callback 

```C++
using endstone::SingleLineChart::Callback =  std::function<int()>;
```




<hr>
## Public Functions Documentation




### function SingleLineChart 

```C++
inline endstone::SingleLineChart::SingleLineChart (
    std::string chart_id,
    Callback get_value
) 
```




<hr>



### function getChartData 

_Gets the data for this chart._ 
```C++
inline virtual std::optional< JsonObject > endstone::SingleLineChart::getChartData () override
```



This is called on the primary server thread. Returning no value omits the chart from the submission.




**Returns:**

the chart data, or no value to skip this chart 





        
Implements [*endstone::CustomChart::getChartData*](classendstone_1_1CustomChart.md#function-getchartdata)


<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/metrics/single_line_chart.h`

