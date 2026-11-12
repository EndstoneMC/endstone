# Comments

- Write no comments unless the user explicitly asks for one, in new code and in code you touch.
- Leave existing comments alone unless your change makes them wrong.
- Put reasoning, evidence and format decoding in the commit message, never in the code.
- Never leave LLM notes: comments that explain why a change was made, reference the development process, or restate what the code does.
- A requested comment is one short line, matching the density of the surrounding or original code; a port stays as terse as its upstream.
- No parenthetical asides, `(e.g. ...)`, semicolon-joined clauses, or trailing comments that explain the line.
- Name concrete things in a comment (a player, a console, a rented server), never abstract restatement.
- Do not write naming-convention comments, `BDS: <Type>` comments, or `// +<offset>` field comments.
- Delete a comment block together with the dead code it describes, including the blank line above.
- Mark invented names in `src/bedrock` with `// TODO(fixme): check the name` on the line above.
- Mark Endstone-invented or Endstone-modified declarations in `src/bedrock` with `// Endstone`, as in `// Endstone: private -> public`; faithful BDS redeclarations get no marker.
- Mark a truncated `src/bedrock` struct with `// ...` where the omitted trailing members would be.
- Config comments (`endstone.default.toml` and friends) say what the setting does; Conan recipes and CMake carry none.
- Locate recipes above `scripts/configs/*.toml` entries are required and follow `re/symbols.md`.
