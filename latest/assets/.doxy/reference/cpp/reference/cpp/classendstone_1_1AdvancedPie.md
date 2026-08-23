

# Class endstone::AdvancedPie



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**AdvancedPie**](classendstone_1_1AdvancedPie.md)



_A bStats pie chart with named values._ 

* `#include <endstone/metrics/advanced_pie.h>`



Inherits the following classes: [endstone::CustomChart](classendstone_1_1CustomChart.md)














## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::function&lt; std::optional&lt; std::unordered\_map&lt; std::string, [**int**](classendstone_1_1Identifier.md) &gt; &gt;()&gt; | [**Callback**](#typedef-callback)  <br> |








































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**AdvancedPie**](#function-advancedpie) (std::string chart\_id, Callback get\_values) <br> |
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
using endstone::AdvancedPie::Callback =  std::function<std::optional<std::unordered_map<std::string, int> >()>;
```




<hr>
## Public Functions Documentation




### function AdvancedPie 

```C++
inline endstone::AdvancedPie::AdvancedPie (
    std::string chart_id,
    Callback get_values
) 
```




<hr>



### function getChartData 

_Gets the data for this chart._ 
```C++
inline virtual std::optional< JsonObject > endstone::AdvancedPie::getChartData () override
```



This is called on the primary server thread. Returning no value omits the chart from the submission.




**Returns:**

the chart data, or no value to skip this chart 





        
Implements [*endstone::CustomChart::getChartData*](classendstone_1_1CustomChart.md#function-getchartdata)


<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/metrics/advanced_pie.h`

