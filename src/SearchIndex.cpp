#include "../include/SearchIndex.h"

#include <iostream>
#include <sstream>
#include <cctype>
#include <algorithm>

using namespace std;

// ==================================================
// NORMALIZE WORD
// ==================================================

string SearchIndex::normalize(
    const string& word) const
{
    string result;

    for (char ch : word)
    {
        if (isalnum(
                static_cast<unsigned char>(ch)))
        {
            result += static_cast<char>(
                tolower(
                    static_cast<unsigned char>(ch)
                )
            );
        }
    }

    return result;
}

// ==================================================
// TOKENIZE TEXT
// ==================================================

vector<string> SearchIndex::tokenize(
    const string& text) const
{
    vector<string> words;

    string word;

    stringstream stream(text);

    while (stream >> word)
    {
        string cleanWord =
            normalize(word);

        if (!cleanWord.empty())
        {
            words.push_back(cleanWord);
        }
    }

    return words;
}

// ==================================================
// ADD PAGE TO INDEX
// ==================================================

void SearchIndex::addPage(
    int pageID,
    const WebPage& page)
{
    // Store page information
    pages.push_back(page);

    // Combine title + content
    string text =
        page.getTitle() + " " +
        page.getContent();

    vector<string> words =
        tokenize(text);

    // Add every unique word to index
    for (const string& word : words)
    {
        vector<int>& pageList =
            index[word];

        if (find(
                pageList.begin(),
                pageList.end(),
                pageID)
            == pageList.end())
        {
            pageList.push_back(pageID);
        }
    }
}

// ==================================================
// KEYWORD SEARCH
// ==================================================

vector<int> SearchIndex::search(
    const string& keyword) const
{
    string normalized =
        normalize(keyword);

    auto it =
        index.find(normalized);

    if (it != index.end())
    {
        return it->second;
    }

    return {};
}

// ==================================================
// PAGE SEARCH WITH RELEVANCE SCORE
// ==================================================

vector<SearchResult> SearchIndex::searchPages(
    const string& query) const
{
    vector<SearchResult> results;

    vector<string> queryWords =
        tokenize(query);

    if (queryWords.empty())
    {
        return results;
    }

    for (size_t i = 0;
         i < pages.size();
         i++)
    {
        const WebPage& page =
            pages[i];

        string title =
            page.getTitle();

        string content =
            page.getContent();

        string fullText =
            title + " " + content;

        vector<string> pageWords =
            tokenize(fullText);

        double score = 0.0;

        // ------------------------------------------
        // Calculate relevance
        // ------------------------------------------

        for (const string& queryWord :
             queryWords)
        {
            // Word frequency in page
            int frequency = 0;

            for (const string& pageWord :
                 pageWords)
            {
                if (pageWord == queryWord)
                {
                    frequency++;
                }
            }

            // Content match
            score += frequency;

            // Title match gets extra weight
            vector<string> titleWords =
                tokenize(title);

            for (const string& titleWord :
                 titleWords)
            {
                if (titleWord == queryWord)
                {
                    score += 5.0;
                }
            }
        }

        // ------------------------------------------
        // Only return matching pages
        // ------------------------------------------

        if (score > 0)
        {
            results.emplace_back(
                static_cast<int>(i + 1),
                page.getTitle(),
                page.getURL(),
                page.getContent(),
                score
            );
        }
    }

    // ----------------------------------------------
    // Sort by relevance
    // ----------------------------------------------

    sort(
        results.begin(),
        results.end(),
        [](const SearchResult& a,
           const SearchResult& b)
        {
            return a.getScore()
                   > b.getScore();
        }
    );

    return results;
}

// ==================================================
// DISPLAY INDEX
// ==================================================

void SearchIndex::displayIndex() const
{
    cout << "\n========== SEARCH INDEX ==========\n";

    for (const auto& item : index)
    {
        cout << item.first
             << " -> ";

        for (int pageID :
             item.second)
        {
            cout << "Page "
                 << pageID
                 << " ";
        }

        cout << endl;
    }
}