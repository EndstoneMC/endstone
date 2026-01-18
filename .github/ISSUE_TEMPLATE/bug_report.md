---
name: Bug report
about: Create a report to help us improve Endstone
type: bug
assignees: ''

---

## Bug Description
A clear and concise description of what the bug is.

## Is this a crash?
- [ ] Yes, the server/plugin crashed
- [ ] No, this is a runtime/behavioral issue

## Environment Information
**Please complete the following information:**

- **Minecraft Bedrock Version**: [e.g. v1.21.50]
- **Endstone Version**: [e.g. v0.10.0]
- **Installation Method**: [pip / Docker / Built from source]
- **Operating System**: [e.g. Windows 11, Ubuntu 22.04]
- **Python Version**: [e.g. Python 3.12.0]
- **Plugin Type**: [Python / C++]
- **Plugin Version/Name**: [if applicable]

## Steps to Reproduce
Steps to reproduce the behavior:
1. Run the command '...'
2. Install plugin '...'
3. Join the server with '...'
4. See error

## Expected Behavior
A clear and concise description of what you expected to happen.

## Actual Behavior
What actually happened? Include any error messages or unexpected behavior.

## Logs & Stack Traces
**Please attach the following:**
- Endstone server logs (from `logs/` directory if applicable)
- Full error message or stack trace
- Crash dump (from `crash_reports/` directory if server crashed)

```log
Paste relevant log output here
```


## Minimal Reproduction Plugin
If possible, provide a minimal plugin code that reproduces the issue:

```python
# or C++ code
from endstone.plugin import Plugin

class ReproductionPlugin(Plugin):
    api_version = "0.10"

    def on_enable(self):
        # Code that triggers the bug
        pass
```

## Additional Context
- [ ] I have checked for existing issues that describe this problem
- [ ] I can reproduce this issue consistently
- [ ] This issue occurs on a fresh server install

Add any other context about the problem here, such as:
- Which events are involved
- Whether this is a regression (worked in previous versions)
- Workarounds you've found
- Related issues or pull requests
