# Deep lab — Trace the highest bit

## Work one example by hand

255 becomes 000000FF and eight one-bits. 2147483648 has only bit 31 set. 4294967295 has every bit set.

Do the example with pencil and paper before running a program. Write down intermediate indices, state, pointers, descriptor ownership or stack content as appropriate. Then compare the trace with the output.

## Algorithm and invariant

For positions 31 down to 0: shift an unsigned 32-bit value right, AND with 1, emit a digit and add it to the population count. Never shift by 32.

Pause at the first state-changing line of your implementation. Describe what was true before that line and what must be true afterward. If an error occurs midway, identify exactly which state has already changed.

## Ownership contract

A uint32_t owns no dynamic resources. The output character array needs 33 slots because its terminator is a separate byte.

Use that sentence as an interface comment in your own code. If you add a new allocation or descriptor, update the contract before adding another feature.

## Debugging experiment

Change the final loop bound from 32 to 33 in a disposable copy; run UBSan and explain the shift diagnostic.

Run deliberate breakage only in a disposable working copy, then restore your correct implementation. Record the input, observed defect, first incorrect state and fix in NOTES.md. A single clear debugging story is more valuable than an unexplained green badge.

## Defend the design aloud

Why are -1 and +1 rejected even though a conversion library might accept signs? Why is UINT32_MAX portable? What changes if unsigned int is wider than 32 bits?

Answer these questions without reading the reference. If you cannot explain one, return to the relevant section of docs/DEEP-DIVES.md and repeat the experiment.

## Personal extension

Add binary input, byte-order inspection through unsigned char, and rotate-left with a zero-shift guard.

Keep the baseline contract passing while you add tests for the new behavior. Explain the new limit or cost: allocation count, asymptotic complexity, descriptor lifetime, normalization policy, or durability semantics. Your explanation turns this exercise into useful GitHub portfolio evidence.
