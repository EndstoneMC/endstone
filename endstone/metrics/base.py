import asyncio
import concurrent.futures
import gzip
import json
import logging
import random
import uuid
from abc import ABC, abstractmethod
from typing import Any, Callable, Dict, Set, final

import aiohttp

import endstone.asyncio
from endstone.metrics.charts.custom_chart import CustomChart


class MetricsBase(ABC):
    """
    The MetricsBase class to handle sending metrics to bStats.

    Attributes:
        METRICS_VERSION (str): The version of the Metrics class.
        REPORT_URL (str): The URL to which data is reported.
    """

    METRICS_VERSION = "3.0.3"
    REPORT_URL = "https://bStats.org/api/v2/data/{platform}"

    def __init__(
        self,
        platform: str,
        server_uuid: uuid.UUID,
        service_id: int,
        log_errors: bool,
        log_sent_data: bool,
        log_response_status_text: bool,
    ):
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
        self._platform = platform
        self._server_uuid = server_uuid
        self._service_id = service_id
        self._log_errors = log_errors
        self._log_sent_data = log_sent_data
        self._log_response_status_text = log_response_status_text
        self._custom_charts: Set[CustomChart] = set()
        self._future: concurrent.futures.Future[Any] | None = None
        self._send_futures: Set[concurrent.futures.Future[Any]] = set()
        self._shutdown = False

        if self.enabled:
            submitting = self._start_submitting()
            try:
                self._future = endstone.asyncio.submit(submitting)
            except Exception:
                submitting.close()
                raise
            self._future.add_done_callback(self._submission_done)

    def _submission_done(self, future: concurrent.futures.Future[Any]) -> None:
        if self._shutdown:
            return
        try:
            future.result()
        except concurrent.futures.CancelledError:
            return
        except Exception as e:
            if self._log_errors:
                self.log_error("Metrics submission task stopped unexpectedly", e)

    def shutdown(self) -> None:
        if self._shutdown:
            return
        self._shutdown = True
        future = self._future
        self._future = None
        if future is not None:
            future.cancel()
        for future in tuple(self._send_futures):
            future.cancel()
        self._send_futures.clear()
        self._custom_charts.clear()

    @property
    @abstractmethod
    def enabled(self) -> bool:
        """
        Whether data sending is enabled.
        """

    @property
    @abstractmethod
    def service_enabled(self) -> bool:
        """
        Whether the service is enabled.
        """

    def append_platform_data(self, data: Dict[str, Any]) -> None:
        """
        Append platform-specific data.

        Args:
            data (Dict[str, Any]): The data to append platform-specific values to.
        """
        pass

    def append_service_data(self, data: Dict[str, Any]) -> None:
        """
        Append service-specific data.

        Args:
            data (Dict[str, Any]): The data to append service-specific values to.
        """
        pass

    def submit_task(self, task: Callable[[], None]) -> None:
        """
        Submit the given task

        Args:
            task (Callable[[], None]): The task to be submitted.
        """
        if not self._shutdown:
            task()

    def log_info(self, message: str) -> None:
        """
        Logs info message.

        Args:
            message (str): The info message.
        """
        if not self._shutdown:
            logging.info(message)

    def log_error(self, message: str, exception: Exception) -> None:
        """
        Logs error message.

        Args:
            message (str): The error message.
            exception (Exception): The exception that occurred.
        """
        if not self._shutdown:
            logging.warning(message, exc_info=exception)

    @final
    def add_custom_chart(self, chart: CustomChart) -> None:
        """
        Adds a custom chart.

        Args:
            chart: The custom chart to add.
        """
        if self._shutdown:
            raise RuntimeError("metrics has been shut down")
        self._custom_charts.add(chart)

    @final
    async def _start_submitting(self) -> None:
        """
        Starts the submitting process with initial and periodic delays.
        """

        initial_delay = int((3 + random.random() * 3) * 60)
        second_delay = int((random.random() * 30) * 60)

        await asyncio.sleep(initial_delay)
        if self._shutdown or not (self.enabled and self.service_enabled):
            return

        self.submit_task(self._submit_data)
        await asyncio.sleep(second_delay)

        while not self._shutdown and self.enabled and self.service_enabled:
            self.submit_task(self._submit_data)
            await asyncio.sleep(30 * 60)

    @final
    def _submit_data(self) -> None:
        """
        Constructs the JSON data and sends it to bStats.
        """

        if self._shutdown:
            return

        platform_data: dict[str, Any] = {}
        self.append_platform_data(platform_data)

        service_data: dict[str, Any] = {}
        self.append_service_data(service_data)

        chart_data = []
        for chart in self._custom_charts:
            try:
                chart_json = chart._get_request_json_object()  # noqa
                if chart_json is not None:
                    chart_data.append(chart_json)
            except Exception as e:
                if not self._shutdown and self._log_errors:
                    self.log_error(
                        f"Failed to get data for custom chart with id {chart.chart_id}",
                        e,
                    )

        service_data["id"] = self._service_id
        service_data["customCharts"] = chart_data
        platform_data["service"] = service_data
        platform_data["serverUUID"] = str(self._server_uuid)

        def send_callback(fut: concurrent.futures.Future[Any]) -> None:
            self._send_futures.discard(fut)
            try:
                fut.result()
            except concurrent.futures.CancelledError:
                return
            except Exception as e:
                if not self._shutdown and self._log_errors:
                    self.log_error("Could not submit bStats metrics data", e)

        sending = self._send_data(platform_data)
        try:
            future = endstone.asyncio.submit(sending)
        except Exception as e:
            sending.close()
            if not self._shutdown and self._log_errors:
                self.log_error("Could not submit bStats metrics data", e)
            return
        if self._shutdown:
            future.cancel()
            return
        self._send_futures.add(future)
        future.add_done_callback(send_callback)

    @final
    async def _send_data(self, data: Dict[str, Any]) -> None:
        """
        Sends the JSON data to bStats.

        Args:
            data: The JSON data to send.
        """
        if self._shutdown:
            return

        if self._log_sent_data and not self._shutdown:
            self.log_info(f"Sent bStats metrics data: {data}")

        url = self.REPORT_URL.format(platform=self._platform)
        compressed_data = gzip.compress(json.dumps(data).encode("utf-8"))

        headers = {
            "Accept": "application/json",
            "Connection": "close",
            "Content-Encoding": "gzip",
            "Content-Length": str(len(compressed_data)),
            "Content-Type": "application/json",
            "User-Agent": "Metrics-Service/1",
        }

        async with aiohttp.ClientSession() as session:
            async with session.post(url, data=compressed_data, headers=headers) as resp:
                resp.raise_for_status()
                if self._log_response_status_text and not self._shutdown:
                    text = await resp.text()
                    if not self._shutdown:
                        self.log_info(f"Sent data to bStats and received response: {text}")
