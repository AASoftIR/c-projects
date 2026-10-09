# 20 — Document Search API

**Mission:** doc-api PORT CORPUS → READY=PORT; GET /health or /search?q=ASCIIword returns JSON

**Stage:** 06 / integrate systems · **Planning estimate:** 24 focused hours (not a deadline).

**Concepts:** Service integration, URL decoding, API errors, operational limits.

## Why this exists

A capstone proves interfaces compose. Correct HTTP, ownership and search semantics matter more than adding a frontend.

This project is a small, inspectable piece of the final local document service. Your goal is to explain every allocation, every rejected input and every return value. The shipped solution is a working baseline under the contract below; the stretch work is optional and is not silently counted by the tests.

## Exact contract

`doc-api PORT CORPUS → READY=PORT; GET /health or /search?q=ASCIIword returns JSON`

Read `../../tests/cases.py` for exact fixtures and `../../reference/20-doc-api/main.c` only after your first attempt. Project-specific limits and exclusions are intentional: widening the input contract also means extending the tests. Do not infer production guarantees from a green test badge.

## Build in small steps

1. Write down inputs, outputs, failure statuses and resource limits in `NOTES.md`. Identify who owns each pointer or descriptor before coding.
2. Load a corpus, build an inverted index, serve loopback HTTP with the strict framing from projects 12/13, decode %XX and +, validate one word and return IDs. Define 400/404 and missing-corpus behavior.
3. Implement the smallest happy path. Compile with strict C17 flags before adding another feature. Keep diagnostics on stderr so stdout remains machine-readable.
4. Add the edge case that most threatens this project's invariant; predict the output before running it.
5. Run `python3 scripts/course.py test 20-doc-api`. On failure, inspect the compiler output or the named test log in `build/logs/learner/`.
6. Run `python3 scripts/course.py test 20-doc-api --sanitize` when your baseline works. Use GDB on the first failing input; fix the cause before optimizing.
7. Compare your ownership and error paths with the reference, then close it and implement one improvement yourself. Record the tradeoff in `NOTES.md`.

## Hint ladder — reveal progressively

<details><summary>Hint 1: model the data</summary>

Start from the exact contract, not the entire famous tool. Separate representation, input parsing and the operation that produces the result. Make the failure path just as explicit as the success path.
</details>

<details><summary>Hint 2: choose an invariant</summary>

A capstone proves interfaces compose. Correct HTTP, ownership and search semantics matter more than adding a frontend. For each loop, write one sentence describing what must remain true before and after its body. Check arithmetic before it changes sizes, indices or values.
</details>

<details><summary>Hint 3: algorithm direction</summary>

The reference combines the index and HTTP modules without duplicating protocol parsing. It is sequential, in-memory and ASCII-search only.
</details>

## Solution walkthrough

The reference combines the index and HTTP modules without duplicating protocol parsing. It is sequential, in-memory and ASCII-search only. Read the source beside this description: locate validation first, the state-changing operation second, and cleanup last. Explain why each bound exists. Find one input that reaches each error branch and add a regression case if it is missing.

The reference is intentionally readable rather than maximally optimized. It uses common numeric support in `include/` and reference-only modules in `reference/modules/` for checked numeric parsing, UTF-8 decoding, indexing and strict HTTP I/O where those mechanisms are reused by a later project. Early projects still implement their own central concept; the capstone assembles tested components.

## Verification and acceptance

- [ ] I implemented the contract in `main.c` and can explain it without the reference.
- [ ] Normal inputs and at least one invalid input have tests.
- [ ] Strict compilation and the official project tests pass.
- [ ] Sanitizer checks pass where supported; I can explain any platform limitation.
- [ ] I documented allocations, borrowed pointers, or descriptor lifetimes.
- [ ] I recorded one real debugging lesson and one design limitation.

Only the automated contract result controls the repository's generated checkbox. This local checklist tracks learning evidence and is deliberately not auto-ticked.

## Boss challenge

Add Persian tokenization, persistent snapshots, LRU results, a bounded worker queue and a Python client; create benchmarks before claiming performance.

Do the challenge in a separate commit and extend tests before expanding the contract. Write a short public explanation with a command, its output and the hardest bug. That is stronger portfolio evidence than copying a reference and calling it complete.

## Bridge to the next mission

Follow the three capstone extensions in docs/BIG-PROJECTS.md, beginning with Persian-aware search.
