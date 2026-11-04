import uuid

import pytest
from endstone import Server
from endstone.command import CommandSenderWrapper
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
from endstone.permissions import Permission, PermissionDefault, PermissionLevel
from endstone.plugin import (
    Plugin,
    PluginDescription,
    PluginLoadOrder,
    Service,
    ServicePriority,
)


def unique(prefix: str = "apiforms") -> str:
    return f"{prefix}.{uuid.uuid4().hex}"


@pytest.fixture
def registered(server: Server):
    manager = server.plugin_manager
    added: list[str] = []

    def register(perm: Permission) -> Permission:
        result = manager.add_permission(perm)
        added.append(result.name)
        return result

    yield register
    for name in added:
        manager.remove_permission(name)


def listed(server: Server, name: str) -> set[PermissionLevel]:
    levels = (PermissionLevel.DEFAULT, PermissionLevel.OPERATOR, PermissionLevel.CONSOLE)
    manager = server.plugin_manager
    return {level for level in levels if name in [p.name for p in manager.get_default_permissions(level)]}


class TestButton:
    def test_defaults(self):
        button = Button()
        assert button.text == ""
        assert button.icon is None
        assert not button.on_click

    def test_constructor_keywords(self):
        button = Button(text="go", icon="textures/items/apple", on_click=lambda player: None)
        assert button.text == "go"
        assert button.icon == "textures/items/apple"
        assert button.on_click

    def test_setters(self):
        button = Button("a")
        button.text = "b"
        button.icon = "https://x.y/z.png"
        assert not button.on_click
        button.on_click = lambda player: None
        assert button.text == "b"
        assert button.icon == "https://x.y/z.png"
        assert button.on_click

    def test_icon_can_be_cleared(self):
        button = Button("a", "textures/items/apple")
        button.icon = None
        assert button.icon is None

    def test_mutation_through_a_form_is_seen_by_the_form(self):
        form = ActionForm("t", "c").add_button("old")
        form.controls[0].text = "new"
        form.controls[0].icon = "textures/items/apple"
        assert form.controls[0].text == "new"
        assert form.controls[0].icon == "textures/items/apple"


class TestLabelAndHeader:
    def test_label(self):
        assert Label().text == ""
        label = Label(text="one")
        assert label.text == "one"
        label.text = "two"
        assert label.text == "two"

    def test_header(self):
        assert Header().label == ""
        header = Header(label="one")
        assert header.label == "one"
        header.label = "two"
        assert header.label == "two"

    def test_a_label_added_to_a_form_is_copied(self):
        label = Label("a")
        form = ModalForm("t", [label])
        label.text = "changed"
        assert form.controls[0].text == "a"


class TestToggle:
    def test_defaults(self):
        toggle = Toggle()
        assert toggle.label == ""
        assert toggle.default_value is False

    def test_constructor_keywords(self):
        toggle = Toggle(label="tg", default_value=True)
        assert toggle.label == "tg"
        assert toggle.default_value is True

    def test_setters(self):
        toggle = Toggle("a")
        toggle.label = "b"
        toggle.default_value = True
        assert (toggle.label, toggle.default_value) == ("b", True)
        toggle.default_value = False
        assert toggle.default_value is False


class TestTextInput:
    def test_defaults(self):
        field = TextInput()
        assert (field.label, field.placeholder, field.default_value) == ("", "", None)

    def test_constructor_keywords(self):
        field = TextInput(label="l", placeholder="p", default_value="d")
        assert (field.label, field.placeholder, field.default_value) == ("l", "p", "d")

    def test_setters(self):
        field = TextInput()
        field.label = "l"
        field.placeholder = "p"
        field.default_value = "d"
        assert (field.label, field.placeholder, field.default_value) == ("l", "p", "d")
        field.default_value = None
        assert field.default_value is None


class TestSlider:
    def test_defaults(self):
        slider = Slider()
        assert slider.label == ""
        assert (slider.min, slider.max, slider.step) == (0, 100, 20)
        assert slider.default_value is None

    def test_constructor_keywords(self):
        slider = Slider(label="s", min=1, max=10, step=2, default_value=4)
        assert slider.label == "s"
        assert (slider.min, slider.max, slider.step, slider.default_value) == (1, 10, 2, 4)

    def test_setters(self):
        slider = Slider()
        slider.label = "s"
        slider.min = -5.5
        slider.max = 5.5
        slider.step = 0.5
        slider.default_value = 1.5
        assert slider.label == "s"
        assert (slider.min, slider.max, slider.step, slider.default_value) == (-5.5, 5.5, 0.5, 1.5)
        slider.default_value = None
        assert slider.default_value is None


