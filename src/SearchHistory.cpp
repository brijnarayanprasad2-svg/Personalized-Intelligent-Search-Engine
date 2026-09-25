#include "../include/SearchHistory.h"

#include <iostream>
#include <cctype>
#include <ctime>

using namespace std;


// ==================================================
// NORMALIZE SEARCH QUERY
// ==================================================

string normalizeQuery(const string& query)
{
    string result;

    for (char ch : query)
    {
        unsigned char c =
            static_cast<unsigned char>(ch);

        if (isalnum(c) || ch == ' ')
        {
            result +=
                static_cast<char>(tolower(c));
        }
    }

    return result;
}


// ==================================================
// RECORD SEARCH
// ==================================================

void SearchHistory::recordSearch(
    const string& query)
{
    string normalized =
        normalizeQuery(query);

    if (normalized.empty())
    {
        return;
    }

    // Increase search frequency
    searchFrequency[normalized]++;

    // Store current time
    lastSearchTime[normalized] =
        time(nullptr);
}


// ==================================================
// GET SEARCH FREQUENCY
// ==================================================

int SearchHistory::getFrequency(
    const string& query) const
{
    string normalized =
        normalizeQuery(query);

    auto it =
        searchFrequency.find(normalized);

    if (it != searchFrequency.end())
    {
        return it->second;
    }

    return 0;
}


// ==================================================
// GET LAST SEARCH TIME
// ==================================================

time_t SearchHistory::getLastSearchTime(
    const string& query) const
{
    string normalized =
        normalizeQuery(query);

    auto it =
        lastSearchTime.find(normalized);

    if (it != lastSearchTime.end())
    {
        return it->second;
    }

    return 0;
}

void SearchHistory::recordPageSearch(int pageID)
{
    pageSearchFrequency[pageID]++;
}

int SearchHistory::getPageSearchFrequency(int pageID) const
{
    auto it = pageSearchFrequency.find(pageID);

    if (it != pageSearchFrequency.end())
    {
        return it->second;
    }

    return 0;
}
void SearchHistory::recordQueryPage(
    const string& query,
    int pageID)
{
    string normalized = normalizeQuery(query);

    if (normalized.empty())
    {
        return;
    }

    queryPageFrequency[normalized][pageID]++;
}

int SearchHistory::getQueryPageFrequency(
    const string& query,
    int pageID) const
{
    string normalized = normalizeQuery(query);

    auto queryIt = queryPageFrequency.find(normalized);

    if (queryIt != queryPageFrequency.end())
    {
        auto pageIt =
            queryIt->second.find(pageID);

        if (pageIt != queryIt->second.end())
        {
            return pageIt->second;
        }
    }

    return 0;
}


// ==================================================
// DISPLAY SEARCH HISTORY
// ==================================================

void SearchHistory::displayHistory() const
{
    cout << "\n========== SEARCH HISTORY ==========\n";

    if (searchFrequency.empty())
    {
        cout << "No searches recorded.\n";
        return;
    }

    for (const auto& item :
         searchFrequency)
    {
        cout << item.first
             << " -> "
             << item.second
             << " searches";

        auto timeIt =
            lastSearchTime.find(item.first);

        if (timeIt != lastSearchTime.end())
        {
            cout << " | Last searched: "
                 << ctime(&timeIt->second);
        }
        else
        {
            cout << endl;
        }
    }
}