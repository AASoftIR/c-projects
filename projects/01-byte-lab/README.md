# 01 — Byte Lab

**Mission:** byte-lab 0..4294967295 → dec=N hex=XXXXXXXX bits=32-bit-string ones=N

**Stage:** 01 / inspect bytes · **Planning estimate:** 3 focused hours (not a deadline).

**Concepts:** Representation, integer widths, parsing, bit operations.

## Why this exists

An integer value is abstract; its in-memory byte order is a separate concern. Use PRI macros rather than assuming unsigned long is 32 bits.

This project is a small, inspectable piece of the final local document service. Your goal is to explain every allocation, every rejected input and every return value. The shipped solution is a working baseline under the contract below; the stretch work is optional and is not silently counted by the tests.

## Exact contract

`byte-lab 0..4294967295 → dec=N hex=XXXXXXXX bits=32-bit-string ones=N`

Read `../../tests/cases.py` for exact fixtures and `../../reference/01-byte-lab/main.c` only after your first attempt. Project-specific limits and exclusions are intentional: widening the input contract also means extending the tests. Do not infer production guarantees from a green test badge.

## Build in small steps

1. Write down inputs, outputs, failure statuses and resource limits in `NOTES.md`. Identify who owns each pointer or descriptor before coding.
2. Parse with strtoull; reject signs, whitespace, suffixes and range errors. Convert using shifts on uint32_t. Count bits without shifting a signed value.
3. Implement the smallest happy path. Compile with strict C17 flags before adding another feature. Keep diagnostics on stderr so stdout remains machine-readable.
4. Add the edge case that most threatens this project's invariant; predict the output before running it.
5. Run `python3 scripts/course.py test 01-byte-lab`. On failure, inspect the compiler output or the named test log in `build/logs/learner/`.
6. Run `python3 scripts/course.py test 01-byte-lab --sanitize` when your baseline works. Use GDB on the first failing input; fix the cause before optimizing.
7. Compare your ownership and error paths with the reference, then close it and implement one improvement yourself. Record the tradeoff in `NOTES.md`.

## Hint ladder — reveal progressively

<details><summary>Hint 1: model the data</summary>

Start from the exact contract, not the entire famous tool. Separate representation, input parsing and the operation that produces the result. Make the failure path just as explicit as the success path.
</details>

<details><summary>Hint 2: choose an invariant</summary>

An integer value is abstract; its in-memory byte order is a separate concern. Use PRI macros rather than assuming unsigned long is 32 bits. For each loop, write one sentence describing what must remain true before and after its body. Check arithmetic before it changes sizes, indices or values.
</details>

<details><summary>Hint 3: algorithm direction</summary>

An end pointer distinguishes valid zero from conversion failure; fixed-width unsigned arithmetic makes every shift well-defined.
</details>

## Solution walkthrough

An end pointer distinguishes valid zero from conversion failure; fixed-width unsigned arithmetic makes every shift well-defined. Read the source beside this description: locate validation first, the state-changing operation second, and cleanup last. Explain why each bound exists. Find one input that reaches each error branch and add a regression case if it is missing.

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

Add binary input, byte-order inspection through unsigned char, and rotate-left with a zero-shift guard.

Do the challenge in a separate commit and extend tests before expanding the contract. Write a short public explanation with a command, its output and the hardest bug. That is stronger portfolio evidence than copying a reference and calling it complete.

## Bridge to the next mission

Hex Detective turns abstract integers into actual bytes from files.
