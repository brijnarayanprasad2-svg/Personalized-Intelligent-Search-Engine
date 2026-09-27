#ifndef SEARCHINDEX_H
#define SEARCHINDEX_H

#include "WebPage.h"
#include "SearchResult.h"
#include "SearchHistory.h"

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class SearchIndex
{
private:

    // Inverted index:
    // word -> (page ID -> frequency)
    unordered_map<
        string,
        unordered_map<int, int>
    > index;

    // Stores all crawled pages
    vector<WebPage> pages;

    // Normalize a single word
    string normalize(
        const string& word
    ) const;

    // Convert text into normalized words
    vector<string> tokenize(
        const string& text
    ) const;

    // Normalize complete query
    // and remove duplicate words
    string normalizeQuery(
        const string& query
    ) const;

public:

    void addPage(
        int pageID,
        const WebPage& page
    );

    vector<int> search(
        const string& keyword
    ) const;

    vector<SearchResult> searchPages(
        const string& query,
        const SearchHistory& history
    ) const;

    void displayIndex() const;
};

#endif