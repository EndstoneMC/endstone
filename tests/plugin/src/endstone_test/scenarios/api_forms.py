import json

from endstone.form import (
    ActionForm,
    Button,
    Divider,
    Dropdown,
    Header,
    Label,
    MessageForm,
    ModalForm,
    Slider,
    StepSlider,
    TextInput,
    Toggle,
)

from endstone_test.harness import Context, scenario
from endstone_test.scenarios.player_apis import join, mark, next_form, wait_for


def raw(text: str) -> dict:
    return {"rawtext": [{"text": text}]}


async def received(ctx: Context, bot, since: int, count: int) -> dict:
    found = await wait_for(
        ctx, bot, "ModalFormRequestPacket", since, lambda f: len(f) >= count, f"form request {count}"
    )
    return found[count - 1]


@scenario("apis.forms.setters", budget=2500)
async def forms_setters(ctx: Context) -> None:
    bot, player = await join(ctx, "ApFSet")
    await ctx.ticks(20)
    since = await mark(bot)
    heard: list[tuple] = []

    dropdown = Dropdown()
    dropdown.label = "dd"
    dropdown.options = ["a"]
    dropdown.add_option("b")
    dropdown.add_option("c")
    dropdown.default_index = 2

    step = StepSlider()
    step.label = "ss"
    step.add_option("x")
    step.options = ["x", "y"]
    step.add_option("z")
    step.default_index = 1

    slider = Slider()
    slider.label = "sl"
    slider.min = 2
    slider.max = 20
    slider.step = 3
    slider.default_value = 5

    field = TextInput()
    field.label = "ti"
    field.placeholder = "ph"
    field.default_value = "df"

    toggle = Toggle()
    toggle.label = "tg"
    toggle.default_value = True

    header = Header()
    header.label = "hd"
    label = Label()
    label.text = "lb"

    modal = ModalForm()
    modal.title = "Modal"
    modal.submit_button = "Go"
    modal.icon = "textures/items/apple"
    modal.controls = [header, Divider()]
    for control in (label, dropdown, slider, step, field, toggle):
        modal.add_control(control)
    modal.on_submit = lambda p, v: heard.append(("modal", p.name, list(v)))
    modal.on_close = lambda p: heard.append(("modal closed", p.name))
    player.send_form(modal)
    first = await received(ctx, bot, since, 1)
    assert json.loads(first["form_json"]) == {
        "type": "custom_form",
        "title": raw("Modal"),
        "icon": {"type": "path", "data": "textures/items/apple"},
        "submit": raw("Go"),
        "content": [
            {"type": "header", "text": raw("hd")},
            {"type": "divider", "text": ""},
            {"type": "label", "text": raw("lb")},
            {"type": "dropdown", "text": raw("dd"), "options": ["a", "b", "c"], "default": 2},
            {"type": "slider", "text": raw("sl"), "min": 2.0, "max": 20.0, "step": 3.0, "default": 5.0},
            {"type": "step_slider", "text": raw("ss"), "steps": ["x", "y", "z"], "default": 1},
            {"type": "input", "text": raw("ti"), "placeholder": raw("ph"), "default": "df"},
            {"type": "toggle", "text": raw("tg"), "default": True},
        ],
    }, first["form_json"]
    values = [None, None, None, 2, 8, 1, "typed", False]
    await bot.call("local_player.answer_form", values, first["form_id"])
    await ctx.until(lambda: len(heard) == 1, timeout=60, what="the modal on_submit set through its setter")
    assert heard[-1] == ("modal", "ApFSet", values), heard

    clicked: list[str] = []
    first_button = Button()
    first_button.text = "b0"
    first_button.icon = "textures/items/apple"
    first_button.on_click = lambda p: clicked.append("b0")
    second_button = Button("b1")
    second_button.icon = "https://x.y/z.png"
    second_button.on_click = lambda p: clicked.append("b1")

    action = ActionForm()
    action.title = "Action"
    action.content = "ac"
    action.controls = [first_button]
    action.add_header("hh").add_divider().add_label("ll")
    action.controls = action.controls + [second_button]
    action.on_submit = lambda p, s: heard.append(("action", s))
    action.on_close = lambda p: heard.append(("action closed", p.name))
    player.send_form(action)
    second = await received(ctx, bot, since, 2)
    assert json.loads(second["form_json"]) == {
        "type": "form",
        "title": raw("Action"),
        "content": raw("ac"),
        "elements": [
            {"type": "button", "text": raw("b0"), "image": {"type": "path", "data": "textures/items/apple"}},
            {"type": "header", "text": raw("hh")},
            {"type": "divider", "text": ""},
            {"type": "label", "text": raw("ll")},
            {"type": "button", "text": raw("b1"), "image": {"type": "url", "data": "https://x.y/z.png"}},
        ],
    }, second["form_json"]
    await bot.call("local_player.send_form_response", second["form_id"], "1")
    await ctx.until(lambda: len(heard) == 2, timeout=60, what="the action on_submit set through its setter")
    assert heard[-1] == ("action", 1) and clicked == ["b1"], (heard, clicked)

    message = MessageForm()
    message.title = "Message"
    message.content = "mc"
    message.button1 = "yes"
    message.button2 = "no"
    message.on_submit = lambda p, s: heard.append(("message", s))
    message.on_close = lambda p: heard.append(("message closed", p.name))
    player.send_form(message)
    third = await received(ctx, bot, since, 3)
    assert json.loads(third["form_json"]) == {
        "type": "modal",
        "title": raw("Message"),
        "content": raw("mc"),
        "button1": raw("yes"),
        "button2": raw("no"),
    }, third["form_json"]
    await bot.call("local_player.answer_form", False, third["form_id"])
    await ctx.until(lambda: len(heard) == 3, timeout=60, what="the message on_submit set through its setter")
    assert heard[-1] == ("message", 1), heard

    player.send_form(message)
    fourth = await received(ctx, bot, since, 4)
    await bot.call("local_player.close_form", "USER_CLOSED", fourth["form_id"])
    await ctx.until(lambda: len(heard) == 4, timeout=60, what="the message on_close set through its setter")
    assert heard[-1] == ("message closed", "ApFSet"), heard
    assert await bot.call("is_connected")


@scenario("apis.forms.resend", budget=2000)
async def forms_resend(ctx: Context) -> None:
    bot, player = await join(ctx, "ApFRe")
    await ctx.ticks(20)
    since = await mark(bot)
    heard: list[str] = []
    form = MessageForm("One", "c", "y", "n")
    form.on_submit = lambda p, s: heard.append(form.title)
    player.send_form(form)
    first = await received(ctx, bot, since, 1)
    form.title = "Two"
    form.button1 = "changed"
    player.send_form(form)
    second = await received(ctx, bot, since, 2)
    assert json.loads(first["form_json"])["title"] == raw("One")
    assert json.loads(first["form_json"])["button1"] == raw("y")
    assert json.loads(second["form_json"])["title"] == raw("Two")
    assert json.loads(second["form_json"])["button1"] == raw("changed")
    assert first["form_id"] != second["form_id"]
    await bot.call("local_player.answer_form", True, second["form_id"])
    await ctx.until(lambda: heard, timeout=60, what="the callback of the resent form")
    assert heard == ["Two"], heard
    assert await next_form(ctx, bot, since, 2) == second["form_id"]
