#ifndef RANKINGENGINE_H
#define RANKINGENGINE_H

#include <string>
#include <vector>

#include "SearchHistory.h"

using namespace std;

class RankingEngine
{
public:

    vector<string> rankSuggestions(
        const vector<string>& suggestions,
        const SearchHistory& history
    );
};

#endif