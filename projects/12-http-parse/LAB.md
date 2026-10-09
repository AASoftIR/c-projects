# Deep lab — Reject ambiguous framing

## Work one example by hand

GET /health HTTP/1.1 with exactly one Host and CRLFCRLF is valid. Duplicate Host, Transfer-Encoding, nonzero Content-Length and a supplied body are rejected.

Do the example with pencil and paper before running a program. Write down intermediate indices, state, pointers, descriptor ownership or stack content as appropriate. Then compare the trace with the output.

## Algorithm and invariant

Bound input at 8192 → verify complete header framing and no embedded NUL → split exact request line → validate target → parse token header names → enforce Host/framing constraints.

Pause at the first state-changing line of your implementation. Describe what was true before that line and what must be true afterward. If an error occurs midway, identify exactly which state has already changed.

## Ownership contract

The parser mutates its caller-owned buffer while extracting fields and copies the target into Request. This reference logic lives in reference/modules/http.h; implement or promote your own for learner work.

Use that sentence as an interface comment in your own code. If you add a new allocation or descriptor, update the contract before adding another feature.

## Debugging experiment

Place NUL, bare LF and duplicate framing headers into fixtures. Compare each rejection to the stated contract rather than to arbitrary server behavior.

Run deliberate breakage only in a disposable working copy, then restore your correct implementation. Record the input, observed defect, first incorrect state and fix in NOTES.md. A single clear debugging story is more valuable than an unexplained green badge.

## Defend the design aloud

Why does HTTP need a byte contract? Why are duplicate length headers risky? What does this GET-only parser deliberately exclude from HTTP/1.1?

Answer these questions without reading the reference. If you cannot explain one, return to the relevant section of docs/DEEP-DIVES.md and repeat the experiment.

## Personal extension

Add body framing in a separate parser with an explicit byte limit; use a fuzzer on every parser state.

Keep the baseline contract passing while you add tests for the new behavior. Explain the new limit or cost: allocation count, asymptotic complexity, descriptor lifetime, normalization policy, or durability semantics. Your explanation turns this exercise into useful GitHub portfolio evidence.