@pytest.mark.parametrize("cls", [Dropdown, StepSlider])
class TestChoiceControls:
    def test_defaults(self, cls):
        control = cls()
        assert control.label == ""
        assert control.options == []
        assert control.default_index is None

    def test_constructor_keywords(self, cls):
        control = cls(label="c", options=["a", "b", "c"], default_index=2)
        assert control.label == "c"
        assert control.options == ["a", "b", "c"]
        assert control.default_index == 2

    def test_setters(self, cls):
        control = cls()
        control.label = "c"
        control.options = ["x", "y"]
        control.default_index = 1
        assert control.label == "c"
        assert control.options == ["x", "y"]
        assert control.default_index == 1
        control.default_index = None
        assert control.default_index is None

    def test_add_option_appends_in_order(self, cls):
        control = cls("c", ["a"])
        control.add_option("b")
        control.add_option("b")
        assert control.options == ["a", "b", "b"]

    def test_setting_options_replaces_them(self, cls):
        control = cls("c", ["a", "b"])
        control.options = ["z"]
        control.add_option("y")
        assert control.options == ["z", "y"]

    def test_options_getter_is_a_copy(self, cls):
        control = cls("c", ["a"])
        control.options.append("b")
        assert control.options == ["a"]


class TestMessageForm:
    def test_defaults(self):
        form = MessageForm()
        assert (form.title, form.content, form.button1, form.button2) == ("", "", "", "")
        assert not form.on_submit
        assert not form.on_close

    def test_constructor_keywords(self):
        form = MessageForm(
            title="t",
            content="c",
            button1="y",
            button2="n",
            on_submit=lambda player, selection: None,
            on_close=lambda player: None,
        )
        assert (form.title, form.content, form.button1, form.button2) == ("t", "c", "y", "n")
        assert form.on_submit
        assert form.on_close

    def test_setters(self):
        form = MessageForm()
        form.title = "t"
        form.content = "c"
        form.button1 = "y"
        form.button2 = "n"
        form.on_submit = lambda player, selection: None
        form.on_close = lambda player: None
        assert (form.title, form.content, form.button1, form.button2) == ("t", "c", "y", "n")
        assert form.on_submit
        assert form.on_close

    def test_two_forms_do_not_share_state(self):
        a = MessageForm("a")
        b = MessageForm("b")
        a.title = "changed"
        assert b.title == "b"


class TestActionForm:
    def test_defaults(self):
        form = ActionForm()
        assert (form.title, form.content) == ("", "")
        assert form.controls == []
        assert not form.on_submit
        assert not form.on_close

    def test_constructor_keywords(self):
        form = ActionForm(
            title="t",
            content="c",
            buttons=[Button("b"), Label("l")],
            on_submit=lambda player, selection: None,
            on_close=lambda player: None,
        )
        assert (form.title, form.content) == ("t", "c")
        assert [type(c) for c in form.controls] == [Button, Label]
        assert form.on_submit
        assert form.on_close

    def test_setters(self):
        form = ActionForm()
        form.title = "t"
        form.content = "c"
        form.on_submit = lambda player, selection: None
        form.on_close = lambda player: None
        form.controls = [Header("h"), Button("b")]
        assert (form.title, form.content) == ("t", "c")
        assert [type(c) for c in form.controls] == [Header, Button]
        assert form.on_submit
        assert form.on_close

    def test_adders_chain_and_keep_order(self):
        form = ActionForm("t", "c")
        chained = form.add_header("h").add_label("l").add_divider().add_button("b", "textures/items/apple")
        assert chained is form
        kinds = [type(c) for c in form.controls]
        assert kinds == [Header, Label, Divider, Button]
        assert form.controls[0].label == "h"
        assert form.controls[1].text == "l"
        assert form.controls[3].text == "b"
        assert form.controls[3].icon == "textures/items/apple"

    def test_button_callback_is_kept(self):
        form = ActionForm().add_button("b").add_button("c", on_click=lambda player: None)
        assert not form.controls[0].on_click
        assert form.controls[1].on_click

    def test_setting_controls_replaces_earlier_ones(self):
        form = ActionForm().add_button("old").add_label("old")
        form.controls = [Button("new")]
        assert len(form.controls) == 1
        assert form.controls[0].text == "new"


