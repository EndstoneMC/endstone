from __future__ import annotations

from typing import Callable

from .custom_chart import CustomChart


class AdvancedPie(CustomChart):
    def __init__(self, chart_id: str, get_values: Callable[[], dict[str, int] | None]) -> None:
        """
        Class constructor.

        Args:
            chart_id (str): The id of the chart.
            get_values (Callable[[], dict[str, int] | None]): The callable which is used to request the chart data.
        """

        super().__init__(chart_id)
        self.get_values = get_values

    def get_chart_data(self) -> dict | None:
        """
        Gets the data for the advanced pie chart.

        Returns:
            dict | None: A dictionary with the chart data.
        """

        values = {}
        map_values = self.get_values()
        if not map_values:
            return None

        for key, value in map_values.items():
            if value == 0:
                continue
            values[key] = value

        if not values:
            return None

        return {"values": values}
