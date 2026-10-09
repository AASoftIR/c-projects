# Deep lab — Serve a stream rather than a packet

## Work one example by hand

Port 0 requests an ephemeral test port. The server prints READY=N. /health yields JSON and a Content-Length exactly matching body bytes.

Do the example with pencil and paper before running a program. Write down intermediate indices, state, pointers, descriptor ownership or stack content as appropriate. Then compare the trace with the output.

## Algorithm and invariant

Bind loopback → accept → set receive/send timeouts → accumulate until header terminator → parse → send all header/body bytes → close the client and repeat.

Pause at the first state-changing line of your implementation. Describe what was true before that line and what must be true afterward. If an error occurs midway, identify exactly which state has already changed.

## Ownership contract

The listening descriptor lives for the server loop; one accepted descriptor is owned per request. Normal Ctrl+C is not a graceful-shutdown leak proof.

Use that sentence as an interface comment in your own code. If you add a new allocation or descriptor, update the contract before adding another feature.

## Debugging experiment

Send a request in multiple fragments. Then send one without Host. A successful complete response and a deliberate 400 exercise different paths.

Run deliberate breakage only in a disposable working copy, then restore your correct implementation. Record the input, observed defect, first incorrect state and fix in NOTES.md. A single clear debugging story is more valuable than an unexplained green badge.

## Defend the design aloud

Why can recv return a partial header? What happens when a client disconnects during send? Why does loopback binding matter for a teaching server?

Answer these questions without reading the reference. If you cannot explain one, return to the relevant section of docs/DEEP-DIVES.md and repeat the experiment.

## Personal extension

Use poll with many sockets; measure slow-client behavior before implementing keep-alive.

Keep the baseline contract passing while you add tests for the new behavior. Explain the new limit or cost: allocation count, asymptotic complexity, descriptor lifetime, normalization policy, or durability semantics. Your explanation turns this exercise into useful GitHub portfolio evidence.
