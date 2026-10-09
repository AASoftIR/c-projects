# Deep lab — Use partitioned ownership

## Work one example by hand

With 8 threads and count 3, some workers receive empty ranges; the result is still 6. Count 0 is valid and returns 0.

Do the example with pencil and paper before running a program. Write down intermediate indices, state, pointers, descriptor ownership or stack content as appropriate. Then compare the trace with the output.

## Algorithm and invariant

Worker i receives [1 + count*i/threads, 1 + count*(i+1)/threads). Each writes its own sum. Parent joins every successfully started thread, then reduces private results.

Pause at the first state-changing line of your implementation. Describe what was true before that line and what must be true afterward. If an error occurs midway, identify exactly which state has already changed.

## Ownership contract

Jobs live in main until joins finish. Each worker borrows exactly one Job pointer. This baseline is a reduction lab; a general task queue is a separate extension.

Use that sentence as an interface comment in your own code. If you add a new allocation or descriptor, update the contract before adding another feature.

## Debugging experiment

Use uneven partitions and more threads than elements. As a stretch, intentionally introduce a shared counter and demonstrate the race under ThreadSanitizer.

Run deliberate breakage only in a disposable working copy, then restore your correct implementation. Record the input, observed defect, first incorrect state and fix in NOTES.md. A single clear debugging story is more valuable than an unexplained green badge.

## Defend the design aloud

Why does this design need no sum mutex? When can the parent safely read results? What happens if the fifth pthread_create fails?

Answer these questions without reading the reference. If you cannot explain one, return to the relevant section of docs/DEEP-DIVES.md and repeat the experiment.

## Personal extension

Turn it into a bounded producer/consumer queue with condition variables; test cancellation, shutdown and ThreadSanitizer separately.

Keep the baseline contract passing while you add tests for the new behavior. Explain the new limit or cost: allocation count, asymptotic complexity, descriptor lifetime, normalization policy, or durability semantics. Your explanation turns this exercise into useful GitHub portfolio evidence.
