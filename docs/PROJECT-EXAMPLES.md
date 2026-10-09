# Run the reference baselines

From the repository root. Build outputs exist only after compiling. First build each selected reference with `python3 scripts/course.py build ID --mode reference`. Replace `reference` with `learner` after implementing your version. The build command prints the binary path.

```sh
# Values, bytes and Persian scalars
build/reference/normal/01-byte-lab/main 255
printf '\000\377A' | build/reference/normal/02-hexview/main
printf 'سلام' | build/reference/normal/03-utf8/main

# Collections and parsers
build/reference/normal/04-vector/main 3 -1 2
printf 'Book book C' | build/reference/normal/05-wordmap/main
printf '"book,title",pages\nC,250\n' | build/reference/normal/06-csv/main
printf 'cat\ndog\nbobcat\n' | build/reference/normal/07-grep/main cat
build/reference/normal/08-arena/main 1 20 reset 4

# Your reading notes and expressions
build/reference/normal/09-ledger/main build/reading.log add 'یادداشت فصل نادر'
build/reference/normal/09-ledger/main build/reading.log list
build/reference/normal/10-calc/main '2 + 3 * (4 - 1)'

# Processes and protocols
build/reference/normal/11-shell/main -c 'seq 1 10000 | wc -l'
printf 'GET /health HTTP/1.1\r\nHost: localhost\r\n\r\n' | build/reference/normal/12-http-parse/main
build/reference/normal/13-http-server/main 8081
# In another terminal: curl http://127.0.0.1:8081/health
# Stop the server with Ctrl+C.

# Storage, caching, concurrency and bytecode
build/reference/normal/14-kvlog/main build/data.kv put book 'Sociology notes'
build/reference/normal/14-kvlog/main build/data.kv get book
build/reference/normal/15-lru/main 2 a b a c
build/reference/normal/16-workers/main 4 100000
printf 'PUSH 2\nPUSH 3\nADD\nPRINT\n' | build/reference/normal/17-vm/main

# Search and Python ABI
build/reference/normal/18-search/main examples/corpus.txt memory
python3 tests/ffi_check.py build/reference/normal/19-ffi/libcourse.so
build/reference/normal/20-doc-api/main 8082 examples/corpus.txt
# In another terminal:
# curl 'http://127.0.0.1:8082/search?q=memory'
```

The shell requires spaces around `|` and has no quoting syntax inside its command string. Corpus matching is ASCII-only in the baseline; UTF-8 validation is a separate capability. Servers listen only on loopback and process connections sequentially. Ctrl+C stops them, but graceful shutdown and exit-time leak accounting are stretch work.