class TestModalForm:
    def test_defaults(self):
        form = ModalForm()
        assert form.title == ""
        assert form.controls == []
        assert form.submit_button is None
        assert form.icon is None
        assert not form.on_submit
        assert not form.on_close

    def test_constructor_keywords(self):
        form = ModalForm(
            title="t",
            controls=[Toggle("tg"), Slider("sl")],
            submit_button="Go",
            icon="textures/items/apple",
            on_submit=lambda player, values: None,
            on_close=lambda player: None,
        )
        assert form.title == "t"
        assert [type(c) for c in form.controls] == [Toggle, Slider]
        assert form.submit_button == "Go"
        assert form.icon == "textures/items/apple"
        assert form.on_submit
        assert form.on_close

    def test_setters(self):
        form = ModalForm()
        form.title = "t"
        form.submit_button = "Send"
        form.icon = "https://x.y/z.png"
        form.controls = [TextInput("ti")]
        form.on_submit = lambda player, values: None
        form.on_close = lambda player: None
        assert form.title == "t"
        assert form.submit_button == "Send"
        assert form.icon == "https://x.y/z.png"
        assert [type(c) for c in form.controls] == [TextInput]
        assert form.on_submit
        assert form.on_close
        form.submit_button = None
        form.icon = None
        assert form.submit_button is None
        assert form.icon is None

    def test_add_control_appends_in_order(self):
        form = ModalForm("t")
        form.add_control(Header("h"))
        form.add_control(Dropdown("d", ["a"]))
        form.add_control(Divider())
        assert [type(c) for c in form.controls] == [Header, Dropdown, Divider]
        form.add_control(Toggle("t"))
        assert type(form.controls[-1]) is Toggle

    def test_control_mutation_is_seen_by_the_form(self):
        form = ModalForm("t", [Dropdown("d", ["a"])])
        form.controls[0].add_option("b")
        form.controls[0].default_index = 1
        assert form.controls[0].options == ["a", "b"]
        assert form.controls[0].default_index == 1

    def test_add_control_rejects_a_non_control(self):
        with pytest.raises(TypeError):
            ModalForm().add_control("not a control")

    def test_controls_setter_rejects_a_non_control(self):
        with pytest.raises(TypeError):
            ModalForm().controls = ["not a control"]


