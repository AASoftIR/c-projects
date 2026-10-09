# Deep lab — Distinguish keys from hashes

## Work one example by hand

Cat cat DOG yields cat=2 then dog=1, in lexical order. Numeric punctuation separates tokens. Persian bytes are deliberately not ASCII word characters.

Do the example with pencil and paper before running a program. Write down intermediate indices, state, pointers, descriptor ownership or stack content as appropriate. Then compare the trace with the output.

## Algorithm and invariant

For each token, calculate a bucket, compare complete keys along the chain, increment a match or prepend an owned entry. Sort a collected pointer array only for output.

Pause at the first state-changing line of your implementation. Describe what was true before that line and what must be true afterward. If an error occurs midway, identify exactly which state has already changed.

## Ownership contract

The table owns entries and copied keys. The sorted array borrows entry pointers and must be freed before or independently of entry teardown.

Use that sentence as an interface comment in your own code. If you add a new allocation or descriptor, update the contract before adding another feature.

## Debugging experiment

Lower the bucket count in a disposable build to force collisions; the same oracle should still pass.

Run deliberate breakage only in a disposable working copy, then restore your correct implementation. Record the input, observed defect, first incorrect state and fix in NOTES.md. A single clear debugging story is more valuable than an unexplained green badge.

## Defend the design aloud

What happens when every key collides? Why does sorted output not imply an ordered dictionary? Where does a newly inserted key get its lifetime?

Answer these questions without reading the reference. If you cannot explain one, return to the relevant section of docs/DEEP-DIVES.md and repeat the experiment.

## Personal extension

Implement resizing and a collision-heavy test; extend tokenization using project 03 without confusing byte and character boundaries.

Keep the baseline contract passing while you add tests for the new behavior. Explain the new limit or cost: allocation count, asymptotic complexity, descriptor lifetime, normalization policy, or durability semantics. Your explanation turns this exercise into useful GitHub portfolio evidence.
