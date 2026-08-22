"""
Classes relating to the bStats metrics charts.
"""

import abc
import collections.abc
import pathlib
import typing
import uuid

from endstone.plugin import Plugin

__all__ = [
    "AdvancedBarChart",
    "AdvancedPie",
    "CustomChart",
    "DrilldownPie",
    "Metrics",
    "MetricsBase",
    "MetricsConfig",
    "MultiLineChart",
    "SimpleBarChart",
    "SimplePie",
    "SingleLineChart",
]

class CustomChart:
    """
    Represents a custom chart.

    Subclass this to report a chart shape bStats supports but Endstone does not wrap.
    """
    def __init__(self, chart_id: str) -> None:
        """
        Creates a chart with the given bStats chart id.

        Args:
            chart_id: The id of the chart.

        Raises:
            ValueError: If `chart_id` is empty.
        """

    @property
    def chart_id(self) -> str:
        """
        The id of this chart.
        """

    def get_chart_data(self) -> typing.Any:
        """
        Gets the data for this chart.

        This is called on the primary server thread. Returning `None` omits the chart from the submission.

        Returns:
            The chart data, or `None` to skip this chart.
        """

class SimplePie(CustomChart):
    """
    A bStats pie chart with a single value.
    """
    def __init__(self, chart_id: str, get_value: collections.abc.Callable[[], str | None]) -> None:
        """
        Creates a simple pie chart.

        Args:
            chart_id: The id of the chart.
            get_value: The callable which is used to request the chart data.
        """

class AdvancedPie(CustomChart):
    """
    A bStats pie chart with named values.
    """
    def __init__(self, chart_id: str, get_values: collections.abc.Callable[[], dict[str, int] | None]) -> None:
        """
        Creates an advanced pie chart.

        Args:
            chart_id: The id of the chart.
            get_values: The callable which is used to request the chart data.
        """

class DrilldownPie(CustomChart):
    """
    A bStats pie chart with grouped named values.
    """
    def __init__(
        self, chart_id: str, get_values: collections.abc.Callable[[], dict[str, dict[str, int]] | None]
    ) -> None:
        """
        Creates a drilldown pie chart.

        Args:
            chart_id: The id of the chart.
            get_values: The callable which is used to request the chart data.
        """

class SimpleBarChart(CustomChart):
    """
    A bStats bar chart with a single bar per category.
    """
    def __init__(self, chart_id: str, get_values: collections.abc.Callable[[], dict[str, int] | None]) -> None:
        """
        Creates a simple bar chart.

        Args:
            chart_id: The id of the chart.
            get_values: The callable which is used to request the chart data.
        """

class AdvancedBarChart(CustomChart):
    """
    A bStats bar chart with several bars per category.
    """
    def __init__(self, chart_id: str, get_values: collections.abc.Callable[[], dict[str, list[int]] | None]) -> None:
        """
        Creates an advanced bar chart.

        Args:
            chart_id: The id of the chart.
            get_values: The callable which is used to request the chart data.
        """

class SingleLineChart(CustomChart):
    """
    A bStats line chart with a single line.
    """
    def __init__(self, chart_id: str, get_value: collections.abc.Callable[[], int]) -> None:
        """
        Creates a single line chart.

        Args:
            chart_id: The id of the chart.
            get_value: The callable which is used to request the chart data.
        """

class MultiLineChart(CustomChart):
    """
    A bStats line chart with a line per named value.
    """
    def __init__(self, chart_id: str, get_values: collections.abc.Callable[[], dict[str, int] | None]) -> None:
        """
        Creates a multi line chart.

        Args:
            chart_id: The id of the chart.
            get_values: The callable which is used to request the chart data.
        """

