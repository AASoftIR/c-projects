# Deep lab — Check list invariants after eviction

## Work one example by hand

For capacity 2 and a b a c b, there is one hit, four misses, and cache order b,c from most to least recent.

Do the example with pencil and paper before running a program. Write down intermediate indices, state, pointers, descriptor ownership or stack content as appropriate. Then compare the trace with the output.

## Algorithm and invariant

Hit → detach node → prepend. Miss → allocate/copy → prepend → if len > capacity detach/free the tail. Keep head.prev and tail.next null.

Pause at the first state-changing line of your implementation. Describe what was true before that line and what must be true afterward. If an error occurs midway, identify exactly which state has already changed.

## Ownership contract

The cache owns nodes and copied key strings. A lookup borrows a node while the cache is unchanged. Eviction ends both node and key lifetimes.

Use that sentence as an interface comment in your own code. If you add a new allocation or descriptor, update the contract before adding another feature.

## Debugging experiment

Trace a repeated hit on the head, a hit on the tail and an eviction. Assert forward/backward traversal visits the same node set.

Run deliberate breakage only in a disposable working copy, then restore your correct implementation. Record the input, observed defect, first incorrect state and fix in NOTES.md. A single clear debugging story is more valuable than an unexplained green badge.

## Defend the design aloud

Why is lookup still O(capacity) here? What second structure would make it O(1) on average? What should change at capacity 1?

Answer these questions without reading the reference. If you cannot explain one, return to the relevant section of docs/DEEP-DIVES.md and repeat the experiment.

## Personal extension

Reuse project 05 for O(1)-average lookup; test capacity 1, repeated eviction and failed allocation.

Keep the baseline contract passing while you add tests for the new behavior. Explain the new limit or cost: allocation count, asymptotic complexity, descriptor lifetime, normalization policy, or durability semantics. Your explanation turns this exercise into useful GitHub portfolio evidence.
