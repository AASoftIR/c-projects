# Deep lab — Recover a torn append tail

## Work one example by hand

P<TAB>key<TAB>value<LF> assigns a value. D<TAB>key<LF> deletes it. A final incomplete record is ignored on replay and truncated before a later append.

Do the example with pencil and paper before running a program. Write down intermediate indices, state, pointers, descriptor ownership or stack content as appropriate. Then compare the trace with the output.

## Algorithm and invariant

Replay complete validated records → retain offset of last good boundary → apply operations to owned in-memory entries → on mutation repair tail → append → flush/fsync.

Pause at the first state-changing line of your implementation. Describe what was true before that line and what must be true afterward. If an error occurs midway, identify exactly which state has already changed.

## Ownership contract

The store owns replayed key/value copies. It requires a regular file and one writer; the baseline is not a transactional database. No tabs, control bytes or Unicode values are accepted.

Use that sentence as an interface comment in your own code. If you add a new allocation or descriptor, update the contract before adding another feature.

## Debugging experiment

Append an unfinished put record by hand, then perform another put. Reopen and verify the new key survives without incorporating the torn record.

Run deliberate breakage only in a disposable working copy, then restore your correct implementation. Record the input, observed defect, first incorrect state and fix in NOTES.md. A single clear debugging story is more valuable than an unexplained green badge.

## Defend the design aloud

Why must repair happen before appending another complete record? How is an incomplete tail different from a corrupt complete record? What guarantees require checksums or locking?

Answer these questions without reading the reference. If you cannot explain one, return to the relevant section of docs/DEEP-DIVES.md and repeat the experiment.

## Personal extension

Add record length and checksum, atomic compaction with directory fsync, indexing and file locks; study SQLite rather than calling this a SQLite clone.

Keep the baseline contract passing while you add tests for the new behavior. Explain the new limit or cost: allocation count, asymptotic complexity, descriptor lifetime, normalization policy, or durability semantics. Your explanation turns this exercise into useful GitHub portfolio evidence.
