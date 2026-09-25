#ifndef RANKINGENGINE_H
#define RANKINGENGINE_H

#include <string>
#include <vector>

#include "SearchHistory.h"

using namespace std;


struct RankedSuggestion
{
    string word;
    int frequency;
    long long recencyScore;
    double personalizationScore;
    double relevanceScore;
};


class RankingEngine
{
public:
    vector<string> rankSuggestions(
        const vector<string>& words,
        const string& query,
        const SearchHistory& history
    );
};

#endif