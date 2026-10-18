# PISE — Minimal Upgrade Notes

## Scope
This is a targeted maintenance pass. The existing classes, API response fields, data structures, and frontend layout were kept intact.

## Changes made

1. **Personalization is now based on prior history.** In both API and console search paths, the current query is recorded after suggestion ranking and page-result scoring. Previously, the query was recorded first, so the request boosted its own result scores with its own frequency and recency values. The query is still recorded before the response is serialized, so the suggestion frequency shown to the user includes the current search.
2. **Duplicate dictionary entries are skipped in the Trie.** `words.txt` contains repeated terms. The Trie already prevented duplicate autocomplete output structurally, but its spelling-correction dictionary retained duplicates and repeated the same edit-distance work. New insertions now add a dictionary entry only when the word is new.
3. **The sample page title is distinct.** `page1.html` now uses `Computer Science Overview`, avoiding two different URLs appearing with the same `Computer Science` title.
4. **README corpus count corrected.** Documentation now reflects the 13 bundled local HTML pages rather than the earlier three-page description.

## Validation performed in this environment

- Compiled and ran a `SearchIndex` + `SearchHistory` smoke test with `-std=c++17 -Wall -Wextra -Wpedantic`.
- Confirmed first-search results have no self-derived personalization bonus and that recorded history affects later scoring.
- Compiled and ran a Trie smoke test covering `computr` → `computer`, `machne` → `machine`, and repeated insertion without duplicate autocomplete output.
- Syntax-checked `src/main.cpp` with a small Winsock declaration stub because this environment is Linux and the project's API server uses Windows Winsock. This does **not** replace the required full Windows/MSYS2 build and live API test.

## Deliberately not changed

- The inverted-index all-terms intersection behavior.
- The API's existing JSON schema and frontend contract.
- The ranking weights and data structures.
- The local crawler architecture and frontend files.

These were left alone to avoid unrelated behavioral changes. The Windows/MSYS2 build and `/search` API should be retested after applying this upgrade.
