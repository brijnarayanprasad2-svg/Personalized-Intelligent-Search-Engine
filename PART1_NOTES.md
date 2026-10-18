# PISE — Part 1: Startup and Local Corpus Fix

## What changed
- `src/main.cpp` now creates `WebCrawler crawler(13)` to match the 13 local HTML pages shipped in `data/`.
- Replaced the single IANA network seed with the 13 local page seeds: ten topic pages and `page1`–`page3`.
- Kept one crawler invocation and builds the search index from the returned pages.

## Why
The previous startup crawled only `https://iana.org/domains/example` with a five-page limit. That does not exercise the local search corpus bundled with the project, so a query such as `computer` could return no local results.

## Validation performed
- Compiled the non-Windows C++ core modules with `g++ -std=c++17 -Wall -Wextra -Wpedantic`.
- Ran a local-corpus smoke test: 13 pages crawled and `computer` returned 2 results.
- The complete Windows API executable was not compiled in this Linux environment because the API module uses Windows Winsock headers (`winsock2.h`). Please compile it on the target Windows/MSYS2 environment as the final check.

## Windows/MSYS2 compile command
Run from this project folder in the same UCRT64 shell where libcurl is installed:

```bash
g++ src/*.cpp -Iinclude -std=c++17 -lcurl -lws2_32 -o search_engine.exe
```

Then run:

```bash
./search_engine.exe
```

API mode:

```bash
./search_engine.exe --api
```
