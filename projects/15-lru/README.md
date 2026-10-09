# 15 — LRU Cache Simulator

**Mission:** lru CAPACITY KEY... → hit=N miss=N cache=most-recent,...

**Stage:** 05 / operate Linux · **Planning estimate:** 10 focused hours (not a deadline).

**Concepts:** Linked lists, eviction, invariants, complexity.

## Why this exists

A cache is an explicit policy plus lookup; a linked list alone makes lookup O(capacity).

This project is a small, inspectable piece of the final local document service. Your goal is to explain every allocation, every rejected input and every return value. The shipped solution is a working baseline under the contract below; the stretch work is optional and is not silently counted by the tests.

## Exact contract

`lru CAPACITY KEY... → hit=N miss=N cache=most-recent,...`

Read `../../tests/cases.py` for exact fixtures and `../../reference/15-lru/main.c` only after your first attempt. Project-specific limits and exclusions are intentional: widening the input contract also means extending the tests. Do not infer production guarantees from a green test badge.

## Build in small steps

1. Write down inputs, outputs, failure statuses and resource limits in `NOTES.md`. Identify who owns each pointer or descriptor before coding.
2. Move a hit to the front; allocate a miss; evict the tail above capacity. Maintain head/tail and prev/next links. Cap capacity at 10000.
3. Implement the smallest happy path. Compile with strict C17 flags before adding another feature. Keep diagnostics on stderr so stdout remains machine-readable.
4. Add the edge case that most threatens this project's invariant; predict the output before running it.
5. Run `python3 scripts/course.py test 15-lru`. On failure, inspect the compiler output or the named test log in `build/logs/learner/`.
6. Run `python3 scripts/course.py test 15-lru --sanitize` when your baseline works. Use GDB on the first failing input; fix the cause before optimizing.
7. Compare your ownership and error paths with the reference, then close it and implement one improvement yourself. Record the tradeoff in `NOTES.md`.

## Hint ladder — reveal progressively

<details><summary>Hint 1: model the data</summary>

Start from the exact contract, not the entire famous tool. Separate representation, input parsing and the operation that produces the result. Make the failure path just as explicit as the success path.
</details>

<details><summary>Hint 2: choose an invariant</summary>

A cache is an explicit policy plus lookup; a linked list alone makes lookup O(capacity). For each loop, write one sentence describing what must remain true before and after its body. Check arithmetic before it changes sizes, indices or values.
</details>

<details><summary>Hint 3: algorithm direction</summary>

The reference favors a transparent list over an optimized hash/list combination, so you can inspect every invariant.
</details>

## Solution walkthrough

The reference favors a transparent list over an optimized hash/list combination, so you can inspect every invariant. Read the source beside this description: locate validation first, the state-changing operation second, and cleanup last. Explain why each bound exists. Find one input that reaches each error branch and add a regression case if it is missing.

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

Reuse project 05 for O(1)-average lookup; test capacity 1, repeated eviction and failed allocation.

Do the challenge in a separate commit and extend tests before expanding the contract. Write a short public explanation with a command, its output and the hardest bug. That is stronger portfolio evidence than copying a reference and calling it complete.

## Bridge to the next mission

Move beyond one execution thread while preserving ownership.
