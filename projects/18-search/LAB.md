# Deep lab — Build postings instead of rescanning

## Work one example by hand

cat cat dog on document 1 contains cat once in its posting list. Blank lines still occupy document IDs. CAT and cat share the same ASCII-normalized term.

Do the example with pencil and paper before running a program. Write down intermediate indices, state, pointers, descriptor ownership or stack content as appropriate. Then compare the trace with the output.

## Algorithm and invariant

For each document, split ASCII-letter tokens, normalize, lookup/create a term, append doc ID only when it differs from the term tail. Query one normalized term.

Pause at the first state-changing line of your implementation. Describe what was true before that line and what must be true afterward. If an error occurs midway, identify exactly which state has already changed.

## Ownership contract

The index owns terms, keys and postings; query results borrow posting nodes until index destruction. Files must be LF-delimited text without embedded NUL.

Use that sentence as an interface comment in your own code. If you add a new allocation or descriptor, update the contract before adding another feature.

## Debugging experiment

Compare postings with a Python scan over randomly generated documents. Add a term repeated many times in one document and check there is one ID.

Run deliberate breakage only in a disposable working copy, then restore your correct implementation. Record the input, observed defect, first incorrect state and fix in NOTES.md. A single clear debugging story is more valuable than an unexplained green badge.

## Defend the design aloud

Why is the tail deduplication sufficient while indexing documents in order? Where is build time spent? What changes when documents can be edited later?

Answer these questions without reading the reference. If you cannot explain one, return to the relevant section of docs/DEEP-DIVES.md and repeat the experiment.

## Personal extension

Add AND/OR intersections and BM25; integrate project 03 for Persian-aware tokenization and measure retrieval quality.

Keep the baseline contract passing while you add tests for the new behavior. Explain the new limit or cost: allocation count, asymptotic complexity, descriptor lifetime, normalization policy, or durability semantics. Your explanation turns this exercise into useful GitHub portfolio evidence.
