

# Class endstone::ConsoleCommandSender



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**ConsoleCommandSender**](classendstone_1_1ConsoleCommandSender.md)



_Represents a console command sender._ 

* `#include <endstone/command/console_command_sender.h>`



Inherits the following classes: [endstone::CommandSender](classendstone_1_1CommandSender.md)












































































## Public Functions inherited from endstone::CommandSender

See [endstone::CommandSender](classendstone_1_1CommandSender.md)

| Type | Name |
| ---: | :--- |
| virtual [**Actor**](classendstone_1_1Actor.md) \* | [**asActor**](classendstone_1_1CommandSender.md#function-asactor) () const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Actor**_](classendstone_1_1Actor.md) _._ |
| virtual [**CommandSender**](classendstone_1_1CommandSender.md) \* | [**asCommandSender**](classendstone_1_1CommandSender.md#function-ascommandsender) () override const<br>_Casts a_ [_**Permissible**_](classendstone_1_1Permissible.md) _as_[_**CommandSender**_](classendstone_1_1CommandSender.md) _._ |
| virtual [**ConsoleCommandSender**](classendstone_1_1ConsoleCommandSender.md) \* | [**asConsole**](classendstone_1_1CommandSender.md#function-asconsole) () const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as Console._ |
| virtual [**Mob**](classendstone_1_1Mob.md) \* | [**asMob**](classendstone_1_1CommandSender.md#function-asmob) () const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Mob**_](classendstone_1_1Mob.md) _._ |
| virtual [**Player**](classendstone_1_1Player.md) \* | [**asPlayer**](classendstone_1_1CommandSender.md#function-asplayer) () const<br>_Gets a_ [_**CommandSender**_](classendstone_1_1CommandSender.md) _as_[_**Player**_](classendstone_1_1Player.md) _._ |
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
| virtual Result&lt; [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) \* &gt; | [**addAttachment**](classendstone_1_1Permissible.md#function-addattachment-12) ([**Plugin**](classendstone_1_1Plugin.md) & plugin, [**const**](classendstone_1_1Vector.md) std::string & name, [**bool**](classendstone_1_1Vector.md) value) = 0<br> |
| virtual Result&lt; [**PermissionAttachment**](classendstone_1_1PermissionAttachment.md) \* &gt; | [**addAttachment**](classendstone_1_1Permissible.md#function-addattachment-22) ([**Plugin**](classendstone_1_1Plugin.md) & plugin) = 0<br> |
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

















































































------------------------------
The documentation for this class was generated from the following file `include/endstone/command/console_command_sender.h`

