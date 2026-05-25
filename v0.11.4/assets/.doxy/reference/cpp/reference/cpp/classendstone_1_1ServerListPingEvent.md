

# Class endstone::ServerListPingEvent



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ServerListPingEvent**](classendstone_1_1ServerListPingEvent.md)



_Called when a server ping is coming in._ 

* `#include <endstone/event/server/server_list_ping_event.h>`



Inherits the following classes: [endstone::Cancellable](classendstone_1_1Cancellable.md)










































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ENDSTONE\_EVENT**](#function-endstone_event) ([**ServerListPingEvent**](classendstone_1_1ServerListPingEvent.md)) <br> |
|   | [**ServerListPingEvent**](#function-serverlistpingevent) ([**SocketAddress**](classendstone_1_1SocketAddress.md) address, std::string motd, [**int**](classendstone_1_1Identifier.md) network\_protocol\_version, std::string minecraft\_version\_network, [**int**](classendstone_1_1Identifier.md) num\_players, [**int**](classendstone_1_1Identifier.md) max\_players, std::string server\_guid, std::string level\_name, [**GameMode**](namespaceendstone.md#enum-gamemode) game\_mode, [**int**](classendstone_1_1Identifier.md) local\_port, [**int**](classendstone_1_1Identifier.md) local\_port\_v6) <br> |
|  [**SocketAddress**](classendstone_1_1SocketAddress.md) | [**getAddress**](#function-getaddress) () const<br>_Get the address the ping is coming from._  |
|  [**GameMode**](namespaceendstone.md#enum-gamemode) | [**getGameMode**](#function-getgamemode) () const<br>_Get the current game mode._  |
|  std::string | [**getLevelName**](#function-getlevelname) () const<br>_Get the level name._  |
|  [**int**](classendstone_1_1Identifier.md) | [**getLocalPort**](#function-getlocalport) () const<br>_Get the local port of the server._  |
|  [**int**](classendstone_1_1Identifier.md) | [**getLocalPortV6**](#function-getlocalportv6) () const<br>_Get the local port of the server for IPv6 support._  |
|  [**int**](classendstone_1_1Identifier.md) | [**getMaxPlayers**](#function-getmaxplayers) () const<br>_Get the maximum number of players allowed._  |
|  std::string | [**getMinecraftVersionNetwork**](#function-getminecraftversionnetwork) () const<br>_Get the network version of Minecraft that is supported by this server._  |
|  std::string | [**getMotd**](#function-getmotd) () const<br>_Get the message of the day message._  |
|  [**int**](classendstone_1_1Identifier.md) | [**getNetworkProtocolVersion**](#function-getnetworkprotocolversion) () const<br>_Get the network protocol version of this server._  |
|  [**int**](classendstone_1_1Identifier.md) | [**getNumPlayers**](#function-getnumplayers) () const<br>_Get the number of players online._  |
|  std::string | [**getServerGuid**](#function-getserverguid) () const<br>_Get the unique identifier of the server._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setGameMode**](#function-setgamemode) ([**GameMode**](namespaceendstone.md#enum-gamemode) game\_mode) <br>_Set the current game mode._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setLevelName**](#function-setlevelname) (std::string level\_name) <br>_Set the level name._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setLocalPort**](#function-setlocalport) ([**int**](classendstone_1_1Identifier.md) port) <br>_Set the local port of the server._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setLocalPortV6**](#function-setlocalportv6) ([**int**](classendstone_1_1Identifier.md) port) <br>_Set the local port of the server for IPv6 support._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setMaxPlayers**](#function-setmaxplayers) ([**int**](classendstone_1_1Identifier.md) max\_players) <br>_Set the maximum number of players allowed._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setMinecraftVersionNetwork**](#function-setminecraftversionnetwork) (std::string minecraft\_version\_network) <br>_Set the network version of Minecraft that is supported by this server._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setMotd**](#function-setmotd) (std::string motd) <br>_Change the message of the day message._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setNumPlayers**](#function-setnumplayers) ([**int**](classendstone_1_1Identifier.md) num\_players) <br>_Set the number of players online._  |
|  [**void**](classendstone_1_1Identifier.md) | [**setServerGuid**](#function-setserverguid) (std::string guid) <br>_Set the unique identifier of the server._  |


## Public Functions inherited from endstone::Cancellable

See [endstone::Cancellable](classendstone_1_1Cancellable.md)

| Type | Name |
| ---: | :--- |
| virtual [**void**](classendstone_1_1Identifier.md) | [**cancel**](classendstone_1_1Cancellable.md#function-cancel) () <br>_Cancel this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isCancelled**](classendstone_1_1Cancellable.md#function-iscancelled) () override const<br>_Gets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setCancelled**](classendstone_1_1Cancellable.md#function-setcancelled) ([**bool**](classendstone_1_1Identifier.md) cancel) override<br>_Sets the cancellation state of this event. A cancelled event will not be executed in the server, but will still pass to other plugins._  |


## Public Functions inherited from endstone::ICancellable

See [endstone::ICancellable](classendstone_1_1ICancellable.md)

| Type | Name |
| ---: | :--- |
| virtual [**void**](classendstone_1_1Identifier.md) | [**cancel**](classendstone_1_1ICancellable.md#function-cancel) () = 0<br> |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**isCancelled**](classendstone_1_1ICancellable.md#function-iscancelled) () const = 0<br> |
| virtual [**void**](classendstone_1_1Identifier.md) | [**setCancelled**](classendstone_1_1ICancellable.md#function-setcancelled) ([**bool**](classendstone_1_1Identifier.md) cancel) = 0<br> |
| virtual  | [**~ICancellable**](classendstone_1_1ICancellable.md#function-icancellable) () = default<br> |
















































































## Public Functions Documentation




### function ENDSTONE\_EVENT 

```C++
endstone::ServerListPingEvent::ENDSTONE_EVENT (
    ServerListPingEvent
) 
```




<hr>



### function ServerListPingEvent 

```C++
inline endstone::ServerListPingEvent::ServerListPingEvent (
    SocketAddress address,
    std::string motd,
    int network_protocol_version,
    std::string minecraft_version_network,
    int num_players,
    int max_players,
    std::string server_guid,
    std::string level_name,
    GameMode game_mode,
    int local_port,
    int local_port_v6
) 
```




<hr>



### function getAddress 

_Get the address the ping is coming from._ 
```C++
inline SocketAddress endstone::ServerListPingEvent::getAddress () const
```





**Returns:**

the address 





        

<hr>



### function getGameMode 

_Get the current game mode._ 
```C++
inline GameMode endstone::ServerListPingEvent::getGameMode () const
```





**Returns:**

the game mode 





        

<hr>



### function getLevelName 

_Get the level name._ 
```C++
inline std::string endstone::ServerListPingEvent::getLevelName () const
```





**Returns:**

the level name 





        

<hr>



### function getLocalPort 

_Get the local port of the server._ 
```C++
inline int endstone::ServerListPingEvent::getLocalPort () const
```





**Returns:**

The local port 





        

<hr>



### function getLocalPortV6 

_Get the local port of the server for IPv6 support._ 
```C++
inline int endstone::ServerListPingEvent::getLocalPortV6 () const
```





**Returns:**

The local port for IPv6 





        

<hr>



### function getMaxPlayers 

_Get the maximum number of players allowed._ 
```C++
inline int endstone::ServerListPingEvent::getMaxPlayers () const
```





**Returns:**

the maximum number of players 





        

<hr>



### function getMinecraftVersionNetwork 

_Get the network version of Minecraft that is supported by this server._ 
```C++
inline std::string endstone::ServerListPingEvent::getMinecraftVersionNetwork () const
```





**Returns:**

the network version of Minecraft 





        

<hr>



### function getMotd 

_Get the message of the day message._ 
```C++
inline std::string endstone::ServerListPingEvent::getMotd () const
```





**Returns:**

the message of the day 





        

<hr>



### function getNetworkProtocolVersion 

_Get the network protocol version of this server._ 
```C++
inline int endstone::ServerListPingEvent::getNetworkProtocolVersion () const
```





**Returns:**

the network protocol version 





        

<hr>



### function getNumPlayers 

_Get the number of players online._ 
```C++
inline int endstone::ServerListPingEvent::getNumPlayers () const
```





**Returns:**

the number of players 





        

<hr>



### function getServerGuid 

_Get the unique identifier of the server._ 
```C++
inline std::string endstone::ServerListPingEvent::getServerGuid () const
```





**Returns:**

The server guid 





        

<hr>



### function setGameMode 

_Set the current game mode._ 
```C++
inline void endstone::ServerListPingEvent::setGameMode (
    GameMode game_mode
) 
```





**Parameters:**


* `game_mode` the game mode 




        

<hr>



### function setLevelName 

_Set the level name._ 
```C++
inline void endstone::ServerListPingEvent::setLevelName (
    std::string level_name
) 
```





**Parameters:**


* `level_name` the level name 




        

<hr>



### function setLocalPort 

_Set the local port of the server._ 
```C++
inline void endstone::ServerListPingEvent::setLocalPort (
    int port
) 
```





**Parameters:**


* `port` the local port 




        

<hr>



### function setLocalPortV6 

_Set the local port of the server for IPv6 support._ 
```C++
inline void endstone::ServerListPingEvent::setLocalPortV6 (
    int port
) 
```





**Parameters:**


* `port` the local port for IPv6 




        

<hr>



### function setMaxPlayers 

_Set the maximum number of players allowed._ 
```C++
inline void endstone::ServerListPingEvent::setMaxPlayers (
    int max_players
) 
```





**Parameters:**


* `max_players` the maximum number of players 




        

<hr>



### function setMinecraftVersionNetwork 

_Set the network version of Minecraft that is supported by this server._ 
```C++
inline void endstone::ServerListPingEvent::setMinecraftVersionNetwork (
    std::string minecraft_version_network
) 
```





**Parameters:**


* `minecraft_version_network` the network version of Minecraft 




        

<hr>



### function setMotd 

_Change the message of the day message._ 
```C++
inline void endstone::ServerListPingEvent::setMotd (
    std::string motd
) 
```





**Parameters:**


* `motd` the message of the day 




        

<hr>



### function setNumPlayers 

_Set the number of players online._ 
```C++
inline void endstone::ServerListPingEvent::setNumPlayers (
    int num_players
) 
```





**Parameters:**


* `num_players` the number of players 




        

<hr>



### function setServerGuid 

_Set the unique identifier of the server._ 
```C++
inline void endstone::ServerListPingEvent::setServerGuid (
    std::string guid
) 
```





**Parameters:**


* `guid` the server guid 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/event/server/server_list_ping_event.h`

