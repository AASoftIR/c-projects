# Deep lab — Make the ABI contract explicit

## Work one example by hand

سلام returns four code points. A byte buffer A NUL B returns three; it must not be converted to a C string before counting.

Do the example with pencil and paper before running a program. Write down intermediate indices, state, pointers, descriptor ownership or stack content as appropriate. Then compare the trace with the output.

## Algorithm and invariant

Python creates a byte array → ctypes declares pointer,size_t,out-pointer types → C borrows bytes → decoder validates → C stores size_t result → Python reads it.

Pause at the first state-changing line of your implementation. Describe what was true before that line and what must be true afterward. If an error occurs midway, identify exactly which state has already changed.

## Ownership contract

C never retains or frees the Python-owned input. Caller owns output storage. A shared object exports course_utf8_count; sanitizer testing uses a linked C harness instead of loading ASan into an ordinary Python process.

Use that sentence as an interface comment in your own code. If you add a new allocation or descriptor, update the contract before adding another feature.

## Debugging experiment

Call with NULL/0 and a valid out pointer, then NULL/nonzero and a NULL out pointer. Each combination has a specified success/error meaning.

Run deliberate breakage only in a disposable working copy, then restore your correct implementation. Record the input, observed defect, first incorrect state and fix in NOTES.md. A single clear debugging story is more valuable than an unexplained green badge.

## Defend the design aloud

Why must ctypes declare argtypes and restype? Who may free the input? Why does pointer plus length handle embedded NUL while char* string conventions do not?

Answer these questions without reading the reference. If you cannot explain one, return to the relevant section of docs/DEEP-DIVES.md and repeat the experiment.

## Personal extension

Expose search as a handle API with create/query/destroy; package bindings with ABI version checks.

Keep the baseline contract passing while you add tests for the new behavior. Explain the new limit or cost: allocation count, asymptotic complexity, descriptor lifetime, normalization policy, or durability semantics. Your explanation turns this exercise into useful GitHub portfolio evidence.
