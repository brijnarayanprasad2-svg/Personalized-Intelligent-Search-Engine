#include "../include/RankingEngine.h"

#include <queue>

using namespace std;

struct RankedSuggestion
{
    string word;
    int frequency;
};

struct CompareSuggestions
{
    bool operator()(
        const RankedSuggestion& a,
        const RankedSuggestion& b
    ) const
    {
        if (a.frequency != b.frequency)
        {
            return a.frequency < b.frequency;
        }

        return a.word > b.word;
    }
};

vector<string> RankingEngine::rankSuggestions(
    const vector<string>& suggestions,
    const SearchHistory& history)
{
    vector<string> rankedResults;

    priority_queue<
        RankedSuggestion,
        vector<RankedSuggestion>,
        CompareSuggestions
    > rankingQueue;

    for (const string& word : suggestions)
    {
        rankingQueue.push({
            word,
            history.getFrequency(word)
        });
    }

    while (!rankingQueue.empty())
    {
        rankedResults.push_back(
            rankingQueue.top().word
        );

        rankingQueue.pop();
    }

    return rankedResults;
}