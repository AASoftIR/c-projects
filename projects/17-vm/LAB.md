# Deep lab — Never trust the bytecode

## Work one example by hand

PUSH 7, PUSH 2, SUB, PRINT produces 5. Reversing the operand order changes the result. PRINT observes the top without popping it.

Do the example with pencil and paper before running a program. Write down intermediate indices, state, pointers, descriptor ownership or stack content as appropriate. Then compare the trace with the output.

## Algorithm and invariant

PUSH checks capacity and integer syntax. Binary operators require two operands and compute a checked result before storing it. Division rejects zero and INT64_MIN / -1.

Pause at the first state-changing line of your implementation. Describe what was true before that line and what must be true afterward. If an error occurs midway, identify exactly which state has already changed.

## Ownership contract

The stack is fixed caller-owned storage. getline owns a reused instruction buffer. The VM executes textual instructions rather than a compact persisted bytecode file.

Use that sentence as an interface comment in your own code. If you add a new allocation or descriptor, update the contract before adding another feature.

## Debugging experiment

Write one program per arithmetic boundary and stack-underflow path. Compare random small multiplications against Python integers.

Run deliberate breakage only in a disposable working copy, then restore your correct implementation. Record the input, observed defect, first incorrect state and fix in NOTES.md. A single clear debugging story is more valuable than an unexplained green badge.

## Defend the design aloud

Why is signed overflow not a predictable wraparound? What would a jump instruction require from a verifier? How would you stop malicious infinite loops?

Answer these questions without reading the reference. If you cannot explain one, return to the relevant section of docs/DEEP-DIVES.md and repeat the experiment.

## Personal extension

Compile project 10 ASTs to this VM; add jumps, instruction budgets and a verifier before adding loops.

Keep the baseline contract passing while you add tests for the new behavior. Explain the new limit or cost: allocation count, asymptotic complexity, descriptor lifetime, normalization policy, or durability semantics. Your explanation turns this exercise into useful GitHub portfolio evidence.
