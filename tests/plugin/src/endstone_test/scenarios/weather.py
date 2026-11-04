from endstone.event import ThunderChangeEvent, WeatherChangeEvent

from endstone_test.harness import Context, scenario


def weather(ctx: Context) -> str:
    output = ctx.run("weather query").output
    return output.rsplit(":", 1)[-1].strip()


async def clear_sky(ctx: Context) -> None:
    ctx.run("weather clear")
    await ctx.until(lambda: weather(ctx) == "clear", timeout=200, what="clear weather")


@scenario("weather.rain.normal", covers={WeatherChangeEvent: "normal"})
async def rain_normal(ctx: Context) -> None:
    await clear_sky(ctx)
    rain = ctx.capture(WeatherChangeEvent)
    ctx.run("weather rain")
    await ctx.until(lambda: rain.count, what="WeatherChangeEvent")
    await ctx.ticks(5)
    assert rain.count == 1, rain.events
    event = rain.events[0]
    assert event["to_weather_state"] is True
    assert event["level"] == {"name": ctx.level.name}
    assert (state := weather(ctx)) == "rain", state
    ctx.run("weather clear")


@scenario("weather.rain.cancel", covers={WeatherChangeEvent: "cancel"})
async def rain_cancel(ctx: Context) -> None:
    await clear_sky(ctx)
    rain = ctx.capture(WeatherChangeEvent, cancel=True)
    ctx.run("weather rain")
    await ctx.until(lambda: rain.count, what="WeatherChangeEvent")
    await ctx.ticks(40)
    assert rain.events[0]["to_weather_state"] is True
    assert (state := weather(ctx)) == "clear", state


@scenario("weather.rain.stop", covers={WeatherChangeEvent: ("normal", "cancel")})
async def rain_stop(ctx: Context) -> None:
    await clear_sky(ctx)
    ctx.run("weather rain")
    await ctx.ticks(1)
    stop = ctx.capture(WeatherChangeEvent, cancel=True)
    ctx.run("weather clear")
    await ctx.until(lambda: stop.count, what="WeatherChangeEvent")
    assert stop.events[0]["to_weather_state"] is False
    assert (state := weather(ctx)) == "rain", state
    ctx.release(stop)
    clear = ctx.capture(WeatherChangeEvent)
    ctx.run("weather clear")
    await ctx.until(lambda: clear.count, what="WeatherChangeEvent")
    assert clear.count == 1 and clear.events[0]["to_weather_state"] is False
    assert (state := weather(ctx)) == "clear", state


@scenario("weather.thunder.normal", covers={ThunderChangeEvent: "normal"})
async def thunder_normal(ctx: Context) -> None:
    await clear_sky(ctx)
    thunder = ctx.capture(ThunderChangeEvent)
    rain = ctx.capture(WeatherChangeEvent)
    ctx.run("weather thunder")
    await ctx.until(lambda: thunder.count, what="ThunderChangeEvent")
    await ctx.ticks(5)
    assert thunder.count == 1, thunder.events
    assert thunder.events[0]["to_thunder_state"] is True
    assert thunder.events[0]["level"] == {"name": ctx.level.name}
    assert rain.count == 1 and rain.events[0]["to_weather_state"] is True
    assert (state := weather(ctx)) == "thunder", state
    ctx.run("weather clear")


@scenario("weather.thunder.cancel", covers={ThunderChangeEvent: "cancel"})
async def thunder_cancel(ctx: Context) -> None:
    await clear_sky(ctx)
    thunder = ctx.capture(ThunderChangeEvent, cancel=True)
    ctx.run("weather thunder")
    await ctx.until(lambda: thunder.count, what="ThunderChangeEvent")
    await ctx.ticks(40)
    assert (state := weather(ctx)) == "rain", (
        f"cancelling only the thunder left {state}"
    )
    ctx.run("weather clear")
