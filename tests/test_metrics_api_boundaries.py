from pathlib import Path

ROOT = Path(__file__).parents[1]
PUBLIC_METRICS = ROOT / "include" / "endstone" / "metrics"


def test_metrics_public_headers_do_not_leak_backend_types() -> None:
    forbidden = ("pybind11", "Python.h", "aiohttp", "endstone/core", "bedrock", "std::mutex", "std::shared_mutex")
    headers = sorted(PUBLIC_METRICS.glob("*.h"))
    assert headers

    for header in headers:
        text = header.read_text(encoding="utf-8")
        assert not any(token in text for token in forbidden), header


def test_server_exposes_the_metrics_factory() -> None:
    server_header = (ROOT / "include" / "endstone" / "server.h").read_text(encoding="utf-8")
    assert "createMetrics" in server_header
    assert "MetricsBase" in server_header
