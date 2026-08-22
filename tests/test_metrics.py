import types
import uuid

import pytest

import endstone.asyncio
from endstone.metrics import (
    AdvancedBarChart,
    AdvancedPie,
    CustomChart,
    DrilldownPie,
    Metrics,
    MetricsBase,
    MultiLineChart,
    SimpleBarChart,
    SimplePie,
    SingleLineChart,
)

# One case per rule bStats' own chart classes apply, so the bound C++ charts stay in step with
# https://github.com/Bastian/bstats-metrics/tree/master/base/src/main/java/org/bstats/charts.
CHARTS = [
    (SimplePie, "value", {"value": "value"}),
    (SimplePie, None, None),
    (SimplePie, "", None),
    (AdvancedPie, {"kept": 3, "dropped": 0}, {"values": {"kept": 3}}),
    (AdvancedPie, {"dropped": 0}, None),
    (AdvancedPie, None, None),
    (DrilldownPie, {"kept": {"inner": 1}, "dropped": {}}, {"values": {"kept": {"inner": 1}}}),
    (DrilldownPie, {"dropped": {}}, None),
    (SimpleBarChart, {"kept": 2, "zero": 0}, {"values": {"kept": [2], "zero": [0]}}),
    (SimpleBarChart, {}, None),
    (AdvancedBarChart, {"kept": [1, 2], "dropped": []}, {"values": {"kept": [1, 2]}}),
    (AdvancedBarChart, {"dropped": []}, None),
    (SingleLineChart, 5, {"value": 5}),
    (SingleLineChart, 0, None),
    (MultiLineChart, {"kept": 4, "dropped": 0}, {"values": {"kept": 4}}),
    (MultiLineChart, {"dropped": 0}, None),
]


@pytest.mark.parametrize("chart_type,value,expected", CHARTS)
def test_chart_data_matches_bstats(chart_type, value, expected) -> None:
    """Verify each chart reports what bStats reports, including the values it skips."""
    assert chart_type("chart", lambda: value).get_chart_data() == expected


def test_chart_id_is_reported() -> None:
    assert SimplePie("some_id", lambda: "v").chart_id == "some_id"


def test_empty_chart_id_is_rejected() -> None:
    with pytest.raises(ValueError):
        SimplePie("", lambda: "v")


def test_callback_exceptions_propagate() -> None:
    with pytest.raises(RuntimeError, match="callback failed"):
        SimplePie("chart", _boom).get_chart_data()


def _boom() -> str:
    raise RuntimeError("callback failed")


class PythonChart(CustomChart):
    """A chart written in Python rather than wrapped from C++."""

    def __init__(self, chart_id: str, data) -> None:
        super().__init__(chart_id)
        self._data = data

    def get_chart_data(self):
        return self._data


def test_python_subclass_supplies_its_own_data() -> None:
    """Verify CustomChart can be subclassed in Python, i.e. the binding's trampoline works."""
    chart = PythonChart("mine", {"value": "python", "values": {"a": 1}})
    assert chart.chart_id == "mine"
    assert chart.get_chart_data() == {"value": "python", "values": {"a": 1}}


def test_python_subclass_can_skip_itself() -> None:
    assert PythonChart("mine", None).get_chart_data() is None


class RecordingMetrics(MetricsBase):
    """A real MetricsBase that never starts submitting, so a test can drive one pass by hand."""

    enabled = False
    service_enabled = False

    def __init__(self) -> None:
        super().__init__(
            platform="test",
            server_uuid=uuid.uuid4(),
            service_id=1,
            log_errors=True,
            log_sent_data=False,
            log_response_status_text=False,
        )
        self.errors: list[str] = []

    def append_platform_data(self, data: dict) -> None:
        data["platformKey"] = "platformValue"

    def append_service_data(self, data: dict) -> None:
        data["pluginVersion"] = "1.2.3"

    def log_error(self, message: str, exception: Exception) -> None:
        self.errors.append(message)


class _StubFuture:
    def cancel(self) -> None:
        pass

    def add_done_callback(self, callback) -> None:
        pass


@pytest.fixture
def submitted(monkeypatch):
    """Runs MetricsBase._submit_data for real, capturing the payload instead of sending it."""
    payloads = []
    monkeypatch.setattr(MetricsBase, "_send_data", lambda self, data: payloads.append(data))
    monkeypatch.setattr(endstone.asyncio, "submit", lambda _: _StubFuture())
    return payloads


def test_submission_wraps_each_chart_and_drops_the_empty_ones(submitted) -> None:
    """Verify the envelope bStats expects, from a mix of wrapped and Python-defined charts."""
    metrics = RecordingMetrics()
    metrics.add_custom_chart(SimplePie("wrapped", lambda: "v"))
    metrics.add_custom_chart(PythonChart("subclassed", {"value": "python"}))
    metrics.add_custom_chart(SimplePie("skipped", lambda: None))
    metrics._submit_data()

    assert len(submitted) == 1
    payload = submitted[0]
    assert payload["platformKey"] == "platformValue"
    assert payload["serverUUID"] == str(metrics._server_uuid)
    assert payload["service"]["id"] == 1
    assert payload["service"]["pluginVersion"] == "1.2.3"
    assert sorted(payload["service"]["customCharts"], key=lambda c: c["chartId"]) == [
        {"chartId": "subclassed", "data": {"value": "python"}},
        {"chartId": "wrapped", "data": {"value": "v"}},
    ]
    assert metrics.errors == []


def test_a_failing_chart_is_reported_and_skipped(submitted) -> None:
    """Verify one broken chart neither aborts the submission nor lands in it."""
    metrics = RecordingMetrics()
    metrics.add_custom_chart(SimplePie("broken", _boom))
    metrics.add_custom_chart(SimplePie("fine", lambda: "v"))
    metrics._submit_data()

    assert submitted[0]["service"]["customCharts"] == [{"chartId": "fine", "data": {"value": "v"}}]
    assert metrics.errors == ["Failed to get data for custom chart with id broken"]


class FakePlugin:
    """Stands in for either plugin kind: both answer _get_description()."""

    def __init__(self, description) -> None:
        self._description = description

    def _get_description(self):
        return self._description


def test_service_data_carries_the_plugin_version() -> None:
    """Verify the version is read through the trampoline hook, which a C++ plugin answers too."""
    metrics = Metrics.__new__(Metrics)
    metrics._plugin = FakePlugin(types.SimpleNamespace(version="1.2.3"))
    service_data: dict = {}
    metrics.append_service_data(service_data)
    assert service_data == {"pluginVersion": "1.2.3"}


def test_service_data_reports_a_missing_description() -> None:
    """A Python plugin that was never loaded has no description; say so rather than AttributeError."""
    metrics = Metrics.__new__(Metrics)
    metrics._plugin = FakePlugin(None)
    with pytest.raises(RuntimeError, match="Plugin description is not available"):
        metrics.append_service_data({})
