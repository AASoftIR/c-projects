# C engineering field guide

Read the related section just before its project. The examples illustrate one idea each; they are not an invitation to memorize syntax.

## 1. Build a mental model of objects, not just pointers

An object has a type, size, alignment, storage duration and lifetime. A pointer value grants no ownership by itself. An array expression usually becomes a pointer to its first element, so a callee cannot recover its element count from `sizeof(pointer)`. Pass length explicitly. A pointer into a freed object cannot be used even when the address appears unchanged.

For every interface, write: “caller owns this buffer; callee borrows it for this call; callee writes at most capacity bytes; return status tells the caller whether output is valid.” This is the C equivalent of making implicit runtime rules explicit. Related: 03, 04, 19.

## 2. Stack vs heap is not the full language model

Automatic storage often maps to the stack on Linux, but C specifies lifetimes rather than a mandatory stack layout. Static storage lasts for the program. Allocated storage lasts until freed. Returning `&local` leaves the caller with a dead object. A compound literal at block scope also has a block lifetime.

Exercise: write a function that fills a caller-owned array, then another that returns allocated data plus length. Make their ownership contracts visibly different. Do not mix “borrowed” and “must free” results without a status/ownership convention.

## 3. Integer safety starts before the operation

Signed overflow is undefined behavior. Unsigned arithmetic wraps modulo its range, which is useful for some hashes and dangerous for allocation sizes. To allocate `n * sizeof(T)`, first check `n <= SIZE_MAX / sizeof(T)`. A subtraction comparator such as `return a - b` can overflow; use `(a > b) - (a < b)`.

Shifting by the width of the promoted operand or a negative count is invalid. Small integer operands undergo promotions. Use unsigned operands and a carefully bounded shift count. Related: 01, 04, 17.

## 4. realloc is a relocation operation

```c
void *next = realloc(v->data, new_capacity * sizeof *v->data);
if (!next) {
    /* old allocation still belongs to v */
    return false;
}
v->data = next;
v->capacity = new_capacity;
```

Check size arithmetic before this code. If realloc succeeds, do not keep interior pointers derived from the previous buffer. Prefer indices while a vector can grow. Avoid size-zero realloc here; an explicit free has a clearer contract.

Doubling makes a sequence of pushes amortized O(1), because earlier copying work forms a geometric series. One push can still be O(n). That distinction matters in latency-sensitive software.

## 5. C strings are a representation with a terminator

A string needs a NUL terminator within accessible storage. Arbitrary binary data does not. Use `(pointer, length)` for bytes and a terminator-aware interface for text. Check whether a library function returns length excluding the terminator. A `snprintf` return tells you how many characters would have been written, so compare it with capacity before advancing an offset.

Character-classification functions accept EOF or a value representable as unsigned char. Cast plain `char` before passing it. This course chooses explicit ASCII classification for word matching so locale settings cannot silently alter semantics.

## 6. UTF-8 is not a typography engine

A scalar may take one to four bytes. A lead byte determines expected continuation bytes. Validation rejects overlong forms, surrogate values, out-of-range scalars, and truncated encodings. Persian shaping and right-to-left display are later rendering tasks. Combining marks and zero-width non-joiners mean code point count is not visible-letter count.

Never truncate a Persian string at an arbitrary byte offset and call the result valid text. Extend project 03 with a “cut at scalar boundary” operation before adding Unicode-aware search.

## 7. Parsers need a state and an EOF rule

Write a table: current state, input class, next state, emitted value/error. Include EOF as an input class. CSV's after-quote state distinguishes a doubled quote from a field ending. A recursive-descent parser translates precedence into nested functions; the deepest function handles the tightest-binding syntax.

Input depth and total bytes are independent bounds. A 128-level recursion limit prevents stack exhaustion in the calculator, but a very long flat expression can still take time. Add a byte or instruction budget when the input becomes hostile.

## 8. Modules are a way to make ownership reviewable

Put public declarations in a guarded `.h`; put implementation in `.c`. Use `static` for file-private helpers. An opaque struct lets a module preserve invariants while callers keep a handle. Avoid defining ordinary non-static functions in widely included headers: each translation unit would produce another definition.

The shared reference modules use `static inline` for a compact educational distribution. A larger capstone should move them into `.c` files with declarations in headers. The compiler driver supports multiple project-local `.c` files.

## 9. Translate, link, and inspect

