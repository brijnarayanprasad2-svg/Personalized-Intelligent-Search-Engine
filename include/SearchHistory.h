#ifndef SEARCHHISTORY_H
#define SEARCHHISTORY_H

#include <string>
#include <unordered_map>
#include <ctime>

using namespace std;

class SearchHistory
{
private:
    // Stores how many times each query was searched
    unordered_map<string, int> searchFrequency;

    // Stores the latest search time of each query
    unordered_map<string, time_t> lastSearchTime;
    unordered_map<int, int> pageSearchFrequency;
    unordered_map<string, unordered_map<int, int>> queryPageFrequency;

public:
    void recordSearch(const string& query);

    int getFrequency(const string& query) const;

    time_t getLastSearchTime(
        const string& query
    ) const;

    void recordPageSearch(
        int pageID
    );

    int getPageSearchFrequency(
        int pageID
    ) const;
    void recordQueryPage(
    const string& query,
    int pageID
);

int getQueryPageFrequency(
    const string& query,
    int pageID
) const;

    void displayHistory() const;
};
#endif