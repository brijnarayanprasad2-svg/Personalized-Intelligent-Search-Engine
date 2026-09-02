#ifndef SEARCHHISTORY_H
#define SEARCHHISTORY_H

#include <string>
#include <unordered_map>

using namespace std;

class SearchHistory
{
private:
    unordered_map<string, int> frequency;

public:
    void recordSearch(const string& word);

    int getFrequency(const string& word) const;

    void displayHistory() const;
};

#endif