```sh
gcc -std=c17 -E projects/01-byte-lab/main.c > build/preprocessed.c
gcc -std=c17 -c projects/01-byte-lab/main.c -o build/byte.o
nm build/byte.o
objdump -d build/byte.o
```

Create `build/` first. Preprocessing expands includes/macros; compilation checks and translates one unit; linking resolves external symbols. An undefined reference is usually a linkage problem, while an implicit declaration is a declaration problem. A shared library needs a stable ABI, and `-fPIC` permits position-independent code on the targeted Linux toolchain.

## 10. Debug a failing hypothesis

```sh
python3 scripts/course.py build 04
printf 'example\n' > build/input.txt
gdb --args build/learner/normal/04-vector/main 3 -1 2
```

Inside GDB: `break main`, `run`, `next`, `print`, `backtrace`, `watch`, `continue`. First predict what value is wrong. Then stop near the first point where it changes incorrectly. A crash site may be far from the corrupting write.

ASan detects classes of invalid memory access; UBSan detects selected undefined behavior. Neither proves correctness. LeakSanitizer normally runs at orderly exit. In ptrace-restricted runtimes it can fail before reporting leaks; only in that environment use `ASAN_OPTIONS=detect_leaks=0:halt_on_error=1`, and record that leak checks were not performed. Do not use that override in normal CI.

## 11. Tests should challenge an invariant

A test that repeats your algorithm is a weak oracle. Compare vector sorting with Python's sort, bit output with Python's integer representation, and postings with a simple document scan. Generate inputs with a fixed seed so failures reproduce. Separate valid boundary cases from invalid cases.

The course uses black-box process tests with deadlines and captures exit status/stdout/stderr. Add deterministic allocation-failure injection as a stretch task. For a long-running server, forced termination does not establish leak-free graceful shutdown; implement shutdown first if you want an exit-time leak proof.

## 12. POSIX file descriptors are owned resources

A descriptor is a small integer identifying an open description in a process. A fork inherits open descriptors; after an exec they remain open unless close-on-exec is set. A duplicated pipe write end anywhere can postpone EOF. Close every end that the current process does not own.

`read`, `write`, `recv`, and `send` may transfer fewer bytes than requested. Handle positive short counts, zero/EOF, and EINTR separately. A TCP stream does not preserve application message boundaries. Related: 11–13.

## 13. Durability is a sequence, not a magic call

For a normal append: validate, write, flush the stdio buffer, fsync the descriptor, check close. For atomic replacement: write a temporary file in the same directory, flush/fsync it, rename it, and fsync the directory where supported. Still define locks and the recovery format.

The ledger is single-writer and append-only. KV Log replays operations and repairs a torn final line but has no checksums and no transactional crash-atomic promise. Do not extrapolate its tests into a full database guarantee. Study the storage track before adding transactions.

## 14. Concurrency requires ownership or synchronization

Project 16 gives each worker a disjoint input range and private result. The parent reads results after pthread_join. That removes shared-counter races. A queue needs a mutex-protected predicate; condition variables are used in a while loop because waking does not guarantee the predicate is now true.

`volatile` is not a thread synchronization mechanism. C atomics require understanding the memory model. Use a mutex first unless an algorithm's correctness actually demands lock-free primitives. Run ThreadSanitizer in a separate build: it is not combined with AddressSanitizer.

## 15. Complexity and performance need measurements

For every algorithm identify input size and expensive operation. Vector growth is amortized; a hash map is average-case and collision-sensitive; the reference LRU does linear lookup; an inverted index speeds later queries at the cost of build time and memory. A benchmark must distinguish indexing time from query time.

```sh
/usr/bin/time -v build/learner/normal/18-search/main corpus.txt cat
valgrind --leak-check=full build/learner/normal/04-vector/main 3 1 2
```

Run Valgrind on a normal binary, not one instrumented with ASan. Compare identical inputs and flags; avoid performance claims based on one tiny run. First achieve correct results, then profile, then optimize the measured bottleneck.

## 16. A green badge needs a precise meaning

The shipped workflow tests every attempted learner implementation under GCC and Clang, normally and with ASan/UBSan. Untouched starters are “not started,” not falsely passed. Every compiler/instrumentation lane must pass for a project to be checked. A green workflow with zero attempted projects means the course infrastructure and references passed; it does not mean you completed the course.

Tests cover the declared contract, not every possible defect. Public numbers show which reports they came from. Never combine reference passes with learner statistics or manually tick generated checkboxes to imply verified progress.
