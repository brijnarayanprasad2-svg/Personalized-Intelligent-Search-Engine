#include "../include/SearchHistory.h"

#include <iostream>

using namespace std;

void SearchHistory::recordSearch(const string& word)
{
    if (!word.empty())
    {
        frequency[word]++;
    }
}

int SearchHistory::getFrequency(const string& word) const
{
    auto it = frequency.find(word);

    if (it != frequency.end())
    {
        return it->second;
    }

    return 0;
}

void SearchHistory::displayHistory() const
{
    cout << "\n========== SEARCH HISTORY ==========\n";

    if (frequency.empty())
    {
        cout << "No searches recorded yet.\n";
        return;
    }

    for (const auto& item : frequency)
    {
        cout << item.first
             << " -> "
             << item.second
             << " searches\n";
    }
}