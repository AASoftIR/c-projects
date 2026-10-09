# Start here — Alireza's C systems course

You already know the common syntax. This course teaches you to make C programs whose memory, errors and operating-system behavior you can explain. We begin with short tools rather than syntax worksheets, then connect them to a document-search service you can call from Python or a web client.

**Target:** C17 on Linux, with explicitly marked POSIX APIs. **Scope:** 20 complete baseline projects, 212 estimated focused hours, six stages, plus three advanced capstone extension tracks. At 8 hours/week, plan roughly 27 weeks; at 12 hours/week, roughly 18. These are planning estimates, not a promise of mastery. Keep the sequence, adjust the pace.

Open `course-preview.html` directly for a self-contained dashboard snapshot. Running the publish script refreshes that snapshot alongside the Pages data. For daily live local data, use the HTTP-server preview below.

## First ten minutes on EndeavourOS / Arch

```sh
sudo pacman -S --needed base-devel clang gdb python valgrind
cd c-projects
python3 scripts/course.py list
python3 scripts/course.py test 01 --mode reference
code projects/01-byte-lab
```

Open that project's README, predict the test outputs, then implement `main.c`. The starter deliberately returns an error. Do not copy the reference into every learner folder: that would create a green checklist without learning.

```sh
python3 scripts/course.py test 01
python3 scripts/course.py test 01 --sanitize
python3 scripts/course.py report --output progress.local.json
python3 scripts/publish.py --reports progress.local.json
python3 -m http.server 8000 --directory site
```

Open http://localhost:8000. The Pages dashboard is static; it shows course material and test reports, and cannot execute C in your browser. The optional journal is stored in that browser only and never marks projects as verified.

The test driver works from any directory. The publish command assumes the repository root. Use Python 3.10+ and GCC or Clang. Normal builds use `-std=c17 -Wall -Wextra -Wpedantic -Werror -O2 -g`; sanitizer builds use ASan/UBSan. The driver compiles all `.c` files below the selected project, so you may split an implementation into modules. Add headers alongside them; use local quoted includes.

## One repeatable learning session

1. **Predict, 10 minutes.** Read the contract. Write one normal output and one rejected input. Mark ownership and your loop invariant.
2. **Build, 45–60 minutes.** Implement one part. Compile immediately. Keep stdout clean for scripts; errors go to stderr.
3. **Break, 20 minutes.** Empty input, largest valid input, malformed input, allocation/IO failure, partial input. Choose the cases relevant to this project.
4. **Explain, 10 minutes.** Record the hardest bug and why the fix works in `NOTES.md`. Explain the algorithm aloud without reading code.
5. **Compare, after an attempt.** Reveal hint 1, then hint 2. Study the reference only after you have a concrete question. Add one useful regression test, then commit.

## Definition of done

CI passing is evidence about the shipped contract, not a certification of professional skill. Learning is done when you can explain the invariants, resource ownership, complexity and limits; reproduce one debugging lesson; and implement one small extension unaided. Push that story alongside your code.

The auto-checkbox needs all GCC/Clang normal and sanitizer checks on main to pass. Reference solutions are verified separately and never affect learner completion. A later regression unticks the checkbox. Local preview with one report explicitly shows one available test lane; only CI establishes the full four-lane result.

## What connects this to your interests

- Linux: binary tools, stdin/stdout pipelines, processes, descriptors, sockets and threads.
- Persian content: a real UTF-8 decoder and Persian test inputs; later, a Persian-search extension.
- bookish_guy: a reading ledger and a document-search corpus that can contain your book notes.
- Python/AI: a ctypes bridge and the foundations of retrieval; the baseline search API is an exact ASCII-word index, not semantic RAG or PDF extraction.
- Web development: protocol framing, status codes, JSON endpoints, and clear input contracts.

All tools install natively. No Docker, proprietary IDE, paid API or cloud service is needed to learn. GitHub provides the optional public verification and dashboard.

## Choose depth instead of skipping foundations

If Byte Lab is easy, finish it quickly and add a boundary test. If memory is unfamiliar, spend a full week on the vector and map. If networking is your goal, still learn the ownership and parser work first: most network defects begin there.

After project 20, choose one advanced track in `docs/BIG-PROJECTS.md`: Persian document engine, durable database, or compiler runtime. Those are larger extensions with architecture and acceptance milestones, not extra completed implementations.
