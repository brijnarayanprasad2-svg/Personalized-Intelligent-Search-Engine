# Personalized Intelligent Search Engine

### A Data Structures and Object-Oriented Programming Based Search System

A C++ based search engine project developed as part of the B.Tech Computer Science and Engineering PBL project at Graphic Era (Deemed to be University).

The project demonstrates how Data Structures and Object-Oriented Programming concepts can be combined to build the basic components of a personalized search system without depending on a search engine API as the core mechanism.

---

## Project Overview

The Personalized Intelligent Search Engine is designed to perform basic search-engine operations such as:

- Exact word searching
- Autocomplete suggestions
- Search history tracking
- Frequency-based ranking
- Local HTML page crawling
- Inverted search indexing
- Web page searching
- Object-Oriented software organization

Phase 1 focuses on building and integrating the fundamental components required for the search system.

---

## Features

### 1. Trie-Based Search

A Trie data structure is used to store the search vocabulary and perform efficient:

- Exact word searching
- Prefix searching
- Autocomplete

### 2. Search History

The system records successful searches and maintains the frequency of searched words.

This information is used by the ranking component to personalize suggestions.

### 3. Ranking Engine

Search suggestions are ranked using search frequency.

Suggestions searched more frequently receive a higher ranking.

### 4. Local Web Crawler

A controlled crawler is implemented to process local HTML pages.

The crawler uses:

- Queue for URL processing
- Set for visited URL tracking
- HTML parsing for titles, paragraphs and links

The current implementation works with three local HTML pages.

### 5. Search Index

An inverted index is created from the crawled pages.

It maps normalized keywords to the pages where they occur.

### 6. Page Search

Users can search the crawled pages using keywords.

The system calculates a simple relevance score based on keyword frequency and gives additional weight to matches found in page titles.

---

## Data Structures Used

The project integrates several important Data Structures:

| Data Structure | Purpose |
|---|---|
| Trie | Exact search and autocomplete |
| Hash Table | Search history and indexing |
| Queue | Web crawler URL processing |
| Set | Tracking visited URLs |
| Priority Queue | Ranking search suggestions |
| Inverted Index | Mapping keywords to web pages |
| Vector | Storing words, pages and results |

---

## Object-Oriented Programming Concepts

The project is organized using C++ classes and demonstrates:

- Classes and Objects
- Encapsulation
- Abstraction
- Modularity
- Reusability
- Basic Polymorphism through class-based design

Major classes include:

- `Trie`
- `TrieNode`
- `SearchData`
- `SearchHistory`
- `RankingEngine`
- `WebCrawler`
- `WebPage`
- `SearchIndex`
- `SearchResult`

---

## Project Structure

```text
phase1/
│
├── data/
│   ├── page1.html
│   ├── page2.html
│   ├── page3.html
│   └── words.txt
│
├── include/
│   ├── RankingEngine.h
│   ├── SearchData.h
│   ├── SearchHistory.h
│   ├── SearchIndex.h
│   ├── SearchResult.h
│   ├── Trie.h
│   ├── TrieNode.h
│   ├── WebCrawler.h
│   └── WebPage.h
│
└── src/
    ├── main.cpp
    ├── RankingEngine.cpp
    ├── SearchData.cpp
    ├── SearchHistory.cpp
    ├── SearchIndex.cpp
    ├── SearchResult.cpp
    ├── Trie.cpp
    ├── TrieNode.cpp
    ├── WebCrawler.cpp
    └── WebPage.cpp