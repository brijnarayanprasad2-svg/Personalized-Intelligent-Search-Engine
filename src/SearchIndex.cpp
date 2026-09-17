#include "../include/SearchIndex.h"

#include <iostream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <ctime>

using namespace std;


// ==================================================
// RANKING CONSTANTS
// ==================================================

const double TITLE_MATCH_SCORE = 5.0;
const double MULTI_WORD_BONUS = 2.0;
const double EXACT_TITLE_BONUS = 10.0;
const double PHRASE_MATCH_BONUS = 5.0;


// ==================================================
// NORMALIZE WORD
// ==================================================

string SearchIndex::normalize(
    const string& word) const
{
    string result;

    for (char ch : word)
    {
        unsigned char c =
            static_cast<unsigned char>(ch);

        if (isalnum(c))
        {
            result +=
                static_cast<char>(tolower(c));
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
// NORMALIZE COMPLETE QUERY
// ==================================================

string SearchIndex::normalizeQuery(
    const string& query) const
{
    vector<string> words =
        tokenize(query);

    vector<string> uniqueWords;

    // Remove duplicate words
    // while preserving original order
    for (const string& word : words)
    {
        if (find(
                uniqueWords.begin(),
                uniqueWords.end(),
                word)
            == uniqueWords.end())
        {
            uniqueWords.push_back(word);
        }
    }

    string result;

    for (const string& word :
         uniqueWords)
    {
        if (!result.empty())
        {
            result += " ";
        }

        result += word;
    }

    return result;
}


// ==================================================
// ADD PAGE TO INDEX
// ==================================================

void SearchIndex::addPage(
    int pageID,
    const WebPage& page)
{
    pages.push_back(page);

    string text =
        page.getTitle() + " " +
        page.getContent();

    vector<string> words =
        tokenize(text);

    for (const string& word : words)
    {
        index[word][pageID]++;
    }
}


// ==================================================
// KEYWORD SEARCH
// ==================================================

vector<int> SearchIndex::search(
    const string& keyword) const
{
    vector<int> pageIDs;

    string normalized =
        normalize(keyword);

    if (normalized.empty())
    {
        return pageIDs;
    }

    auto it =
        index.find(normalized);

    if (it == index.end())
    {
        return pageIDs;
    }

    vector<pair<int, int>> pageData;

    for (const auto& item :
         it->second)
    {
        pageData.push_back(item);
    }

    sort(
        pageData.begin(),
        pageData.end(),
        [](const pair<int, int>& a,
           const pair<int, int>& b)
        {
            // Higher frequency first
            if (a.second != b.second)
            {
                return a.second > b.second;
            }

            // Smaller page ID first
            return a.first < b.first;
        }
    );

    for (const auto& item :
         pageData)
    {
        pageIDs.push_back(item.first);
    }

    return pageIDs;
}


// ==================================================
// PAGE SEARCH
// RELEVANCE + PERSONALIZATION
// ==================================================

vector<SearchResult> SearchIndex::searchPages(
    const string& query,
    const SearchHistory& history) const
{
    vector<SearchResult> results;


    // ==================================================
    // NORMALIZE QUERY
    // ==================================================

    string normalizedQuery =
        normalizeQuery(query);

    if (normalizedQuery.empty())
    {
        return results;
    }

    vector<string> queryWords =
        tokenize(normalizedQuery);


    // ==================================================
    // CHECK EVERY PAGE
    // ==================================================

    for (size_t i = 0;
         i < pages.size();
         i++)
    {
        const WebPage& page =
            pages[i];

        int pageID =
            static_cast<int>(i + 1);

        double score = 0.0;


        // ==================================================
        // PERSONALIZATION COMPONENT 1
        // QUERY FREQUENCY
        // ==================================================

        double queryFrequencyScore =
            history.getFrequency(query) * 2.0;


        // ==================================================
        // PERSONALIZATION COMPONENT 2
        // PAGE FREQUENCY
        // ==================================================

        double pageFrequencyScore =
            history.getPageSearchFrequency(
                pageID
            ) * 2.0;


        // ==================================================
        // PERSONALIZATION COMPONENT 3
        // QUERY-PAGE PREFERENCE
        // ==================================================

        double queryPageScore =
            history.getQueryPageFrequency(
                query,
                pageID
            ) * 3.0;


        // ==================================================
        // PERSONALIZATION COMPONENT 4
        // RECENCY
        // ==================================================

        double recencyBonus = 0.0;

        time_t lastSearch =
            history.getLastSearchTime(query);

        if (lastSearch != 0)
        {
            long long secondsAgo =
                static_cast<long long>(
                    time(nullptr) - lastSearch
                );

            if (secondsAgo < 60)
            {
                recencyBonus = 3.0;
            }
            else if (secondsAgo < 300)
            {
                recencyBonus = 2.0;
            }
            else if (secondsAgo < 3600)
            {
                recencyBonus = 1.0;
            }
        }


        // ==================================================
        // TOTAL PERSONALIZATION SCORE
        // ==================================================

        double personalizationScore =
            queryFrequencyScore
            + pageFrequencyScore
            + queryPageScore
            + recencyBonus;


        // ==================================================
        // TOKENIZE PAGE TITLE
        // ==================================================

        vector<string> titleWords =
            tokenize(page.getTitle());


        // ==================================================
        // WORD FREQUENCY + TITLE MATCH
        // ==================================================

        int matchedWords = 0;

        for (const string& queryWord :
             queryWords)
        {
            auto indexIt =
                index.find(queryWord);

            if (indexIt == index.end())
            {
                continue;
            }

            auto pageIt =
                indexIt->second.find(pageID);

            if (pageIt ==
                indexIt->second.end())
            {
                continue;
            }


            // ----------------------------------------------
            // WORD FREQUENCY
            // ----------------------------------------------

            score += pageIt->second;

            matchedWords++;


            // ----------------------------------------------
            // TITLE MATCH BONUS
            // ----------------------------------------------

            for (const string& titleWord :
                 titleWords)
            {
                if (titleWord == queryWord)
                {
                    score += TITLE_MATCH_SCORE;
                    break;
                }
            }
        }


        // ==================================================
        // MULTI-WORD QUERY MATCH
        // ==================================================

        bool allWordsMatched =
            (matchedWords ==
             static_cast<int>(
                 queryWords.size()
             ));


        if (matchedWords > 1)
        {
            score +=
                matchedWords *
                MULTI_WORD_BONUS;
        }


        // Extra bonus when ALL query words
        // are present on the page
        if (queryWords.size() > 1 &&
            allWordsMatched)
        {
            score +=
                MULTI_WORD_BONUS;
        }


        // ==================================================
        // CREATE NORMALIZED TITLE
        // ==================================================

        string normalizedTitle;

        for (const string& titleWord :
             titleWords)
        {
            if (!normalizedTitle.empty())
            {
                normalizedTitle += " ";
            }

            normalizedTitle += titleWord;
        }


        // ==================================================
        // CREATE NORMALIZED CONTENT
        // ==================================================

        vector<string> contentWords =
            tokenize(page.getContent());

        string normalizedContent;

        for (const string& contentWord :
             contentWords)
        {
            if (!normalizedContent.empty())
            {
                normalizedContent += " ";
            }

            normalizedContent += contentWord;
        }


        // ==================================================
        // PHRASE MATCH BONUS
        // ==================================================

        if (queryWords.size() > 1)
        {
            string searchText =
                " " +
                normalizedContent +
                " ";

            string searchPhrase =
                " " +
                normalizedQuery +
                " ";

            if (searchText.find(searchPhrase)
                != string::npos)
            {
                score +=
                    PHRASE_MATCH_BONUS;
            }
        }


        // ==================================================
        // EXACT TITLE MATCH
        // ==================================================

        if (normalizedTitle ==
            normalizedQuery)
        {
            score +=
                EXACT_TITLE_BONUS;
        }


        // ==================================================
        // CREATE SEARCH RESULT
        // ==================================================

        if (score > 0)
        {
            SearchResult result(
                pageID,
                page.getTitle(),
                page.getURL(),
                page.getContent(),
                score
            );


            // ----------------------------------------------
            // STORE PERSONALIZATION
            // ----------------------------------------------

            result.setPersonalizationScore(
                personalizationScore
            );


            // ----------------------------------------------
            // STORE SCORE COMPONENTS
            // ----------------------------------------------

            result.setQueryFrequencyScore(
                queryFrequencyScore
            );

            result.setPageFrequencyScore(
                pageFrequencyScore
            );

            result.setQueryPageScore(
                queryPageScore
            );

            result.setRecencyBonus(
                recencyBonus
            );


            // ----------------------------------------------
            // ADD RESULT
            // ----------------------------------------------

            results.push_back(result);
        }
    }


    // ==================================================
    // FINAL RANKING
    // RELEVANCE + PERSONALIZATION
    // ==================================================

    sort(
        results.begin(),
        results.end(),
        [](const SearchResult& a,
           const SearchResult& b)
        {
            double finalScoreA =
                a.getScore()
                + a.getPersonalizationScore();

            double finalScoreB =
                b.getScore()
                + b.getPersonalizationScore();


            // Higher final score first
            if (finalScoreA != finalScoreB)
            {
                return finalScoreA >
                       finalScoreB;
            }


            // Smaller page ID as tie-breaker
            return a.getPageID() <
                   b.getPageID();
        }
    );


    return results;
}


// ==================================================
// DISPLAY SEARCH INDEX
// ==================================================

void SearchIndex::displayIndex() const
{
    cout << "\n========== SEARCH INDEX ==========\n";

    for (const auto& item :
         index)
    {
        cout << item.first
             << " -> ";

        for (const auto& pageData :
             item.second)
        {
            cout << "Page "
                 << pageData.first
                 << " ("
                 << pageData.second
                 << " times) ";
        }

        cout << endl;
    }
}