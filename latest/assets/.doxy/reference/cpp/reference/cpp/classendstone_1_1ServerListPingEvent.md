

# Class endstone::ServerListPingEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ServerListPingEvent**](classendstone_1_1ServerListPingEvent.md)



_Called when a server ping is coming in._ 

* `#include <endstone/event/server/server_list_ping_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)
































## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const std::string | [**NAME**](#variable-name)   = `"ServerListPingEvent"`<br> |










































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ServerListPingEvent**](#function-serverlistpingevent) (std::string remote\_host, int remote\_port, std::string ping\_response) <br> |
|  bool | [**deserialize**](#function-deserialize) () <br> |
| virtual std::string | [**getEventName**](#function-geteventname) () override const<br> |
|  [**GameMode**](namespaceendstone.md#enum-gamemode) | [**getGameMode**](#function-getgamemode) () const<br> |
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
|  std::string | [**serialize**](#function-serialize) () <br> |
|  void | [**setGameMode**](#function-setgamemode) ([**GameMode**](namespaceendstone.md#enum-gamemode) game\_mode) <br> |
|  void | [**setLevelName**](#function-setlevelname) (std::string level\_name) <br> |
|  void | [**setLocalPort**](#function-setlocalport) (int port) <br> |
|  void | [**setLocalPortV6**](#function-setlocalportv6) (int port) <br> |
|  void | [**setMaxPlayers**](#function-setmaxplayers) (int max\_players) <br> |
|  void | [**setMinecraftVersionNetwork**](#function-setminecraftversionnetwork) (std::string minecraft\_version\_network) <br> |
|  void | [**setMotd**](#function-setmotd) (std::string motd) <br> |
|  void | [**setNumPlayers**](#function-setnumplayers) (int num\_players) <br> |
|  void | [**setServerGuid**](#function-setserverguid) (std::string guid) <br> |


## Public Functions inherited from endstone::Cancellable

See [endstone::Cancellable](classendstone_1_1Cancellable.md)

| Type | Name |
| ---: | :--- |
| virtual void | [**cancel**](classendstone_1_1Cancellable.md#function-cancel) () <br>_Cancel this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual bool | [**isCancelled**](classendstone_1_1Cancellable.md#function-iscancelled) () override const<br>_Gets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual void | [**setCancelled**](classendstone_1_1Cancellable.md#function-setcancelled) (bool cancel) override<br>_Sets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |


## Public Functions inherited from endstone::ICancellable

See [endstone::ICancellable](classendstone_1_1ICancellable.md)

| Type | Name |
| ---: | :--- |
| virtual void | [**cancel**](classendstone_1_1ICancellable.md#function-cancel) () = 0<br> |
| virtual bool | [**isCancelled**](classendstone_1_1ICancellable.md#function-iscancelled) () const = 0<br> |
| virtual void | [**setCancelled**](classendstone_1_1ICancellable.md#function-setcancelled) (bool cancel) = 0<br> |
| virtual  | [**~ICancellable**](classendstone_1_1ICancellable.md#function-icancellable) () = default<br> |
















































































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



### function setLocalPort 

```C++
inline void endstone::ServerListPingEvent::setLocalPort (
    int port
) 
```



Set the local port of the server.




**Parameters:**


* `port` the local port 




        

<hr>



### function setLocalPortV6 

```C++
inline void endstone::ServerListPingEvent::setLocalPortV6 (
    int port
) 
```



Set the local port of the server for IPv6 support.




**Parameters:**


* `port` the local port for IPv6 




        

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



### function setServerGuid 

```C++
inline void endstone::ServerListPingEvent::setServerGuid (
    std::string guid
) 
```



Set the unique identifier of the server.




**Parameters:**


* `guid` the server guid 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/server/server_list_ping_event.h`

