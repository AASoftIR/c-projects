# Deep lab — Prove vector growth

## Work one example by hand

Push 3, -1, 2, 2; output -1 2 2 3. Around a growth boundary, length increases by one while capacity may double.

Do the example with pencil and paper before running a program. Write down intermediate indices, state, pointers, descriptor ownership or stack content as appropriate. Then compare the trace with the output.

## Algorithm and invariant

Invariant: 0 <= len <= cap; data points to cap elements when cap > 0. On growth, check multiplication, request realloc into a temporary, then publish pointer and capacity.

Pause at the first state-changing line of your implementation. Describe what was true before that line and what must be true afterward. If an error occurs midway, identify exactly which state has already changed.

## Ownership contract

The vector owns exactly one backing allocation. Sorting moves values inside it; callers must not borrow element addresses across growth.

Use that sentence as an interface comment in your own code. If you add a new allocation or descriptor, update the contract before adding another feature.

## Debugging experiment

Reserve an allocator-failure hook as your stretch. Force the second growth to fail and verify cleanup still releases the original buffer.

Run deliberate breakage only in a disposable working copy, then restore your correct implementation. Record the input, observed defect, first incorrect state and fix in NOTES.md. A single clear debugging story is more valuable than an unexplained green badge.

## Defend the design aloud

What invalidates an interior pointer? Why does a failed realloc preserve the old allocation? Why can comparator subtraction fail on extreme values?

Answer these questions without reading the reference. If you cannot explain one, return to the relevant section of docs/DEEP-DIVES.md and repeat the experiment.

## Personal extension

Expose push/pop/reserve in a header and add allocator injection to exercise allocation failure.

Keep the baseline contract passing while you add tests for the new behavior. Explain the new limit or cost: allocation count, asymptotic complexity, descriptor lifetime, normalization policy, or durability semantics. Your explanation turns this exercise into useful GitHub portfolio evidence.
