# 19 — Python ↔ C Bridge

**Mission:** libcourse.so exports int course_utf8_count(const unsigned char*,size_t,size_t*)

**Stage:** 06 / integrate systems · **Planning estimate:** 10 focused hours (not a deadline).

**Concepts:** Shared libraries, ABI, ctypes, ownership boundaries.

## Why this exists

Python memory ownership does not transfer to C just because a pointer crosses the ABI. C must not keep borrowed buffers.

This project is a small, inspectable piece of the final local document service. Your goal is to explain every allocation, every rejected input and every return value. The shipped solution is a working baseline under the contract below; the stretch work is optional and is not silently counted by the tests.

## Exact contract

`libcourse.so exports int course_utf8_count(const unsigned char*,size_t,size_t*)`

Read `../../tests/cases.py` for exact fixtures and `../../reference/19-ffi/main.c` only after your first attempt. Project-specific limits and exclusions are intentional: widening the input contract also means extending the tests. Do not infer production guarantees from a green test badge.

## Build in small steps

1. Write down inputs, outputs, failure statuses and resource limits in `NOTES.md`. Identify who owns each pointer or descriptor before coding.
2. Export one stable function. Validate pointer/length combinations; report scalar count via an out parameter. Compile -fPIC -shared, declare ctypes argtypes/restype and test embedded NUL.
3. Implement the smallest happy path. Compile with strict C17 flags before adding another feature. Keep diagnostics on stderr so stdout remains machine-readable.
4. Add the edge case that most threatens this project's invariant; predict the output before running it.
5. Run `python3 scripts/course.py test 19-ffi`. On failure, inspect the compiler output or the named test log in `build/logs/learner/`.
6. Run `python3 scripts/course.py test 19-ffi --sanitize` when your baseline works. Use GDB on the first failing input; fix the cause before optimizing.
7. Compare your ownership and error paths with the reference, then close it and implement one improvement yourself. Record the tradeoff in `NOTES.md`.

## Hint ladder — reveal progressively

<details><summary>Hint 1: model the data</summary>

Start from the exact contract, not the entire famous tool. Separate representation, input parsing and the operation that produces the result. Make the failure path just as explicit as the success path.
</details>

<details><summary>Hint 2: choose an invariant</summary>

Python memory ownership does not transfer to C just because a pointer crosses the ABI. C must not keep borrowed buffers. For each loop, write one sentence describing what must remain true before and after its body. Check arithmetic before it changes sizes, indices or values.
</details>

<details><summary>Hint 3: algorithm direction</summary>

Caller-owned byte buffers and an out parameter avoid ambiguous allocation ownership. size_t must map to ctypes.c_size_t.
</details>

## Solution walkthrough

Caller-owned byte buffers and an out parameter avoid ambiguous allocation ownership. size_t must map to ctypes.c_size_t. Read the source beside this description: locate validation first, the state-changing operation second, and cleanup last. Explain why each bound exists. Find one input that reaches each error branch and add a regression case if it is missing.

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

Expose search as a handle API with create/query/destroy; package bindings with ABI version checks.

Do the challenge in a separate commit and extend tests before expanding the contract. Write a short public explanation with a command, its output and the hardest bug. That is stronger portfolio evidence than copying a reference and calling it complete.

## Bridge to the next mission

Assemble indexing and networking into the document API.