class TestPermission:
    def test_constructor_defaults(self):
        perm = Permission("a.b")
        assert perm.name == "a.b"
        assert perm.description == ""
        assert perm.default == PermissionDefault.OP
        assert perm.children == {}
        assert perm.permissibles == set()

    def test_constructor_keywords(self):
        perm = Permission(name="a.b", description="d", default=PermissionDefault.NOT_OP, children={"a.c": False})
        assert (perm.name, perm.description, perm.default) == ("a.b", "d", PermissionDefault.NOT_OP)
        assert perm.children == {"a.c": False}

    def test_description_setter(self, server: Server, registered):
        name = unique()
        registered(Permission(name, "first"))
        registered_perm = server.plugin_manager.get_permission(name)
        registered_perm.description = "second"
        assert server.plugin_manager.get_permission(name).description == "second"
        registered_perm.description = ""
        assert server.plugin_manager.get_permission(name).description == ""

    def test_default_feeds_the_default_lists(self, server: Server, registered):
        name = unique()
        perm = registered(Permission(name, default=PermissionDefault.FALSE))
        assert perm.default == PermissionDefault.FALSE
        assert listed(server, name) == set()

        expected = {
            PermissionDefault.OP: {PermissionLevel.OPERATOR, PermissionLevel.CONSOLE},
            PermissionDefault.NOT_OP: {PermissionLevel.DEFAULT},
            PermissionDefault.TRUE: {PermissionLevel.DEFAULT, PermissionLevel.OPERATOR, PermissionLevel.CONSOLE},
            PermissionDefault.CONSOLE: {PermissionLevel.CONSOLE},
            PermissionDefault.FALSE: set(),
        }
        for value, levels in expected.items():
            perm.default = value
            assert perm.default == value
            assert listed(server, name) == levels, value

    def test_default_change_recalculates_a_subscribed_permissible(self, server: Server, plugin: Plugin, registered):
        name = unique()
        perm = registered(Permission(name, default=PermissionDefault.FALSE))
        sender = CommandSenderWrapper(server.command_sender)
        manager = server.plugin_manager
        manager.subscribe_to_default_perms(PermissionLevel.CONSOLE, sender)
        try:
            sender.recalculate_permissions()
            assert not sender.has_permission(name)
            perm.default = PermissionDefault.TRUE
            assert sender.has_permission(name)
            perm.default = PermissionDefault.FALSE
            assert not sender.has_permission(name)
        finally:
            manager.unsubscribe_from_default_perms(PermissionLevel.CONSOLE, sender)

    def test_permissibles_lists_the_subscribers(self, server: Server, registered):
        name = unique()
        perm = registered(Permission(name, default=PermissionDefault.FALSE))
        manager = server.plugin_manager
        first = CommandSenderWrapper(server.command_sender)
        second = CommandSenderWrapper(server.command_sender)
        assert perm.permissibles == set()
        manager.subscribe_to_permission(name, first)
        assert perm.permissibles == {first}
        manager.subscribe_to_permission(name, second)
        assert perm.permissibles == {first, second}
        manager.unsubscribe_from_permission(name, first)
        assert perm.permissibles == {second}
        manager.unsubscribe_from_permission(name, second)
        assert perm.permissibles == set()

    def test_add_parent_by_name_creates_and_links_the_parent(self, server: Server, registered):
        child_name = unique("apiforms.child")
        parent_name = unique("apiforms.Parent")
        child = registered(Permission(child_name))
        manager = server.plugin_manager
        assert manager.get_permission(parent_name.lower()) is None
        try:
            parent = child.add_parent(parent_name, True)
            assert parent.name == parent_name.lower()
            assert manager.get_permission(parent_name.lower()) is not None
            assert manager.get_permission(parent_name.lower()).children[child_name] is True
        finally:
            manager.remove_permission(parent_name.lower())

    def test_add_parent_by_name_reuses_an_existing_parent(self, server: Server, registered):
        child = registered(Permission(unique("apiforms.child")))
        parent = registered(Permission(unique("apiforms.parent")))
        returned = child.add_parent(parent.name, False)
        assert returned.name == parent.name
        assert server.plugin_manager.get_permission(parent.name).children[child.name] is False

    def test_add_parent_by_permission_links_the_child(self, server: Server, registered):
        child = registered(Permission(unique("apiforms.child")))
        parent = registered(Permission(unique("apiforms.parent")))
        child.add_parent(parent, True)
        assert server.plugin_manager.get_permission(parent.name).children[child.name] is True
        child.add_parent(parent, False)
        assert server.plugin_manager.get_permission(parent.name).children[child.name] is False

    def test_add_parent_grants_the_child_to_a_holder_of_the_parent(self, server: Server, plugin: Plugin, registered):
        child = registered(Permission(unique("apiforms.child"), default=PermissionDefault.FALSE))
        parent = registered(Permission(unique("apiforms.parent"), default=PermissionDefault.FALSE))
        sender = CommandSenderWrapper(server.command_sender)
        attachment = sender.add_attachment(plugin, parent.name, True)
        try:
            assert not sender.has_permission(child.name)
            child.add_parent(parent, True)
            assert sender.has_permission(child.name)
            child.add_parent(parent, False)
            assert not sender.has_permission(child.name)
        finally:
            attachment.remove()

    def test_add_parent_requires_a_registered_permission(self):
        with pytest.raises(RuntimeError):
            Permission("apiforms.unregistered").add_parent("apiforms.parent", True)

    def test_children_getter_reports_the_children(self, server: Server, registered):
        child = registered(Permission(unique("apiforms.child")))
        parent = registered(Permission(unique("apiforms.parent"), children={"apiforms.seed": True}))
        child.add_parent(parent, False)
        assert parent.children == {"apiforms.seed": True, child.name: False}

    def test_children_edits_apply_after_recalculate(self, server: Server, plugin: Plugin, registered):
        child = registered(Permission(unique("apiforms.child"), default=PermissionDefault.FALSE))
        parent = registered(Permission(unique("apiforms.parent"), default=PermissionDefault.FALSE))
        sender = CommandSenderWrapper(server.command_sender)
        attachment = sender.add_attachment(plugin, parent.name, True)
        try:
            assert not sender.has_permission(child.name)
            parent.children[child.name] = True
            parent.recalculate_permissibles()
            assert sender.has_permission(child.name)
        finally:
            attachment.remove()

    def test_recalculate_permissibles_with_no_subscribers_is_harmless(self, registered):
        perm = registered(Permission(unique(), default=PermissionDefault.FALSE))
        perm.recalculate_permissibles()
        assert perm.permissibles == set()

    def test_recalculate_permissibles_reaches_subscribed_permissibles(self, server: Server, plugin: Plugin, registered):
        parent = registered(Permission(unique("apiforms.parent"), default=PermissionDefault.FALSE))
        child = registered(Permission(unique("apiforms.child"), default=PermissionDefault.FALSE))
        sender = CommandSenderWrapper(server.command_sender)
        attachment = sender.add_attachment(plugin, parent.name, True)
        try:
            assert server.command_sender in parent.permissibles
            assert not sender.has_permission(child.name)
            child.add_parent(parent, True)
            parent.recalculate_permissibles()
            assert sender.has_permission(child.name)
        finally:
            attachment.remove()


