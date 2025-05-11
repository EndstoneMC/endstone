

# Class endstone::Server



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Server**](classendstone_1_1Server.md)



_Represents a server implementation._ 

* `#include <endstone/server.h>`























## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  [**const**](classendstone_1_1Vector.md) std::string | [**BroadcastChannelAdmin**](#variable-broadcastchanneladmin)   = `"endstone.broadcast.admin"`<br>_Used for all administrative messages, such as an operator using a command._  |
|  [**const**](classendstone_1_1Vector.md) std::string | [**BroadcastChannelUser**](#variable-broadcastchanneluser)   = `"endstone.broadcast.user"`<br>_Used for all announcement messages, such as informing users that a player has joined._  |














## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Server**](#function-server-12) () = default<br> |
|   | [**Server**](#function-server-22) ([**const**](classendstone_1_1Vector.md) [**Server**](classendstone_1_1Server.md) &) = delete<br> |
| virtual [**void**](classendstone_1_1Vector.md) | [**broadcast**](#function-broadcast) ([**const**](classendstone_1_1Vector.md) Message & message, [**const**](classendstone_1_1Vector.md) std::string & permission) const = 0<br>_Broadcasts the specified message to every user with the given permission name._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**broadcastMessage**](#function-broadcastmessage-12) ([**const**](classendstone_1_1Vector.md) Message & message) const = 0<br>_Broadcasts the specified message to every user with permission endstone.broadcast.user._  |
|  [**void**](classendstone_1_1Vector.md) | [**broadcastMessage**](#function-broadcastmessage-22) ([**const**](classendstone_1_1Vector.md) fmt::format\_string&lt; Args... &gt; format, [**Args**](classendstone_1_1Vector.md) &&... args) const<br> |
| virtual Result&lt; std::unique\_ptr&lt; [**BlockData**](classendstone_1_1BlockData.md) &gt; &gt; | [**createBlockData**](#function-createblockdata-12) (std::string type) const = 0<br>_Creates a new_ [_**BlockData**_](classendstone_1_1BlockData.md) _instance for the specified block type, with all properties initialized to defaults._ |
| virtual Result&lt; std::unique\_ptr&lt; [**BlockData**](classendstone_1_1BlockData.md) &gt; &gt; | [**createBlockData**](#function-createblockdata-22) (std::string type, BlockStates block\_states) const = 0<br>_Creates a new_ [_**BlockData**_](classendstone_1_1BlockData.md) _instance for the specified block type, with all properties initialized to defaults, except for those provided in data._ |
| virtual std::unique\_ptr&lt; [**BossBar**](classendstone_1_1BossBar.md) &gt; | [**createBossBar**](#function-createbossbar-12) (std::string title, BarColor color, [**BarStyle**](namespaceendstone.md#enum-barstyle) style) const = 0<br>_Creates a boss bar instance to display to players. The progress defaults to 1.0._  |
| virtual std::unique\_ptr&lt; [**BossBar**](classendstone_1_1BossBar.md) &gt; | [**createBossBar**](#function-createbossbar-22) (std::string title, BarColor color, [**BarStyle**](namespaceendstone.md#enum-barstyle) style, std::vector&lt; [**BarFlag**](namespaceendstone.md#enum-barflag) &gt; flags) const = 0<br>_Creates a boss bar instance to display to players. The progress defaults to 1.0._  |
| virtual std::shared\_ptr&lt; [**Scoreboard**](classendstone_1_1Scoreboard.md) &gt; | [**createScoreboard**](#function-createscoreboard) () = 0<br>_Creates a new_ [_**Scoreboard**_](classendstone_1_1Scoreboard.md) _to be tracked by the server._ |
| virtual [**bool**](classendstone_1_1Vector.md) | [**dispatchCommand**](#function-dispatchcommand) ([**CommandSender**](classendstone_1_1CommandSender.md) & sender, std::string command\_line) const = 0<br>_Dispatches a command on this server, and executes it if found._  |
| virtual [**float**](classendstone_1_1Vector.md) | [**getAverageMillisecondsPerTick**](#function-getaveragemillisecondspertick) () = 0<br>_Gets the average milliseconds per tick (MSPT)._  |
| virtual [**float**](classendstone_1_1Vector.md) | [**getAverageTickUsage**](#function-getaveragetickusage) () = 0<br>_Gets the average tick usage of the server._  |
| virtual [**float**](classendstone_1_1Vector.md) | [**getAverageTicksPerSecond**](#function-getaveragetickspersecond) () = 0<br>_Gets the average ticks per second (TPS)._  |
| virtual [**PlayerBanList**](classendstone_1_1PlayerBanList.md) & | [**getBanList**](#function-getbanlist) () const = 0<br> |
| virtual [**ConsoleCommandSender**](classendstone_1_1ConsoleCommandSender.md) & | [**getCommandSender**](#function-getcommandsender) () const = 0<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _for this server._ |
| virtual [**float**](classendstone_1_1Vector.md) | [**getCurrentMillisecondsPerTick**](#function-getcurrentmillisecondspertick) () = 0<br>_Gets the current milliseconds per tick (MSPT)._  |
| virtual [**float**](classendstone_1_1Vector.md) | [**getCurrentTickUsage**](#function-getcurrenttickusage) () = 0<br>_Gets the current tick usage of the server._  |
| virtual [**float**](classendstone_1_1Vector.md) | [**getCurrentTicksPerSecond**](#function-getcurrenttickspersecond) () = 0<br>_Gets the current ticks per second (TPS)._  |
| virtual [**Registry**](classendstone_1_1Registry.md)&lt; [**Enchantment**](classendstone_1_1Enchantment.md) &gt; & | [**getEnchantmentRegistry**](#function-getenchantmentregistry) () const = 0<br> |
| virtual [**IpBanList**](classendstone_1_1IpBanList.md) & | [**getIpBanList**](#function-getipbanlist) () const = 0<br> |
| virtual [**ItemFactory**](classendstone_1_1ItemFactory.md) & | [**getItemFactory**](#function-getitemfactory) () const = 0<br> |
| virtual [**Language**](classendstone_1_1Language.md) & | [**getLanguage**](#function-getlanguage) () const = 0<br>_Gets the current language interface used by the server._  |
| virtual [**Level**](classendstone_1_1Level.md) \* | [**getLevel**](#function-getlevel) () const = 0<br>_Gets the server level._  |
| virtual [**Logger**](classendstone_1_1Logger.md) & | [**getLogger**](#function-getlogger) () const = 0<br>_Returns the primary logger associated with this server instance._  |
| virtual [**int**](classendstone_1_1Vector.md) | [**getMaxPlayers**](#function-getmaxplayers) () const = 0<br>_Get the maximum amount of players which can login to this server._  |
| virtual std::string | [**getMinecraftVersion**](#function-getminecraftversion) () const = 0<br>_Gets the Minecraft version that this server is running._  |
| virtual std::string | [**getName**](#function-getname) () const = 0<br>_Gets the name of this server implementation._  |
| virtual [**bool**](classendstone_1_1Vector.md) | [**getOnlineMode**](#function-getonlinemode) () const = 0<br>_Gets whether the_ [_**Server**_](classendstone_1_1Server.md) _is in online mode or not._ |
| virtual std::vector&lt; [**Player**](classendstone_1_1Player.md) \* &gt; | [**getOnlinePlayers**](#function-getonlineplayers) () const = 0<br>_Gets a list of all currently online players._  |
| virtual [**Player**](classendstone_1_1Player.md) \* | [**getPlayer**](#function-getplayer-12) ([**endstone::UUID**](classendstone_1_1UUID.md) id) const = 0<br>_Gets the player with the given_ [_**UUID**_](classendstone_1_1UUID.md) _._ |
| virtual [**Player**](classendstone_1_1Player.md) \* | [**getPlayer**](#function-getplayer-22) (std::string name) const = 0<br>_Gets the player with the exact given name, case-insensitive._  |
| virtual [**PluginCommand**](classendstone_1_1PluginCommand.md) \* | [**getPluginCommand**](#function-getplugincommand) (std::string name) const = 0<br>_Gets a_ [_**PluginCommand**_](classendstone_1_1PluginCommand.md) _with the given name or alias._ |
| virtual [**PluginManager**](classendstone_1_1PluginManager.md) & | [**getPluginManager**](#function-getpluginmanager) () const = 0<br>_Gets the plugin manager for interfacing with plugins._  |
| virtual [**Scheduler**](classendstone_1_1Scheduler.md) & | [**getScheduler**](#function-getscheduler) () const = 0<br>_Gets the scheduler for managing scheduled events._  |
| virtual [**Scoreboard**](classendstone_1_1Scoreboard.md) \* | [**getScoreboard**](#function-getscoreboard) () const = 0<br>_Gets the primary_ [_**Scoreboard**_](classendstone_1_1Scoreboard.md) _controlled by the server._ |
| virtual [**ServiceManager**](classendstone_1_1ServiceManager.md) & | [**getServiceManager**](#function-getservicemanager) () const = 0<br> |
| virtual std::chrono::system\_clock::time\_point | [**getStartTime**](#function-getstarttime) () = 0<br>_Gets the start time of the server._  |
| virtual std::string | [**getVersion**](#function-getversion) () const = 0<br>_Gets the version string of this server implementation._  |
| virtual [**bool**](classendstone_1_1Vector.md) | [**isPrimaryThread**](#function-isprimarythread) () const = 0<br>_Checks the current thread against the expected primary server thread._  |
|  [**Server**](classendstone_1_1Server.md) & | [**operator=**](#function-operator) ([**const**](classendstone_1_1Vector.md) [**Server**](classendstone_1_1Server.md) &) = delete<br> |
| virtual [**void**](classendstone_1_1Vector.md) | [**reload**](#function-reload) () = 0<br>_Reloads the server configuration, functions, scripts and plugins._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**reloadData**](#function-reloaddata) () = 0<br>_Reload only the Minecraft data for the server._  |
| virtual Result&lt; [**void**](classendstone_1_1Vector.md) &gt; | [**setMaxPlayers**](#function-setmaxplayers) ([**int**](classendstone_1_1Vector.md) max\_players) = 0<br>_Set the maximum amount of players allowed to be logged in at once._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**shutdown**](#function-shutdown) () = 0<br>_Shutdowns the server, stopping everything._  |
| virtual  | [**~Server**](#function-server) () = default<br> |




























## Public Static Attributes Documentation




### variable BroadcastChannelAdmin 

_Used for all administrative messages, such as an operator using a command._ 
```C++
const std::string endstone::Server::BroadcastChannelAdmin;
```




<hr>



### variable BroadcastChannelUser 

_Used for all announcement messages, such as informing users that a player has joined._ 
```C++
const std::string endstone::Server::BroadcastChannelUser;
```




<hr>
## Public Functions Documentation




### function Server [1/2]

```C++
endstone::Server::Server () = default
```




<hr>



### function Server [2/2]

```C++
endstone::Server::Server (
    const  Server &
) = delete
```




<hr>



### function broadcast 

_Broadcasts the specified message to every user with the given permission name._ 
```C++
virtual void endstone::Server::broadcast (
    const Message & message,
    const std::string & permission
) const = 0
```





**Parameters:**


* `message` message to broadcast 
* `permission` the required permission Permissibles must have to receive the broadcast 




        

<hr>



### function broadcastMessage [1/2]

_Broadcasts the specified message to every user with permission endstone.broadcast.user._ 
```C++
virtual void endstone::Server::broadcastMessage (
    const Message & message
) const = 0
```





**Parameters:**


* `message` the message 




        

<hr>



### function broadcastMessage [2/2]

```C++
template<typename... Args>
inline void endstone::Server::broadcastMessage (
    const fmt::format_string< Args... > format,
    Args &&... args
) const
```




<hr>



### function createBlockData [1/2]

_Creates a new_ [_**BlockData**_](classendstone_1_1BlockData.md) _instance for the specified block type, with all properties initialized to defaults._
```C++
virtual Result< std::unique_ptr< BlockData > > endstone::Server::createBlockData (
    std::string type
) const = 0
```





**Parameters:**


* `type` the block type 



**Returns:**

new data instance 





        

<hr>



### function createBlockData [2/2]

_Creates a new_ [_**BlockData**_](classendstone_1_1BlockData.md) _instance for the specified block type, with all properties initialized to defaults, except for those provided in data._
```C++
virtual Result< std::unique_ptr< BlockData > > endstone::Server::createBlockData (
    std::string type,
    BlockStates block_states
) const = 0
```





**Parameters:**


* `type` the block type 
* `block_states` block states, for example {"old\_leaf\_type":"birch", "persistent\_bit":true} 



**Returns:**

new data instance 





        

<hr>



### function createBossBar [1/2]

_Creates a boss bar instance to display to players. The progress defaults to 1.0._ 
```C++
virtual std::unique_ptr< BossBar > endstone::Server::createBossBar (
    std::string title,
    BarColor color,
    BarStyle style
) const = 0
```





**Parameters:**


* `title` the title of the boss bar 
* `color` the color of the boss bar 
* `style` the style of the boss bar 



**Returns:**

the created boss bar 





        

<hr>



### function createBossBar [2/2]

_Creates a boss bar instance to display to players. The progress defaults to 1.0._ 
```C++
virtual std::unique_ptr< BossBar > endstone::Server::createBossBar (
    std::string title,
    BarColor color,
    BarStyle style,
    std::vector< BarFlag > flags
) const = 0
```





**Parameters:**


* `title` the title of the boss bar 
* `color` the color of the boss bar 
* `style` the style of the boss bar 
* `flags` a list of flags to set on the boss bar 



**Returns:**

the created boss bar 





        

<hr>



### function createScoreboard 

_Creates a new_ [_**Scoreboard**_](classendstone_1_1Scoreboard.md) _to be tracked by the server._
```C++
virtual std::shared_ptr< Scoreboard > endstone::Server::createScoreboard () = 0
```



This will not be saved by the server and is not affected by the /scoreboard command.




**Returns:**

the newly created [**Scoreboard**](classendstone_1_1Scoreboard.md) 





        

<hr>



### function dispatchCommand 

_Dispatches a command on this server, and executes it if found._ 
```C++
virtual bool endstone::Server::dispatchCommand (
    CommandSender & sender,
    std::string command_line
) const = 0
```





**Parameters:**


* `sender` the apparent sender of the command 
* `command_line` the command + arguments. 



**Returns:**

true if execution is successful, false otherwise 





        

<hr>



### function getAverageMillisecondsPerTick 

_Gets the average milliseconds per tick (MSPT)._ 
```C++
virtual float endstone::Server::getAverageMillisecondsPerTick () = 0
```





**Returns:**

The average number of milliseconds per tick. 





        

<hr>



### function getAverageTickUsage 

_Gets the average tick usage of the server._ 
```C++
virtual float endstone::Server::getAverageTickUsage () = 0
```





**Returns:**

The average tick usage in percentage. 





        

<hr>



### function getAverageTicksPerSecond 

_Gets the average ticks per second (TPS)._ 
```C++
virtual float endstone::Server::getAverageTicksPerSecond () = 0
```





**Returns:**

The average ticks per second 





        

<hr>



### function getBanList 

```C++
virtual PlayerBanList & endstone::Server::getBanList () const = 0
```



Gets the player ban list.




**Returns:**

The player ban list 





        

<hr>



### function getCommandSender 

_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _for this server._
```C++
virtual ConsoleCommandSender & endstone::Server::getCommandSender () const = 0
```





**Returns:**

a console command sender 





        

<hr>



### function getCurrentMillisecondsPerTick 

_Gets the current milliseconds per tick (MSPT)._ 
```C++
virtual float endstone::Server::getCurrentMillisecondsPerTick () = 0
```





**Returns:**

The average current of milliseconds per tick. 





        

<hr>



### function getCurrentTickUsage 

_Gets the current tick usage of the server._ 
```C++
virtual float endstone::Server::getCurrentTickUsage () = 0
```





**Returns:**

The current tick usage in percentage. 





        

<hr>



### function getCurrentTicksPerSecond 

_Gets the current ticks per second (TPS)._ 
```C++
virtual float endstone::Server::getCurrentTicksPerSecond () = 0
```





**Returns:**

The current ticks per second 





        

<hr>



### function getEnchantmentRegistry 

```C++
virtual Registry < Enchantment > & endstone::Server::getEnchantmentRegistry () const = 0
```



Returns the registry for all the enchantments.




**Returns:**

the enchantment registry. 





        

<hr>



### function getIpBanList 

```C++
virtual IpBanList & endstone::Server::getIpBanList () const = 0
```



Gets the IP ban list.




**Returns:**

The IP ban list 





        

<hr>



### function getItemFactory 

```C++
virtual ItemFactory & endstone::Server::getItemFactory () const = 0
```



Gets the instance of the item factory (for [**ItemMeta**](classendstone_1_1ItemMeta.md)).




**Returns:**

the item factory 




**See also:** [**ItemFactory**](classendstone_1_1ItemFactory.md) 



        

<hr>



### function getLanguage 

_Gets the current language interface used by the server._ 
```C++
virtual Language & endstone::Server::getLanguage () const = 0
```





**Returns:**

[**Language**](classendstone_1_1Language.md) interface used by the server for translations. 





        

<hr>



### function getLevel 

_Gets the server level._ 
```C++
virtual Level * endstone::Server::getLevel () const = 0
```





**Returns:**

the server level 





        

<hr>



### function getLogger 

_Returns the primary logger associated with this server instance._ 
```C++
virtual Logger & endstone::Server::getLogger () const = 0
```





**Returns:**

[**Logger**](classendstone_1_1Logger.md) associated with this server 





        

<hr>



### function getMaxPlayers 

_Get the maximum amount of players which can login to this server._ 
```C++
virtual int endstone::Server::getMaxPlayers () const = 0
```





**Returns:**

the amount of players this server allows 





        

<hr>



### function getMinecraftVersion 

_Gets the Minecraft version that this server is running._ 
```C++
virtual std::string endstone::Server::getMinecraftVersion () const = 0
```





**Returns:**

version of Minecraft 





        

<hr>



### function getName 

_Gets the name of this server implementation._ 
```C++
virtual std::string endstone::Server::getName () const = 0
```





**Returns:**

name of this server implementation 





        

<hr>



### function getOnlineMode 

_Gets whether the_ [_**Server**_](classendstone_1_1Server.md) _is in online mode or not._
```C++
virtual bool endstone::Server::getOnlineMode () const = 0
```





**Returns:**

true if the server authenticates clients, false otherwise 





        

<hr>



### function getOnlinePlayers 

_Gets a list of all currently online players._ 
```C++
virtual std::vector< Player * > endstone::Server::getOnlinePlayers () const = 0
```





**Returns:**

a list of currently online players. 





        

<hr>



### function getPlayer [1/2]

_Gets the player with the given_ [_**UUID**_](classendstone_1_1UUID.md) _._
```C++
virtual Player * endstone::Server::getPlayer (
    endstone::UUID id
) const = 0
```





**Parameters:**


* `id` [**UUID**](classendstone_1_1UUID.md) of the player to retrieve 



**Returns:**

a player object if one was found, null otherwise 





        

<hr>



### function getPlayer [2/2]

_Gets the player with the exact given name, case-insensitive._ 
```C++
virtual Player * endstone::Server::getPlayer (
    std::string name
) const = 0
```





**Parameters:**


* `name` Exact name of the player to retrieve 



**Returns:**

a player object if one was found, null otherwise 





        

<hr>



### function getPluginCommand 

_Gets a_ [_**PluginCommand**_](classendstone_1_1PluginCommand.md) _with the given name or alias._
```C++
virtual PluginCommand * endstone::Server::getPluginCommand (
    std::string name
) const = 0
```





**Parameters:**


* `name` the name of the command to retrieve 



**Returns:**

a plugin command if found, null otherwise 





        

<hr>



### function getPluginManager 

_Gets the plugin manager for interfacing with plugins._ 
```C++
virtual PluginManager & endstone::Server::getPluginManager () const = 0
```





**Returns:**

a plugin manager for this [**Server**](classendstone_1_1Server.md) instance 





        

<hr>



### function getScheduler 

_Gets the scheduler for managing scheduled events._ 
```C++
virtual Scheduler & endstone::Server::getScheduler () const = 0
```





**Returns:**

a scheduling service for this server 





        

<hr>



### function getScoreboard 

_Gets the primary_ [_**Scoreboard**_](classendstone_1_1Scoreboard.md) _controlled by the server._
```C++
virtual Scoreboard * endstone::Server::getScoreboard () const = 0
```



This [**Scoreboard**](classendstone_1_1Scoreboard.md) is saved by the server, is affected by the /scoreboard command, and is the scoreboard shown by default to players. This will only exist after the level has been loaded.




**Returns:**

the default server scoreboard 





        

<hr>



### function getServiceManager 

```C++
virtual ServiceManager & endstone::Server::getServiceManager () const = 0
```



Gets the service manager.




**Returns:**

The service manager 





        

<hr>



### function getStartTime 

_Gets the start time of the server._ 
```C++
virtual std::chrono::system_clock::time_point endstone::Server::getStartTime () = 0
```





**Returns:**

The start time of the server. 





        

<hr>



### function getVersion 

_Gets the version string of this server implementation._ 
```C++
virtual std::string endstone::Server::getVersion () const = 0
```





**Returns:**

version of this server implementation 





        

<hr>



### function isPrimaryThread 

_Checks the current thread against the expected primary server thread._ 
```C++
virtual bool endstone::Server::isPrimaryThread () const = 0
```





**Returns:**

true if the current thread matches the expected primary thread, false otherwise 





        

<hr>



### function operator= 

```C++
Server & endstone::Server::operator= (
    const  Server &
) = delete
```




<hr>



### function reload 

_Reloads the server configuration, functions, scripts and plugins._ 
```C++
virtual void endstone::Server::reload () = 0
```




<hr>



### function reloadData 

_Reload only the Minecraft data for the server._ 
```C++
virtual void endstone::Server::reloadData () = 0
```



This includes functions and script files from all behaviour packs. 


        

<hr>



### function setMaxPlayers 

_Set the maximum amount of players allowed to be logged in at once._ 
```C++
virtual Result< void > endstone::Server::setMaxPlayers (
    int max_players
) = 0
```





**Parameters:**


* `max_players` The maximum amount of concurrent players 




        

<hr>



### function shutdown 

_Shutdowns the server, stopping everything._ 
```C++
virtual void endstone::Server::shutdown () = 0
```




<hr>



### function ~Server 

```C++
virtual endstone::Server::~Server () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/server.h`

