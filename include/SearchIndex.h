#ifndef SEARCHINDEX_H
#define SEARCHINDEX_H

#include "WebPage.h"
#include "SearchResult.h"

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class SearchIndex
{
private:
    unordered_map<string, vector<int>> index;

    vector<WebPage> pages;

    vector<string> tokenize(
        const string& text
    ) const;

    string normalize(
        const string& word
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
        const string& query
    ) const;

    void displayIndex() const;
};

#endif