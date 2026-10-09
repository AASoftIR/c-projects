# C / SYSTEMS FORGE

**From familiar syntax to systems you can explain.** A project-driven C17 course for Alireza: Linux tools, explicit ownership, Persian UTF-8, durable storage, interpreters, networking and a local document-search API. Native Linux. No paid services.

[Start here](START-HERE.md) · [Course route](docs/COURSE.md) · [Engineering field guide](docs/DEEP-DIVES.md) · [Run examples](docs/PROJECT-EXAMPLES.md) · [GitHub setup](docs/GITHUB-SETUP.md) · [Advanced tracks](docs/BIG-PROJECTS.md)

20 baseline projects · 6 stages · 212 estimated focused hours · complete guides, starters, reference solutions and contract tests. The hacker-themed Pages dashboard contains every project, source views, the handbook, a command palette, filters and a private browser journal.

## Run your first mission

```sh
python3 scripts/course.py list
python3 scripts/course.py test 01 --mode reference
# Read projects/01-byte-lab/README.md and implement its main.c.
python3 scripts/course.py test 01
python3 scripts/course.py test 01 --sanitize
```

The starter failing is expected. Check your own work before reading the reference. Tests use strict GCC/Clang compilation, boundary and malformed inputs, deterministic random oracles, real file persistence and live socket requests.

## Verified learner checklist

The managed block is generated from learner tests. Untouched starters are not started; references never count. On main, all four compiler/instrumentation lanes must pass to check an item. A regression unticks it. See [setup and limitations](docs/GITHUB-SETUP.md).

<!-- PROGRESS:START -->
**Verified implementations: 0/20** · Evidence lanes: 0 · Commit: `not-run`

- [ ] [01 · Byte Lab](projects/01-byte-lab/README.md) — not started
- [ ] [02 · Hex Detective](projects/02-hexview/README.md) — not started
- [ ] [03 · Persian UTF-8 Scanner](projects/03-utf8/README.md) — not started
- [ ] [04 · Vector Workshop](projects/04-vector/README.md) — not started
- [ ] [05 · Word Frequency Map](projects/05-wordmap/README.md) — not started
- [ ] [06 · CSV State Machine](projects/06-csv/README.md) — not started
- [ ] [07 · Tiny Grep](projects/07-grep/README.md) — not started
- [ ] [08 · Arena Allocator](projects/08-arena/README.md) — not started
- [ ] [09 · Bookish Reading Ledger](projects/09-ledger/README.md) — not started
- [ ] [10 · Expression Interpreter](projects/10-calc/README.md) — not started
- [ ] [11 · Pocket POSIX Shell](projects/11-shell/README.md) — not started
- [ ] [12 · HTTP Request Inspector](projects/12-http-parse/README.md) — not started
- [ ] [13 · Loopback Health Server](projects/13-http-server/README.md) — not started
- [ ] [14 · Append-Only KV Store](projects/14-kvlog/README.md) — not started
- [ ] [15 · LRU Cache Simulator](projects/15-lru/README.md) — not started
- [ ] [16 · Parallel Checksum Lab](projects/16-workers/README.md) — not started
- [ ] [17 · Stack Bytecode VM](projects/17-vm/README.md) — not started
- [ ] [18 · Local Document Index](projects/18-search/README.md) — not started
- [ ] [19 · Python ↔ C Bridge](projects/19-ffi/README.md) — not started
- [ ] [20 · Document Search API](projects/20-doc-api/README.md) — not started
<!-- PROGRESS:END -->

Open `course-preview.html` directly for the bundled dashboard snapshot.

## Preview the dashboard

```sh
python3 scripts/course.py report --output progress.local.json
python3 scripts/publish.py --reports progress.local.json
python3 -m http.server 8000 --directory site
```

Open http://localhost:8000. Local notes stay in your browser and can be exported; verified progress comes from reports. The dashboard does not execute C. Publish as public `c-projects` and enable Pages → GitHub Actions using the setup guide. No hosted repository or live deployment is implied by this source package.

## What is inside

| Path | Purpose |
| --- | --- |
| `projects/01-* … 20-*` | Your starter, guide, focused lab and engineering notebook |
| `reference/` | Complete baseline solutions; shared reference modules are separate |
| `tests/` | Black-box cases, ABI harness and starter identity hashes |
| `scripts/` | Compiler/test driver and evidence-to-dashboard generator |
| `docs/` | Route, C field guide, references, publishing and advanced tracks |
| `site/` | Dependency-free GitHub Pages dashboard |
| `.github/` | CI, deployment, weekly checks, bundle workflow and templates |

These are educational baselines with explicit limits. CSV is LF-only; shell has whitespace tokens and pipelines but no quoting/job control; HTTP is a strict no-body GET subset; KV Log is single-writer without transactions; Workers is a parallel reduction; baseline document search is ASCII-only. The advanced plans are extensions, not shipped completed projects.

MIT licensed. Contributions should explain ownership, verification and remaining limits.
