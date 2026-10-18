# Personalized Intelligent Search Engine (PISE)

### A C++ Search System Based on Data Structures and Object-Oriented Programming

A B.Tech Computer Science and Engineering Project at Graphic Era (Deemed to be University).

The **Personalized Intelligent Search Engine (PISE)** is a C++ project that demonstrates how data structures, algorithms, and object-oriented programming can be used to build the core components of a search system.

The current Phase 1 implementation works with a collection of local HTML pages. It supports keyword searching, autocomplete, search history, relevance-based ranking, local crawling, an inverted index, basic query correction, and a web-based frontend connected to a local API.

---

## Table of Contents

- [Project Overview](#project-overview)
- [Current Implementation](#current-implementation)
- [Features](#features)
- [Technology Stack](#technology-stack)
- [Data Structures and Algorithms](#data-structures-and-algorithms)
- [Object-Oriented Programming](#object-oriented-programming)
- [Project Architecture](#project-architecture)
- [Project Folder Structure](#project-folder-structure)
- [Requirements](#requirements)
- [Build Instructions](#build-instructions)
- [How to Run the Project](#how-to-run-the-project)
- [Testing and Verification](#testing-and-verification)
- [Current Scope and Limitations](#current-scope-and-limitations)
- [Future Improvements](#future-improvements)
- [Repository](#repository)

---

## Project Overview

Search engines involve several components working together, including vocabulary management, query processing, indexing, ranking, and result presentation.

PISE implements a simplified version of these components to demonstrate their underlying concepts using C++.

The project is organized into separate classes and modules to make the code easier to understand, test, and extend.

### Main objectives

- Implement efficient prefix searching and autocomplete using a Trie.
- Maintain search history and search-frequency information.
- Crawl and parse local HTML pages.
- Build an inverted index for keyword-based document retrieval.
- Rank search results using keyword frequency and title relevance.
- Support basic spelling correction and query normalization.
- Connect the C++ search system to a browser-based frontend through a local API.

## Current Implementation

The current project includes the following major parts:

1. **Search engine core:** Vocabulary management, query processing, search history, and ranking.
2. **Local web crawler:** Processes a collection of local HTML pages.
3. **HTML parser:** Extracts relevant page content, titles, and links.
4. **Inverted search index:** Maps normalized terms to the pages containing them.
5. **HTTP client and API server:** Provides communication between the C++ application and the frontend.
6. **Web frontend:** Provides the search interface and a separate details page.
7. **Documentation:** Includes project notes and records of implementation updates.

The repository contains a local corpus of HTML pages in the `data/` directory.

---

## Features

### 1. Trie-Based Search and Autocomplete

The Trie data structure stores words and supports prefix-based vocabulary lookup.

Current capabilities include:

- Exact word lookup.
- Prefix searching.
- Autocomplete suggestions.
- Dictionary-based vocabulary management.

### 2. Search History

The search history module maintains information about previous successful searches.

Search-frequency information can be used by the ranking component to prioritize frequently searched suggestions.

### 3. Ranking Engine

The ranking engine orders suggestions using search-frequency information.

For document search, the search index also calculates relevance using keyword frequency and additional weight for title matches.

### 4. Local Web Crawler

The crawler processes local HTML documents rather than depending on a live search engine API for its core search functionality.

Its implementation uses:

- A queue for URL processing.
- A set for tracking visited URLs.
- HTML parsing for titles, paragraphs, and links.
- A configurable collection of local pages.

The current repository includes multiple HTML documents in `data/`.

### 5. Inverted Search Index

The inverted index maps normalized terms to the documents in which they occur.

This allows the application to retrieve relevant pages based on search keywords without scanning the complete text of every document for every query.

### 6. Keyword-Based Document Search

Users can search the indexed local pages using keywords.

The current ranking approach considers:

- Keyword frequency.
- Matches found in page titles.
- A relevance score calculated by the search index.

### 7. Basic Query Correction and Normalization

The search implementation includes basic query-processing improvements.

The tested examples include:

- `machine learning` — searches for the corresponding indexed content.
- `protocol` — matches the indexed Internet Protocols page.
- `internet protocols` — normalizes the plural query and retrieves the relevant page.
- `computr` — used to test spelling-correction behavior.
- `zzxyz123` — used to test an invalid or unmatched query.

Results can depend on the current index, vocabulary, and local page content.

### 8. C++ API Server

The project includes an API server that allows the frontend to communicate with the C++ search system.

The local API server is launched using:

```bash
./search_engine.exe --api
```

The configured local server address is:

`http://127.0.0.1:8080`

### 9. Browser-Based Frontend

The `frontend/` directory contains the web interface.

It includes:

- `index.html` — main search page.
- `script.js` — frontend interactions and API communication.
- `style.css` — main interface styling.
- `details.html` — separate details page.
- `details.js` — details-page behavior.
- `details.css` — details-page styling.

The frontend is served locally during development.

---

## Technology Stack

| Technology | Purpose |
|---|---|
| C++ | Search engine logic and backend modules |
| C++17 | Language standard used by the build command |
| HTML | Local documents and frontend pages |
| CSS | Frontend styling |
| JavaScript | Browser interactions and API communication |
| libcurl | HTTP client functionality |
| Git and GitHub | Version control and project hosting |
| Python HTTP server | Local frontend development server |

---

## Data Structures and Algorithms

| Data structure or concept | Application |
|---|---|
| Trie | Word lookup and autocomplete |
| Hash tables | Search history and inverted-index storage |
| Queue | Crawler URL processing |
| Set | Tracking visited URLs |
| Priority queue | Ranking suggestions, where used by the ranking implementation |
| Inverted index | Mapping terms to documents |
| Vector | Storing collections of words, pages, and results |
| String processing | Query normalization and text handling |

These structures demonstrate how different data structures can be combined to solve parts of a search problem.

---

## Object-Oriented Programming

The project is organized around separate C++ classes.

The main OOP concepts demonstrated include:

- Classes and objects.
- Encapsulation.
- Abstraction through module interfaces.
- Separation of responsibilities.
- Reusability and modular design.

The project also contains multiple cooperating classes for search, indexing, crawling, parsing, and HTTP communication.

---

## Project Architecture

The following diagram represents the main logical flow of the current implementation.

```text
              User
               |
               v
       Browser Frontend
        HTML / CSS / JS
               |
               v
         Local API Server
               |
               v
       Query Processing
       /             \
      v               v
  Trie / History   Search Index
      |               |
      v               v
 Suggestions       Local Pages
      |               ^
      |               |
      |          HTML Parser
      |               ^
      |               |
      |          Web Crawler
      |               |
      |               v
      |          data/*.html
      |
      v
   Ranking Engine
      |
      v
 Search Suggestions / Results
```

The exact execution path depends on the selected operation. The diagram illustrates the principal components and their intended responsibilities rather than every internal function call.

---

## Project Folder Structure

The following tree reflects the main files currently present in the Git repository.

```text
Personalized-Intelligent-Search-Engine/
│
├── data/
│   ├── algorithms.html
│   ├── artificial_intelligence.html
│   ├── computer_science.html
│   ├── cpp_programming.html
│   ├── data_science.html
│   ├── data_structures.html
│   ├── database_management.html
│   ├── internet_protocols.html
│   ├── machine_learning.html
│   ├── object_oriented_programming.html
│   ├── page1.html
│   ├── page2.html
│   ├── page3.html
│   └── words.txt
│
├── frontend/
│   ├── index.html
│   ├── script.js
│   ├── style.css
│   ├── details.html
│   ├── details.js
│   └── details.css
│
├── include/
│   ├── ApiServer.h
│   ├── HTMLParser.h
│   ├── HttpClient.h
│   ├── RankingEngine.h
│   ├── SearchData.h
│   ├── SearchHistory.h
│   ├── SearchIndex.h
│   ├── SearchResult.h
│   ├── Trie.h
│   ├── TrieNode.h
│   ├── WebCrawler.h
│   ├── WebPage.h
│   └── WebPageBuilder.h
│
├── src/
│   ├── ApiServer.cpp
│   ├── HTMLParser.cpp
│   ├── HttpClient.cpp
│   ├── RankingEngine.cpp
│   ├── SearchData.cpp
│   ├── SearchHistory.cpp
│   ├── SearchIndex.cpp
│   ├── SearchResult.cpp
│   ├── Trie.cpp
│   ├── TrieNode.cpp
│   ├── WebCrawler.cpp
│   ├── WebPage.cpp
│   ├── WebPageBuilder.cpp
│   └── main.cpp
│
├── .gitignore
├── PART1_NOTES.md
├── SEARCH_FIX_NOTES.md
├── UPGRADE_NOTES.md
└── README.md
```

**Note:** `search_engine.exe` is a local compiled executable and is excluded from Git tracking by `.gitignore`.

---

## Requirements

Before building the project, ensure that you have:

- A C++ compiler with C++17 support.
- A compatible libcurl development library.
- A Windows-compatible toolchain if using the supplied Windows build configuration.
- Python 3 for serving the frontend locally.
- A modern web browser.

The compiler, libcurl headers, and libraries must come from a compatible toolchain. The commands below use a shell that supports the Unix-style `./search_engine.exe` invocation and assumes libcurl and the required Windows socket library are correctly installed.

---

## Build Instructions

Open the project root directory in the **MSYS2 UCRT64 terminal** or another compatible shell.

Compile the C++ source files:

```bash
g++ src/*.cpp -Iinclude -std=c++17 -lcurl -lws2_32 -o search_engine.exe
```

If compilation succeeds, the executable will be created as:

```text
search_engine.exe
```

If the compiler reports missing `curl/curl.h` or linker errors, verify that the libcurl development package is installed and that the compiler is using the correct include and library paths.

---

## How to Run the Project

### Option 1: Run the command-line application

From the project root:

```bash
./search_engine.exe
```

Follow the prompts displayed by the application.

### Option 2: Run the local API server

Open a terminal in the project root and execute:

```bash
./search_engine.exe --api
```

The API server is configured to run at:

`http://127.0.0.1:8080`

Keep this terminal open while using the frontend.

### Option 3: Run the frontend

Open a second terminal and navigate to the frontend directory:

```powershell
cd frontend
py -m http.server 5500
```

Open the following address in your browser:

`http://127.0.0.1:5500/`

Keep both the API server and the frontend server running while testing the browser interface.

---

## Testing and Verification

The project has been manually tested using representative search queries through the frontend.

| Query | Purpose |
|---|---|
| `machine learning` | Normal multi-word search |
| `protocol` | Singular keyword matching |
| `internet protocols` | Plural-query normalization |
| `computr` | Spelling-correction test |
| `zzxyz123` | Unmatched-query test |

The frontend test run reported all five cases passing in the development conversation.

This is a record of those manual checks, not a claim that automated unit tests or comprehensive integration tests are present in the repository.

### GitHub repository verification

The repository was pushed to the `main` branch, and the latest recorded commit is:

`3392dfe` — `Fix search matching and update frontend`

The local Git status was also checked and reported a clean working tree.

---

## Current Scope and Limitations

The current implementation is a learning-focused Phase 1 search system.

- Search is based on the available local HTML corpus.
- It is not a complete web-scale search engine.
- Search quality depends on the indexed documents, vocabulary, and ranking rules.
- Basic spelling correction and plural normalization do not guarantee correction of every possible query.
- The frontend requires the local API server to be running for API-backed functionality.
- Manual test cases do not replace comprehensive automated testing.

---

## Future Improvements

Possible future enhancements include:

- Add more local documents and improve corpus management.
- Improve spelling correction and query normalization.
- Refine relevance scoring and result ranking.
- Add automated tests for the Trie, index, crawler, and API.
- Improve error handling and loading states in the frontend.
- Add search analytics and more detailed result information.
- Improve documentation with screenshots and measured performance results.
- Explore more advanced indexing and personalization methods in later phases.

These are proposed improvements and should not be considered completed features unless they are implemented and verified.

---

## Repository

**GitHub:**  
https://github.com/brijnarayanprasad2-svg/Personalized-Intelligent-Search-Engine

---

## Conclusion

The Personalized Intelligent Search Engine demonstrates how fundamental data structures, algorithms, and OOP concepts can be combined to create a modular search system.

The current phase brings together a C++ search backend, a local web crawler, an inverted index, query-processing improvements, a local API, and a browser-based interface. The project provides a foundation for testing and extending search-engine concepts in future development phases.
