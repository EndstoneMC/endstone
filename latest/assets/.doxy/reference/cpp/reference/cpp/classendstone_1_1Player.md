

# Class endstone::Player



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Player**](classendstone_1_1Player.md)



_Represents a player._ 

* `#include <endstone/player.h>`



Inherits the following classes: [endstone::Mob](classendstone_1_1Mob.md)


















































































































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Player**](#function-player-13) () = default<br> |
|   | [**Player**](#function-player-23) (const [**Player**](classendstone_1_1Player.md) &) = delete<br> |
|   | [**Player**](#function-player-33) ([**Player**](classendstone_1_1Player.md) &&) = delete<br> |
| virtual [**Player**](classendstone_1_1Player.md) \* | [**asPlayer**](#function-asplayer) () override const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Player**_](classendstone_1_1Player.md) _._ |
| virtual void | [**closeForm**](#function-closeform) () = 0<br>_Closes the forms that are currently open for the player._  |
| virtual const [**SocketAddress**](classendstone_1_1SocketAddress.md) & | [**getAddress**](#function-getaddress) () const = 0<br>_Gets the socket address of this player._  |
| virtual bool | [**getAllowFlight**](#function-getallowflight) () const = 0<br>_Determines if the_ [_**Player**_](classendstone_1_1Player.md) _is allowed to fly via jump key double-tap._ |
| virtual std::string | [**getDeviceId**](#function-getdeviceid) () const = 0<br>_Gets the player's current device id._  |
| virtual std::string | [**getDeviceOS**](#function-getdeviceos) () const = 0<br>_Gets the player's current device's operation system (OS)._  |
| virtual int | [**getExpLevel**](#function-getexplevel) () const = 0<br>_Gets the players current experience level._  |
| virtual float | [**getExpProgress**](#function-getexpprogress) () const = 0<br>_Gets the players current experience progress towards the next level._  |
| virtual float | [**getFlySpeed**](#function-getflyspeed) () const = 0<br>_Gets the current allowed speed that a client can fly._  |
| virtual GameMode | [**getGameMode**](#function-getgamemode) () const = 0<br>_Gets this player's current GameMode._  |
| virtual std::string | [**getGameVersion**](#function-getgameversion) () const = 0<br>_Gets the player's current game version._  |
| virtual [**PlayerInventory**](classendstone_1_1PlayerInventory.md) & | [**getInventory**](#function-getinventory) () const = 0<br>_Get the player's inventory._  |
| virtual std::string | [**getLocale**](#function-getlocale) () const = 0<br>_Gets the player's current locale._  |
| virtual std::chrono::milliseconds | [**getPing**](#function-getping) () const = 0<br>_Gets the player's average ping._  |
| virtual [**Scoreboard**](classendstone_1_1Scoreboard.md) & | [**getScoreboard**](#function-getscoreboard) () const = 0<br>_Gets the_ [_**Scoreboard**_](classendstone_1_1Scoreboard.md) _displayed to this player._ |
| virtual const [**Skin**](classendstone_1_1Skin.md) & | [**getSkin**](#function-getskin) () const = 0<br>_Gets the player's skin._  |
| virtual int | [**getTotalExp**](#function-gettotalexp) () const = 0<br>_Gets the players total experience points._  |
| virtual [**endstone::UUID**](classendstone_1_1UUID.md) | [**getUniqueId**](#function-getuniqueid) () const = 0<br>_Returns the_ [_**UUID**_](classendstone_1_1UUID.md) _of this player._ |
| virtual float | [**getWalkSpeed**](#function-getwalkspeed) () const = 0<br>_Gets the current allowed speed that a client can walk._  |
| virtual std::string | [**getXuid**](#function-getxuid) () const = 0<br>_Returns the Xbox User ID (XUID) of this player._  |
| virtual void | [**giveExp**](#function-giveexp) (int amount) = 0<br>_Gives the player the amount of experience specified._  |
| virtual void | [**giveExpLevels**](#function-giveexplevels) (int amount) = 0<br>_Gives the player the amount of experience levels specified._  |
| virtual bool | [**isFlying**](#function-isflying) () const = 0<br>_Checks to see if this player is currently flying or not._  |
| virtual void | [**kick**](#function-kick) (std::string message) const = 0<br>_Kicks player with custom kick message._  |
|  [**Player**](classendstone_1_1Player.md) & | [**operator=**](#function-operator) (const [**Player**](classendstone_1_1Player.md) &) = delete<br> |
|  [**Player**](classendstone_1_1Player.md) & | [**operator=**](#function-operator_1) ([**Player**](classendstone_1_1Player.md) &&) = delete<br> |
| virtual bool | [**performCommand**](#function-performcommand) (std::string command) const = 0<br>_Makes the player perform the given command._  |
| virtual void | [**resetTitle**](#function-resettitle) () const = 0<br>_Resets the title displayed to the player. This will clear the displayed title / subtitle and reset timings to their default values._  |
| virtual void | [**sendForm**](#function-sendform) (FormVariant form) = 0<br>_Sends a form to the player._  |
| virtual void | [**sendPacket**](#function-sendpacket) ([**Packet**](classendstone_1_1Packet.md) & packet) = 0<br>_Sends a packet to the player._  |
| virtual void | [**sendPopup**](#function-sendpopup) (std::string message) const = 0<br>_Sends this player a popup message._  |
| virtual void | [**sendTip**](#function-sendtip) (std::string message) const = 0<br>_Sends this player a tip message._  |
| virtual void | [**sendTitle**](#function-sendtitle-12) (std::string title, std::string subtitle) const = 0<br>_Sends a title and a subtitle message to the player. If they are empty strings, the display will be updated as such. The titles will be displayed with the default timings._  |
| virtual void | [**sendTitle**](#function-sendtitle-22) (std::string title, std::string subtitle, int fade\_in, int stay, int fade\_out) const = 0<br>_Sends a title and a subtitle message to the player. If they are empty strings, the display will be updated as such._  |
| virtual void | [**sendToast**](#function-sendtoast) (std::string title, std::string content) const = 0<br>_Sends this player a toast notification._  |
| virtual void | [**setAllowFlight**](#function-setallowflight) (bool flight) = 0<br>_Sets if the_ [_**Player**_](classendstone_1_1Player.md) _is allowed to fly via jump key double-tap._ |
| virtual void | [**setExpLevel**](#function-setexplevel) (int level) = 0<br>_Sets the players current experience level._  |
| virtual void | [**setExpProgress**](#function-setexpprogress) (float progress) = 0<br>_Sets the players current experience progress towards the next level._  |
| virtual void | [**setFlySpeed**](#function-setflyspeed) (float value) const = 0<br> |
| virtual void | [**setFlying**](#function-setflying) (bool value) = 0<br>_Makes this player start or stop flying._  |
| virtual void | [**setGameMode**](#function-setgamemode) (GameMode mode) = 0<br>_Sets this player's current GameMode._  |
| virtual void | [**setScoreboard**](#function-setscoreboard) ([**Scoreboard**](classendstone_1_1Scoreboard.md) & scoreboard) = 0<br> |
| virtual void | [**setWalkSpeed**](#function-setwalkspeed) (float value) const = 0<br> |
| virtual void | [**transfer**](#function-transfer) (std::string host, int port) const = 0<br>_Transfers the player to another server._  |
| virtual void | [**updateCommands**](#function-updatecommands) () const = 0<br>_Send the list of commands to the client._  |
|   | [**~Player**](#function-player) () override<br> |


## Public Functions inherited from endstone::Mob

See [endstone::Mob](classendstone_1_1Mob.md)

| Type | Name |
| ---: | :--- |
| virtual bool | [**isGliding**](classendstone_1_1Mob.md#function-isgliding) () const = 0<br>_Checks to see if an actor is gliding, such as using an Elytra._  |


## Public Functions inherited from endstone::Actor

See [endstone::Actor](classendstone_1_1Actor.md)

| Type | Name |
| ---: | :--- |
|   | [**Actor**](classendstone_1_1Actor.md#function-actor-13) () = default<br> |
|   | [**Actor**](classendstone_1_1Actor.md#function-actor-23) (const [**Actor**](classendstone_1_1Actor.md) &) = delete<br> |
|   | [**Actor**](classendstone_1_1Actor.md#function-actor-33) ([**Actor**](classendstone_1_1Actor.md) &&) = delete<br> |
| virtual [**Dimension**](classendstone_1_1Dimension.md) & | [**getDimension**](classendstone_1_1Actor.md#function-getdimension) () const = 0<br> |
| virtual std::int64\_t | [**getId**](classendstone_1_1Actor.md#function-getid) () const = 0<br>_Returns a unique id for this actor._  |
| virtual [**Level**](classendstone_1_1Level.md) & | [**getLevel**](classendstone_1_1Actor.md#function-getlevel) () const = 0<br> |
| virtual [**Location**](classendstone_1_1Location.md) | [**getLocation**](classendstone_1_1Actor.md#function-getlocation) () const = 0<br> |
| virtual std::uint64\_t | [**getRuntimeId**](classendstone_1_1Actor.md#function-getruntimeid) () const = 0<br> |
| virtual [**Vector**](classendstone_1_1Vector.md)&lt; float &gt; | [**getVelocity**](classendstone_1_1Actor.md#function-getvelocity) () const = 0<br> |
| virtual bool | [**isDead**](classendstone_1_1Actor.md#function-isdead) () const = 0<br>_Returns true if this actor has been marked for removal._  |
| virtual bool | [**isInLava**](classendstone_1_1Actor.md#function-isinlava) () const = 0<br> |
| virtual bool | [**isInWater**](classendstone_1_1Actor.md#function-isinwater) () const = 0<br> |
| virtual bool | [**isOnGround**](classendstone_1_1Actor.md#function-isonground) () const = 0<br> |
|  [**Actor**](classendstone_1_1Actor.md) & | [**operator=**](classendstone_1_1Actor.md#function-operator) (const [**Actor**](classendstone_1_1Actor.md) &) = delete<br> |
|  [**Actor**](classendstone_1_1Actor.md) & | [**operator=**](classendstone_1_1Actor.md#function-operator_1) ([**Actor**](classendstone_1_1Actor.md) &&) = delete<br> |
| virtual void | [**setRotation**](classendstone_1_1Actor.md#function-setrotation) (float yaw, float pitch) = 0<br> |
| virtual void | [**teleport**](classendstone_1_1Actor.md#function-teleport-12) ([**Location**](classendstone_1_1Location.md) location) = 0<br> |
| virtual void | [**teleport**](classendstone_1_1Actor.md#function-teleport-22) ([**Actor**](classendstone_1_1Actor.md) & target) = 0<br> |
|   | [**~Actor**](classendstone_1_1Actor.md#function-actor) () override<br> |


## Public Functions inherited from endstone::CommandSender

See [endstone::CommandSender](classendstone_1_1CommandSender.md)

| Type | Name |
| ---: | :--- |
|   | [**CommandSender**](classendstone_1_1CommandSender.md#function-commandsender) () = default<br> |
| virtual [**CommandSender**](classendstone_1_1CommandSender.md) \* | [**asCommandSender**](classendstone_1_1CommandSender.md#function-ascommandsender) () override const<br>_Casts a_ [_**Permissible**_](classendstone_1_1Permissible.md) _as_[_**CommandSender**_](classendstone_1_1CommandSender.md) _._ |
| virtual [**ConsoleCommandSender**](classendstone_1_1ConsoleCommandSender.md) \* | [**asConsole**](classendstone_1_1CommandSender.md#function-asconsole) () const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as Console._ |
| virtual [**Player**](classendstone_1_1Player.md) \* | [**asPlayer**](classendstone_1_1CommandSender.md#function-asplayer) () const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Player**_](classendstone_1_1Player.md) _._ |
| virtual std::string | [**getName**](classendstone_1_1CommandSender.md#function-getname) () const = 0<br>_Gets the name of this command sender._  |
| virtual [**Server**](classendstone_1_1Server.md) & | [**getServer**](classendstone_1_1CommandSender.md#function-getserver) () const = 0<br>_Returns the server instance that this command is running on._  |
| virtual void | [**sendErrorMessage**](classendstone_1_1CommandSender.md#function-senderrormessage-12) (const Message & message) const = 0<br>_Sends this sender a error message._  |
|  void | [**sendErrorMessage**](classendstone_1_1CommandSender.md#function-senderrormessage-22) (const fmt::format\_string&lt; Args... &gt; format, Args &&... args) const<br> |
| virtual void | [**sendMessage**](classendstone_1_1CommandSender.md#function-sendmessage-12) (const Message & message) const = 0<br>_Sends this sender a message._  |
|  void | [**sendMessage**](classendstone_1_1CommandSender.md#function-sendmessage-22) (const fmt::format\_string&lt; Args... &gt; format, Args &&... args) const<br> |
|   | [**~CommandSender**](classendstone_1_1CommandSender.md#function-commandsender) () override<br> |


## Public Functions inherited from endstone::Permissible

See [endstone::Permissible](classendstone_1_1Permissible.md)

| Type | Name |
| ---: | :--- |
| virtual [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) \* | [**addAttachment**](classendstone_1_1Permissible.md#function-addattachment-12) ([**Plugin**](classendstone_1_1Plugin.md) & plugin, const std::string & name, bool value) = 0<br> |
| virtual [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) \* | [**addAttachment**](classendstone_1_1Permissible.md#function-addattachment-22) ([**Plugin**](classendstone_1_1Plugin.md) & plugin) = 0<br> |
| virtual [**CommandSender**](classendstone_1_1CommandSender.md) \* | [**asCommandSender**](classendstone_1_1Permissible.md#function-ascommandsender) () const = 0<br> |
| virtual std::unordered\_set&lt; [**PermissionAttachmentInfo**](classendstone_1_1PermissionAttachmentInfo.md) \* &gt; | [**getEffectivePermissions**](classendstone_1_1Permissible.md#function-geteffectivepermissions) () const = 0<br> |
| virtual bool | [**hasPermission**](classendstone_1_1Permissible.md#function-haspermission-12) (std::string name) const = 0<br> |
| virtual bool | [**hasPermission**](classendstone_1_1Permissible.md#function-haspermission-22) (const [**Permission**](classendstone_1_1Permission.md) & perm) const = 0<br> |
| virtual bool | [**isOp**](classendstone_1_1Permissible.md#function-isop) () const = 0<br>_Checks if this object is a server operator._  |
| virtual bool | [**isPermissionSet**](classendstone_1_1Permissible.md#function-ispermissionset-12) (std::string name) const = 0<br> |
| virtual bool | [**isPermissionSet**](classendstone_1_1Permissible.md#function-ispermissionset-22) (const [**Permission**](classendstone_1_1Permission.md) & perm) const = 0<br> |
| virtual void | [**recalculatePermissions**](classendstone_1_1Permissible.md#function-recalculatepermissions) () = 0<br> |
| virtual bool | [**removeAttachment**](classendstone_1_1Permissible.md#function-removeattachment) ([**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) & attachment) = 0<br> |
| virtual void | [**setOp**](classendstone_1_1Permissible.md#function-setop) (bool value) = 0<br>_Sets the operator status of this object._  |
| virtual  | [**~Permissible**](classendstone_1_1Permissible.md#function-permissible) () = default<br> |












## Protected Types

| Type | Name |
| ---: | :--- |
| typedef std::variant&lt; [**MessageForm**](classendstone_1_1MessageForm.md), [**ActionForm**](classendstone_1_1ActionForm.md), [**ModalForm**](classendstone_1_1ModalForm.md) &gt; | [**FormVariant**](#typedef-formvariant)  <br> |
























































































































## Public Functions Documentation




### function Player [1/3]

```C++
endstone::Player::Player () = default
```




<hr>



### function Player [2/3]

```C++
endstone::Player::Player (
    const Player &
) = delete
```




<hr>



### function Player [3/3]

```C++
endstone::Player::Player (
    Player &&
) = delete
```




<hr>



### function asPlayer 

_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Player**_](classendstone_1_1Player.md) _._
```C++
inline virtual Player * endstone::Player::asPlayer () override const
```





**Returns:**

[**Player**](classendstone_1_1Player.md), nullptr if not a [**Player**](classendstone_1_1Player.md) 





        
Implements [*endstone::CommandSender::asPlayer*](classendstone_1_1CommandSender.md#function-asplayer)


<hr>



### function closeForm 

```C++
virtual void endstone::Player::closeForm () = 0
```




<hr>



### function getAddress 

_Gets the socket address of this player._ 
```C++
virtual const SocketAddress & endstone::Player::getAddress () const = 0
```





**Returns:**

the player's socket address 





        

<hr>



### function getAllowFlight 

_Determines if the_ [_**Player**_](classendstone_1_1Player.md) _is allowed to fly via jump key double-tap._
```C++
virtual bool endstone::Player::getAllowFlight () const = 0
```





**Returns:**

True if the player is allowed to fly. 





        

<hr>



### function getDeviceId 

_Gets the player's current device id._ 
```C++
virtual std::string endstone::Player::getDeviceId () const = 0
```





**Returns:**

the player's device id 





        

<hr>



### function getDeviceOS 

_Gets the player's current device's operation system (OS)._ 
```C++
virtual std::string endstone::Player::getDeviceOS () const = 0
```





**Returns:**

the player's device OS 





        

<hr>



### function getExpLevel 

_Gets the players current experience level._ 
```C++
virtual int endstone::Player::getExpLevel () const = 0
```





**Returns:**

Current experience level 





        

<hr>



### function getExpProgress 

_Gets the players current experience progress towards the next level._ 
```C++
virtual float endstone::Player::getExpProgress () const = 0
```



This is a percentage value. 0.0 is "no progress" and 1.0 is "next level".




**Returns:**

Current experience points 





        

<hr>



### function getFlySpeed 

_Gets the current allowed speed that a client can fly._ 
```C++
virtual float endstone::Player::getFlySpeed () const = 0
```





**Returns:**

The current allowed speed, default is 0.05. 





        

<hr>



### function getGameMode 

_Gets this player's current GameMode._ 
```C++
virtual GameMode endstone::Player::getGameMode () const = 0
```





**Returns:**

Current game mode 





        

<hr>



### function getGameVersion 

_Gets the player's current game version._ 
```C++
virtual std::string endstone::Player::getGameVersion () const = 0
```





**Returns:**

the player's game version 





        

<hr>



### function getInventory 

_Get the player's inventory._ 
```C++
virtual PlayerInventory & endstone::Player::getInventory () const = 0
```





**Returns:**

The inventory of the player, this also contains the armor slots. 





        

<hr>



### function getLocale 

_Gets the player's current locale._ 
```C++
virtual std::string endstone::Player::getLocale () const = 0
```





**Returns:**

the player's locale 





        

<hr>



### function getPing 

_Gets the player's average ping._ 
```C++
virtual std::chrono::milliseconds endstone::Player::getPing () const = 0
```





**Returns:**

player ping 





        

<hr>



### function getScoreboard 

_Gets the_ [_**Scoreboard**_](classendstone_1_1Scoreboard.md) _displayed to this player._
```C++
virtual Scoreboard & endstone::Player::getScoreboard () const = 0
```





**Returns:**

The current scoreboard seen by this player 





        

<hr>



### function getSkin 

_Gets the player's skin._ 
```C++
virtual const Skin & endstone::Player::getSkin () const = 0
```





**Returns:**

the player's skin 





        

<hr>



### function getTotalExp 

_Gets the players total experience points._ 
```C++
virtual int endstone::Player::getTotalExp () const = 0
```



This refers to the total amount of experience the player has collected over time and is not currently displayed to the client.




**Returns:**

Current total experience points 





        

<hr>



### function getUniqueId 

_Returns the_ [_**UUID**_](classendstone_1_1UUID.md) _of this player._
```C++
virtual endstone::UUID endstone::Player::getUniqueId () const = 0
```





**Returns:**

[**Player**](classendstone_1_1Player.md) [**UUID**](classendstone_1_1UUID.md) 





        

<hr>



### function getWalkSpeed 

_Gets the current allowed speed that a client can walk._ 
```C++
virtual float endstone::Player::getWalkSpeed () const = 0
```





**Returns:**

The current allowed speed, default is 0.10. 





        

<hr>



### function getXuid 

_Returns the Xbox User ID (XUID) of this player._ 
```C++
virtual std::string endstone::Player::getXuid () const = 0
```





**Returns:**

[**Player**](classendstone_1_1Player.md) XUID 





        

<hr>



### function giveExp 

_Gives the player the amount of experience specified._ 
```C++
virtual void endstone::Player::giveExp (
    int amount
) = 0
```





**Parameters:**


* `amount` Exp amount to give 




        

<hr>



### function giveExpLevels 

_Gives the player the amount of experience levels specified._ 
```C++
virtual void endstone::Player::giveExpLevels (
    int amount
) = 0
```





**Parameters:**


* `amount` amount of experience levels to give or take 




        

<hr>



### function isFlying 

_Checks to see if this player is currently flying or not._ 
```C++
virtual bool endstone::Player::isFlying () const = 0
```





**Returns:**

True if the player is flying, else false. 





        

<hr>



### function kick 

_Kicks player with custom kick message._ 
```C++
virtual void endstone::Player::kick (
    std::string message
) const = 0
```





**Parameters:**


* `message` kick message 




        

<hr>



### function operator= 

```C++
Player & endstone::Player::operator= (
    const Player &
) = delete
```




<hr>



### function operator= 

```C++
Player & endstone::Player::operator= (
    Player &&
) = delete
```




<hr>



### function performCommand 

_Makes the player perform the given command._ 
```C++
virtual bool endstone::Player::performCommand (
    std::string command
) const = 0
```





**Parameters:**


* `command` [**Command**](classendstone_1_1Command.md) to perform 



**Returns:**

true if the command was successful, otherwise false 





        

<hr>



### function resetTitle 

```C++
virtual void endstone::Player::resetTitle () const = 0
```




<hr>



### function sendForm 

_Sends a form to the player._ 
```C++
virtual void endstone::Player::sendForm (
    FormVariant form
) = 0
```





**Parameters:**


* `form` The form to send 




        

<hr>



### function sendPacket 

_Sends a packet to the player._ 
```C++
virtual void endstone::Player::sendPacket (
    Packet & packet
) = 0
```





**Parameters:**


* `packet` The packet to be sent. 




        

<hr>



### function sendPopup 

_Sends this player a popup message._ 
```C++
virtual void endstone::Player::sendPopup (
    std::string message
) const = 0
```





**Parameters:**


* `message` Message to be displayed 




        

<hr>



### function sendTip 

_Sends this player a tip message._ 
```C++
virtual void endstone::Player::sendTip (
    std::string message
) const = 0
```





**Parameters:**


* `message` Message to be displayed 




        

<hr>



### function sendTitle [1/2]

_Sends a title and a subtitle message to the player. If they are empty strings, the display will be updated as such. The titles will be displayed with the default timings._ 
```C++
virtual void endstone::Player::sendTitle (
    std::string title,
    std::string subtitle
) const = 0
```





**Parameters:**


* `title` Title text 
* `subtitle` Subtitle text 




        

<hr>



### function sendTitle [2/2]

_Sends a title and a subtitle message to the player. If they are empty strings, the display will be updated as such._ 
```C++
virtual void endstone::Player::sendTitle (
    std::string title,
    std::string subtitle,
    int fade_in,
    int stay,
    int fade_out
) const = 0
```





**Parameters:**


* `title` Title text 
* `subtitle` Subtitle text 
* `fade_in` time in ticks for titles to fade in. Defaults to 10. 
* `stay` time in ticks for titles to stay. Defaults to 70. 
* `fade_out` time in ticks for titles to fade out. Defaults to 20. 




        

<hr>



### function sendToast 

_Sends this player a toast notification._ 
```C++
virtual void endstone::Player::sendToast (
    std::string title,
    std::string content
) const = 0
```





**Parameters:**


* `title` The title of the toast notification. 
* `content` The content of the toast notification. 




        

<hr>



### function setAllowFlight 

_Sets if the_ [_**Player**_](classendstone_1_1Player.md) _is allowed to fly via jump key double-tap._
```C++
virtual void endstone::Player::setAllowFlight (
    bool flight
) = 0
```





**Parameters:**


* `flight` If flight should be allowed. 




        

<hr>



### function setExpLevel 

_Sets the players current experience level._ 
```C++
virtual void endstone::Player::setExpLevel (
    int level
) = 0
```





**Parameters:**


* `level` New experience level 




        

<hr>



### function setExpProgress 

_Sets the players current experience progress towards the next level._ 
```C++
virtual void endstone::Player::setExpProgress (
    float progress
) = 0
```



This is a percentage value. 0.0 is "no progress" and 1.0 is "next level".




**Parameters:**


* `progress` New experience progress 




        

<hr>



### function setFlySpeed 


```C++
virtual void endstone::Player::setFlySpeed (
    float value
) const = 0
```



Sets the speed at which a client will fly.




**Parameters:**


* `value` The new speed. 




        

<hr>



### function setFlying 

_Makes this player start or stop flying._ 
```C++
virtual void endstone::Player::setFlying (
    bool value
) = 0
```





**Parameters:**


* `value` True to fly. 




        

<hr>



### function setGameMode 

_Sets this player's current GameMode._ 
```C++
virtual void endstone::Player::setGameMode (
    GameMode mode
) = 0
```





**Parameters:**


* `mode` New game mode 




        

<hr>



### function setScoreboard 


```C++
virtual void endstone::Player::setScoreboard (
    Scoreboard & scoreboard
) = 0
```



@breif Sets the player's visible [**Scoreboard**](classendstone_1_1Scoreboard.md).




**Parameters:**


* `scoreboard` New [**Scoreboard**](classendstone_1_1Scoreboard.md) for the player 




        

<hr>



### function setWalkSpeed 


```C++
virtual void endstone::Player::setWalkSpeed (
    float value
) const = 0
```



Sets the speed at which a client will walk.




**Parameters:**


* `value` The new speed. 




        

<hr>



### function transfer 

_Transfers the player to another server._ 
```C++
virtual void endstone::Player::transfer (
    std::string host,
    int port
) const = 0
```





**Parameters:**


* `host` [**Server**](classendstone_1_1Server.md) address to transfer the player to. 
* `port` [**Server**](classendstone_1_1Server.md) port to transfer the player to 




        

<hr>



### function updateCommands 

_Send the list of commands to the client._ 
```C++
virtual void endstone::Player::updateCommands () const = 0
```



Generally useful to ensure the client has a complete list of commands after permission changes are done. 


        

<hr>



### function ~Player 

```C++
endstone::Player::~Player () override
```




<hr>
## Protected Types Documentation




### typedef FormVariant 

```C++
using endstone::Player::FormVariant =  std::variant<MessageForm, ActionForm, ModalForm>;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/player.h`

