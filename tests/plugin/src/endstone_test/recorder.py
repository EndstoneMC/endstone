import pytest
from endstone import ColorFormat
from endstone.boss import BarColor, BarStyle, BossBar
from endstone.event import Event

MAX_SNAPSHOTS = 8


class EventRecorder:
    def __init__(self, plugin):
        self.plugin = plugin
        self.boss_bar: BossBar | None = None
        self._counts: dict[str, int] = {}
        self._snapshots: dict[str, list[dict]] = {}
        self._hints: dict[str, str] = {}
        self._order: list[str] = []

    def create_boss_bar(self) -> None:
        self.boss_bar = self.plugin.server.create_boss_bar(
            "", BarColor.GREEN, BarStyle.SEGMENTED_10
        )
        self._refresh_boss_bar()

    def remove_boss_bar(self) -> None:
        if self.boss_bar is not None:
            self.boss_bar.remove_all()
            self.boss_bar = None

    def expect(self, event_cls: type[Event]) -> None:
        name = event_cls.__name__
        if name not in self._counts:
            self._order.append(name)
        self._counts.setdefault(name, 0)
        self._snapshots.setdefault(name, [])

    def expect_check(self, key: str, hint: str) -> None:
        if key not in self._counts:
            self._order.append(key)
        self._counts.setdefault(key, 0)
        self._snapshots.setdefault(key, [])
        self._hints[key] = hint

    def checked(self, key: str) -> bool:
        return self._counts.get(key, 0) > 0

    def pass_check(self, key: str, **fields) -> None:
        self._counts[key] = self._counts.get(key, 0) + 1
        self._snapshots.setdefault(key, []).append(fields)
        self.plugin.logger.info(
            ColorFormat.GOLD + f"Check {key} passed! " + ColorFormat.RESET + str(fields)
        )
        self._refresh_boss_bar()
        self.plugin.restock_all()

    def record(
        self, event: Event, summary: str = "", *, always_log: bool = False, **fields
    ) -> None:
        name = type(event).__name__
        first = self._counts.get(name, 0) == 0
        self._counts[name] = self._counts.get(name, 0) + 1

        snapshots = self._snapshots.setdefault(name, [])
        if len(snapshots) < MAX_SNAPSHOTS:
            snapshots.append(fields)

        if first or always_log:
            self.plugin.logger.info(
                ColorFormat.GREEN
                + f"Event {name} triggered! "
                + ColorFormat.RESET
                + summary
            )
        self._refresh_boss_bar()

    def count(self, name: str) -> int:
        return self._counts.get(name, 0)

    def snapshots(self, name: str) -> list[dict]:
        return list(self._snapshots.get(name, []))

    def require(self, name: str) -> list[dict]:
        if name not in self._counts:
            pytest.fail(f"{name} is not tracked by any listener")
        snapshots = self.snapshots(name)
        if not snapshots:
            pytest.skip(f"{name} not yet triggered")
        return snapshots

    @property
    def tracked(self) -> list[str]:
        return sorted(self._counts)

    @property
    def triggered(self) -> list[str]:
        return sorted(name for name, count in self._counts.items() if count)

    @property
    def missing(self) -> list[str]:
        return [name for name in self._order if not self._counts.get(name)]

    def summary(self) -> str:
        return f"{len(self.triggered)}/{len(self._counts)} items exercised"

    def _refresh_boss_bar(self) -> None:
        if self.boss_bar is None:
            return

        total = len(self._counts)
        if not total:
            return

        triggered = len(self.triggered)
        self.boss_bar.progress = triggered / total

        missing = self.missing
        if missing:
            hint = self._hints.get(missing[0])
            suffix = f" ({hint})" if hint else ""
            self.boss_bar.title = (
                f"Events: {triggered}/{total}, Next: {missing[0]}{suffix}"
            )
        else:
            self.boss_bar.title = "All events and checks exercised!"
