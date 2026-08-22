

# Class endstone::CustomChart



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**CustomChart**](classendstone_1_1CustomChart.md)



_Represents a custom chart._ 

* `#include <endstone/metrics/custom_chart.h>`





Inherited by the following classes: [endstone::AdvancedBarChart](classendstone_1_1AdvancedBarChart.md),  [endstone::AdvancedPie](classendstone_1_1AdvancedPie.md),  [endstone::DrilldownPie](classendstone_1_1DrilldownPie.md),  [endstone::MultiLineChart](classendstone_1_1MultiLineChart.md),  [endstone::SimpleBarChart](classendstone_1_1SimpleBarChart.md),  [endstone::SimplePie](classendstone_1_1SimplePie.md),  [endstone::SingleLineChart](classendstone_1_1SingleLineChart.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**CustomChart**](#function-customchart-13) (std::string chart\_id) <br>_Creates a chart with the given bStats chart id._  |
|   | [**CustomChart**](#function-customchart-23) ([**const**](classendstone_1_1Identifier.md) [**CustomChart**](classendstone_1_1CustomChart.md) &) = delete<br> |
|   | [**CustomChart**](#function-customchart-33) ([**CustomChart**](classendstone_1_1CustomChart.md) &&) = delete<br> |
| virtual std::optional&lt; JsonObject &gt; | [**getChartData**](#function-getchartdata) () = 0<br>_Gets the data for this chart._  |
|  [**const**](classendstone_1_1Identifier.md) std::string & | [**getChartId**](#function-getchartid) () noexcept const<br>_Gets the id of this chart._  |
|  [**CustomChart**](classendstone_1_1CustomChart.md) & | [**operator=**](#function-operator) ([**const**](classendstone_1_1Identifier.md) [**CustomChart**](classendstone_1_1CustomChart.md) &) = delete<br> |
|  [**CustomChart**](classendstone_1_1CustomChart.md) & | [**operator=**](#function-operator_1) ([**CustomChart**](classendstone_1_1CustomChart.md) &&) = delete<br> |
| virtual  | [**~CustomChart**](#function-customchart) () = default<br> |




























## Public Functions Documentation




### function CustomChart [1/3]

_Creates a chart with the given bStats chart id._ 
```C++
inline explicit endstone::CustomChart::CustomChart (
    std::string chart_id
) 
```





**Parameters:**


* `chart_id` the id of the chart 



**Exception:**


* `std::invalid_argument` if chart\_id is empty 




        

<hr>



### function CustomChart [2/3]

```C++
endstone::CustomChart::CustomChart (
    const  CustomChart &
) = delete
```




<hr>



### function CustomChart [3/3]

```C++
endstone::CustomChart::CustomChart (
    CustomChart &&
) = delete
```




<hr>



### function getChartData 

_Gets the data for this chart._ 
```C++
virtual std::optional< JsonObject > endstone::CustomChart::getChartData () = 0
```



This is called on the primary server thread. Returning no value omits the chart from the submission.




**Returns:**

the chart data, or no value to skip this chart 





        

<hr>



### function getChartId 

_Gets the id of this chart._ 
```C++
inline const std::string & endstone::CustomChart::getChartId () noexcept const
```





**Returns:**

the id of this chart 





        

<hr>



### function operator= 

```C++
CustomChart & endstone::CustomChart::operator= (
    const  CustomChart &
) = delete
```




<hr>



### function operator= 

```C++
CustomChart & endstone::CustomChart::operator= (
    CustomChart &&
) = delete
```




<hr>



### function ~CustomChart 

```C++
virtual endstone::CustomChart::~CustomChart () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/metrics/custom_chart.h`

