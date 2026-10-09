# Deep lab — Make a composable Unix filter

## Work one example by hand

cat matches both cat and bobcat. No match returns 1. A missing final newline is normalized to a newline in printed results.

Do the example with pencil and paper before running a program. Write down intermediate indices, state, pointers, descriptor ownership or stack content as appropriate. Then compare the trace with the output.

## Algorithm and invariant

For each line, compare the literal pattern at each candidate start. With -i, lower ASCII bytes for comparisons only; do not modify displayed lines.

Pause at the first state-changing line of your implementation. Describe what was true before that line and what must be true afterward. If an error occurs midway, identify exactly which state has already changed.

## Ownership contract

getline owns/reuses a heap buffer through a caller-maintained pointer and capacity. Free that buffer at the end. Text matching is not binary-safe grep.

Use that sentence as an interface comment in your own code. If you add a new allocation or descriptor, update the contract before adding another feature.

## Debugging experiment

Pipe the tool into wc and separately capture its exit status. Demonstrate why no-result and error cannot share one status.

Run deliberate breakage only in a disposable working copy, then restore your correct implementation. Record the input, observed defect, first incorrect state and fix in NOTES.md. A single clear debugging story is more valuable than an unexplained green badge.

## Defend the design aloud

What is the meaning of statuses 0, 1 and 2? Why should diagnostics use stderr? Should an empty pattern match an empty line?

Answer these questions without reading the reference. If you cannot explain one, return to the relevant section of docs/DEEP-DIVES.md and repeat the experiment.

## Personal extension

Add -n, -v and multiple files; implement a separate KMP matcher and measure worst-case input.

Keep the baseline contract passing while you add tests for the new behavior. Explain the new limit or cost: allocation count, asymptotic complexity, descriptor lifetime, normalization policy, or durability semantics. Your explanation turns this exercise into useful GitHub portfolio evidence.
