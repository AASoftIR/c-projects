# Reference shelf — primary sources first

Consult a source with a specific question from your current project. Do not turn the course into months of reading before building anything.

- C language baseline: [WG14 N1570 C11 committee draft](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf). A publicly available draft, not the final C17 standard; use it for the foundational object/integer/library model with that distinction in mind.
- Compiler diagnostics: [GCC warning options](https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html).
- Memory checks: [Clang AddressSanitizer](https://clang.llvm.org/docs/AddressSanitizer.html), [UndefinedBehaviorSanitizer](https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html), [ThreadSanitizer](https://clang.llvm.org/docs/ThreadSanitizer.html).
- Debugging: [GDB manual](https://sourceware.org/gdb/current/onlinedocs/gdb.html/).
- Linux interfaces: local `man 2 read`, `man 2 write`, `man 2 fork`, `man 2 pipe`, `man 2 socket`, `man 3 pthread_create`; [Linux write(2)](https://man7.org/linux/man-pages/man2/write.2.html), [pthreads(7)](https://man7.org/linux/man-pages/man7/pthreads.7.html).
- Protocol: [RFC 9112 HTTP/1.1](https://www.rfc-editor.org/rfc/rfc9112.html). This course implements a narrow no-body GET subset, not the entire RFC.
- CSV background: [RFC 4180](https://www.rfc-editor.org/rfc/rfc4180.html). The project deliberately uses LF rather than full RFC-style CRLF support.
- Text encoding: [RFC 3629 UTF-8](https://www.rfc-editor.org/rfc/rfc3629.html), [Unicode Standard](https://www.unicode.org/versions/latest/).
- Python ABI: [ctypes documentation](https://docs.python.org/3/library/ctypes.html).
- Persistence: [SQLite atomic commit](https://www.sqlite.org/atomiccommit.html).
- Language project inspiration: [Crafting Interpreters](https://craftinginterpreters.com/), [Lua source](https://www.lua.org/source/).
- Terminal editor inspiration: [Kilo by its author](https://github.com/antirez/kilo).
- Automation: [custom GitHub Pages workflows](https://docs.github.com/en/pages/getting-started-with-github-pages/using-custom-workflows-with-github-pages), [GITHUB_TOKEN permissions](https://docs.github.com/en/actions/tutorials/authenticate-with-github_token).

Famous tools inspire mechanisms and goals. The reference implementations here are original educational baselines and do not claim source or feature compatibility with those tools.
