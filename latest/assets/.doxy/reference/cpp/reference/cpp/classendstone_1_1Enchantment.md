

# Class endstone::Enchantment



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**Enchantment**](classendstone_1_1Enchantment.md)



_Represents an enchantment that may be added to armor, weapons, tools or other items._ 

* `#include <endstone/enchantments/enchantment.h>`



Inherits the following classes: [endstone::Registry::Type](classendstone_1_1Registry_1_1Type.md)
















## Public Types inherited from endstone::Registry::Type

See [endstone::Registry::Type](classendstone_1_1Registry_1_1Type.md)

| Type | Name |
| ---: | :--- |
| typedef [**Identifier**](classendstone_1_1Identifier.md)&lt; [**T**](classendstone_1_1Identifier.md) &gt; | [**Id**](classendstone_1_1Registry_1_1Type.md#typedef-id)  <br>_The identifier type used to look up this object in its registry._  |










## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**AquaAffinity**](#variable-aquaaffinity)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("aqua\_affinity")`<br>_Increases the speed at which a player may mine underwater._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**BaneOfArthropods**](#variable-baneofarthropods)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("bane\_of\_arthropods")`<br>_Increases damage against arthropod targets._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**BlastProtection**](#variable-blastprotection)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("blast\_protection")`<br>_Provides protection against explosive damage._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Breach**](#variable-breach)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("breach")`<br>_Reduces armor effectiveness against maces._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Channeling**](#variable-channeling)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("channeling")`<br>_Strikes lightning when a mob is hit with a trident if conditions are stormy._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**CurseOfBinding**](#variable-curseofbinding)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("binding")`<br>[_**Item**_](classendstone_1_1Item.md) _cannot be removed._ |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**CurseOfVanishing**](#variable-curseofvanishing)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("vanishing")`<br>[_**Item**_](classendstone_1_1Item.md) _disappears instead of dropping._ |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Density**](#variable-density)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("density")`<br>_Increases fall damage of maces._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**DepthStrider**](#variable-depthstrider)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("depth\_strider")`<br>_Increases walking speed while in water._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Efficiency**](#variable-efficiency)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("efficiency")`<br>_Increases the rate at which you mine/dig._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**FeatherFalling**](#variable-featherfalling)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("feather\_falling")`<br>_Provides protection against fall damage._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**FireAspect**](#variable-fireaspect)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("fire\_aspect")`<br>_When attacking a target, has a chance to set them on fire._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**FireProtection**](#variable-fireprotection)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("fire\_protection")`<br>_Provides protection against fire damage._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Flame**](#variable-flame)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("flame")`<br>_Sets entities on fire when hit by arrows shot from a bow._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Fortune**](#variable-fortune)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("fortune")`<br>_Provides a chance of gaining extra loot when destroying blocks._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**FrostWalker**](#variable-frostwalker)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("frost\_walker")`<br>_Freezes any still water adjacent to ice / frost which player is walking on._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Impaling**](#variable-impaling)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("impaling")`<br>_Deals more damage to mobs that live in the ocean._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Infinity**](#variable-infinity)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("infinity")`<br>_Provides infinite arrows when shooting a bow._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Knockback**](#variable-knockback)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("knockback")`<br>_All damage to other targets will knock them back when hit._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Looting**](#variable-looting)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("looting")`<br>_Provides a chance of gaining extra loot when killing monsters._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Loyalty**](#variable-loyalty)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("loyalty")`<br>_Causes a thrown trident to return to the player who threw it._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**LuckOfTheSea**](#variable-luckofthesea)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("luck\_of\_the\_sea")`<br>_Decreases odds of catching worthless junk._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Lunge**](#variable-lunge)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("lunge")`<br>_Applied to spears, causes jab attacks to propel the wielder forward horizontally._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Lure**](#variable-lure)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("lure")`<br>_Increases rate of fish biting your hook._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Mending**](#variable-mending)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("mending")`<br>_Allows mending the item using experience orbs._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Multishot**](#variable-multishot)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("multishot")`<br>_Shoot multiple arrows from crossbows._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Piercing**](#variable-piercing)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("piercing")`<br>_Crossbow projectiles pierce entities._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Power**](#variable-power)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("power")`<br>_Provides extra damage when shooting arrows from bows._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**ProjectileProtection**](#variable-projectileprotection)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("projectile\_protection")`<br>_Provides protection against projectile damage._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Protection**](#variable-protection)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("protection")`<br>_Provides protection against environmental damage._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Punch**](#variable-punch)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("punch")`<br>_Provides a knockback when an entity is hit by an arrow from a bow._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**QuickCharge**](#variable-quickcharge)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("quick\_charge")`<br>_Charges crossbows quickly._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Respiration**](#variable-respiration)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("respiration")`<br>_Decreases the rate of air loss whilst underwater._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Riptide**](#variable-riptide)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("riptide")`<br>_When it is rainy, launches the player in the direction their trident is thrown._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Sharpness**](#variable-sharpness)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("sharpness")`<br>_Increases damage against all targets._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**SilkTouch**](#variable-silktouch)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("silk\_touch")`<br>_Allows blocks to drop themselves instead of fragments (for example, stone instead of cobblestone)._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Smite**](#variable-smite)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("smite")`<br>_Increases damage against undead targets._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**SoulSpeed**](#variable-soulspeed)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("soul\_speed")`<br>_Walk quicker on soul blocks._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**SwiftSneak**](#variable-swiftsneak)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("swift\_sneak")`<br>_Walk quicker while sneaking._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Thorns**](#variable-thorns)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("thorns")`<br>_Damages the attacker._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**Unbreaking**](#variable-unbreaking)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("unbreaking")`<br>_Decreases the rate at which a tool looses durability._  |
|  [**constexpr**](classendstone_1_1Identifier.md) [**auto**](classendstone_1_1Identifier.md) | [**WindBurst**](#variable-windburst)   = `[**EnchantmentId::minecraft**](classendstone_1_1Identifier.md#function-minecraft)("wind\_burst")`<br>_Emits wind burst upon hitting enemy._  |




























## Public Functions

| Type | Name |
| ---: | :--- |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**canEnchantItem**](#function-canenchantitem) ([**const**](classendstone_1_1Identifier.md) [**ItemStack**](classendstone_1_1ItemStack.md) & item) const = 0<br>_Checks if this_ [_**Enchantment**_](classendstone_1_1Enchantment.md) _may be applied to the given_[_**ItemStack**_](classendstone_1_1ItemStack.md) _._ |
| virtual [**bool**](classendstone_1_1Identifier.md) | [**conflictsWith**](#function-conflictswith) ([**const**](classendstone_1_1Identifier.md) [**Enchantment**](classendstone_1_1Enchantment.md) & other) const = 0<br>_Check if this enchantment conflicts with another enchantment._  |
| virtual [**int**](classendstone_1_1Identifier.md) | [**getMaxLevel**](#function-getmaxlevel) () const = 0<br>_Gets the maximum level that this_ [_**Enchantment**_](classendstone_1_1Enchantment.md) _may become._ |
| virtual [**int**](classendstone_1_1Identifier.md) | [**getStartLevel**](#function-getstartlevel) () const = 0<br>_Gets the level that this_ [_**Enchantment**_](classendstone_1_1Enchantment.md) _should start at (also known as minimum level)._ |


## Public Functions inherited from endstone::Registry::Type

See [endstone::Registry::Type](classendstone_1_1Registry_1_1Type.md)

| Type | Name |
| ---: | :--- |
| virtual [**Id**](classendstone_1_1Registry_1_1Type.md#typedef-id) | [**getId**](classendstone_1_1Registry_1_1Type.md#function-getid) () const = 0<br>_Return the identifier of this object._  |
| virtual std::string | [**getTranslationKey**](classendstone_1_1Registry_1_1Type.md#function-gettranslationkey) () const = 0<br>_Get the translation key, suitable for use in a translation component._  |
|   | [**operator Id**](classendstone_1_1Registry_1_1Type.md#function-operator-id) () const<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**operator!=**](classendstone_1_1Registry_1_1Type.md#function-operator) ([**const**](classendstone_1_1Identifier.md) [**Id**](classendstone_1_1Registry_1_1Type.md#typedef-id) & other) const<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**operator!=**](classendstone_1_1Registry_1_1Type.md#function-operator_1) ([**const**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md) & other) const<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**operator==**](classendstone_1_1Registry_1_1Type.md#function-operator_2) ([**const**](classendstone_1_1Identifier.md) [**Id**](classendstone_1_1Registry_1_1Type.md#typedef-id) & other) const<br> |
|  [**bool**](classendstone_1_1Identifier.md) | [**operator==**](classendstone_1_1Registry_1_1Type.md#function-operator_3) ([**const**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md) & other) const<br> |
| virtual  | [**~Type**](classendstone_1_1Registry_1_1Type.md#function-type) () = default<br> |




## Public Static Functions inherited from endstone::Registry::Type

See [endstone::Registry::Type](classendstone_1_1Registry_1_1Type.md)

| Type | Name |
| ---: | :--- |
|  [**const**](classendstone_1_1Identifier.md) [**T**](classendstone_1_1Identifier.md) \* | [**get**](classendstone_1_1Registry_1_1Type.md#function-get) ([**Id**](classendstone_1_1Registry_1_1Type.md#typedef-id) id) <br>_Look up a registry entry by identifier._  |


















































## Public Static Attributes Documentation




### variable AquaAffinity 

_Increases the speed at which a player may mine underwater._ 
```C++
constexpr auto endstone::Enchantment::AquaAffinity;
```




<hr>



### variable BaneOfArthropods 

_Increases damage against arthropod targets._ 
```C++
constexpr auto endstone::Enchantment::BaneOfArthropods;
```




<hr>



### variable BlastProtection 

_Provides protection against explosive damage._ 
```C++
constexpr auto endstone::Enchantment::BlastProtection;
```




<hr>



### variable Breach 

_Reduces armor effectiveness against maces._ 
```C++
constexpr auto endstone::Enchantment::Breach;
```




<hr>



### variable Channeling 

_Strikes lightning when a mob is hit with a trident if conditions are stormy._ 
```C++
constexpr auto endstone::Enchantment::Channeling;
```




<hr>



### variable CurseOfBinding 

[_**Item**_](classendstone_1_1Item.md) _cannot be removed._
```C++
constexpr auto endstone::Enchantment::CurseOfBinding;
```




<hr>



### variable CurseOfVanishing 

[_**Item**_](classendstone_1_1Item.md) _disappears instead of dropping._
```C++
constexpr auto endstone::Enchantment::CurseOfVanishing;
```




<hr>



### variable Density 

_Increases fall damage of maces._ 
```C++
constexpr auto endstone::Enchantment::Density;
```




<hr>



### variable DepthStrider 

_Increases walking speed while in water._ 
```C++
constexpr auto endstone::Enchantment::DepthStrider;
```




<hr>



### variable Efficiency 

_Increases the rate at which you mine/dig._ 
```C++
constexpr auto endstone::Enchantment::Efficiency;
```




<hr>



### variable FeatherFalling 

_Provides protection against fall damage._ 
```C++
constexpr auto endstone::Enchantment::FeatherFalling;
```




<hr>



### variable FireAspect 

_When attacking a target, has a chance to set them on fire._ 
```C++
constexpr auto endstone::Enchantment::FireAspect;
```




<hr>



### variable FireProtection 

_Provides protection against fire damage._ 
```C++
constexpr auto endstone::Enchantment::FireProtection;
```




<hr>



### variable Flame 

_Sets entities on fire when hit by arrows shot from a bow._ 
```C++
constexpr auto endstone::Enchantment::Flame;
```




<hr>



### variable Fortune 

_Provides a chance of gaining extra loot when destroying blocks._ 
```C++
constexpr auto endstone::Enchantment::Fortune;
```




<hr>



### variable FrostWalker 

_Freezes any still water adjacent to ice / frost which player is walking on._ 
```C++
constexpr auto endstone::Enchantment::FrostWalker;
```




<hr>



### variable Impaling 

_Deals more damage to mobs that live in the ocean._ 
```C++
constexpr auto endstone::Enchantment::Impaling;
```




<hr>



### variable Infinity 

_Provides infinite arrows when shooting a bow._ 
```C++
constexpr auto endstone::Enchantment::Infinity;
```




<hr>



### variable Knockback 

_All damage to other targets will knock them back when hit._ 
```C++
constexpr auto endstone::Enchantment::Knockback;
```




<hr>



### variable Looting 

_Provides a chance of gaining extra loot when killing monsters._ 
```C++
constexpr auto endstone::Enchantment::Looting;
```




<hr>



### variable Loyalty 

_Causes a thrown trident to return to the player who threw it._ 
```C++
constexpr auto endstone::Enchantment::Loyalty;
```




<hr>



### variable LuckOfTheSea 

_Decreases odds of catching worthless junk._ 
```C++
constexpr auto endstone::Enchantment::LuckOfTheSea;
```




<hr>



### variable Lunge 

_Applied to spears, causes jab attacks to propel the wielder forward horizontally._ 
```C++
constexpr auto endstone::Enchantment::Lunge;
```




<hr>



### variable Lure 

_Increases rate of fish biting your hook._ 
```C++
constexpr auto endstone::Enchantment::Lure;
```




<hr>



### variable Mending 

_Allows mending the item using experience orbs._ 
```C++
constexpr auto endstone::Enchantment::Mending;
```




<hr>



### variable Multishot 

_Shoot multiple arrows from crossbows._ 
```C++
constexpr auto endstone::Enchantment::Multishot;
```




<hr>



### variable Piercing 

_Crossbow projectiles pierce entities._ 
```C++
constexpr auto endstone::Enchantment::Piercing;
```




<hr>



### variable Power 

_Provides extra damage when shooting arrows from bows._ 
```C++
constexpr auto endstone::Enchantment::Power;
```




<hr>



### variable ProjectileProtection 

_Provides protection against projectile damage._ 
```C++
constexpr auto endstone::Enchantment::ProjectileProtection;
```




<hr>



### variable Protection 

_Provides protection against environmental damage._ 
```C++
constexpr auto endstone::Enchantment::Protection;
```




<hr>



### variable Punch 

_Provides a knockback when an entity is hit by an arrow from a bow._ 
```C++
constexpr auto endstone::Enchantment::Punch;
```




<hr>



### variable QuickCharge 

_Charges crossbows quickly._ 
```C++
constexpr auto endstone::Enchantment::QuickCharge;
```




<hr>



### variable Respiration 

_Decreases the rate of air loss whilst underwater._ 
```C++
constexpr auto endstone::Enchantment::Respiration;
```




<hr>



### variable Riptide 

_When it is rainy, launches the player in the direction their trident is thrown._ 
```C++
constexpr auto endstone::Enchantment::Riptide;
```




<hr>



### variable Sharpness 

_Increases damage against all targets._ 
```C++
constexpr auto endstone::Enchantment::Sharpness;
```




<hr>



### variable SilkTouch 

_Allows blocks to drop themselves instead of fragments (for example, stone instead of cobblestone)._ 
```C++
constexpr auto endstone::Enchantment::SilkTouch;
```




<hr>



### variable Smite 

_Increases damage against undead targets._ 
```C++
constexpr auto endstone::Enchantment::Smite;
```




<hr>



### variable SoulSpeed 

_Walk quicker on soul blocks._ 
```C++
constexpr auto endstone::Enchantment::SoulSpeed;
```




<hr>



### variable SwiftSneak 

_Walk quicker while sneaking._ 
```C++
constexpr auto endstone::Enchantment::SwiftSneak;
```




<hr>



### variable Thorns 

_Damages the attacker._ 
```C++
constexpr auto endstone::Enchantment::Thorns;
```




<hr>



### variable Unbreaking 

_Decreases the rate at which a tool looses durability._ 
```C++
constexpr auto endstone::Enchantment::Unbreaking;
```




<hr>



### variable WindBurst 

_Emits wind burst upon hitting enemy._ 
```C++
constexpr auto endstone::Enchantment::WindBurst;
```




<hr>
## Public Functions Documentation




### function canEnchantItem 

_Checks if this_ [_**Enchantment**_](classendstone_1_1Enchantment.md) _may be applied to the given_[_**ItemStack**_](classendstone_1_1ItemStack.md) _._
```C++
virtual bool endstone::Enchantment::canEnchantItem (
    const  ItemStack & item
) const = 0
```



This does not check if it conflicts with any enchantments already applied to the item.




**Parameters:**


* `item` [**Item**](classendstone_1_1Item.md) to test 



**Returns:**

True if the enchantment may be applied, otherwise False 





        

<hr>



### function conflictsWith 

_Check if this enchantment conflicts with another enchantment._ 
```C++
virtual bool endstone::Enchantment::conflictsWith (
    const  Enchantment & other
) const = 0
```





**Parameters:**


* `other` The enchantment to check against 



**Returns:**

True if there is a conflict. 





        

<hr>



### function getMaxLevel 

_Gets the maximum level that this_ [_**Enchantment**_](classendstone_1_1Enchantment.md) _may become._
```C++
virtual int endstone::Enchantment::getMaxLevel () const = 0
```





**Returns:**

Maximum level of the [**Enchantment**](classendstone_1_1Enchantment.md) 





        

<hr>



### function getStartLevel 

_Gets the level that this_ [_**Enchantment**_](classendstone_1_1Enchantment.md) _should start at (also known as minimum level)._
```C++
virtual int endstone::Enchantment::getStartLevel () const = 0
```





**Returns:**

Starting level of the [**Enchantment**](classendstone_1_1Enchantment.md) 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/enchantments/enchantment.h`

