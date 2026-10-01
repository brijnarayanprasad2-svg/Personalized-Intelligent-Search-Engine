#include "../include/SearchIndex.h"

#include <iostream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <ctime>
#include <unordered_set>

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

    // Reduce memory reallocations
    result.reserve(word.size());

    for (char ch : word)
    {
        unsigned char c =
            static_cast<unsigned char>(ch);

        if (isalnum(c))
        {
            result +=
                static_cast<char>(
                    tolower(c)
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
            words.push_back(
                cleanWord
            );
        }
    }

    return words;
}


// ==================================================
// NORMALIZE COMPLETE QUERY
// REMOVE DUPLICATES
// ==================================================

string SearchIndex::normalizeQuery(
    const string& query) const
{
    vector<string> words =
        tokenize(query);

    vector<string> uniqueWords;

    unordered_set<string> seen;


    // Reserve memory in advance
    uniqueWords.reserve(
        words.size()
    );

    seen.reserve(
        words.size()
    );


    // --------------------------------------------------
    // Remove duplicate words
    // while preserving original order
    // --------------------------------------------------

    for (const string& word :
         words)
    {
        if (seen.insert(word).second)
        {
            uniqueWords.push_back(
                word
            );
        }
    }


    // --------------------------------------------------
    // Rebuild normalized query
    // --------------------------------------------------

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
    pages.push_back(
        page
    );


    string text =
        page.getTitle() +
        " " +
        page.getContent();


    vector<string> words =
        tokenize(text);


    for (const string& word :
         words)
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

    pageData.reserve(
        it->second.size()
    );


    for (const auto& item :
         it->second)
    {
        pageData.push_back(
            item
        );
    }


    // --------------------------------------------------
    // Rank pages by keyword frequency
    // --------------------------------------------------

    sort(
        pageData.begin(),
        pageData.end(),

        [](const pair<int, int>& a,
           const pair<int, int>& b)
        {
            // Higher frequency first
            if (a.second != b.second)
            {
                return a.second >
                       b.second;
            }

            // Smaller page ID first
            return a.first <
                   b.first;
        }
    );


    for (const auto& item :
         pageData)
    {
        pageIDs.push_back(
            item.first
        );
    }


    return pageIDs;
}


