

# Class endstone::ServerListPingEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ServerListPingEvent**](classendstone_1_1ServerListPingEvent.md)



_Called when a server ping is coming in._ 

* `#include <endstone/event/server/server_list_ping_event.h>`



Inherits the following classes: [endstone::ServerEvent](classendstone_1_1ServerEvent.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**NAME**](#variable-name)   = = "ServerListPingEvent"<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ServerListPingEvent**](#function-serverlistpingevent) (std::string remote\_host, int remote\_port, std::string ping\_response) <br> |
|  bool | [**deserialize**](#function-deserialize) () <br> |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|  GameMode | [**getGameMode**](#function-getgamemode) () const<br> |
|  std::string | [**getLevelName**](#function-getlevelname) () const<br> |
|  int | [**getLocalPort**](#function-getlocalport) () const<br> |
|  int | [**getLocalPortV6**](#function-getlocalportv6) () const<br> |
|  int | [**getMaxPlayers**](#function-getmaxplayers) () const<br> |
|  std::string | [**getMinecraftVersionNetwork**](#function-getminecraftversionnetwork) () const<br> |
|  std::string | [**getMotd**](#function-getmotd) () const<br> |
|  int | [**getNetworkProtocolVersion**](#function-getnetworkprotocolversion) () const<br> |
|  int | [**getNumPlayers**](#function-getnumplayers) () const<br> |
|  std::string | [**getRemoteHost**](#function-getremotehost) () const<br> |
|  int | [**getRemotePort**](#function-getremoteport) () const<br> |
|  std::string | [**getServerGuid**](#function-getserverguid) () const<br> |
| virtual bool | [**isCancellable**](#function-iscancellable) () override const<br> |
|  std::string | [**serialize**](#function-serialize) () <br> |
|  void | [**setGameMode**](#function-setgamemode) (GameMode game\_mode) <br> |
|  void | [**setLevelName**](#function-setlevelname) (std::string level\_name) <br> |
|  void | [**setMaxPlayers**](#function-setmaxplayers) (int max\_players) <br> |
|  void | [**setMinecraftVersionNetwork**](#function-setminecraftversionnetwork) (std::string minecraft\_version\_network) <br> |
|  void | [**setMotd**](#function-setmotd) (std::string motd) <br> |
|  void | [**setNumPlayers**](#function-setnumplayers) (int num\_players) <br> |


## Public Functions inherited from endstone::ServerEvent

See [endstone::ServerEvent](classendstone_1_1ServerEvent.md)

| Type | Name |
| ---: | :--- |
|   | [**Event**](classendstone_1_1ServerEvent.md#function-event-12) (bool async=false) <br> |
|   | [**Event**](classendstone_1_1ServerEvent.md#function-event-22) (const [**Event**](classendstone_1_1Event.md) &) = delete<br> |


## Public Functions inherited from endstone::Event

See [endstone::Event](classendstone_1_1Event.md)

| Type | Name |
| ---: | :--- |
|   | [**Event**](classendstone_1_1Event.md#function-event-12) (bool async=false) <br> |
|   | [**Event**](classendstone_1_1Event.md#function-event-22) (const [**Event**](classendstone_1_1Event.md) &) = delete<br> |
| virtual std::string | [**getEventName**](classendstone_1_1Event.md#function-geteventname) () const = 0<br> |
|  bool | [**isAsynchronous**](classendstone_1_1Event.md#function-isasynchronous) () const<br> |
| virtual bool | [**isCancellable**](classendstone_1_1Event.md#function-iscancellable) () const = 0<br> |
|  bool | [**isCancelled**](classendstone_1_1Event.md#function-iscancelled) () const<br> |
|  [**Event**](classendstone_1_1Event.md) & | [**operator=**](classendstone_1_1Event.md#function-operator) (const [**Event**](classendstone_1_1Event.md) &) = delete<br> |
|  void | [**setCancelled**](classendstone_1_1Event.md#function-setcancelled) (bool cancel) <br> |
| virtual  | [**~Event**](classendstone_1_1Event.md#function-event) () = default<br> |
















































































## Public Static Attributes Documentation




### variable NAME 

```C++
const std::string endstone::ServerListPingEvent::NAME;
```




<hr>
## Public Functions Documentation




### function ServerListPingEvent 

```C++
inline endstone::ServerListPingEvent::ServerListPingEvent (
    std::string remote_host,
    int remote_port,
    std::string ping_response
) 
```




<hr>



### function deserialize 

```C++
bool endstone::ServerListPingEvent::deserialize () 
```




<hr>



### function getEventName 


```C++
inline virtual std::string endstone::ServerListPingEvent::getEventName () override const
```



Gets a user-friendly identifier for this event.




**Returns:**

name of this event 





        
Implements [*endstone::Event::getEventName*](classendstone_1_1Event.md#function-geteventname)


<hr>



### function getGameMode 


```C++
inline GameMode endstone::ServerListPingEvent::getGameMode () const
```



Get the current game mode.




**Returns:**

the game mode 





        

<hr>



### function getLevelName 


```C++
inline std::string endstone::ServerListPingEvent::getLevelName () const
```



Get the level name.




**Returns:**

the level name 





        

<hr>



### function getLocalPort 


```C++
inline int endstone::ServerListPingEvent::getLocalPort () const
```



Get the local port of the server.




**Returns:**

The local port 





        

<hr>



### function getLocalPortV6 


```C++
inline int endstone::ServerListPingEvent::getLocalPortV6 () const
```



Get the local port of the server for IPv6 support




**Returns:**

The local port for IPv6 





        

<hr>



### function getMaxPlayers 


```C++
inline int endstone::ServerListPingEvent::getMaxPlayers () const
```



Get the maximum number of players allowed.




**Returns:**

the maximum number of players 





        

<hr>



### function getMinecraftVersionNetwork 


```C++
inline std::string endstone::ServerListPingEvent::getMinecraftVersionNetwork () const
```



Get the network version of Minecraft that is supported by this server




**Returns:**

the network version of Minecraft 





        

<hr>



### function getMotd 


```C++
inline std::string endstone::ServerListPingEvent::getMotd () const
```



Get the message of the day message.




**Returns:**

the message of the day 





        

<hr>



### function getNetworkProtocolVersion 


```C++
inline int endstone::ServerListPingEvent::getNetworkProtocolVersion () const
```



Get the network protocol version of this server




**Returns:**

the network protocol version 





        

<hr>



### function getNumPlayers 


```C++
inline int endstone::ServerListPingEvent::getNumPlayers () const
```



Get the number of players online




**Returns:**

the number of players 





        

<hr>



### function getRemoteHost 


```C++
inline std::string endstone::ServerListPingEvent::getRemoteHost () const
```



Get the host the ping is coming from.




**Returns:**

The host 





        

<hr>



### function getRemotePort 


```C++
inline int endstone::ServerListPingEvent::getRemotePort () const
```



Get the port the ping is coming from.




**Returns:**

The port 





        

<hr>



### function getServerGuid 


```C++
inline std::string endstone::ServerListPingEvent::getServerGuid () const
```



Get the unique identifier of the server.




**Returns:**

The server guid 





        

<hr>



### function isCancellable 


```C++
inline virtual bool endstone::ServerListPingEvent::isCancellable () override const
```



Whether the event can be cancelled by a plugin or the server.




**Returns:**

true if this event can be cancelled 





        
Implements [*endstone::Event::isCancellable*](classendstone_1_1Event.md#function-iscancellable)


<hr>



### function serialize 

```C++
std::string endstone::ServerListPingEvent::serialize () 
```




<hr>



### function setGameMode 


```C++
inline void endstone::ServerListPingEvent::setGameMode (
    GameMode game_mode
) 
```



Set the current game mode.




**Parameters:**


* `game_mode` the game mode 




        

<hr>



### function setLevelName 


```C++
inline void endstone::ServerListPingEvent::setLevelName (
    std::string level_name
) 
```



Set the level name.




**Parameters:**


* `level_name` the level name 




        

<hr>



### function setMaxPlayers 


```C++
inline void endstone::ServerListPingEvent::setMaxPlayers (
    int max_players
) 
```



Set the maximum number of players allowed.




**Parameters:**


* `max_players` the maximum number of players 




        

<hr>



### function setMinecraftVersionNetwork 


```C++
inline void endstone::ServerListPingEvent::setMinecraftVersionNetwork (
    std::string minecraft_version_network
) 
```



Set the network version of Minecraft that is supported by this server




**Parameters:**


* `minecraft_version_network` the network version of Minecraft 




        

<hr>



### function setMotd 


```C++
inline void endstone::ServerListPingEvent::setMotd (
    std::string motd
) 
```



Change the message of the day message.




**Parameters:**


* `motd` the message of the day 




        

<hr>



### function setNumPlayers 


```C++
inline void endstone::ServerListPingEvent::setNumPlayers (
    int num_players
) 
```



Set the number of players online.




**Parameters:**


* `num_players` the number of players 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/server/server_list_ping_event.h`

