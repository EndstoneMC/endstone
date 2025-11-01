from __future__ import annotations

from abc import ABC, abstractmethod
from typing import final

from endstone_bstats._errors import ChartDataError


class CustomChart(ABC):
    """
    Represents a custom chart.
    """

    def __init__(self, chart_id: str) -> None:
        """
        Class constructor.

        Args:
            chart_id (str): The id of the chart.

        Raises:
            ValueError: If chart_id is None or empty.
        """
        if not chart_id:
            raise ValueError("chart_id cannot be None or empty!")

        self.chart_id = chart_id

    @final
    def _get_request_json_object(self) -> dict | None:
        """
        Gets the JSON object for the chart request.

        Returns:
            dict: A dictionary representing the chart, or None if the data is invalid or an exception occurs.

        Raises:
            ChartDataError: If there is an error getting the chart data.
        """
        chart = {"chartId": self.chart_id}
        try:
            data = self.get_chart_data()
            if not data:  # If the data is empty we don't send the chart.
                return None

            chart["data"] = data

        except Exception as e:
            raise ChartDataError(f"Failed to get data for custom chart with id {self.chart_id}") from e

        return chart

    @abstractmethod
    def get_chart_data(self) -> dict | None:
        """
        Gets the data for the chart.

        Returns:
            dict: A dictionary with the chart data.

        Raises:
            Exception: If there is an error getting the chart data.
        """