class TestPluginDescription:
    def test_defaults(self):
        desc = PluginDescription("d", "1")
        assert desc.api_version
        assert desc.commands == []
        assert desc.contributors == []
        assert desc.default_permission == PermissionDefault.OP
        assert desc.depend == []
        assert desc.load == PluginLoadOrder.POSTWORLD
        assert desc.load_before == []
        assert desc.permissions == []
        assert desc.provides == []
        assert desc.soft_depend == []
        assert desc.website == ""

    def test_every_keyword_is_reported(self):
        desc = PluginDescription(
            name="full",
            version="2.0",
            description="what it does",
            load=PluginLoadOrder.STARTUP,
            authors=["a1", "a2"],
            contributors=["c1"],
            website="https://example.test",
            prefix="Full",
            provides=["svc"],
            depend=["dep1", "dep2"],
            soft_depend=["soft"],
            load_before=["later"],
            default_permission=PermissionDefault.TRUE,
            permissions=[Permission("full.perm", "pd", PermissionDefault.FALSE)],
        )
        assert desc.name == "full"
        assert desc.version == "2.0"
        assert desc.full_name == "full v2.0"
        assert desc.description == "what it does"
        assert desc.load == PluginLoadOrder.STARTUP
        assert desc.authors == ["a1", "a2"]
        assert desc.contributors == ["c1"]
        assert desc.website == "https://example.test"
        assert desc.prefix == "Full"
        assert desc.provides == ["svc"]
        assert desc.depend == ["dep1", "dep2"]
        assert desc.soft_depend == ["soft"]
        assert desc.load_before == ["later"]
        assert desc.default_permission == PermissionDefault.TRUE
        assert [(p.name, p.description, p.default) for p in desc.permissions] == [
            ("full.perm", "pd", PermissionDefault.FALSE)
        ]

    def test_load_phases_are_distinct(self):
        assert PluginDescription("a", "1", load=PluginLoadOrder.STARTUP).load == PluginLoadOrder.STARTUP
        assert PluginDescription("a", "1", load=PluginLoadOrder.POSTWORLD).load == PluginLoadOrder.POSTWORLD

    def test_getters_are_read_only(self):
        desc = PluginDescription("a", "1")
        for attr in ("api_version", "commands", "contributors", "default_permission", "depend", "load", "website"):
            with pytest.raises(AttributeError):
                setattr(desc, attr, None)

    def test_the_loaded_plugin_description(self, server: Server, plugin: Plugin):
        desc = plugin._get_description()
        assert desc.name == plugin.name == "test"
        assert desc.api_version == plugin.api_version == "0.12"
        assert desc.prefix == "Test"
        assert desc.load == PluginLoadOrder.POSTWORLD
        assert desc.default_permission == PermissionDefault.OP
        assert desc.depend == [] and desc.soft_depend == [] and desc.load_before == [] and desc.provides == []
        assert desc.contributors == []
        assert isinstance(desc.website, str)
        command_names = {c.name for c in desc.commands}
        assert {"test", "testoptmsg"} <= command_names
        permissions = {p.name: p for p in desc.permissions}
        assert permissions["endstone_test.command.test"].default == PermissionDefault.OP
        assert permissions["endstone_test.granted"].default == PermissionDefault.FALSE


class TestService:
    def test_construct(self):
        assert isinstance(Service(), Service)
        assert Service() is not Service()

    def test_subclass_is_loadable_by_name(self, server: Server, plugin: Plugin):
        class Provider(Service):
            def ping(self) -> str:
                return "pong"

        manager = server.service_manager
        name = unique("apiforms.service")
        first = Provider()
        second = Provider()
        assert manager.load(name) is None
        manager.register(name, first, plugin, ServicePriority.LOW)
        try:
            assert manager.load(name) is first
            manager.register(name, second, plugin, ServicePriority.HIGH)
            assert manager.load(name) is second
            assert manager.load(name).ping() == "pong"
            manager.unregister(name, second)
            assert manager.load(name) is first
        finally:
            manager.unregister(first)
        assert manager.load(name) is None
