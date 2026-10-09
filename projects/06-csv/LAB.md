# Deep lab — Treat EOF as an input

## Work one example by hand

The record a,b, has three fields. A quoted newline does not finish a record. A doubled quote inside quotes is one literal quote.

Do the example with pencil and paper before running a program. Write down intermediate indices, state, pointers, descriptor ownership or stack content as appropriate. Then compare the trace with the output.

## Algorithm and invariant

START accepts a quote or plain data; PLAIN rejects quotes; QUOTED treats delimiters as data; AFTER accepts a doubled quote or a delimiter. EOF in QUOTED is an error.

Pause at the first state-changing line of your implementation. Describe what was true before that line and what must be true afterward. If an error occurs midway, identify exactly which state has already changed.

## Ownership contract

The baseline keeps counts and state only, so it has bounded memory independent of input length. Emitting cells later introduces buffer ownership.

Use that sentence as an interface comment in your own code. If you add a new allocation or descriptor, update the contract before adding another feature.

## Debugging experiment

Write down transitions for every state with comma, quote, newline, ordinary byte and EOF. Find any unspecified cell before coding.

Run deliberate breakage only in a disposable working copy, then restore your correct implementation. Record the input, observed defect, first incorrect state and fix in NOTES.md. A single clear debugging story is more valuable than an unexplained green badge.

## Defend the design aloud

Why does splitting on commas fail? Is an empty file one empty record? What exact decision distinguishes blank input from a blank line?

Answer these questions without reading the reference. If you cannot explain one, return to the relevant section of docs/DEEP-DIVES.md and repeat the experiment.

## Personal extension

Add CRLF support and emit structured records; document whether a blank line is an empty record.

Keep the baseline contract passing while you add tests for the new behavior. Explain the new limit or cost: allocation count, asymptotic complexity, descriptor lifetime, normalization policy, or durability semantics. Your explanation turns this exercise into useful GitHub portfolio evidence.
