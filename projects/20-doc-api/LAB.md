# Deep lab — Compose the document service

## Work one example by hand

For corpus cat dog, DOG, cat bird, /search?q=cat returns documents [1,3]. /search?q=%63at gives the same result. Empty or multiword queries are 400.

Do the example with pencil and paper before running a program. Write down intermediate indices, state, pointers, descriptor ownership or stack content as appropriate. Then compare the trace with the output.

## Algorithm and invariant

Startup loads an immutable index → each request goes through strict HTTP parsing → route dispatch decodes the query → validate one ASCII word → lookup postings → build bounded JSON → send complete response.

Pause at the first state-changing line of your implementation. Describe what was true before that line and what must be true afterward. If an error occurs midway, identify exactly which state has already changed.

## Ownership contract

The index belongs to the server lifetime, responses own temporary JSON buffers, clients own one accepted descriptor. This baseline is sequential and in-memory; the larger Persian engine is your next track.

Use that sentence as an interface comment in your own code. If you add a new allocation or descriptor, update the contract before adding another feature.

## Debugging experiment

Exercise unknown terms, escaped NUL, malformed percent escapes, a missing corpus, repeated connections and a fragmented request. Keep protocol and search failures distinct.

Run deliberate breakage only in a disposable working copy, then restore your correct implementation. Record the input, observed defect, first incorrect state and fix in NOTES.md. A single clear debugging story is more valuable than an unexplained green badge.

## Defend the design aloud

Which limits belong to HTTP and which to query semantics? How can an immutable index support concurrent readers? What does the service need before it can search Persian documents?

Answer these questions without reading the reference. If you cannot explain one, return to the relevant section of docs/DEEP-DIVES.md and repeat the experiment.

## Personal extension

Add Persian tokenization, persistent snapshots, LRU results, a bounded worker queue and a Python client; create benchmarks before claiming performance.

Keep the baseline contract passing while you add tests for the new behavior. Explain the new limit or cost: allocation count, asymptotic complexity, descriptor lifetime, normalization policy, or durability semantics. Your explanation turns this exercise into useful GitHub portfolio evidence.
