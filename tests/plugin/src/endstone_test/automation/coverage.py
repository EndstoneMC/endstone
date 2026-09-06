from __future__ import annotations

from .manifest import EVENT_MANIFEST


def build_coverage(runner, *, final: bool) -> dict[str, object]:
    counts = runner.recorder.snapshot_counts()
    category_entries = {
        category: entries
        for category, entries in EVENT_MANIFEST.items()
        if category != "version"
    }
    
    categories: dict[str, dict[str, object]] = {}
    manifest_names: set[str] = set()
    duplicate_names: set[str] = set()
    executable_uncovered: list[str] = []
    for category, entries in category_entries.items():
        names = list(entries)
        for name in names:
            if name in manifest_names:
                duplicate_names.add(name)
            manifest_names.add(name)
    
        scoped_names = names
        observed = [name for name in scoped_names if counts.get(name, 0) > 0]
        uncovered = [name for name in scoped_names if counts.get(name, 0) == 0]
        executable = bool(scoped_names) and category != "no_fire_site_or_unsupported"
        if executable:
            executable_uncovered.extend(uncovered)
    
        category_report: dict[str, object] = {
            "eligible": executable,
            "total": len(scoped_names),
            "observed": observed,
            "uncovered": uncovered if executable else [],
        }
        if isinstance(entries, dict):
            category_report["reasons"] = {
                name: reason for name, reason in entries.items() if name in scoped_names
            }
        categories[category] = category_report
    
    scenario_names = sorted(
        {
            event_name
            for scenario in runner._scenarios
            for event_name in scenario.expected_events
        }
    )
    unmanifested_scenario_events = [
        name for name in scenario_names if name not in manifest_names
    ]
    observed_unmanifested_events = sorted(
        name
        for name, count in counts.items()
        if count > 0 and name not in manifest_names
    )
    manifest_errors = {
        "duplicate_events": sorted(duplicate_names),
        "scenario_events_missing_from_manifest": unmanifested_scenario_events,
        "observed_events_missing_from_manifest": observed_unmanifested_events,
    }
    scenario_summary = {
        "total": len(runner._scenarios),
        "completed": len(runner._outcomes),
        "passed": sum(outcome.passed for outcome in runner._outcomes),
        "failed": sum(not outcome.passed for outcome in runner._outcomes),
        "unrun": max(0, len(runner._scenarios) - len(runner._outcomes)),
    }
    disabled_categories = [
        category
        for category, report in categories.items()
        if category != "no_fire_site_or_unsupported"
        and report["total"]
        and not report["eligible"]
    ]
    selected_run = runner._selected_scenario_names is not None
    complete = (
        final
        and (selected_run or not executable_uncovered)
        and not disabled_categories
        and not any(manifest_errors.values())
        and scenario_summary["failed"] == 0
        and scenario_summary["unrun"] == 0
    )
    return {
        "manifest_version": EVENT_MANIFEST["version"],
        "scope": "selected" if selected_run else "complete",
        "status": (
            "selected" if complete and selected_run else
            "complete" if complete else
            "incomplete" if final else "in_progress"
        ),
        "player_driver": "bedrock-protocol-1.26.45",
        "categories": categories,
        "executable_total": sum(
            int(category["total"])
            for name, category in categories.items()
            if category["eligible"]
        ),
        "executable_observed": sum(
            len(category["observed"])
            for name, category in categories.items()
            if category["eligible"]
        ),
        "executable_uncovered": sorted(set(executable_uncovered)),
        "disabled_categories": disabled_categories,
        "observed_unmanifested_events": observed_unmanifested_events,
        "manifest_errors": manifest_errors,
        "scenarios": scenario_summary,
    }
