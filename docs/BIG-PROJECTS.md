# Three advanced tracks after the baseline

These are larger project plans built on the 20 completed reference baselines. They are not implemented solutions in this release. Choose one, split it into reviewable milestones, and extend its contracts/tests as you work.

## A. Persian Book Search Engine — your main track

Goal: query your own Persian reading notes from a Python or React client. Keep PDF ingestion in Python initially so C work focuses on indexing and serving.

Architecture: Python extracts text from text-based documents → C validates UTF-8 and normalizes a documented subset → inverted index stores document IDs and term frequencies → query engine intersects postings and ranks → HTTP layer returns JSON IDs/scores → Python owns presentation and metadata.

1. **Unicode tokenization (12–20h).** Decode scalar boundaries; define exactly how Arabic/Persian ی and ک, punctuation, combining marks and ZWNJ behave. Keep display text unchanged. Acceptance: hand-labeled Persian token fixtures, invalid encodings rejected, query and index normalization identical. Avoid a claim of full Unicode conformance.
2. **Boolean queries and ranking (12–20h).** Add AND/OR intersections, term frequency and a specified BM25 variant. Acceptance: known ranking corpus, deterministic tie-breaking, empty query and unknown-term semantics.
3. **Persistent snapshots (16–24h).** Versioned binary format with explicit byte order, bounds and checksum. Write via atomic replacement. Acceptance: round-trip, truncated snapshots, wrong version, corrupt lengths, crash-recovery tests.
4. **Bounded worker queue (16–24h).** Accept loop hands owned connection jobs to fixed workers. Define queue-full behavior and graceful shutdown. Acceptance: repeated concurrent requests, slow client, shutdown with outstanding jobs, ThreadSanitizer lane.
5. **Portfolio release (8–12h).** Document benchmark corpus, resident memory, build time and query latency. Add a Python demo client. Compare indexed search to scanning and publish the measured tradeoff.

Solution direction: promote your UTF-8, map and posting modules into stable APIs, isolate immutable index state for readers, and keep network input parsing separate from query evaluation. Start with a single-writer snapshot model; concurrency and mutable indexes multiply complexity.

## B. Durable Mini Database — inspired by SQLite / Bitcask

Goal: a small embedded key/value engine with a deliberate durability model. It is not a SQLite-compatible database.

1. Binary records containing opcode, lengths, payload and checksum; reject lengths before allocation. Test corruption at every truncation position.
2. Replay into a hash index, distinguish an incomplete tail from a corrupt completed record, and define what recovery accepts.
3. Lock one writer; specify readers' snapshot behavior. Test two processes instead of relying only on threads.
4. Compact into a temporary file and atomically replace it. Inject termination between each durability step. Check post-restart state against the contract.
5. Add transactions with commit records only after specifying failure ordering. Build an oracle that compares generated operation histories against Python's dict.
6. Export a stable handle API to Python. Check open/close ownership and busy/error statuses.

Estimate: 60–100 focused hours. Solution direction: keep append/recovery/index/compaction modules separate; a readable format and explicit state machine matter more than early micro-optimizations. Read SQLite's atomic-commit explanation before making any transactional claim.

## C. Tiny Language + VM — inspired by Crafting Interpreters / Lua

Goal: a tiny language you can embed in a tool. Reuse the calculator grammar and safe stack VM; add a compiler with an explicit instruction format.

1. Lexer with spans and line/column diagnostics. Test malformed literals and operators.
2. AST parser for arithmetic, variables, blocks and if/while. Enforce depth and input bounds.
3. Tree evaluator with lexical scope, then compile to bytecode. Compare both implementations against the same generated programs.
4. Bytecode verifier checks instruction boundaries, operand counts, jump targets and stack behavior before execution.
5. Add instruction and memory budgets; deliberately terminate infinite loops without crashing the host.
6. Add strings with explicit ownership; choose reference counting or tracing only after studying cycles and root discovery.

Estimate: 60–120 focused hours. Solution direction: separate parsing from evaluation, preserve source spans through compilation, and compare two execution engines to expose semantic bugs. Garbage collection is a later milestone, not a hidden prerequisite.

## Other famous projects to try after one track

A terminal text editor (Kilo-inspired), a poll-based chat server, CHIP-8 emulator, a malloc-like free-list allocator, an ELF inspector, a regex engine, a miniature make implementation, an HTTP reverse proxy, a ray tracer, and a filesystem image reader. Each is a serious next project; start with one written contract and one reference oracle. Avoid kernel modules until your memory, synchronization and test discipline are strong.
