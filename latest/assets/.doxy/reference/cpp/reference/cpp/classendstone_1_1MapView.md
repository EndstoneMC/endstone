

# Class endstone::MapView



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**MapView**](classendstone_1_1MapView.md)



_Represents a map item._ 

* `#include <endstone/map/map_view.h>`

















## Public Types

| Type | Name |
| ---: | :--- |
| enum uint8\_t | [**Scale**](#enum-scale)  <br>_An enum representing all possible scales a map can be set to._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
| virtual void | [**addRenderer**](#function-addrenderer) (std::shared\_ptr&lt; [**MapRenderer**](classendstone_1_1MapRenderer.md) &gt; renderer) = 0<br>_Add a renderer to this map._  |
| virtual int | [**getCenterX**](#function-getcenterx) () const = 0<br>_Get the center X position of this map._  |
| virtual int | [**getCenterZ**](#function-getcenterz) () const = 0<br>_Get the center Z position of this map._  |
| virtual [**Dimension**](classendstone_1_1Dimension.md) \* | [**getDimension**](#function-getdimension) () const = 0<br>_Get the dimension that this map is associated with._  |
| virtual std::int64\_t | [**getId**](#function-getid) () const = 0<br>_Get the ID of this map item for use with_ [_**MapMeta**_](classendstone_1_1MapMeta.md) _._ |
| virtual std::vector&lt; [**MapRenderer**](classendstone_1_1MapRenderer.md) \* &gt; | [**getRenderers**](#function-getrenderers) () const = 0<br>_Get a list of MapRenderers currently in effect._  |
| virtual [**Scale**](classendstone_1_1MapView.md#enum-scale) | [**getScale**](#function-getscale) () const = 0<br>_Get the scale of this map._  |
| virtual bool | [**isLocked**](#function-islocked) () const = 0<br>_Gets whether the map is locked or not. A locked map may not be explored further._  |
| virtual bool | [**isUnlimitedTracking**](#function-isunlimitedtracking) () const = 0<br>_Whether the map will show a smaller position cursor (true), or no position cursor (false) when cursor is outside of map's range._  |
| virtual bool | [**isVirtual**](#function-isvirtual) () const = 0<br>_Check whether this map is virtual._  |
| virtual bool | [**removeRenderer**](#function-removerenderer) ([**MapRenderer**](classendstone_1_1MapRenderer.md) \* renderer) = 0<br>_Remove a renderer from this map._  |
| virtual void | [**setCenterX**](#function-setcenterx) (int x) = 0<br>_Set the center X position of this map._  |
| virtual void | [**setCenterZ**](#function-setcenterz) (int z) = 0<br>_Set the center Z position of this map._  |
| virtual void | [**setDimension**](#function-setdimension) (const [**Dimension**](classendstone_1_1Dimension.md) & dimension) = 0<br>_Set the dimension that this map is associated with._  |
| virtual void | [**setLocked**](#function-setlocked) (bool locked) = 0<br>_Sets whether the map is locked or not. A locked map may not be explored further._  |
| virtual void | [**setScale**](#function-setscale) ([**Scale**](classendstone_1_1MapView.md#enum-scale) scale) = 0<br>_Set the scale of this map._  |
| virtual void | [**setUnlimitedTracking**](#function-setunlimitedtracking) (bool unlimited) = 0<br>_Whether the map will show a smaller position cursor (true), or no position cursor (false) when cursor is outside of map's range._  |
| virtual  | [**~MapView**](#function-mapview) () = default<br> |




























## Public Types Documentation




### enum Scale 

_An enum representing all possible scales a map can be set to._ 
```C++
enum endstone::MapView::Scale {
    Closest = 0,
    Close = 1,
    Normal = 2,
    Far = 3,
    Farthest = 4
};
```




<hr>
## Public Functions Documentation




### function addRenderer 

_Add a renderer to this map._ 
```C++
virtual void endstone::MapView::addRenderer (
    std::shared_ptr< MapRenderer > renderer
) = 0
```





**Parameters:**


* `renderer` The [**MapRenderer**](classendstone_1_1MapRenderer.md) to add. 




        

<hr>



### function getCenterX 

_Get the center X position of this map._ 
```C++
virtual int endstone::MapView::getCenterX () const = 0
```





**Returns:**

The center X position. 





        

<hr>



### function getCenterZ 

_Get the center Z position of this map._ 
```C++
virtual int endstone::MapView::getCenterZ () const = 0
```





**Returns:**

The center Z position. 





        

<hr>



### function getDimension 

_Get the dimension that this map is associated with._ 
```C++
virtual Dimension * endstone::MapView::getDimension () const = 0
```





**Returns:**

Pointer to the World this map is associated with, or nullptr. 





        

<hr>



### function getId 

_Get the ID of this map item for use with_ [_**MapMeta**_](classendstone_1_1MapMeta.md) _._
```C++
virtual std::int64_t endstone::MapView::getId () const = 0
```





**Returns:**

The unique ID of the map. 





        

<hr>



### function getRenderers 

_Get a list of MapRenderers currently in effect._ 
```C++
virtual std::vector< MapRenderer * > endstone::MapView::getRenderers () const = 0
```





**Returns:**

A list of references to each [**MapRenderer**](classendstone_1_1MapRenderer.md). 





        

<hr>



### function getScale 

_Get the scale of this map._ 
```C++
virtual Scale endstone::MapView::getScale () const = 0
```





**Returns:**

The scale of the map. 





        

<hr>



### function isLocked 

_Gets whether the map is locked or not. A locked map may not be explored further._ 
```C++
virtual bool endstone::MapView::isLocked () const = 0
```





**Returns:**

True if the map is locked. 





        

<hr>



### function isUnlimitedTracking 

_Whether the map will show a smaller position cursor (true), or no position cursor (false) when cursor is outside of map's range._ 
```C++
virtual bool endstone::MapView::isUnlimitedTracking () const = 0
```





**Returns:**

True if unlimited tracking is enabled. 





        

<hr>



### function isVirtual 

_Check whether this map is virtual._ 
```C++
virtual bool endstone::MapView::isVirtual () const = 0
```



A map is virtual if its lowermost [**MapRenderer**](classendstone_1_1MapRenderer.md) is plugin-provided.




**Returns:**

True if the map is virtual. 





        

<hr>



### function removeRenderer 

_Remove a renderer from this map._ 
```C++
virtual bool endstone::MapView::removeRenderer (
    MapRenderer * renderer
) = 0
```





**Parameters:**


* `renderer` The [**MapRenderer**](classendstone_1_1MapRenderer.md) to remove.



**Returns:**

True if the renderer was successfully removed. 





        

<hr>



### function setCenterX 

_Set the center X position of this map._ 
```C++
virtual void endstone::MapView::setCenterX (
    int x
) = 0
```





**Parameters:**


* `x` The center X position. 




        

<hr>



### function setCenterZ 

_Set the center Z position of this map._ 
```C++
virtual void endstone::MapView::setCenterZ (
    int z
) = 0
```





**Parameters:**


* `z` The center Z position. 




        

<hr>



### function setDimension 

_Set the dimension that this map is associated with._ 
```C++
virtual void endstone::MapView::setDimension (
    const Dimension & dimension
) = 0
```





**Parameters:**


* `dimension` The [**Dimension**](classendstone_1_1Dimension.md) to associate this map with. 




        

<hr>



### function setLocked 

_Sets whether the map is locked or not. A locked map may not be explored further._ 
```C++
virtual void endstone::MapView::setLocked (
    bool locked
) = 0
```





**Parameters:**


* `locked` True to lock the map. 




        

<hr>



### function setScale 

_Set the scale of this map._ 
```C++
virtual void endstone::MapView::setScale (
    Scale scale
) = 0
```





**Parameters:**


* `scale` The scale to set. 




        

<hr>



### function setUnlimitedTracking 

_Whether the map will show a smaller position cursor (true), or no position cursor (false) when cursor is outside of map's range._ 
```C++
virtual void endstone::MapView::setUnlimitedTracking (
    bool unlimited
) = 0
```





**Parameters:**


* `unlimited` True to enable unlimited tracking. 




        

<hr>



### function ~MapView 

```C++
virtual endstone::MapView::~MapView () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/map/map_view.h`