// ==================================================
// PAGE SEARCH
// RELEVANCE + PERSONALIZATION
// OPTIMIZED WITH INVERTED-INDEX INTERSECTION
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


    // ==================================================
    // TOKENIZE NORMALIZED QUERY
    // ==================================================

    vector<string> queryWords =
        tokenize(normalizedQuery);


    if (queryWords.empty())
    {
        return results;
    }


    // ==================================================
    // FIND POSTING LISTS
    //
    // Each query word maps to:
    // pageID -> frequency
    // ==================================================

    vector<
        const unordered_map<int, int>*
    > postingLists;


    postingLists.reserve(
        queryWords.size()
    );


    for (const string& queryWord :
         queryWords)
    {
        auto indexIt =
            index.find(queryWord);


        // If even one query word does not
        // exist in the index, no complete
        // result can exist.
        if (indexIt == index.end())
        {
            return results;
        }


        postingLists.push_back(
            &indexIt->second
        );
    }


    // ==================================================
    // FIND SMALLEST POSTING LIST
    // ==================================================

    size_t smallestIndex = 0;


    for (size_t i = 1;
         i < postingLists.size();
         i++)
    {
        if (
            postingLists[i]->size() <
            postingLists[smallestIndex]->size()
        )
        {
            smallestIndex =
                i;
        }
    }


    // ==================================================
    // BUILD INITIAL CANDIDATE SET
    // ==================================================

    unordered_set<int> candidatePages;


    candidatePages.reserve(
        postingLists[smallestIndex]->size()
    );


    for (const auto& item :
         *postingLists[smallestIndex])
    {
        candidatePages.insert(
            item.first
        );
    }


    // ==================================================
    // INTERSECT POSTING LISTS
    // ==================================================

    for (size_t i = 0;
         i < postingLists.size();
         i++)
    {
        if (i == smallestIndex)
        {
            continue;
        }


        const auto& postingList =
            *postingLists[i];


        for (
            auto it = candidatePages.begin();
            it != candidatePages.end();
        )
        {
            if (
                postingList.find(*it) ==
                postingList.end()
            )
            {
                it =
                    candidatePages.erase(it);
            }
            else
            {
                ++it;
            }
        }


        // No common page remains
        if (candidatePages.empty())
        {
            return results;
        }
    }


    // ==================================================
    // COMMON PERSONALIZATION
    // ==================================================

    double queryFrequencyScore =
        history.getFrequency(
            normalizedQuery
        ) * 2.0;


    double recencyBonus = 0.0;


    time_t lastSearch =
        history.getLastSearchTime(
            normalizedQuery
        );


    // ==================================================
    // RECENCY BONUS
    // ==================================================

    if (lastSearch != 0)
    {
        long long secondsAgo =
            static_cast<long long>(
                time(nullptr) -
                lastSearch
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
    // RESERVE RESULT MEMORY
    // ==================================================

    results.reserve(
        candidatePages.size()
    );


    // ==================================================
    // PROCESS ONLY CANDIDATE PAGES
    // ==================================================

    for (int pageID :
         candidatePages)
    {
        // --------------------------------------------------
        // Safety check
        // --------------------------------------------------

        if (
            pageID <= 0 ||
            static_cast<size_t>(pageID) >
                pages.size()
        )
        {
            continue;
        }


        // Page IDs are 1-based
        const WebPage& page =
            pages[
                static_cast<size_t>(
                    pageID - 1
                )
            ];


        double score = 0.0;


        // ==================================================
        // PAGE PERSONALIZATION
        // ==================================================

        double pageFrequencyScore =
            history.getPageSearchFrequency(
                pageID
            ) * 2.0;


        double queryPageScore =
            history.getQueryPageFrequency(
                normalizedQuery,
                pageID
            ) * 3.0;


        double personalizationScore =
            queryFrequencyScore
            + pageFrequencyScore
            + queryPageScore
            + recencyBonus;


        // ==================================================
        // TOKENIZE TITLE
        // ==================================================

        vector<string> titleWords =
            tokenize(
                page.getTitle()
            );


        // ==================================================
        // FAST TITLE LOOKUP
        // ==================================================

        unordered_set<string> titleWordSet;


        titleWordSet.reserve(
            titleWords.size()
        );


        for (const string& titleWord :
             titleWords)
        {
            titleWordSet.insert(
                titleWord
            );
        }


        // ==================================================
        // WORD FREQUENCY
        // + TITLE MATCH
        // ==================================================

        for (size_t i = 0;
             i < queryWords.size();
             i++)
        {
            const string& queryWord =
                queryWords[i];


            const auto& postingList =
                *postingLists[i];


            auto pageIt =
                postingList.find(
                    pageID
                );


            if (pageIt ==
                postingList.end())
            {
                continue;
            }


            // --------------------------------------------------
            // WORD FREQUENCY
            // --------------------------------------------------

            score +=
                pageIt->second;


            // --------------------------------------------------
            // TITLE MATCH
            // --------------------------------------------------

            if (
                titleWordSet.find(
                    queryWord
                ) != titleWordSet.end()
            )
            {
                score +=
                    TITLE_MATCH_SCORE;
            }
        }


        // ==================================================
        // MULTI-WORD BONUS
        // ==================================================

        if (queryWords.size() > 1)
        {
            score +=
                static_cast<double>(
                    queryWords.size()
                ) *
                MULTI_WORD_BONUS;


            // Complete query bonus
            score +=
                MULTI_WORD_BONUS;
        }


        // ==================================================
        // CREATE NORMALIZED TITLE
        // ==================================================

        string normalizedTitle;


        normalizedTitle.reserve(
            page.getTitle().size()
        );


        for (const string& titleWord :
             titleWords)
        {
            if (!normalizedTitle.empty())
            {
                normalizedTitle += " ";
            }

            normalizedTitle +=
                titleWord;
        }


        // ==================================================
        // CREATE NORMALIZED CONTENT
        // ==================================================

        vector<string> contentWords =
            tokenize(
                page.getContent()
            );


        string normalizedContent;


        normalizedContent.reserve(
            page.getContent().size()
        );


        for (const string& contentWord :
             contentWords)
        {
            if (!normalizedContent.empty())
            {
                normalizedContent += " ";
            }

            normalizedContent +=
                contentWord;
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


            if (
                searchText.find(
                    searchPhrase
                ) != string::npos
            )
            {
                score +=
                    PHRASE_MATCH_BONUS;
            }
        }


        // ==================================================
        // EXACT TITLE MATCH
        // ==================================================

        if (
            normalizedTitle ==
            normalizedQuery
        )
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


            // --------------------------------------------------
            // PERSONALIZATION SCORE
            // --------------------------------------------------

            result.setPersonalizationScore(
                personalizationScore
            );


            // --------------------------------------------------
            // SCORE COMPONENTS
            // --------------------------------------------------

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


            // --------------------------------------------------
            // STORE RESULT
            // --------------------------------------------------

            results.push_back(
                result
            );
        }
    }


    // ==================================================
    // FINAL RANKING
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
            if (
                finalScoreA !=
                finalScoreB
            )
            {
                return finalScoreA >
                       finalScoreB;
            }


            // Smaller page ID first
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
    cout
        << "\n========== SEARCH INDEX ==========\n";


    for (const auto& item :
         index)
    {
        cout
            << item.first
            << " -> ";


        for (const auto& pageData :
             item.second)
        {
            cout
                << "Page "
                << pageData.first
                << " ("
                << pageData.second
                << " times) ";
        }


        cout << endl;
    }
}