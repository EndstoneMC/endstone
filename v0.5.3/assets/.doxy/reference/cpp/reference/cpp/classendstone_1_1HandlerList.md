

# Class endstone::HandlerList



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**HandlerList**](classendstone_1_1HandlerList.md)



_A list of event handlers. Should be instantiated on a per-event basis._ 

* `#include <endstone/event/handler_list.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**HandlerList**](#function-handlerlist) (std::string event) <br> |
|  std::vector&lt; [**EventHandler**](classendstone_1_1EventHandler.md) \* &gt; | [**getHandlers**](#function-gethandlers) () const<br> |
|  [**EventHandler**](classendstone_1_1EventHandler.md) \* | [**registerHandler**](#function-registerhandler) (std::unique\_ptr&lt; [**EventHandler**](classendstone_1_1EventHandler.md) &gt; handler) <br> |
|  void | [**unregister**](#function-unregister-12) (const [**EventHandler**](classendstone_1_1EventHandler.md) & handler) <br> |
|  void | [**unregister**](#function-unregister-22) (const [**Plugin**](classendstone_1_1Plugin.md) & plugin) <br> |
























## Protected Functions

| Type | Name |
| ---: | :--- |
|  void | [**bake**](#function-bake) () const<br> |




## Public Functions Documentation




### function HandlerList 

```C++
inline explicit endstone::HandlerList::HandlerList (
    std::string event
) 
```




<hr>



### function getHandlers 


```C++
inline std::vector< EventHandler * > endstone::HandlerList::getHandlers () const
```



Get the baked registered handlers associated with this handler list




**Returns:**

the array of registered handlers 





        

<hr>



### function registerHandler 


```C++
inline EventHandler * endstone::HandlerList::registerHandler (
    std::unique_ptr< EventHandler > handler
) 
```



Register a new handler




**Parameters:**


* `handler` [**Event**](classendstone_1_1Event.md) handler to register 



**Returns:**

the pointer to the registered handler 





        

<hr>



### function unregister [1/2]


```C++
inline void endstone::HandlerList::unregister (
    const EventHandler & handler
) 
```



Remove a handler from a specific order slot




**Parameters:**


* `handler` [**Event**](classendstone_1_1Event.md) handler to remove 




        

<hr>



### function unregister [2/2]


```C++
inline void endstone::HandlerList::unregister (
    const Plugin & plugin
) 
```



Remove a specific plugin's handlers from this handler




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

