from endstone_test.harness import Context, Skip, scenario
from endstone_test.harness.pytest_pass import PytestOutcome, run_pytest, run_repo_tests

NO_TESTS_COLLECTED = 5
REPO_TESTS_EXCLUDED: dict[str, str] = {
    f"tests/test_nbt.py::test_dump_load_roundtrip[{case}-True-big]": "skips by design outside the server too: "
    "network format is always little-endian"
    for case in (
        "byte",
        "short",
        "int",
        "long",
        "float",
        "double",
        "string",
        "byte_array",
        "int_array",
        "list",
        "empty_list",
        "compound",
        "empty_compound",
    )
}


def pytest_pass(ctx: Context, marker: str, repo_tests: bool = False, **context) -> PytestOutcome:
    options = ctx.driver.pytest_options
    narrowed = bool(options.get("targets") or options.get("args"))
    outcome = run_pytest(
        ctx.plugin,
        marker,
        options.get("targets"),
        options.get("args"),
        plot=ctx.plot,
        **context,
    )
    if repo_tests and not narrowed and options.get("repo_tests"):
        repo = run_repo_tests(ctx.plugin, options["repo_tests"], REPO_TESTS_EXCLUDED)
        ctx.info["repo_tests"] = {**repo.summary(), "folder": options["repo_tests"], "excluded": REPO_TESTS_EXCLUDED}
        outcome = outcome.merged(repo)
    ctx.info.update(outcome.summary())
    if narrowed and outcome.exit_code == NO_TESTS_COLLECTED:
        raise Skip(f"none of the selected tests are in the {marker!r} pass")
    assert outcome.exit_code == 0, (
        f"pytest exited with {outcome.exit_code}: {outcome.failed}"
    )
    return outcome


@scenario("pytest.server", plot=True, budget=10)
async def server_suite(ctx: Context) -> None:
    pytest_pass(ctx, "not player", repo_tests=True)


@scenario("pytest.player", plot=True, budget=1200)
async def player_suite(ctx: Context) -> None:
    targets = ctx.driver.pytest_options.get("targets") or []
    if targets and not any(t.startswith("endstone_test.tests.player") for t in targets):
        raise Skip("none of the selected tests are in the 'player' pass")
    await ctx.bot("PytestBot")
    player = await ctx.until(
        lambda: ctx.server.get_player("PytestBot"), timeout=600, what="the bot to join"
    )
    pytest_pass(ctx, "player", player=player)
