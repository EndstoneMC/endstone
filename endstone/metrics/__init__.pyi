"""
Classes relating to the bStats metrics charts.
"""

import collections.abc

from endstone import JsonObject

__all__ = [
    "AdvancedBarChart",
    "AdvancedPie",
    "CustomChart",
    "DrilldownPie",
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

    def get_chart_data(self) -> JsonObject | None:
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
