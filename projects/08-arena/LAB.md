# Deep lab — Allocate by lifetime region

## Work one example by hand

With 16-byte alignment on the target platform, sizes 1 and 1 begin at 0 and 16. Reset makes the next allocation begin at 0 again.

Do the example with pencil and paper before running a program. Write down intermediate indices, state, pointers, descriptor ownership or stack content as appropriate. Then compare the trace with the output.

## Algorithm and invariant

padding = (alignment - used % alignment) % alignment. Check padding against remaining capacity before adding it; then check size against capacity - start.

Pause at the first state-changing line of your implementation. Describe what was true before that line and what must be true afterward. If an error occurs midway, identify exactly which state has already changed.

## Ownership contract

One aligned storage union owns all bytes. Returned pointers would be borrowed until reset or arena destruction. Baseline prints offsets instead of exposing pointers.

Use that sentence as an interface comment in your own code. If you add a new allocation or descriptor, update the contract before adding another feature.

## Debugging experiment

Allocate until exact capacity, then request one more byte. Explain why checking after addition is too late if arithmetic can overflow.

Run deliberate breakage only in a disposable working copy, then restore your correct implementation. Record the input, observed defect, first incorrect state and fix in NOTES.md. A single clear debugging story is more valuable than an unexplained green badge.

## Defend the design aloud

Why is alignment about the address and not only the size? What makes a borrowed arena pointer invalid after reset? Which objects belong in separate lifetime regions?

Answer these questions without reading the reference. If you cannot explain one, return to the relevant section of docs/DEEP-DIVES.md and repeat the experiment.

## Personal extension

Add marks/rewind and poisoning in debug builds; use an arena for parsing expression nodes.

Keep the baseline contract passing while you add tests for the new behavior. Explain the new limit or cost: allocation count, asymptotic complexity, descriptor lifetime, normalization policy, or durability semantics. Your explanation turns this exercise into useful GitHub portfolio evidence.
