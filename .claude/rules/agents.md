# Agents

- Each team in `.claude/agents/<team>/` has an opus lead that plans the task, dispatches its members and reviews their work.
- A team's own rules live in `.claude/rules/<team>/`; rules every team follows sit directly in `.claude/rules/`.

| Team | Lead | Members | Skills |
| --- | --- | --- | --- |
| Product | `product-lead` | `product-designer`, `technical-writer`, `project-manager` | `design-feature`, `plan-roadmap`, `track-roadmap` |
| Dev | `dev-lead` | `api-developer`, `devops-engineer` | `implement-api`, `fix-issue`, `package-dependency`, `build-image` |
| RE | `re-lead` | `reverse-engineer`, `bedrock-developer` | `upgrade-bds`, `analyze-profile` |
| QA | `qa-lead` | `test-engineer`, `code-reviewer` | `verify-change`, `validate-change`, `review-pr` |

- Product owns the plugin API in C++ and Python, the server-admin UX, the roadmap on GitHub project #1 and the CHANGELOG; dev builds what product approved and owns the Docker images, CI, wheels and recipes; RE owns BDS binaries, symbols and `src/bedrock`; QA proves and reviews.
- Give a big task to its team's lead.
- Each team adds its CHANGELOG line when it lands a change; `product-lead` owns the wording and curates `[Unreleased]` before a release.
- `release-version` runs in the main session only when the user invokes it, handing the CHANGELOG curation to `product-lead`, the pre-release runs to `qa-lead` and the publish checks to `devops-engineer`.
- Hand cross-team work back to the main session, which passes it to the next lead; a lead never spawns another lead.
- Work out a cross-team plan, such as the roadmap or a release decision, with every lead: each team writes its input, the owning lead drafts, and the other leads review the draft before the user sees it.
- A lead may dispatch another team's member for one job, such as a single `reverse-engineer` lookup for `dev-lead`.
- Members are leaf workers and have no `Agent` tool.
- Members run on their frontmatter model; a lead passes `model: opus` for a job with no recipe where a wrong answer is silent, and `model: haiku` for a mechanical one.
- Brief each member with the goal, the files it owns, what is ruled out and the rule files to read; give each file one writer.
- Review each member's result against its brief before it counts as done, and send fixes back to the same member with SendMessage.
- Rerun an `escalate` item on opus, or take it to the user.
- Run at most three members per lead at once; the session caps concurrent subagents at 20, nested ones included.
- Run one `reverse-engineer` per idalib worker.
- Never SendMessage an agent a Workflow runs; it forks a second copy.
- No agent commits, pushes or tags unless the user asked in this conversation.
