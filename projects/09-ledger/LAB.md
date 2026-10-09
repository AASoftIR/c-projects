# Deep lab — Separate an append from a transaction

## Work one example by hand

Add two reading notes, exit, then list them in a new process. Reject an embedded newline because it could inject an extra record.

Do the example with pencil and paper before running a program. Write down intermediate indices, state, pointers, descriptor ownership or stack content as appropriate. Then compare the trace with the output.

## Algorithm and invariant

Validate → open append mode → write one record → fflush → fsync → close and check close. Listing is a separate read-only operation.

Pause at the first state-changing line of your implementation. Describe what was true before that line and what must be true afterward. If an error occurs midway, identify exactly which state has already changed.

## Ownership contract

main owns the FILE it opens and the getline buffer. UTF-8 entry bytes are preserved but the ledger does not validate Unicode or lock concurrent writes.

Use that sentence as an interface comment in your own code. If you add a new allocation or descriptor, update the contract before adding another feature.

## Debugging experiment

Use /dev/full as the add target on Linux and inspect the error status. It demonstrates why testing fopen success alone is insufficient.

Run deliberate breakage only in a disposable working copy, then restore your correct implementation. Record the input, observed defect, first incorrect state and fix in NOTES.md. A single clear debugging story is more valuable than an unexplained green badge.

## Defend the design aloud

Where are bytes after fprintf but before fflush? What does fsync not promise about multiple concurrent writers? How would an update differ from an append?

Answer these questions without reading the reference. If you cannot explain one, return to the relevant section of docs/DEEP-DIVES.md and repeat the experiment.

## Personal extension

Add timestamps and atomic export; inject a write error with /dev/full and design a multi-writer lock.

Keep the baseline contract passing while you add tests for the new behavior. Explain the new limit or cost: allocation count, asymptotic complexity, descriptor lifetime, normalization policy, or durability semantics. Your explanation turns this exercise into useful GitHub portfolio evidence.
