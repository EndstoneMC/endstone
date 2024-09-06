
# Class Hierarchy

This inheritance list is sorted roughly, but not completely, alphabetically:


* **class** [**endstone::Form**](classendstone_1_1Form.md) _Represents a generic form._ 
* **class** [**endstone::ActionForm::Button**](classendstone_1_1ActionForm_1_1Button.md) _Represents a button with text and an optional icon._ 
* **class** [**endstone::Permissible**](classendstone_1_1Permissible.md) _Represents an object that may become a server operator and can be assigned permissions._     
    * **class** [**endstone::CommandSender**](classendstone_1_1CommandSender.md) _Represents a command sender._     
        * **class** [**endstone::Actor**](classendstone_1_1Actor.md) _Represents a base actor in the level._     
            * **class** [**endstone::Mob**](classendstone_1_1Mob.md) _Represents a mobile entity (i.e. living entity), such as a monster or player._     
                * **class** [**endstone::Player**](classendstone_1_1Player.md) _Represents a player._ 
        * **class** [**endstone::ConsoleCommandSender**](classendstone_1_1ConsoleCommandSender.md) _Represents a console command sender._ 
* **class** [**endstone::Event**](classendstone_1_1Event.md) _Represents an event._     
    * **class** [**endstone::ActorEvent**](classendstone_1_1ActorEvent.md) _Represents an Actor-related event._     
        * **class** [**endstone::ActorDeathEvent**](classendstone_1_1ActorDeathEvent.md) _Called when an_ [_**Actor**_](classendstone_1_1Actor.md) _dies._    
            * **class** [**endstone::PlayerDeathEvent**](classendstone_1_1PlayerDeathEvent.md) _Called when a_ [_**Player**_](classendstone_1_1Player.md) _dies._
        * **class** [**endstone::ActorKnockbackEvent**](classendstone_1_1ActorKnockbackEvent.md) _Called when a living entity receives knockback._ 
        * **class** [**endstone::ActorRemoveEvent**](classendstone_1_1ActorRemoveEvent.md) _Called when an_ [_**Actor**_](classendstone_1_1Actor.md) _is removed._
        * **class** [**endstone::ActorSpawnEvent**](classendstone_1_1ActorSpawnEvent.md) _Called when an_ [_**Actor**_](classendstone_1_1Actor.md) _is spawned into a world._
        * **class** [**endstone::ActorTeleportEvent**](classendstone_1_1ActorTeleportEvent.md) _Called when a non-player entity is teleported from one location to another._ 
    * **class** [**endstone::BlockEvent**](classendstone_1_1BlockEvent.md) _Represents an Block-related event._     
        * **class** [**endstone::BlockBreakEvent**](classendstone_1_1BlockBreakEvent.md) _Called when a block is broken by a player._ 
        * **class** [**endstone::BlockPlaceEvent**](classendstone_1_1BlockPlaceEvent.md) _Called when a block is placed by a player._ 
    * **class** [**endstone::PlayerEvent**](classendstone_1_1PlayerEvent.md) _Represents a player related event._     
        * **class** [**endstone::PlayerChatEvent**](classendstone_1_1PlayerChatEvent.md) _Called when a player sends a chat message._ 
        * **class** [**endstone::PlayerCommandEvent**](classendstone_1_1PlayerCommandEvent.md) _Called whenever a player runs a command._ 
        * **class** [**endstone::PlayerDeathEvent**](classendstone_1_1PlayerDeathEvent.md) _Called when a_ [_**Player**_](classendstone_1_1Player.md) _dies._
        * **class** [**endstone::PlayerInteractActorEvent**](classendstone_1_1PlayerInteractActorEvent.md) _Represents an event that is called when a player right-clicks an actor._ 
        * **class** [**endstone::PlayerInteractEvent**](classendstone_1_1PlayerInteractEvent.md) _Represents an event that is called when a player right-clicks a block._ 
        * **class** [**endstone::PlayerJoinEvent**](classendstone_1_1PlayerJoinEvent.md) _Called when a player joins a server._ 
        * **class** [**endstone::PlayerKickEvent**](classendstone_1_1PlayerKickEvent.md) _Called when a player gets kicked from the server._ 
        * **class** [**endstone::PlayerLoginEvent**](classendstone_1_1PlayerLoginEvent.md) _Called when a player attempts to login in._ 
        * **class** [**endstone::PlayerQuitEvent**](classendstone_1_1PlayerQuitEvent.md) _Called when a player leaves a server._ 
        * **class** [**endstone::PlayerTeleportEvent**](classendstone_1_1PlayerTeleportEvent.md) _Called when a player is teleported from one location to another._ 
    * **class** [**endstone::ServerEvent**](classendstone_1_1ServerEvent.md) _Represents an Server-related event._     
        * **class** [**endstone::BroadcastMessageEvent**](classendstone_1_1BroadcastMessageEvent.md) [_**Event**_](classendstone_1_1Event.md) _triggered for server broadcast messages such as from_[_**Server::broadcast**_](classendstone_1_1Server.md#function-broadcast) _._
        * **class** [**endstone::PluginDisableEvent**](classendstone_1_1PluginDisableEvent.md) _Called when a plugin is disabled._ 
        * **class** [**endstone::PluginEnableEvent**](classendstone_1_1PluginEnableEvent.md) _Called when a plugin is enabled._ 
        * **class** [**endstone::ServerCommandEvent**](classendstone_1_1ServerCommandEvent.md) _Called when the console runs a command, early in the process._ 
        * **class** [**endstone::ServerListPingEvent**](classendstone_1_1ServerListPingEvent.md) _Called when a server ping is coming in._ 
        * **class** [**endstone::ServerLoadEvent**](classendstone_1_1ServerLoadEvent.md) _Called when either the server startup or reload has completed._ 
    * **class** [**endstone::WeatherEvent**](classendstone_1_1WeatherEvent.md) _Represents a Weather-related event._     
        * **class** [**endstone::ThunderChangeEvent**](classendstone_1_1ThunderChangeEvent.md) _Called when the thunder state in a world is changing._ 
        * **class** [**endstone::WeatherChangeEvent**](classendstone_1_1WeatherChangeEvent.md) _Called when the weather (rain) state in a world is changing._ 
* **class** [**endstone::Block**](classendstone_1_1Block.md) _Represents a block._ 
* **class** [**endstone::BlockState**](classendstone_1_1BlockState.md) _Represents a captured state of a block, which will not update automatically._ 
* **class** [**endstone::BossBar**](classendstone_1_1BossBar.md) _Represents a boss bar that is displayed to players._ 
* **class** [**endstone::Command**](classendstone_1_1Command.md) _Represents a_ [_**Command**_](classendstone_1_1Command.md) _, which executes various tasks upon user input._    
    * **class** [**endstone::PluginCommand**](classendstone_1_1PluginCommand.md) _Represents a_ [_**Command**_](classendstone_1_1Command.md) _belonging to a_[_**Plugin**_](classendstone_1_1Plugin.md) _._
* **class** [**endstone::CommandExecutor**](classendstone_1_1CommandExecutor.md) _Represents a class which contains a single method for executing commands._     
    * **class** [**endstone::Plugin**](classendstone_1_1Plugin.md) _Represents a_ [_**Plugin**_](classendstone_1_1Plugin.md) _._
* **class** [**endstone::CommandMap**](classendstone_1_1CommandMap.md) _Represents a command map that manages all commands of the_ [_**Server**_](classendstone_1_1Server.md) _._
* **class** [**endstone::Criteria**](classendstone_1_1Criteria.md) _Represents a scoreboard criteria._ 
* **class** [**endstone::Dimension**](classendstone_1_1Dimension.md) _Represents a dimension within a_ [_**Level**_](classendstone_1_1Level.md) _._
* **class** [**endstone::Dropdown**](classendstone_1_1Dropdown.md) _Represents a dropdown with a set of predefined options._     
    * **class** [**endstone::StepSlider**](classendstone_1_1StepSlider.md) _Represents a step slider with a set of predefined options._ 
* **class** [**endstone::EventHandler**](classendstone_1_1EventHandler.md) _Represents a registered_ [_**EventHandler**_](classendstone_1_1EventHandler.md) _which associates with a_[_**Plugin**_](classendstone_1_1Plugin.md) _._
* **class** [**endstone::HandlerList**](classendstone_1_1HandlerList.md) _A list of event handlers. Should be instantiated on a per-event basis._ 
* **class** [**endstone::Inventory**](classendstone_1_1Inventory.md) _Interface to the various inventories._     
    * **class** [**endstone::PlayerInventory**](classendstone_1_1PlayerInventory.md) _Interface to the inventory of a_ [_**Player**_](classendstone_1_1Player.md) _, including the four armor slots and any extra slots._
* **class** [**endstone::ItemStack**](classendstone_1_1ItemStack.md) _Represents a stack of items._ 
* **class** [**endstone::Label**](classendstone_1_1Label.md) _Represents a text label._ 
* **class** [**endstone::Level**](classendstone_1_1Level.md) _Represents a level, which may contain actors, chunks and blocks._ 
* **class** [**endstone::Vector**](classendstone_1_1Vector.md) _Represents a 3-dimensional vector._ 
* **class** [**endstone::Logger**](classendstone_1_1Logger.md) [_**Logger**_](classendstone_1_1Logger.md) _class which can format and output varies levels of logs._
* **class** [**endstone::Objective**](classendstone_1_1Objective.md) _Represents an objective on a scoreboard that can show scores specific to entries._ 
* **class** [**endstone::Packet**](classendstone_1_1Packet.md) _Represents a packet._     
    * **class** [**endstone::SpawnParticleEffectPacket**](classendstone_1_1SpawnParticleEffectPacket.md) _Represents a packet for spawning a particle effect._ 
* **class** [**endstone::Permission**](classendstone_1_1Permission.md) _Represents a unique permission that may be attached to a_ [_**Permissible**_](classendstone_1_1Permissible.md) _._
* **class** [**endstone::PermissionAttachment**](classendstone_1_1PermissionAttachment.md) _Holds information about a permission attachment on a_ [_**Permissible**_](classendstone_1_1Permissible.md) _object._
* **class** [**endstone::PermissionAttachmentInfo**](classendstone_1_1PermissionAttachmentInfo.md) _Holds information on a permission and which_ [_**PermissionAttachment**_](classendstone_1_1PermissionAttachment.md) _provides it._
* **class** [**endstone::PluginDescription**](classendstone_1_1PluginDescription.md) _Represents the basic information about a plugin that the plugin loader needs to know._ 
* **class** [**endstone::PluginLoader**](classendstone_1_1PluginLoader.md) _Represents a plugin loader, which handles direct access to specific types of plugins._ 
* **class** [**endstone::PluginManager**](classendstone_1_1PluginManager.md) _Represents a plugin manager that handles all plugins from the_ [_**Server**_](classendstone_1_1Server.md) _._
* **class** [**endstone::Scheduler**](classendstone_1_1Scheduler.md) _Represents a scheduler that executes various tasks._ 
* **class** [**endstone::Score**](classendstone_1_1Score.md) _Represents a score for an objective on a scoreboard._ 
* **class** [**endstone::Server**](classendstone_1_1Server.md) _Represents a server implementation._ 
* **class** [**endstone::Skin**](classendstone_1_1Skin.md) _Represents a player skin._ 
* **class** [**endstone::Slider**](classendstone_1_1Slider.md) _Represents a slider with a label._ 
* **class** [**endstone::SocketAddress**](classendstone_1_1SocketAddress.md) _Represents an IP Socket Address (hostname + port number)._ 
* **class** [**endstone::Task**](classendstone_1_1Task.md) _Represents a task being executed by the scheduler._ 
* **class** [**endstone::TextInput**](classendstone_1_1TextInput.md) _Represents a text input field._ 
* **class** [**endstone::Toggle**](classendstone_1_1Toggle.md) _Represents a toggle button with a label._ 
* **class** [**endstone::Translatable**](classendstone_1_1Translatable.md) _Represents an object with a text representation that can be translated by the Minecraft client._ 
* **class** [**endstone::UUID**](classendstone_1_1UUID.md) _Implementation of Universally Unique Identifier (_ [_**UUID**_](classendstone_1_1UUID.md) _)_
* **struct** [**endstone::ColorFormat**](structendstone_1_1ColorFormat.md) _All supported color and format codes._ 
* **struct** [**endstone::Skin::ImageData**](structendstone_1_1Skin_1_1ImageData.md) 
* **struct** [**std::hash&lt; endstone::UUID &gt;**](structstd_1_1hash_3_01endstone_1_1UUID_01_4.md) 
* **class** **std::enable_shared_from_this< BlockData >**    
    * **class** [**endstone::BlockData**](classendstone_1_1BlockData.md) _Represents the data related to a live block._ 
* **class** **std::enable_shared_from_this< Scoreboard >**    
    * **class** [**endstone::Scoreboard**](classendstone_1_1Scoreboard.md) _Represents a scoreboard._ 
* **class** **endstone::Func**    
    * **struct** [**endstone::overloaded**](structendstone_1_1overloaded.md) _Helper type for visitors._ 
* **class** **formatter< string_view >**    
    * **struct** [**fmt::formatter&lt; endstone::Block &gt;**](structfmt_1_1formatter_3_01endstone_1_1Block_01_4.md) 
    * **struct** [**fmt::formatter&lt; endstone::BlockData &gt;**](structfmt_1_1formatter_3_01endstone_1_1BlockData_01_4.md) 
    * **struct** [**fmt::formatter&lt; endstone::BlockState &gt;**](structfmt_1_1formatter_3_01endstone_1_1BlockState_01_4.md) 
    * **struct** [**fmt::formatter&lt; endstone::BlockStates &gt;**](structfmt_1_1formatter_3_01endstone_1_1BlockStates_01_4.md) 
    * **struct** [**fmt::formatter&lt; endstone::BlockStates::mapped\_type &gt;**](structfmt_1_1formatter_3_01endstone_1_1BlockStates_1_1mapped__type_01_4.md) 
    * **struct** [**fmt::formatter&lt; endstone::BlockStates::value\_type &gt;**](structfmt_1_1formatter_3_01endstone_1_1BlockStates_1_1value__type_01_4.md) 

