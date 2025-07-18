LLM Todo List — Requirement Specification

1 Purpose

Provide a concise, CLI-only todo-list application whose entire lifecycle (requirements → design → implementation → tests → containerisation) can be co-created with a large-language model (LLM). The document formalises the functional and non-functional requirements so that an LLM can reliably generate and iteratively refine the C++20 code-base.

2 Scope
	•	In scope — command-line interaction, local JSON persistence, unit tests, cross-platform (Linux & macOS) builds via CMake, and a Docker image for repeatable execution.
	•	Out of scope — graphical UI, remote/network features, sync across devices, user authentication.

3 Stakeholders

Role	Interest
End-user	Add/list/complete tasks quickly from terminal.
Developer	Use LLM to scaffold, extend and refactor code.
QA / Reviewer	Verify feature completeness & reliability.

4 Definitions & Abbreviations

Term	Definition
Task	A record with an id, description, optional tags, created_at, done, done_at.
CLI	Command-Line Interface.
Store	JSON file that persists all tasks.

5 System Overview

The program exposes a single binary llm_todo that operates on a JSON store (default: ~/.llm_todo.json). Each invocation performs one command (sub-command style) and exits with an appropriate status code.

6 Functional Requirements

6.1 Commands

Command	Parameters / Flags	Behaviour
add	<description> [-t, --tag TAG]…	Append a new task; prints its numeric id.
list	[--all | --pending | --done]	Display tasks filtered by status.
done	<id>	Mark task as completed; idempotent.
rm	<id>	Remove task permanently.
edit (stretch)	<id> --desc NEW_DESC --tag TAGS…	Modify description/tags.
search (stretch)	<pattern> [--regex]	Fuzzy/regex search in descriptions & tags.
export (stretch)	[--output FILE] [--format json | csv]	Export tasks.

6.2 ID allocation
	•	Incremental, starts at 1 and never re-uses ids (even if tasks are removed).

6.3 Persistence
	•	JSON Schema v1 (flexible for future migration).
	•	File created lazily; safe write using temp file + atomic rename.

6.4 Exit codes

Code	Meaning
0	Success
1	Recoverable user error
2	Unrecoverable/internal error

7 Non-Functional Requirements

Area	Requirement
Language	Modern C++20 with no proprietary extensions.
Dependencies	Header-only libraries allowed (e.g. cxxopts, nlohmann/json, Catch2).
Build	Cross-platform via CMake ≥3.20; RelWithDebInfo default; clang-tidy optional target.
Performance	list of 10 000 tasks completes < 50 ms on mid-2020 laptop.
Threading	Single-threaded; no data races.
Error logging	Human-readable messages to stderr.
Internationalisation	UTF-8 throughout; no localisation of messages required.

8 Quality & Testing
	•	Unit tests: ≥90 % line coverage on core logic, written with Catch2.
	•	Fuzz tests (stretch): random command sequences against an in-memory store.
	•	CI example: GitHub Actions matrix {ubuntu-latest, macos-latest} running cmake, ctest, clang-tidy.

9 Security Considerations
	•	Reject task descriptions larger than 10 kB.
	•	Use std::filesystem::path sanitation to prevent path traversal when --store overrides default.

10 Deployment & Containerisation
	•	Provide multi-stage Dockerfile:
	1.	Build stage (Ubuntu 24.04, clang, cmake).
	2.	Runtime stage (distroless or ubuntu:24.04); copy binary & default store path.
	•	Image size target < 50 MB compressed.

11 Deliverables
	1.	Compilable source tree (see §12 folder layout).
	2.	Dockerfile.
	3.	README with usage examples & build instructions.
	4.	Optional: architecture diagram (ASCII or PlantUML).

12 Suggested Repository Layout

llm_todo/
 ├─ CMakeLists.txt
 ├─ include/llm_todo/…
 ├─ src/…
 ├─ tests/…
 ├─ third_party/…
 ├─ docker/Dockerfile
 └─ README.md

13 Acceptance Criteria
	•	All mandatory commands behave as specified.
	•	Unit tests pass with ctest.
	•	docker run llm_todo --help prints usage.
	•	Code passes clang-tidy with zero critical warnings.

14 Evolution & Stretch Goals
	•	Task recurrence (every Monday, daily), reminders.
	•	YAML or SQLite back-end.
	•	Interactive TUI using ncurses (explicitly out of current scope).

⸻

End of Document
