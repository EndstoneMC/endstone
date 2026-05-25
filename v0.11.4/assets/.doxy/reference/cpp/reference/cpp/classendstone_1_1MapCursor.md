

# Class endstone::MapCursor



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**MapCursor**](classendstone_1_1MapCursor.md)



_Represents a cursor on a map._ 

* `#include <endstone/map/map_cursor.h>`

















## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Type**](#enum-type)  <br>_Represents the standard types of map cursors._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**MapCursor**](#function-mapcursor) (std::int8\_t x, std::int8\_t y, std::int8\_t direction, [**Type**](classendstone_1_1MapCursor.md#enum-type) type, [**bool**](classendstone_1_1Identifier.md) visible, std::string caption="") <br>_Initialize the map cursor._  |
|  std::string | [**getCaption**](#function-getcaption) () const<br>_Gets the caption on this cursor._  |
|  std::int8\_t | [**getDirection**](#function-getdirection) () const<br>_Get the direction of this cursor._  |
|  [**Type**](classendstone_1_1MapCursor.md#enum-type) | [**getType**](#function-gettype) () const<br>_Get the type of this cursor._  |
|  std::int8\_t | [**getX**](#function-getx) () const<br>_Get the X position of this cursor._  |
|  std::int8\_t | [**getY**](#function-gety) () const<br>_Get the Y position of this cursor._  |
|  [**bool**](classendstone_1_1Identifier.md) | [**isVisible**](#function-isvisible) () const<br>_Get the visibility status of this cursor._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setCaption**](#function-setcaption) (std::string caption) <br>_Sets the caption on this cursor._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setDirection**](#function-setdirection) (std::int8\_t direction) <br>_Set the direction of this cursor._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setType**](#function-settype) ([**Type**](classendstone_1_1MapCursor.md#enum-type) type) <br>_Set the type of this cursor._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setVisible**](#function-setvisible) ([**bool**](classendstone_1_1Identifier.md) visible) <br> |
|  [**void**](classendstone_1_1Identifier.md) | [**setX**](#function-setx) ([**const**](classendstone_1_1Identifier.md) std::int8\_t x) <br>_Set the X position of this cursor._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setY**](#function-sety) ([**const**](classendstone_1_1Identifier.md) std::int8\_t y) <br>_Set the Y position of this cursor._  |




























## Public Types Documentation




### enum Type 

_Represents the standard types of map cursors._ 
```C++
enum endstone::MapCursor::Type {
    Player = 0,
    Frame = 1,
    RedMarker = 2,
    BlueMarker = 3,
    TargetX = 4,
    TargetPoint = 5,
    PlayerOffMap = 6,
    SignMarker = 7,
    PinkMarker = 8,
    OrangeMarker = 9,
    YellowMarker = 10,
    CyanMarker = 11,
    GreenPoint = 12,
    PlayerOffLimits = 13,
    Mansion = 14,
    Monument = 15,
    VillageDesert = 17,
    VillagePlains = 18,
    VillageSavanna = 19,
    VillageSnowy = 20,
    VillageTaiga = 21,
    JungleTemple = 22,
    SwampHut = 23,
    TrialChambers = 24
};
```




<hr>
## Public Functions Documentation




### function MapCursor 

_Initialize the map cursor._ 
```C++
inline endstone::MapCursor::MapCursor (
    std::int8_t x,
    std::int8_t y,
    std::int8_t direction,
    Type type,
    bool visible,
    std::string caption=""
) 
```





**Parameters:**


* `x` The x coordinate, from -128 to 127. 
* `y` The y coordinate, from -128 to 127. 
* `direction` The facing of the cursor, from 0 to 15. 
* `type` The type (color/style) of the map cursor. 
* `visible` Whether the cursor is visible by default. 
* `caption` cursor caption 




        

<hr>



### function getCaption 

_Gets the caption on this cursor._ 
```C++
inline std::string endstone::MapCursor::getCaption () const
```





**Returns:**

caption 





        

<hr>



### function getDirection 

_Get the direction of this cursor._ 
```C++
inline std::int8_t endstone::MapCursor::getDirection () const
```





**Returns:**

The facing of the cursor, from 0 to 15. 





        

<hr>



### function getType 

_Get the type of this cursor._ 
```C++
inline Type endstone::MapCursor::getType () const
```





**Returns:**

The type (color/style) of the map cursor. 





        

<hr>



### function getX 

_Get the X position of this cursor._ 
```C++
inline std::int8_t endstone::MapCursor::getX () const
```





**Returns:**

The X coordinate. 





        

<hr>



### function getY 

_Get the Y position of this cursor._ 
```C++
inline std::int8_t endstone::MapCursor::getY () const
```





**Returns:**

The Y coordinate. 





        

<hr>



### function isVisible 

_Get the visibility status of this cursor._ 
```C++
inline bool endstone::MapCursor::isVisible () const
```





**Returns:**

True if visible, false otherwise. 





        

<hr>



### function setCaption 

_Sets the caption on this cursor._ 
```C++
inline void endstone::MapCursor::setCaption (
    std::string caption
) 
```





**Parameters:**


* `caption` new caption 




        

<hr>



### function setDirection 

_Set the direction of this cursor._ 
```C++
inline void endstone::MapCursor::setDirection (
    std::int8_t direction
) 
```





**Parameters:**


* `direction` The facing of the cursor, from 0 to 15. 




        

<hr>



### function setType 

_Set the type of this cursor._ 
```C++
inline void endstone::MapCursor::setType (
    Type type
) 
```





**Parameters:**


* `type` The type (color/style) of the map cursor. 




        

<hr>



### function setVisible 

```C++
inline void endstone::MapCursor::setVisible (
    bool visible
) 
```



Set the visibility status of this cursor.




**Parameters:**


* `visible` True if visible. 




        

<hr>



### function setX 

_Set the X position of this cursor._ 
```C++
inline void endstone::MapCursor::setX (
    const std::int8_t x
) 
```





**Parameters:**


* `x` The X coordinate. 




        

<hr>



### function setY 

_Set the Y position of this cursor._ 
```C++
inline void endstone::MapCursor::setY (
    const std::int8_t y
) 
```





**Parameters:**


* `y` The Y coordinate. 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/map/map_cursor.h`

