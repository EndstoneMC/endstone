# Mob AI controls

Mob AI controls reduce random strolling and looking around, space out new target searches, and control villager sleep
attempts. They are disabled by default.

## Enable the controls {#configuration-and-first-setup}

Open `endstone.toml` in your server directory. The launcher adds these defaults:

```toml
[mob-ai]
enabled = false

[mob-ai.idle]
enabled = true
player-range-blocks = 32
disable-random-stroll = true
disable-random-look-around = true

[mob-ai.target-search]
enabled = true
player-range-blocks = 32
interval-ticks = 20

[mob-ai.villagers]
sleep-failure-skip-night = false
```

Set `enabled = true` under `[mob-ai]` and restart the server. The master switch controls every feature and override.

- **Idle:** Outside `player-range-blocks`, stop random strolling and looking around on supported mobs. Set either
  `disable-*` option to `false` to keep that behavior.
- **Target search:** Outside its own `player-range-blocks`, use `interval-ticks` to space out searches for new targets.
  Existing targets and attacks continue normally.
- **Villagers:** Set `sleep-failure-skip-night = true` to skip further sleep attempts for the night after a modern
  villager's (`minecraft:villager_v2`) bed path fails.

Idle and target search each have an `enabled` switch and a separate range. A nonspectator player in the same dimension,
at or within that distance, keeps normal behavior for that feature. Distance includes vertical separation and does not
change how far mobs detect targets. Use positive, finite ranges and an integer from 1 to 4294967295 for `interval-ticks`.

A skipped sleep attempt starts when an owned-bed search returns no path or an empty path, or movement toward that bed
fails. Having no owned bed, a schedule check, an accepted partial path or normal waking does not start skipping. The
setting does not wake sleeping villagers or change their sleep and wake rules. Movement, bed changes and unloading the
villager do not end it early; restarting the server clears failed attempts.

The night runs from 12000 until the next day's 12000. A failed attempt stays blocked through dawn and the following
daytime, including a nitwit's schedule across midnight. Normal checks resume in the next period, subject to the game's
own schedule. Time changes within one period keep failures blocked; observing another period clears them, forward or
backward. Returning to an earlier period does not restore them.

## Set options for individual mobs {#per-type-overrides}

Add `[[mob-ai.overrides]]` entries to give particular types different settings. For example:

```toml
[[mob-ai.overrides]]
types = ["minecraft:cow", "minecraft:pig", "minecraft:skeleton"]

[mob-ai.overrides.idle]
player-range-blocks = 16

[mob-ai.overrides.target-search]
enabled = true
player-range-blocks = 48
interval-ticks = 40
```

Use `idle`, `target-search` or `villagers` subtables with the global options. Omitted options inherit global values. Set
`enabled = false` in an override's `idle` or `target-search` subtable to disable that feature for its types. In the
`villagers` subtable, `sleep-failure-skip-night = false` keeps normal sleep attempts.

Each `types` list must be nonempty and contain unique, exact identifiers; wildcards are not supported. A type can appear
in separate rules for different features, but two rules cannot configure the same feature for the same type. Overrides
cannot make unsupported mobs eligible, and types outside the `minecraft:` namespace are unaffected.

## Supported mobs and activity protection {#capability-boundaries-and-activity-protection}

Idle controls affect mobs with random strolling or looking-around behaviors, such as cows, pigs and spiders. Exceptions
include villagers and traders, trader llamas, bosses, wardens, creakings, golems, allays, guardians and shulkers. Special
entities such as NPCs and armor stands are also excluded.

Target-search controls support zombies, husks, drowned, zombie villagers, skeletons, strays, wither skeletons, bogged,
parched, creepers and witches. Other mobs keep their normal target searches. The sleep setting applies only to
`minecraft:villager_v2`.

Mobs keep normal idle and target-search behavior while breeding, tamed or leashed, while they have a target, or while
riding another entity or carrying passengers. Recently hurt or interacted-with mobs also keep normal behavior.

## Apply or disable changes {#apply-changes-disable-and-restore}

Settings are copied at startup. Restart after editing `endstone.toml`; `/reload` does not apply them. Check for
`[Perf] Mob AI policies loaded` in the startup log. Invalid settings are reported and leave controls disabled. Unknown
fields, invalid values and conflicting overrides are rejected even when the master switch is off. Malformed TOML may stop
the launcher before startup.

To disable all controls, set `enabled = false` under `[mob-ai]` and restart. Other settings and overrides are preserved.

Try these settings with your farms and mob transport systems before relying on them.
