# Deep lab — Decode Persian safely

## Work one example by hand

سلام is 8 bytes and 4 code points. A followed by an embedded NUL followed by B is 3 bytes and 3 code points. A four-byte book emoji is one scalar.

Do the example with pencil and paper before running a program. Write down intermediate indices, state, pointers, descriptor ownership or stack content as appropriate. Then compare the trace with the output.

## Algorithm and invariant

Read a lead byte → choose continuation count and minimum scalar value → assemble scalar → enforce shortest encoding and scalar range → increment count.

Pause at the first state-changing line of your implementation. Describe what was true before that line and what must be true afterward. If an error occurs midway, identify exactly which state has already changed.

## Ownership contract

The input buffer is owned by main and released once. The decoder borrows pointer/length and writes to a caller-owned size_t. See reference/modules/utf8.h.

Use that sentence as an interface comment in your own code. If you add a new allocation or descriptor, update the contract before adding another feature.

## Debugging experiment

Replace one continuation byte in a valid Persian fixture with ASCII. Predict the rejection before running the test.

Run deliberate breakage only in a disposable working copy, then restore your correct implementation. Record the input, observed defect, first incorrect state and fix in NOTES.md. A single clear debugging story is more valuable than an unexplained green badge.

## Defend the design aloud

Why is ED A0 80 invalid? Why is C0 AF invalid even though it could encode a slash? Why can a valid code point count differ from what the viewer sees?

Answer these questions without reading the reference. If you cannot explain one, return to the relevant section of docs/DEEP-DIVES.md and repeat the experiment.

## Personal extension

Print each scalar as U+XXXX; later explore combining marks and why grapheme segmentation needs Unicode data.

Keep the baseline contract passing while you add tests for the new behavior. Explain the new limit or cost: allocation count, asymptotic complexity, descriptor lifetime, normalization policy, or durability semantics. Your explanation turns this exercise into useful GitHub portfolio evidence.
