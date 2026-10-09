# Deep lab — Encode precedence in the grammar

## Work one example by hand

2+3*4 evaluates to 14. -(2+3)/2 evaluates to -2.5. Trailing text, division by zero and a non-finite result are errors.

Do the example with pencil and paper before running a program. Write down intermediate indices, state, pointers, descriptor ownership or stack content as appropriate. Then compare the trace with the output.

## Algorithm and invariant

expression parses addition/subtraction of terms; term parses multiplication/division of unary values; primary parses a number or a parenthesized expression. Consume all remaining input.

Pause at the first state-changing line of your implementation. Describe what was true before that line and what must be true afterward. If an error occurs midway, identify exactly which state has already changed.

## Ownership contract

The baseline borrows argv text and allocates no AST. An AST extension should use individually freed nodes or the arena lifetime from project 08.

Use that sentence as an interface comment in your own code. If you add a new allocation or descriptor, update the contract before adding another feature.

## Debugging experiment

Introduce the same arithmetic expression with and without parentheses, then show the parser call nesting that changes the result.

Run deliberate breakage only in a disposable working copy, then restore your correct implementation. Record the input, observed defect, first incorrect state and fix in NOTES.md. A single clear debugging story is more valuable than an unexplained green badge.

## Defend the design aloud

Why is a unary sign different from subtraction? How does the recursion depth bound differ from an input-length bound? Why is double unsuitable for exact money arithmetic?

Answer these questions without reading the reference. If you cannot explain one, return to the relevant section of docs/DEEP-DIVES.md and repeat the experiment.

## Personal extension

Add variables and ASTs; compare a tree interpreter against project 17 bytecode.

Keep the baseline contract passing while you add tests for the new behavior. Explain the new limit or cost: allocation count, asymptotic complexity, descriptor lifetime, normalization policy, or durability semantics. Your explanation turns this exercise into useful GitHub portfolio evidence.
