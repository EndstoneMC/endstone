from endstone.map import MapRenderer

from endstone_test.harness import Context, scenario
from endstone_test.scenarios.player_apis import join, map_item

BASE = (40, 50, 60, 255)
TOP = (200, 100, 50, 255)


class BaseThenTopRenderer(MapRenderer):
    def __init__(self, paint: tuple[int, int, int, int] | None, record: list):
        MapRenderer.__init__(self, is_contextual=False)
        self.paint = paint
        self.record = record

    def render(self, view, canvas, player):
        if self.paint is not None:
            canvas.set_pixel_color(3, 4, self.paint)
        self.record.append((canvas.get_base_pixel_color(3, 4), canvas.get_pixel_color(3, 4)))


@scenario("apis.map.base_pixel_color", budget=1500)
async def map_base_pixel_color(ctx: Context) -> None:
    bot, player = await join(ctx, "ApBase")
    await ctx.ticks(20)
    seen: list = []
    view, item = map_item(ctx, player, BaseThenTopRenderer(BASE, seen))
    top_seen: list = []
    view.add_renderer(BaseThenTopRenderer(TOP, top_seen))
    player.inventory.add_item(item)
    await ctx.until(lambda: seen and top_seen, timeout=200, what="both renders")
    assert tuple(top_seen[0][0]) == BASE, top_seen[0]
    assert tuple(top_seen[0][1]) == TOP, top_seen[0]
    assert tuple(seen[0][1]) == BASE, seen[0]