class MetricsBase(abc.ABC):
    """
    The MetricsBase class to handle sending metrics to bStats.

    Attributes:
        METRICS_VERSION (str): The version of the Metrics class.
        REPORT_URL (str): The URL to which data is reported.
    """

    METRICS_VERSION: str = "3.0.3"
    REPORT_URL: str = "https://bStats.org/api/v2/data/{platform}"
    def __init__(
        self,
        platform: str,
        server_uuid: uuid.UUID,
        service_id: int,
        log_errors: bool,
        log_sent_data: bool,
        log_response_status_text: bool,
    ) -> None:
        """
        Initializes the MetricsBase instance.

        Args:
            platform (str): The platform of the service.
            server_uuid (uuid.UUID): The server UUID.
            service_id (int): The service ID.
            log_errors (bool): Whether errors should be logged.
            log_sent_data (bool): Whether the data sent should be logged.
            log_response_status_text (bool): Whether the response status text should be logged.
        """

    def shutdown(self) -> None: ...
    @property
    def enabled(self) -> bool:
        """
        Whether data sending is enabled.
        """

    @property
    def service_enabled(self) -> bool:
        """
        Whether the service is enabled.
        """

    def append_platform_data(self, data: dict[str, typing.Any]) -> None:
        """
        Append platform-specific data.

        Args:
            data (dict[str, typing.Any]): The data to append platform-specific values to.
        """

    def append_service_data(self, data: dict[str, typing.Any]) -> None:
        """
        Append service-specific data.

        Args:
            data (dict[str, typing.Any]): The data to append service-specific values to.
        """

    def submit_task(self, task: collections.abc.Callable[[], None]) -> None:
        """
        Submit the given task

        Args:
            task (collections.abc.Callable[[], None]): The task to be submitted.
        """

    def log_info(self, message: str) -> None:
        """
        Logs info message.

        Args:
            message (str): The info message.
        """

    def log_error(self, message: str, exception: Exception) -> None:
        """
        Logs error message.

        Args:
            message (str): The error message.
            exception (Exception): The exception that occurred.
        """

    def add_custom_chart(self, chart: CustomChart) -> None:
        """
        Adds a custom chart.

        Args:
            chart: The custom chart to add.
        """

class MetricsConfig:
    """
    A simple config for bStats.
    """
    def __init__(self, file: pathlib.Path, default_enabled: bool) -> None: ...
    @property
    def server_uuid(self) -> uuid.UUID: ...
    @property
    def enabled(self) -> bool: ...
    @property
    def log_errors_enabled(self) -> bool: ...
    @property
    def log_sent_data_enabled(self) -> bool: ...
    @property
    def log_response_status_text_enabled(self) -> bool: ...
    @property
    def did_exist_before(self) -> bool: ...
    def setup_config(self) -> None:
        """
        Creates the config file if it does not exist and read its content.
        """

    def save_default_config(self) -> None:
        """
        Creates a config file with the default content.
        """

    def load_config(self) -> None:
        """
        Reads the content of the config file.
        """

class Metrics(MetricsBase):
    def __init__(self, plugin: Plugin, service_id: int) -> None:
        """
        Creates a new Metrics instance.

        Args:
            plugin (Plugin): Your plugin instance.
            service_id (int): The id of the service.
                              It can be found at https://bstats.org/what-is-my-plugin-id
        """

    @property
    def enabled(self) -> bool: ...
    @property
    def service_enabled(self) -> bool: ...
    def append_platform_data(self, platform_data: dict[str, typing.Any]) -> None:
        """
        Appends platform-specific data to the provided dict.

        Args:
            platform_data (dict[str, typing.Any]): The dict to append data to.
        """

    def append_service_data(self, service_data: dict[str, typing.Any]) -> None:
        """
        Appends service-specific data to the provided dict.

        Args:
            service_data (dict[str, typing.Any]): The dict to append data to.
        """

    def submit_task(self, task: collections.abc.Callable[[], None]) -> None: ...
    def log_info(self, message: str) -> None: ...
    def log_error(self, message: str, exception: Exception) -> None: ...
    def shutdown(self) -> None: ...
