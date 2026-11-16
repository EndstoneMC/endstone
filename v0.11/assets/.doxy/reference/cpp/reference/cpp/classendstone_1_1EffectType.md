

# Class endstone::EffectType



[**ClassList**](annotated.md) **>** [**endstone**](namespaceendstone.md) **>** [**EffectType**](classendstone_1_1EffectType.md)



_All effect types._ 

* `#include <endstone/effect/effect_type.h>`























## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  auto | [**Absorption**](#variable-absorption)   = `EffectId::minecraft("absorption")`<br> |
|  auto | [**BadOmen**](#variable-badomen)   = `EffectId::minecraft("bad\_omen")`<br> |
|  auto | [**Blindness**](#variable-blindness)   = `EffectId::minecraft("blindness")`<br> |
|  auto | [**BreathOfTheNautilus**](#variable-breathofthenautilus)   = `EffectId::minecraft("breath\_of\_the\_nautilus")`<br> |
|  auto | [**ConduitPower**](#variable-conduitpower)   = `EffectId::minecraft("conduit\_power")`<br> |
|  auto | [**Darkness**](#variable-darkness)   = `EffectId::minecraft("darkness")`<br> |
|  auto | [**FatalPoison**](#variable-fatalpoison)   = `EffectId::minecraft("fatal\_poison")`<br> |
|  auto | [**FireResistance**](#variable-fireresistance)   = `EffectId::minecraft("fire\_resistance")`<br> |
|  auto | [**Haste**](#variable-haste)   = `EffectId::minecraft("haste")`<br> |
|  auto | [**HealthBoost**](#variable-healthboost)   = `EffectId::minecraft("health\_boost")`<br> |
|  auto | [**HeroOfTheVillage**](#variable-heroofthevillage)   = `EffectId::minecraft("hero\_of\_the\_village")`<br> |
|  auto | [**Hunger**](#variable-hunger)   = `EffectId::minecraft("hunger")`<br> |
|  auto | [**Infested**](#variable-infested)   = `EffectId::minecraft("infested")`<br> |
|  auto | [**InstantDamage**](#variable-instantdamage)   = `EffectId::minecraft("instant\_damage")`<br> |
|  auto | [**InstantHealth**](#variable-instanthealth)   = `EffectId::minecraft("instant\_health")`<br> |
|  auto | [**Invisibility**](#variable-invisibility)   = `EffectId::minecraft("invisibility")`<br> |
|  auto | [**JumpBoost**](#variable-jumpboost)   = `EffectId::minecraft("jump\_boost")`<br> |
|  auto | [**Levitation**](#variable-levitation)   = `EffectId::minecraft("levitation")`<br> |
|  auto | [**MiningFatigue**](#variable-miningfatigue)   = `EffectId::minecraft("mining\_fatigue")`<br> |
|  auto | [**Nausea**](#variable-nausea)   = `EffectId::minecraft("nausea")`<br> |
|  auto | [**NightVision**](#variable-nightvision)   = `EffectId::minecraft("night\_vision")`<br> |
|  auto | [**Oozing**](#variable-oozing)   = `EffectId::minecraft("oozing")`<br> |
|  auto | [**Poison**](#variable-poison)   = `EffectId::minecraft("poison")`<br> |
|  auto | [**RaidOmen**](#variable-raidomen)   = `EffectId::minecraft("raid\_omen")`<br> |
|  auto | [**Regeneration**](#variable-regeneration)   = `EffectId::minecraft("regeneration")`<br> |
|  auto | [**Resistance**](#variable-resistance)   = `EffectId::minecraft("resistance")`<br> |
|  auto | [**Saturation**](#variable-saturation)   = `EffectId::minecraft("saturation")`<br> |
|  auto | [**SlowFalling**](#variable-slowfalling)   = `EffectId::minecraft("slow\_falling")`<br> |
|  auto | [**Slowness**](#variable-slowness)   = `EffectId::minecraft("slowness")`<br> |
|  auto | [**Speed**](#variable-speed)   = `EffectId::minecraft("speed")`<br> |
|  auto | [**Strength**](#variable-strength)   = `EffectId::minecraft("strength")`<br> |
|  auto | [**TrialOmen**](#variable-trialomen)   = `EffectId::minecraft("trial\_omen")`<br> |
|  auto | [**WaterBreathing**](#variable-waterbreathing)   = `EffectId::minecraft("water\_breathing")`<br> |
|  auto | [**Weakness**](#variable-weakness)   = `EffectId::minecraft("weakness")`<br> |
|  auto | [**Weaving**](#variable-weaving)   = `EffectId::minecraft("weaving")`<br> |
|  auto | [**WindCharged**](#variable-windcharged)   = `EffectId::minecraft("wind\_charged")`<br> |
|  auto | [**Wither**](#variable-wither)   = `EffectId::minecraft("wither")`<br> |










































## Public Static Attributes Documentation




### variable Absorption 

```C++
auto endstone::EffectType::Absorption;
```



Increases the maximum health of an entity with health that cannot be regenerated, but is refilled every 30 seconds. 


        

<hr>



### variable BadOmen 

```C++
auto endstone::EffectType::BadOmen;
```



Triggers an ominous event when the player enters a village or trial chambers.
 oof. 


        

<hr>



### variable Blindness 

```C++
auto endstone::EffectType::Blindness;
```



Blinds an entity. 


        

<hr>



### variable BreathOfTheNautilus 

```C++
auto endstone::EffectType::BreathOfTheNautilus;
```



Prevents the rider's oxygen bar from depleting while riding a nautilus or zombie nautilus. 


        

<hr>



### variable ConduitPower 

```C++
auto endstone::EffectType::ConduitPower;
```



Effects granted by a nearby conduit. Includes enhanced underwater abilities. 


        

<hr>



### variable Darkness 

```C++
auto endstone::EffectType::Darkness;
```



Causes the player's vision to dim occasionally. 


        

<hr>



### variable FatalPoison 

```C++
auto endstone::EffectType::FatalPoison;
```



Inflicts damage to the entity over time, and can kill both undead and non-undead mobs, similar to Wither 


        

<hr>



### variable FireResistance 

```C++
auto endstone::EffectType::FireResistance;
```



Stops fire damage. 


        

<hr>



### variable Haste 

```C++
auto endstone::EffectType::Haste;
```



Increases dig speed. 


        

<hr>



### variable HealthBoost 

```C++
auto endstone::EffectType::HealthBoost;
```



Increases the maximum health of an entity. 


        

<hr>



### variable HeroOfTheVillage 

```C++
auto endstone::EffectType::HeroOfTheVillage;
```



Reduces the cost of villager trades.
 \o/. 


        

<hr>



### variable Hunger 

```C++
auto endstone::EffectType::Hunger;
```



Increases hunger. 


        

<hr>



### variable Infested 

```C++
auto endstone::EffectType::Infested;
```



Chance of spawning silverfish when hurt. 


        

<hr>



### variable InstantDamage 

```C++
auto endstone::EffectType::InstantDamage;
```



Hurts an entity. 


        

<hr>



### variable InstantHealth 

```C++
auto endstone::EffectType::InstantHealth;
```



Heals an entity. 


        

<hr>



### variable Invisibility 

```C++
auto endstone::EffectType::Invisibility;
```



Grants invisibility. 


        

<hr>



### variable JumpBoost 

```C++
auto endstone::EffectType::JumpBoost;
```



Increases jump height. 


        

<hr>



### variable Levitation 

```C++
auto endstone::EffectType::Levitation;
```



Causes the entity to float into the air. 


        

<hr>



### variable MiningFatigue 

```C++
auto endstone::EffectType::MiningFatigue;
```



Decreases dig speed. 


        

<hr>



### variable Nausea 

```C++
auto endstone::EffectType::Nausea;
```



Warps vision on the client. 


        

<hr>



### variable NightVision 

```C++
auto endstone::EffectType::NightVision;
```



Allows an entity to see in the dark. 


        

<hr>



### variable Oozing 

```C++
auto endstone::EffectType::Oozing;
```



Causes slimes to spawn upon death. 


        

<hr>



### variable Poison 

```C++
auto endstone::EffectType::Poison;
```



Deals damage to an entity over time. 


        

<hr>



### variable RaidOmen 

```C++
auto endstone::EffectType::RaidOmen;
```



Triggers a raid when a player enters a village. 


        

<hr>



### variable Regeneration 

```C++
auto endstone::EffectType::Regeneration;
```



Regenerates health. 


        

<hr>



### variable Resistance 

```C++
auto endstone::EffectType::Resistance;
```



Decreases damage dealt to an entity. 


        

<hr>



### variable Saturation 

```C++
auto endstone::EffectType::Saturation;
```



Increases the food level of an entity each tick. 


        

<hr>



### variable SlowFalling 

```C++
auto endstone::EffectType::SlowFalling;
```



Slows entity fall rate. 


        

<hr>



### variable Slowness 

```C++
auto endstone::EffectType::Slowness;
```



Decreases movement speed. 


        

<hr>



### variable Speed 

```C++
auto endstone::EffectType::Speed;
```



Increases movement speed. 


        

<hr>



### variable Strength 

```C++
auto endstone::EffectType::Strength;
```



Increases damage dealt. 


        

<hr>



### variable TrialOmen 

```C++
auto endstone::EffectType::TrialOmen;
```



Causes trial spawners to become ominous. 


        

<hr>



### variable WaterBreathing 

```C++
auto endstone::EffectType::WaterBreathing;
```



Allows breathing underwater. 


        

<hr>



### variable Weakness 

```C++
auto endstone::EffectType::Weakness;
```



Decreases damage dealt by an entity. 


        

<hr>



### variable Weaving 

```C++
auto endstone::EffectType::Weaving;
```



Creates cobwebs upon death. 


        

<hr>



### variable WindCharged 

```C++
auto endstone::EffectType::WindCharged;
```



Emits a wind burst upon death. 


        

<hr>



### variable Wither 

```C++
auto endstone::EffectType::Wither;
```



Deals damage to an entity over time and gives the health to the shooter. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/endstone/effect/effect_type.h`

