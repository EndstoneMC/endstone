from endstone_test.harness import Context
from endstone_test.scenarios.player_apis import mark, settle, wait_for


async def available(ctx: Context, bot, player) -> dict:
    since = await mark(bot)
    player.update_commands()
    await settle(ctx, player, bot, since)
    found = await wait_for(
        ctx,
        bot,
        "AvailableCommandsPacket",
        since,
        lambda f: len(f) >= 1,
        "the command list",
    )
    return found[-1]
