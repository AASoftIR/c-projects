# The course route

Follow the project numbers in order. The prerequisites on the dashboard express that route; earlier ownership and parser skills reappear in later work. Estimated hours exclude ambitious stretch goals.

| Stage | Projects | What you can demonstrate | Milestone defense |
| --- | --- | --- | --- |
| Inspect bytes | 01–03 | Integers, binary streams, scalar-valid UTF-8 | Why does Persian text need more bytes than code points? |
| Own memory | 04–05 | Resizing, sorting, hashed keys | Draw each allocation and its owner; explain collision handling. |
| Parse and allocate | 06–08 | CSV states, Unix filters, aligned regions | Show one input that breaks naive splitting and one invalid arena lifetime. |
| Persist and interpret | 09–10 | Durable appends, expression grammar | Explain what fsync does and why grammar encodes precedence. |
| Operate Linux | 11–16 | Pipelines, HTTP framing, TCP, logs, LRU, threads | Trace every descriptor and reproduce a short-read or recovery case. |
| Integrate systems | 17–20 | VM safety, indexing, ABI, local API | Demonstrate a rejected hostile input, an API request, and a regression fix. |

## Concrete weekly plan at about eight hours

Weeks 1–2: 01–03. Weeks 3–4: 04–05. Weeks 5–7: 06–08. Weeks 8–10: 09–10 plus a parser review. Weeks 11–14: 11–13. Weeks 15–19: 14–16. Weeks 20–24: 17–19. Weeks 25–27: 20, integration review and portfolio write-up. Move a week whenever understanding is slower or faster; never use the timetable to rush past undefined behavior.

## Professional skills beyond these baselines

The core covers the mechanisms needed to build small systems. It does not exhaust the language: strict aliasing, effective types, atomics and memory ordering, custom allocators, binary serialization, internationalized text segmentation, advanced linkers, SIMD, cross-compilation and production service hardening need later study. `DEEP-DIVES.md` gives a starting map; the capstone tracks make several of them practical.

## Your public portfolio rhythm

For each project: one implementation commit, one test/debugging commit, and one brief design note with a runnable example. Add a screenshot only when it teaches something; command output and a memory diagram are often better than decoration. Keep copied reference code identified as reference code. Include compiler flags and input size in performance claims.

The repository is educational and intentionally public. Do not commit personal book scans, passwords, cookies, API keys or private documents as test data. Use your own short notes as the sample corpus.
