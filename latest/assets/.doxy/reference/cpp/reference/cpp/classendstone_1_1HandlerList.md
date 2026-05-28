

# Class endstone::HandlerList



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**HandlerList**](classendstone_1_1HandlerList.md)



_A list of event handlers._ [More...](#detailed-description)

* `#include <endstone/event/handler_list.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**HandlerList**](#function-handlerlist) (std::string event) <br> |
|  std::vector&lt; [**EventHandler**](classendstone_1_1EventHandler.md) \* &gt; | [**getHandlers**](#function-gethandlers) () const<br>_Get the baked registered handlers associated with this handler list._  |
|  [**EventHandler**](classendstone_1_1EventHandler.md) \* | [**registerHandler**](#function-registerhandler) (std::unique\_ptr&lt; [**EventHandler**](classendstone_1_1EventHandler.md) &gt; handler) <br>_Register a new handler._  |
|  [**void**](classendstone_1_1Identifier.md) | [**unregister**](#function-unregister-12) ([**const**](classendstone_1_1Identifier.md) [**EventHandler**](classendstone_1_1EventHandler.md) & handler) <br>_Remove a handler from a specific order slot._  |
|  [**void**](classendstone_1_1Identifier.md) | [**unregister**](#function-unregister-22) ([**const**](classendstone_1_1Identifier.md) [**Plugin**](classendstone_1_1Plugin.md) & plugin) <br>_Remove a specific plugin's handlers from this handler._  |
























## Protected Functions

| Type | Name |
| ---: | :--- |
|  [**void**](classendstone_1_1Identifier.md) | [**bake**](#function-bake) () const<br> |




## Detailed Description


Should be instantiated on a per-event basis. 


    
## Public Functions Documentation




### function HandlerList 

```C++
inline explicit endstone::HandlerList::HandlerList (
    std::string event
) 
```




<hr>



### function getHandlers 

_Get the baked registered handlers associated with this handler list._ 
```C++
inline std::vector< EventHandler * > endstone::HandlerList::getHandlers () const
```





**Returns:**

the array of registered handlers 





        

<hr>



### function registerHandler 

_Register a new handler._ 
```C++
inline EventHandler * endstone::HandlerList::registerHandler (
    std::unique_ptr< EventHandler > handler
) 
```





**Parameters:**


* `handler` [**Event**](classendstone_1_1Event.md) handler to register 



**Returns:**

the pointer to the registered handler 





        

<hr>



### function unregister [1/2]

_Remove a handler from a specific order slot._ 
```C++
inline void endstone::HandlerList::unregister (
    const  EventHandler & handler
) 
```





**Parameters:**


* `handler` [**Event**](classendstone_1_1Event.md) handler to remove 




        

<hr>



### function unregister [2/2]

_Remove a specific plugin's handlers from this handler._ 
```C++
inline void endstone::HandlerList::unregister (
    const  Plugin & plugin
) 
```





**Parameters:**


* `plugin` [**Plugin**](classendstone_1_1Plugin.md) to remove 




        

<hr>
## Protected Functions Documentation




### function bake 

```C++
inline void endstone::HandlerList::bake () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/handler_list.h`

