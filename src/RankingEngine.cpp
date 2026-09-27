#include "../include/RankingEngine.h"

#include <queue>
#include <ctime>
#include <cctype>

using namespace std;


// ==================================================
// NORMALIZE RANKING TEXT
// ==================================================

string normalizeRankingText(const string& text)
{
    string result;

    for (char ch : text)
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
// COMPARATOR FOR PERSONALIZED RANKING
// ==================================================

struct CompareSuggestions
{
    bool operator()(
        const RankedSuggestion& a,
        const RankedSuggestion& b
    ) const
    {
        // 1. Higher personalization score gets priority
        if (a.personalizationScore !=
            b.personalizationScore)
        {
            return a.personalizationScore <
                   b.personalizationScore;
        }

        // 2. More recently searched word gets priority
        if (a.recencyScore !=
            b.recencyScore)
        {
            return a.recencyScore <
                   b.recencyScore;
        }

        // 3. Higher search frequency gets priority
        if (a.frequency != b.frequency)
        {
            return a.frequency < b.frequency;
        }

        // 4. Final alphabetical tie-breaker
        return a.word > b.word;
    }
};


// ==================================================
// RANK SUGGESTIONS
// ==================================================

vector<string> RankingEngine::rankSuggestions(
    const vector<string>& suggestions,
    const string& query,
    const SearchHistory& history)
{
    vector<string> rankedResults;

    priority_queue<
        RankedSuggestion,
        vector<RankedSuggestion>,
        CompareSuggestions
    > rankingQueue;

    time_t currentTime = time(nullptr);

    string normalizedQuery =
        normalizeRankingText(query);


    // ==================================================
    // CALCULATE SCORE FOR EACH SUGGESTION
    // ==================================================

    for (const string& word : suggestions)
    {
        int frequency =
            history.getFrequency(word);

        time_t lastSearch =
            history.getLastSearchTime(word);


        // --------------------------------------------------
        // RECENCY BONUS
        // --------------------------------------------------

        double recencyBonus = 0;

        if (lastSearch != 0)
        {
            long long secondsAgo =
                static_cast<long long>(
                    currentTime - lastSearch
                );

           if (secondsAgo < 60)
            {
              recencyBonus = 8;
            }
            else if (secondsAgo < 300)
            {
             recencyBonus = 5;
            }
             else if (secondsAgo < 3600)
            { 
              recencyBonus = 2;
            }
        }


        // --------------------------------------------------
        // QUERY RELEVANCE
        // --------------------------------------------------

        string normalizedWord =
            normalizeRankingText(word);

        double relevanceScore = 0;

        // Exact query match
        if (normalizedWord == normalizedQuery)
        {
            relevanceScore = 10;
        }

        // Suggestion starts with current query
        else if (
            normalizedWord.find(normalizedQuery) == 0)
        {
            relevanceScore = 5;
        }


        // --------------------------------------------------
        // PERSONALIZATION SCORE
        // --------------------------------------------------

       double frequencyScore = frequency * 10;

       double personalizationScore =
       frequencyScore
       + recencyBonus
       + relevanceScore;


        // Add suggestion to priority queue
        rankingQueue.push({
            word,
            frequency,
            static_cast<long long>(lastSearch),
            personalizationScore,
            relevanceScore
        });
    }


    // ==================================================
    // EXTRACT RANKED RESULTS
    // ==================================================

    while (!rankingQueue.empty())
    {
        rankedResults.push_back(
            rankingQueue.top().word
        );

        rankingQueue.pop();
    }


    return rankedResults;
}