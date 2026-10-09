# Deep lab — Keep bytes intact

## Work one example by hand

Input bytes 00 FF 41 produce 00000000: 00 ff 41. Empty input produces no row. A 17-byte input produces two rows.

Do the example with pencil and paper before running a program. Write down intermediate indices, state, pointers, descriptor ownership or stack content as appropriate. Then compare the trace with the output.

## Algorithm and invariant

Read up to 16 bytes; if the count is positive, print the offset and those bytes only. Increase the offset by the returned count, not the buffer capacity.

Pause at the first state-changing line of your implementation. Describe what was true before that line and what must be true afterward. If an error occurs midway, identify exactly which state has already changed.

## Ownership contract

The stack buffer belongs to main. The FILE object for stdin is supplied by the runtime; no heap allocation is necessary.

Use that sentence as an interface comment in your own code. If you add a new allocation or descriptor, update the contract before adding another feature.

## Debugging experiment

Create a fixture containing every byte value, then compare each parsed hex byte with the original fixture.

Run deliberate breakage only in a disposable working copy, then restore your correct implementation. Record the input, observed defect, first incorrect state and fix in NOTES.md. A single clear debugging story is more valuable than an unexplained green badge.

## Defend the design aloud

Why is ferror needed after EOF? Why is an unsigned char buffer better than char for hex output? Why does a NUL byte not end the file?

Answer these questions without reading the reference. If you cannot explain one, return to the relevant section of docs/DEEP-DIVES.md and repeat the experiment.

## Personal extension

Add an ASCII sidebar and reverse mode; compare against xxd, then benchmark 1 MiB blocks.

Keep the baseline contract passing while you add tests for the new behavior. Explain the new limit or cost: allocation count, asymptotic complexity, descriptor lifetime, normalization policy, or durability semantics. Your explanation turns this exercise into useful GitHub portfolio evidence.
