

# Class endstone::Player



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Player**](classendstone_1_1Player.md)



_Represents a player._ 

* `#include <endstone/player.h>`



Inherits the following classes: [endstone::Mob](classendstone_1_1Mob.md),  [endstone::OfflinePlayer](classendstone_1_1OfflinePlayer.md)






































































































































## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**Player**](classendstone_1_1Player.md) \* | [**asPlayer**](#function-asplayer) () override const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Player**_](classendstone_1_1Player.md) _._ |
| virtual [**void**](classendstone_1_1Vector.md) | [**closeForm**](#function-closeform) () = 0<br>_Closes the forms that are currently open for the player._  |
| virtual [**SocketAddress**](classendstone_1_1SocketAddress.md) | [**getAddress**](#function-getaddress) () const = 0<br>_Gets the socket address of this player._  |
| virtual [**bool**](classendstone_1_1Vector.md) | [**getAllowFlight**](#function-getallowflight) () const = 0<br>_Determines if the_ [_**Player**_](classendstone_1_1Player.md) _is allowed to fly via jump key double-tap._ |
| virtual std::string | [**getDeviceId**](#function-getdeviceid) () const = 0<br>_Gets the player's current device id._  |
| virtual std::string | [**getDeviceOS**](#function-getdeviceos) () const = 0<br>_Gets the player's current device's operation system (OS)._  |
| virtual [**int**](classendstone_1_1Vector.md) | [**getExpLevel**](#function-getexplevel) () const = 0<br>_Gets the players current experience level._  |
| virtual [**float**](classendstone_1_1Vector.md) | [**getExpProgress**](#function-getexpprogress) () const = 0<br>_Gets the players current experience progress towards the next level._  |
| virtual [**float**](classendstone_1_1Vector.md) | [**getFlySpeed**](#function-getflyspeed) () const = 0<br>_Gets the current allowed speed that a client can fly._  |
| virtual [**GameMode**](namespaceendstone.md#enum-gamemode) | [**getGameMode**](#function-getgamemode) () const = 0<br>_Gets this player's current GameMode._  |
| virtual std::string | [**getGameVersion**](#function-getgameversion) () const = 0<br>_Gets the player's current game version._  |
| virtual [**PlayerInventory**](classendstone_1_1PlayerInventory.md) & | [**getInventory**](#function-getinventory) () const = 0<br>_Get the player's inventory._  |
| virtual std::string | [**getLocale**](#function-getlocale) () const = 0<br>_Gets the player's current locale._  |
| virtual std::string | [**getName**](#function-getname) () override const = 0<br>_Returns the name of this player._  |
| virtual std::chrono::milliseconds | [**getPing**](#function-getping) () const = 0<br>_Gets the player's average ping._  |
| virtual [**Scoreboard**](classendstone_1_1Scoreboard.md) & | [**getScoreboard**](#function-getscoreboard) () const = 0<br>_Gets the_ [_**Scoreboard**_](classendstone_1_1Scoreboard.md) _displayed to this player._ |
| virtual [**const**](classendstone_1_1Vector.md) [**Skin**](classendstone_1_1Skin.md) & | [**getSkin**](#function-getskin) () const = 0<br>_Gets the player's skin._  |
| virtual [**int**](classendstone_1_1Vector.md) | [**getTotalExp**](#function-gettotalexp) () const = 0<br>_Gets the players total experience points._  |
| virtual [**float**](classendstone_1_1Vector.md) | [**getWalkSpeed**](#function-getwalkspeed) () const = 0<br>_Gets the current allowed speed that a client can walk._  |
| virtual std::string | [**getXuid**](#function-getxuid) () const = 0<br>_Returns the Xbox User ID (XUID) of this player._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**giveExp**](#function-giveexp) ([**int**](classendstone_1_1Vector.md) amount) = 0<br>_Gives the player the amount of experience specified._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**giveExpLevels**](#function-giveexplevels) ([**int**](classendstone_1_1Vector.md) amount) = 0<br>_Gives the player the amount of experience levels specified._  |
| virtual [**bool**](classendstone_1_1Vector.md) | [**isFlying**](#function-isflying) () const = 0<br>_Checks to see if this player is currently flying or not._  |
| virtual [**bool**](classendstone_1_1Vector.md) | [**isSneaking**](#function-issneaking) () const = 0<br>_Returns if the player is in sneak mode._  |
| virtual [**bool**](classendstone_1_1Vector.md) | [**isSprinting**](#function-issprinting) () const = 0<br>_Gets whether the player is sprinting or not._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**kick**](#function-kick) (std::string message) const = 0<br>_Kicks player with custom kick message._  |
| virtual [**bool**](classendstone_1_1Vector.md) | [**performCommand**](#function-performcommand) (std::string command) const = 0<br>_Makes the player perform the given command._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**playSound**](#function-playsound) ([**Location**](classendstone_1_1Location.md) location, std::string sound, [**float**](classendstone_1_1Vector.md) volume, [**float**](classendstone_1_1Vector.md) pitch) = 0<br>_Play a sound for a player at the location._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**resetTitle**](#function-resettitle) () const = 0<br>_Resets the title displayed to the player. This will clear the displayed title / subtitle and reset timings to their default values._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**sendForm**](#function-sendform) (FormVariant form) = 0<br>_Sends a form to the player._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**sendPacket**](#function-sendpacket) ([**int**](classendstone_1_1Vector.md) packet\_id, std::string\_view payload) const = 0<br>_Sends a packet to the player._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**sendPopup**](#function-sendpopup) (std::string message) const = 0<br>_Sends this player a popup message._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**sendTip**](#function-sendtip) (std::string message) const = 0<br>_Sends this player a tip message._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**sendTitle**](#function-sendtitle-12) (std::string title, std::string subtitle) const = 0<br>_Sends a title and a subtitle message to the player. If they are empty strings, the display will be updated as such. The titles will be displayed with the default timings._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**sendTitle**](#function-sendtitle-22) (std::string title, std::string subtitle, [**int**](classendstone_1_1Vector.md) fade\_in, [**int**](classendstone_1_1Vector.md) stay, [**int**](classendstone_1_1Vector.md) fade\_out) const = 0<br>_Sends a title and a subtitle message to the player. If they are empty strings, the display will be updated as such._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**sendToast**](#function-sendtoast) (std::string title, std::string content) const = 0<br>_Sends this player a toast notification._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**setAllowFlight**](#function-setallowflight) ([**bool**](classendstone_1_1Vector.md) flight) = 0<br>_Sets if the_ [_**Player**_](classendstone_1_1Player.md) _is allowed to fly via jump key double-tap._ |
| virtual Result&lt; [**void**](classendstone_1_1Vector.md) &gt; | [**setExpLevel**](#function-setexplevel) ([**int**](classendstone_1_1Vector.md) level) = 0<br>_Sets the players current experience level._  |
| virtual Result&lt; [**void**](classendstone_1_1Vector.md) &gt; | [**setExpProgress**](#function-setexpprogress) ([**float**](classendstone_1_1Vector.md) progress) = 0<br>_Sets the players current experience progress towards the next level._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**setFlySpeed**](#function-setflyspeed) ([**float**](classendstone_1_1Vector.md) value) const = 0<br> |
| virtual Result&lt; [**void**](classendstone_1_1Vector.md) &gt; | [**setFlying**](#function-setflying) ([**bool**](classendstone_1_1Vector.md) value) = 0<br>_Makes this player start or stop flying._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**setGameMode**](#function-setgamemode) ([**GameMode**](namespaceendstone.md#enum-gamemode) mode) = 0<br>_Sets this player's current GameMode._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**setScoreboard**](#function-setscoreboard) ([**Scoreboard**](classendstone_1_1Scoreboard.md) & scoreboard) = 0<br> |
| virtual [**void**](classendstone_1_1Vector.md) | [**setSneaking**](#function-setsneaking) ([**bool**](classendstone_1_1Vector.md) sneak) = 0<br>_Sets the sneak mode of the player._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**setSprinting**](#function-setsprinting) ([**bool**](classendstone_1_1Vector.md) sprinting) = 0<br>_Sets whether the player is sprinting or not._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**setWalkSpeed**](#function-setwalkspeed) ([**float**](classendstone_1_1Vector.md) value) const = 0<br> |
| virtual [**void**](classendstone_1_1Vector.md) | [**spawnParticle**](#function-spawnparticle-14) (std::string name, [**Location**](classendstone_1_1Location.md) location) const = 0<br>_Spawns the particle at the target location._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**spawnParticle**](#function-spawnparticle-24) (std::string name, [**float**](classendstone_1_1Vector.md) x, [**float**](classendstone_1_1Vector.md) y, [**float**](classendstone_1_1Vector.md) z) const = 0<br>_Spawns the particle at the target location._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**spawnParticle**](#function-spawnparticle-34) (std::string name, [**Location**](classendstone_1_1Location.md) location, std::optional&lt; std::string &gt; molang\_variables\_json) const = 0<br>_Spawns the particle at the target location._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**spawnParticle**](#function-spawnparticle-44) (std::string name, [**float**](classendstone_1_1Vector.md) x, [**float**](classendstone_1_1Vector.md) y, [**float**](classendstone_1_1Vector.md) z, std::optional&lt; std::string &gt; molang\_variables\_json) const = 0<br>_Spawns the particle at the target location._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**stopAllSounds**](#function-stopallsounds) () = 0<br>_Stop all sounds from playing._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**stopSound**](#function-stopsound) (std::string sound) = 0<br>_Stop the specified sound from playing._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**transfer**](#function-transfer) (std::string host, [**int**](classendstone_1_1Vector.md) port) const = 0<br>_Transfers the player to another server._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**updateCommands**](#function-updatecommands) () const = 0<br>_Send the list of commands to the client._  |


## Public Functions inherited from endstone::Mob

See [endstone::Mob](classendstone_1_1Mob.md)

| Type | Name |
| ---: | :--- |
| virtual [**Mob**](classendstone_1_1Mob.md) \* | [**asMob**](classendstone_1_1Mob.md#function-asmob) () override const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Mob**_](classendstone_1_1Mob.md) _._ |
| virtual [**bool**](classendstone_1_1Vector.md) | [**isGliding**](classendstone_1_1Mob.md#function-isgliding) () const = 0<br>_Checks to see if an actor is gliding, such as using an Elytra._  |


## Public Functions inherited from endstone::Actor

See [endstone::Actor](classendstone_1_1Actor.md)

| Type | Name |
| ---: | :--- |
| virtual [**bool**](classendstone_1_1Vector.md) | [**addScoreboardTag**](classendstone_1_1Actor.md#function-addscoreboardtag) (std::string tag) const = 0<br>_Adds a tag to this actor._  |
| virtual [**Actor**](classendstone_1_1Actor.md) \* | [**asActor**](classendstone_1_1Actor.md#function-asactor) () override const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Actor**_](classendstone_1_1Actor.md) _._ |
| virtual [**Dimension**](classendstone_1_1Dimension.md) & | [**getDimension**](classendstone_1_1Actor.md#function-getdimension) () const = 0<br> |
| virtual [**int**](classendstone_1_1Vector.md) | [**getHealth**](classendstone_1_1Actor.md#function-gethealth) () const = 0<br>_Gets the entity's health from 0 to its max possible value, where 0 is dead._  |
| virtual std::int64\_t | [**getId**](classendstone_1_1Actor.md#function-getid) () const = 0<br>_Returns a unique id for this actor._  |
| virtual [**Level**](classendstone_1_1Level.md) & | [**getLevel**](classendstone_1_1Actor.md#function-getlevel) () const = 0<br> |
| virtual [**Location**](classendstone_1_1Location.md) | [**getLocation**](classendstone_1_1Actor.md#function-getlocation) () const = 0<br> |
| virtual [**int**](classendstone_1_1Vector.md) | [**getMaxHealth**](classendstone_1_1Actor.md#function-getmaxhealth) () const = 0<br>_Gets the maximum health this entity has._  |
| virtual std::string | [**getNameTag**](classendstone_1_1Actor.md#function-getnametag) () const = 0<br>_Gets the current name tag of the actor._  |
| virtual std::uint64\_t | [**getRuntimeId**](classendstone_1_1Actor.md#function-getruntimeid) () const = 0<br> |
| virtual std::string | [**getScoreTag**](classendstone_1_1Actor.md#function-getscoretag) () const = 0<br>_Gets the current score tag of the actor._  |
| virtual std::vector&lt; std::string &gt; | [**getScoreboardTags**](classendstone_1_1Actor.md#function-getscoreboardtags) () const = 0<br>_Returns a list of scoreboard tags for this actor._  |
| virtual std::string | [**getType**](classendstone_1_1Actor.md#function-gettype) () const = 0<br>_Get the type of the actor._  |
| virtual [**Vector**](classendstone_1_1Vector.md)&lt; [**float**](classendstone_1_1Vector.md) &gt; | [**getVelocity**](classendstone_1_1Actor.md#function-getvelocity) () const = 0<br> |
| virtual [**bool**](classendstone_1_1Vector.md) | [**isDead**](classendstone_1_1Actor.md#function-isdead) () const = 0<br>_Returns true if this actor has been marked for removal._  |
| virtual [**bool**](classendstone_1_1Vector.md) | [**isInLava**](classendstone_1_1Actor.md#function-isinlava) () const = 0<br> |
| virtual [**bool**](classendstone_1_1Vector.md) | [**isInWater**](classendstone_1_1Actor.md#function-isinwater) () const = 0<br> |
| virtual [**bool**](classendstone_1_1Vector.md) | [**isNameTagAlwaysVisible**](classendstone_1_1Actor.md#function-isnametagalwaysvisible) () const = 0<br>_Checks if the actor's name tag is always visible._  |
| virtual [**bool**](classendstone_1_1Vector.md) | [**isNameTagVisible**](classendstone_1_1Actor.md#function-isnametagvisible) () const = 0<br>_Checks if the actor's name tag is currently visible._  |
| virtual [**bool**](classendstone_1_1Vector.md) | [**isOnGround**](classendstone_1_1Actor.md#function-isonground) () const = 0<br> |
| virtual [**bool**](classendstone_1_1Vector.md) | [**isValid**](classendstone_1_1Actor.md#function-isvalid) () const = 0<br> |
| virtual [**void**](classendstone_1_1Vector.md) | [**remove**](classendstone_1_1Actor.md#function-remove) () = 0<br> |
| virtual [**bool**](classendstone_1_1Vector.md) | [**removeScoreboardTag**](classendstone_1_1Actor.md#function-removescoreboardtag) (std::string tag) const = 0<br>_Removes a given tag from this actor._  |
| virtual Result&lt; [**void**](classendstone_1_1Vector.md) &gt; | [**setHealth**](classendstone_1_1Actor.md#function-sethealth) ([**int**](classendstone_1_1Vector.md) health) const = 0<br> |
| virtual [**void**](classendstone_1_1Vector.md) | [**setNameTag**](classendstone_1_1Actor.md#function-setnametag) (std::string name) = 0<br>_Sets the name tag for the actor._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**setNameTagAlwaysVisible**](classendstone_1_1Actor.md#function-setnametagalwaysvisible) ([**bool**](classendstone_1_1Vector.md) visible) = 0<br>_Sets whether the actor's name tag should always be visible._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**setNameTagVisible**](classendstone_1_1Actor.md#function-setnametagvisible) ([**bool**](classendstone_1_1Vector.md) visible) = 0<br>_Sets if the actor's name tag is visible or not._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**setRotation**](classendstone_1_1Actor.md#function-setrotation) ([**float**](classendstone_1_1Vector.md) yaw, [**float**](classendstone_1_1Vector.md) pitch) = 0<br> |
| virtual [**void**](classendstone_1_1Vector.md) | [**setScoreTag**](classendstone_1_1Actor.md#function-setscoretag) (std::string score) = 0<br>_Sets the score tag for the actor._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**teleport**](classendstone_1_1Actor.md#function-teleport-12) ([**Location**](classendstone_1_1Location.md) location) = 0<br> |
| virtual [**void**](classendstone_1_1Vector.md) | [**teleport**](classendstone_1_1Actor.md#function-teleport-22) ([**Actor**](classendstone_1_1Actor.md) & target) = 0<br> |


## Public Functions inherited from endstone::CommandSender

See [endstone::CommandSender](classendstone_1_1CommandSender.md)

| Type | Name |
| ---: | :--- |
| virtual [**Actor**](classendstone_1_1Actor.md) \* | [**asActor**](classendstone_1_1CommandSender.md#function-asactor) () const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Actor**_](classendstone_1_1Actor.md) _._ |
| virtual [**CommandSender**](classendstone_1_1CommandSender.md) \* | [**asCommandSender**](classendstone_1_1CommandSender.md#function-ascommandsender) () override const<br>_Casts a_ [_**Permissible**_](classendstone_1_1Permissible.md) _as_[_**CommandSender**_](classendstone_1_1CommandSender.md) _._ |
| virtual [**ConsoleCommandSender**](classendstone_1_1ConsoleCommandSender.md) \* | [**asConsole**](classendstone_1_1CommandSender.md#function-asconsole) () const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as Console._ |
| virtual [**Mob**](classendstone_1_1Mob.md) \* | [**asMob**](classendstone_1_1CommandSender.md#function-asmob) () const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Mob**_](classendstone_1_1Mob.md) _._ |
| virtual [**Player**](classendstone_1_1Player.md) \* | [**asPlayer**](classendstone_1_1CommandSender.md#function-asplayer) () const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Player**_](classendstone_1_1Player.md) _._ |
| virtual [**ProxiedCommandSender**](classendstone_1_1ProxiedCommandSender.md) \* | [**asProxiedCommandSender**](classendstone_1_1CommandSender.md#function-asproxiedcommandsender) () const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**ProxiedCommandSender**_](classendstone_1_1ProxiedCommandSender.md) _._ |
| virtual std::string | [**getName**](classendstone_1_1CommandSender.md#function-getname) () const = 0<br>_Gets the name of this command sender._  |
| virtual [**Server**](classendstone_1_1Server.md) & | [**getServer**](classendstone_1_1CommandSender.md#function-getserver) () const = 0<br>_Returns the server instance that this command is running on._  |
| virtual [**void**](classendstone_1_1Vector.md) | [**sendErrorMessage**](classendstone_1_1CommandSender.md#function-senderrormessage-12) ([**const**](classendstone_1_1Vector.md) Message & message) const = 0<br>_Sends this sender a error message._  |
|  [**void**](classendstone_1_1Vector.md) | [**sendErrorMessage**](classendstone_1_1CommandSender.md#function-senderrormessage-22) ([**const**](classendstone_1_1Vector.md) fmt::format\_string&lt; Args... &gt; format, [**Args**](classendstone_1_1Vector.md) &&... args) const<br> |
| virtual [**void**](classendstone_1_1Vector.md) | [**sendMessage**](classendstone_1_1CommandSender.md#function-sendmessage-12) ([**const**](classendstone_1_1Vector.md) Message & message) const = 0<br>_Sends this sender a message._  |
|  [**void**](classendstone_1_1Vector.md) | [**sendMessage**](classendstone_1_1CommandSender.md#function-sendmessage-22) ([**const**](classendstone_1_1Vector.md) fmt::format\_string&lt; Args... &gt; format, [**Args**](classendstone_1_1Vector.md) &&... args) const<br> |


## Public Functions inherited from endstone::Permissible

See [endstone::Permissible](classendstone_1_1Permissible.md)

| Type | Name |
| ---: | :--- |
| virtual [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) \* | [**addAttachment**](classendstone_1_1Permissible.md#function-addattachment-12) ([**Plugin**](classendstone_1_1Plugin.md) & plugin, [**const**](classendstone_1_1Vector.md) std::string & name, [**bool**](classendstone_1_1Vector.md) value) = 0<br> |
| virtual [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) \* | [**addAttachment**](classendstone_1_1Permissible.md#function-addattachment-22) ([**Plugin**](classendstone_1_1Plugin.md) & plugin) = 0<br> |
| virtual [**CommandSender**](classendstone_1_1CommandSender.md) \* | [**asCommandSender**](classendstone_1_1Permissible.md#function-ascommandsender) () const = 0<br>_Casts a_ [_**Permissible**_](classendstone_1_1Permissible.md) _as_[_**CommandSender**_](classendstone_1_1CommandSender.md) _._ |
| virtual std::unordered\_set&lt; [**PermissionAttachmentInfo**](classendstone_1_1PermissionAttachmentInfo.md) \* &gt; | [**getEffectivePermissions**](classendstone_1_1Permissible.md#function-geteffectivepermissions) () const = 0<br> |
| virtual [**bool**](classendstone_1_1Vector.md) | [**hasPermission**](classendstone_1_1Permissible.md#function-haspermission-12) (std::string name) const = 0<br> |
| virtual [**bool**](classendstone_1_1Vector.md) | [**hasPermission**](classendstone_1_1Permissible.md#function-haspermission-22) ([**const**](classendstone_1_1Vector.md) [**Permission**](classendstone_1_1Permission.md) & perm) const = 0<br> |
| virtual [**bool**](classendstone_1_1Vector.md) | [**isOp**](classendstone_1_1Permissible.md#function-isop) () const = 0<br>_Checks if this object is a server operator._  |
| virtual [**bool**](classendstone_1_1Vector.md) | [**isPermissionSet**](classendstone_1_1Permissible.md#function-ispermissionset-12) (std::string name) const = 0<br> |
| virtual [**bool**](classendstone_1_1Vector.md) | [**isPermissionSet**](classendstone_1_1Permissible.md#function-ispermissionset-22) ([**const**](classendstone_1_1Vector.md) [**Permission**](classendstone_1_1Permission.md) & perm) const = 0<br> |
| virtual [**void**](classendstone_1_1Vector.md) | [**recalculatePermissions**](classendstone_1_1Permissible.md#function-recalculatepermissions) () = 0<br> |
| virtual Result&lt; [**void**](classendstone_1_1Vector.md) &gt; | [**removeAttachment**](classendstone_1_1Permissible.md#function-removeattachment) ([**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) & attachment) = 0<br> |
| virtual [**void**](classendstone_1_1Vector.md) | [**setOp**](classendstone_1_1Permissible.md#function-setop) ([**bool**](classendstone_1_1Vector.md) value) = 0<br>_Sets the operator status of this object._  |
| virtual  | [**~Permissible**](classendstone_1_1Permissible.md#function-permissible) () = default<br> |


## Public Functions inherited from endstone::OfflinePlayer

See [endstone::OfflinePlayer](classendstone_1_1OfflinePlayer.md)

| Type | Name |
| ---: | :--- |
| virtual std::string | [**getName**](classendstone_1_1OfflinePlayer.md#function-getname) () const = 0<br>_Returns the name of this player._  |
| virtual [**UUID**](classendstone_1_1UUID.md) | [**getUniqueId**](classendstone_1_1OfflinePlayer.md#function-getuniqueid) () const = 0<br>_Returns the_ [_**UUID**_](classendstone_1_1UUID.md) _of this player._ |
| virtual  | [**~OfflinePlayer**](classendstone_1_1OfflinePlayer.md#function-offlineplayer) () = default<br> |














## Protected Types

| Type | Name |
| ---: | :--- |
| typedef std::variant&lt; [**MessageForm**](classendstone_1_1MessageForm.md), [**ActionForm**](classendstone_1_1ActionForm.md), [**ModalForm**](classendstone_1_1ModalForm.md) &gt; | [**FormVariant**](#typedef-formvariant)  <br> |
















































































































































## Public Functions Documentation




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

_Closes the forms that are currently open for the player._ 
```C++
virtual void endstone::Player::closeForm () = 0
```




<hr>



### function getAddress 

_Gets the socket address of this player._ 
```C++
virtual SocketAddress endstone::Player::getAddress () const = 0
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



### function getName 

_Returns the name of this player._ 
```C++
virtual std::string endstone::Player::getName () override const = 0
```





**Returns:**

[**Player**](classendstone_1_1Player.md) name or null if we have not seen a name for this player yet 





        
Implements [*endstone::CommandSender::getName*](classendstone_1_1CommandSender.md#function-getname)


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
virtual const  Skin & endstone::Player::getSkin () const = 0
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



### function isSneaking 

_Returns if the player is in sneak mode._ 
```C++
virtual bool endstone::Player::isSneaking () const = 0
```





**Returns:**

true if player is in sneak mode 





        

<hr>



### function isSprinting 

_Gets whether the player is sprinting or not._ 
```C++
virtual bool endstone::Player::isSprinting () const = 0
```





**Returns:**

true if player is sprinting. 





        

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



### function playSound 

_Play a sound for a player at the location._ 
```C++
virtual void endstone::Player::playSound (
    Location location,
    std::string sound,
    float volume,
    float pitch
) = 0
```





**Parameters:**


* `location` The location to play the sound 
* `sound` The internal sound name to play 
* `volume` The volume of the sound 
* `pitch` The pitch of the sound 




        

<hr>



### function resetTitle 

_Resets the title displayed to the player. This will clear the displayed title / subtitle and reset timings to their default values._ 
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
    int packet_id,
    std::string_view payload
) const = 0
```





**Parameters:**


* `packet_id` The packet ID to be sent. 
* `payload` The payload of the packet to be transmitted. 




        

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
virtual Result< void > endstone::Player::setExpLevel (
    int level
) = 0
```





**Parameters:**


* `level` New experience level 




        

<hr>



### function setExpProgress 

_Sets the players current experience progress towards the next level._ 
```C++
virtual Result< void > endstone::Player::setExpProgress (
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
virtual Result< void > endstone::Player::setFlying (
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



### function setSneaking 

_Sets the sneak mode of the player._ 
```C++
virtual void endstone::Player::setSneaking (
    bool sneak
) = 0
```





**Parameters:**


* `sneak` true if player should appear sneaking 




        

<hr>



### function setSprinting 

_Sets whether the player is sprinting or not._ 
```C++
virtual void endstone::Player::setSprinting (
    bool sprinting
) = 0
```





**Parameters:**


* `sprinting` true if the player should be sprinting 




        

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



### function spawnParticle [1/4]

_Spawns the particle at the target location._ 
```C++
virtual void endstone::Player::spawnParticle (
    std::string name,
    Location location
) const = 0
```





**Parameters:**


* `name` the name of the particle effect to spawn 
* `location` the location to spawn at 




        

<hr>



### function spawnParticle [2/4]

_Spawns the particle at the target location._ 
```C++
virtual void endstone::Player::spawnParticle (
    std::string name,
    float x,
    float y,
    float z
) const = 0
```





**Parameters:**


* `name` the name of the particle effect to spawn 
* `x` the position on the x axis to spawn at 
* `y` the position on the y axis to spawn at 
* `z` the position on the z axis to spawn at 




        

<hr>



### function spawnParticle [3/4]

_Spawns the particle at the target location._ 
```C++
virtual void endstone::Player::spawnParticle (
    std::string name,
    Location location,
    std::optional< std::string > molang_variables_json
) const = 0
```





**Parameters:**


* `name` the name of the particle effect to spawn 
* `location` the location to spawn at 
* `molang_variables_json` the customizable molang variables that can be adjusted for this particle, in json 




        

<hr>



### function spawnParticle [4/4]

_Spawns the particle at the target location._ 
```C++
virtual void endstone::Player::spawnParticle (
    std::string name,
    float x,
    float y,
    float z,
    std::optional< std::string > molang_variables_json
) const = 0
```





**Parameters:**


* `name` the name of the particle effect to spawn 
* `x` the position on the x axis to spawn at 
* `y` the position on the y axis to spawn at 
* `z` the position on the z axis to spawn at 
* `molang_variables_json` the customizable molang variables that can be adjusted for this particle, in json 




        

<hr>



### function stopAllSounds 

_Stop all sounds from playing._ 
```C++
virtual void endstone::Player::stopAllSounds () = 0
```




<hr>



### function stopSound 

_Stop the specified sound from playing._ 
```C++
virtual void endstone::Player::stopSound (
    std::string sound
) = 0
```





**Parameters:**


* `sound` the sound to stop 




        

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
## Protected Types Documentation




### typedef FormVariant 

```C++
using endstone::Player::FormVariant =  std::variant<MessageForm, ActionForm, ModalForm>;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/player.h`

