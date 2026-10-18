# Focused search fixes

This patch addresses the failures seen in API tests without changing the API schema, ranking weights, frontend layout, or inverted-index architecture.

1. Dictionary loading now inserts both the original dictionary entry (preserving phrase autocomplete) and its individual tokens. This prevents a valid word such as `learning` from being treated as unknown merely because it appeared in `words.txt` as part of `machine learning`.
2. SearchIndex word normalization applies a conservative trailing-`s` rule to longer tokens (excluding tokens ending in `es`, `ss`, `us`, and `is` to avoid damaging common words such as `classes`). Thus `protocol` and `protocols` use the same inverted-index key, helping singular/plural queries match.

## Windows/MSYS2 validation

Rebuild and restart the API server after replacing files:

```bash
g++ src/*.cpp -Iinclude -std=c++17 -Wall -Wextra -Wpedantic -lcurl -lws2_32 -o search_engine.exe
./search_engine.exe --api
```

Then test from PowerShell:

```powershell
Invoke-RestMethod "http://127.0.0.1:8080/search?q=machine%20learning" | ConvertTo-Json -Depth 8
Invoke-RestMethod "http://127.0.0.1:8080/search?q=protocol" | ConvertTo-Json -Depth 8
Invoke-RestMethod "http://127.0.0.1:8080/search?q=internet%20protocols" | ConvertTo-Json -Depth 8
```

Expected: `machine learning` remains uncorrected and returns the machine-learning page; `protocol` and `internet protocols` should find the internet-protocols page. Full Windows build/live API verification must be performed on the user's machine.
