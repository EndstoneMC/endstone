from __future__ import annotations

from typing import Callable

from endstone_bstats._charts.custom_chart import CustomChart


class SimplePie(CustomChart):
    def __init__(self, chart_id: str, get_value: Callable[[], str | None]) -> None:
        """
        Class constructor.

        Args:
            chart_id (str): The id of the chart.
            get_value (Callable[[], str | None]): The callable which is used to request the chart data.
        """

        super().__init__(chart_id)
        self.get_value = get_value

    def get_chart_data(self) -> dict | None:
        """
        Gets the data for the simple pie chart.

        Returns:
            dict | None: A dictionary with the chart data.
        """

        value = self.get_value()
        if not value:
            return None  # skip the chart

        return {"value": value}
