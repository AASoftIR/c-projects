# Deep lab — Trace every pipe end

## Work one example by hand

seq 1 100000 | wc -l must finish rather than deadlock. true | false returns the last command status. An unknown program returns 127.

Do the example with pencil and paper before running a program. Write down intermediate indices, state, pointers, descriptor ownership or stack content as appropriate. Then compare the trace with the output.

## Algorithm and invariant

Parse commands → for each command create next pipe if needed → fork → duplicate only the intended stdin/stdout → close unused descriptors → exec. Parent launches all, closes ends and reaps all.

Pause at the first state-changing line of your implementation. Describe what was true before that line and what must be true afterward. If an error occurs midway, identify exactly which state has already changed.

## Ownership contract

Parent owns child PIDs and the previous read end. Each child owns its standard descriptors after dup2. Command strings borrow one mutable parsed buffer. Spaces around | are required.

Use that sentence as an interface comment in your own code. If you add a new allocation or descriptor, update the contract before adding another feature.

## Debugging experiment

Keep a write descriptor open in a disposable build and reproduce the hang under a timeout. Then draw the descriptor ownership table that fixes it.

Run deliberate breakage only in a disposable working copy, then restore your correct implementation. Record the input, observed defect, first incorrect state and fix in NOTES.md. A single clear debugging story is more valuable than an unexplained green badge.

## Defend the design aloud

Which writer keeps EOF from arriving? Why can waiting before starting the consumer deadlock? Why must the child use _exit after exec failure?

Answer these questions without reading the reference. If you cannot explain one, return to the relevant section of docs/DEEP-DIVES.md and repeat the experiment.

## Personal extension

Add quoting with a tokenizer, then redirection, process groups, signals and job control as separate milestones.

Keep the baseline contract passing while you add tests for the new behavior. Explain the new limit or cost: allocation count, asymptotic complexity, descriptor lifetime, normalization policy, or durability semantics. Your explanation turns this exercise into useful GitHub portfolio evidence.
