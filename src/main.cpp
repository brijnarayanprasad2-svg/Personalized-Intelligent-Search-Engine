#include "../include/ApiServer.h"
#include "../include/Trie.h"
#include "../include/SearchData.h"
#include "../include/SearchHistory.h"
#include "../include/RankingEngine.h"
#include "../include/WebCrawler.h"
#include "../include/SearchIndex.h"
#include "../include/SearchResult.h"

#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>
#include <sstream>
#include <utility>
#include <unordered_set>

using namespace std;
typedef unsigned char byte;
string createSearchSnippet(
    const string& content,
    const string& query
);

// =========================================================
// QUERY NORMALIZATION
// =========================================================

string normalizeUserQuery(
    const string& query
)
{
    string result;

    bool lastWasSpace = true;

    for (char ch : query)
    {
        unsigned char c =
            static_cast<unsigned char>(
                ch
            );

        if (isalnum(c))
        {
            result +=
                static_cast<char>(
                    tolower(c)
                );

            lastWasSpace = false;
        }
        else if (!lastWasSpace)
        {
            result += ' ';
            lastWasSpace = true;
        }
    }

    // Remove trailing space
    if (!result.empty() &&
        result.back() == ' ')
    {
        result.pop_back();
    }

    return result;
}
// =========================================================
// COMMON PREFIX LENGTH
// =========================================================

size_t commonPrefixLength(
    const string& first,
    const string& second
)
{
    size_t length =
        min(
            first.length(),
            second.length()
        );

    size_t i = 0;

    while (i < length &&
           first[i] == second[i])
    {
        i++;
    }

    return i;
}


// =========================================================
// SAFE WORD CORRECTION
// =========================================================

string findSafeCorrection(
    const string& word,
    const Trie& trie
)
{
    string correctedWord =
        trie.findClosestWord(
            word
        );

    if (correctedWord.empty() ||
        correctedWord == word)
    {
        return correctedWord;
    }

    // Avoid aggressive short-word corrections.
    // Example:
    // dts -> dns  (should NOT happen)
    //
    // Valid examples remain:
    // dats   -> data
    // dnss   -> dns
    // protcol -> protocol
    // internt -> internet
    // comptur -> computer

    if (word.length() >= 3 &&
        commonPrefixLength(
            word,
            correctedWord
        ) < 2)
    {
        return word;
    }

    return correctedWord;
}


// =========================================================
// QUERY SPELLING CORRECTION + DUPLICATE REMOVAL
// =========================================================

string correctQuery(
    const string& query,
    const Trie& trie
)
{
    string correctedQuery;

    unordered_set<string> seen;

    istringstream input(query);

    string word;


    while (input >> word)
    {
        string correctedWord =
            findSafeCorrection(
                word,
                trie
            );


        if (correctedWord.empty())
        {
            correctedWord =
                word;
        }


        // Remove duplicate corrected words
        // while preserving first occurrence.
        if (!seen.insert(
                correctedWord
            ).second)
        {
            continue;
        }


        if (!correctedQuery.empty())
        {
            correctedQuery += ' ';
        }


        correctedQuery +=
            correctedWord;
    }


    return correctedQuery;
}


// =========================================================
// CHECK WHETHER ANY WORD WAS ACTUALLY CORRECTED
// =========================================================

bool hasSpellingCorrection(
    const string& query,
    const Trie& trie
)
{
    istringstream input(query);

    string word;


    while (input >> word)
    {
        string correctedWord =
            findSafeCorrection(
                word,
                trie
            );


        if (!correctedWord.empty() &&
            correctedWord != word)
        {
            return true;
        }
    }


    return false;
}


// =========================================================
// CHECK WHETHER ALL WORDS EXIST IN TRIE
// =========================================================

bool allWordsExistInTrie(
    const string& query,
    const Trie& trie
)
{
    istringstream input(query);

    string word;

    bool hasWord = false;


    while (input >> word)
    {
        hasWord = true;

        if (!trie.search(word))
        {
            return false;
        }
    }


    return hasWord;
}


// =========================================================
// JSON ESCAPING
// =========================================================

string escapeJson(
    const string& value
)
{
    string result;

    for (char ch : value)
    {
        switch (ch)
        {
            case '\"':
                result += "\\\"";
                break;

            case '\\':
                result += "\\\\";
                break;

            case '\b':
                result += "\\b";
                break;

            case '\f':
                result += "\\f";
                break;

            case '\n':
                result += "\\n";
                break;

            case '\r':
                result += "\\r";
                break;

            case '\t':
                result += "\\t";
                break;

            default:
                result += ch;
                break;
        }
    }

    return result;
}

// =========================================================
// CREATE SEARCH SNIPPET
// =========================================================

string createSearchSnippet(
    const string& content,
    const string& query
)
{
    if (content.empty())
    {
        return "";
    }

    // -----------------------------------------------------
    // NORMALIZE QUERY
    // -----------------------------------------------------

    string normalizedQuery =
        normalizeUserQuery(
            query
        );

    if (normalizedQuery.empty())
    {
        return content.substr(
            0,
            180
        );
    }

    // -----------------------------------------------------
    // NORMALIZE CONTENT
    // -----------------------------------------------------

    string normalizedContent =
        normalizeUserQuery(
            content
        );

    if (normalizedContent.empty())
    {
        return "";
    }

    // -----------------------------------------------------
    // FIND FIRST QUERY WORD
    // -----------------------------------------------------

    istringstream input(
        normalizedQuery
    );

    string queryWord;

    size_t bestPosition =
        string::npos;

    while (input >> queryWord)
    {
        size_t position =
            normalizedContent.find(
                queryWord
            );

        if (position != string::npos &&
            (
                bestPosition == string::npos ||
                position < bestPosition
            ))
        {
            bestPosition = position;
        }
    }

    // -----------------------------------------------------
    // NO QUERY WORD FOUND
    // -----------------------------------------------------

    if (bestPosition == string::npos)
    {
        return content.substr(
            0,
            180
        );
    }

    // -----------------------------------------------------
    // CREATE SNIPPET WINDOW
    // -----------------------------------------------------

    const size_t CONTEXT_BEFORE = 70;
    const size_t CONTEXT_AFTER  = 110;

    size_t start =
        (
            bestPosition > CONTEXT_BEFORE
            ?
            bestPosition - CONTEXT_BEFORE
            :
            0
        );

    size_t snippetLength =
        CONTEXT_BEFORE +
        CONTEXT_AFTER;

    string snippet =
        normalizedContent.substr(
            start,
            snippetLength
        );

    // -----------------------------------------------------
    // ADD ELLIPSIS
    // -----------------------------------------------------

    if (start > 0)
    {
        snippet =
            "... " +
            snippet;
    }

    if (start + snippetLength <
        normalizedContent.length())
    {
        snippet +=
            " ...";
    }

    return snippet;
}

// =========================================================
// API SEARCH RESPONSE
// =========================================================

string buildApiSearchResponse(
    const string& query,
    Trie& trie,
    RankingEngine& rankingEngine,
    SearchHistory& searchHistory,
    SearchIndex& searchIndex
)
{
    // -----------------------------------------------------
    // NORMALIZE QUERY
    // -----------------------------------------------------

    string normalizedQuery =
        normalizeUserQuery(
            query
        );

    if (normalizedQuery.empty())
    {
        return
            "{\"error\":\"Please enter a valid search query.\"}";
    }


    // -----------------------------------------------------
    // SPELLING CORRECTION
    // -----------------------------------------------------

    string correctedQuery =
        correctQuery(
            normalizedQuery,
            trie
        );

    // Final query used by autocomplete,
    // ranking, personalization and page search.
    string searchQuery =
        correctedQuery;

        bool spellingCorrected =
         hasSpellingCorrection(
          normalizedQuery,
          trie
        );

     bool queryWasCorrected =
     spellingCorrected;



    // -----------------------------------------------------
    // EXACT MATCH
    // -----------------------------------------------------

    bool exactMatch =
        allWordsExistInTrie(
            normalizedQuery,
            trie
        );

    bool correctedMatch =
        (
            queryWasCorrected &&
            allWordsExistInTrie(
                correctedQuery,
                trie
            )
        );


    // -----------------------------------------------------
    // RECORD USER SEARCH HISTORY
    // -----------------------------------------------------
    // Store the final processed/corrected query so that
    // spelling variants do not split personalization data.

    searchHistory.recordSearch(
        searchQuery
    );


    // -----------------------------------------------------
    // AUTOCOMPLETE
    // -----------------------------------------------------

    vector<string> suggestions =
        trie.autocomplete(
            searchQuery
        );


    // -----------------------------------------------------
    // RANK SUGGESTIONS
    // -----------------------------------------------------

    vector<string> rankedSuggestions =
        rankingEngine.rankSuggestions(
            suggestions,
            searchQuery,
            searchHistory
        );


    // -----------------------------------------------------
    // SEARCH WEB PAGES
    // -----------------------------------------------------

    vector<SearchResult> pageResults =
        searchIndex.searchPages(
            searchQuery,
            searchHistory
        );


    // -----------------------------------------------------
    // RECORD PAGE SEARCH HISTORY
    // -----------------------------------------------------

    for (const SearchResult& result :
         pageResults)
    {
        searchHistory.recordPageSearch(
            result.getPageID()
        );

        searchHistory.recordQueryPage(
            searchQuery,
            result.getPageID()
        );
    }


    // -----------------------------------------------------
    // BUILD JSON
    // -----------------------------------------------------

    ostringstream json;

    json << "{";


    // Original query
    json
        << "\"query\":\""
        << escapeJson(query)
        << "\",";


    // Processed query = normalized + corrected + deduplicated
    json
        << "\"processedQuery\":\""
        << escapeJson(correctedQuery)
        << "\",";


    // Corrected query
    json
        << "\"correctedQuery\":\""
        << escapeJson(correctedQuery)
        << "\",";


    // Correction flag
    json
        << "\"queryWasCorrected\":"
        << (
            queryWasCorrected
            ? "true"
            : "false"
        )
        << ",";


    // Exact match
    json
        << "\"exactMatch\":"
        << (
            exactMatch
            ? "true"
            : "false"
        )
        << ",";


    // Corrected match
    json
        << "\"correctedMatch\":"
        << (
            correctedMatch
            ? "true"
            : "false"
        )
        << ",";


    // -----------------------------------------------------
    // RANKED SUGGESTIONS
    // -----------------------------------------------------

    json
        << "\"suggestions\":[";

    for (size_t i = 0;
         i < rankedSuggestions.size();
         i++)
    {
        if (i > 0)
        {
            json << ",";
        }

        const string& suggestion =
            rankedSuggestions[i];

        json
            << "{"
            << "\"text\":\""
            << escapeJson(
                   suggestion
               )
            << "\","
            << "\"frequency\":"
            << searchHistory.getFrequency(
                   suggestion
               )
            << "}";
    }

    json
        << "],";


    // -----------------------------------------------------
    // SEARCH RESULTS
    // -----------------------------------------------------

    json
        << "\"results\":[";

    for (size_t i = 0;
         i < pageResults.size();
         i++)
    {
        if (i > 0)
        {
            json << ",";
        }

        const SearchResult& result =
            pageResults[i];

        double finalScore =
            result.getScore()
            +
            result.getPersonalizationScore();

        string snippet =
            createSearchSnippet(
                result.getContent(),
                searchQuery
            );

        json
            << "{";

        json
            << "\"title\":\""
            << escapeJson(
                   result.getTitle()
               )
            << "\",";

        json
            << "\"url\":\""
            << escapeJson(
                   result.getURL()
               )
            << "\",";

        json
            << "\"relevance\":"
            << result.getScore()
            << ",";

        json
            << "\"queryFrequency\":"
            << result.getQueryFrequencyScore()
            << ",";

        json
            << "\"pageFrequency\":"
            << result.getPageFrequencyScore()
            << ",";

        json
            << "\"queryPageScore\":"
            << result.getQueryPageScore()
            << ",";

        json
            << "\"recencyBonus\":"
            << result.getRecencyBonus()
            << ",";

        json
            << "\"personalization\":"
            << result.getPersonalizationScore()
            << ",";

        json
            << "\"finalScore\":"
            << finalScore
            << ",";

        json
            << "\"snippet\":\""
            << escapeJson(
                   snippet
               )
            << "\"";

        json
            << "}";
    }

    json
        << "]";

    json
        << "}";

    return json.str();
}


// =========================================================
// MAIN
// =========================================================

int main(
    int argc,
    char* argv[]
)
{
    bool apiMode =
    (
        argc > 1 &&
        string(argv[1]) == "--api"
    );


    // =====================================================
    // 1. SYSTEM INITIALIZATION
    // =====================================================

    Trie trie;

    SearchData dataLoader;

    SearchHistory searchHistory;

    RankingEngine rankingEngine;

    SearchIndex searchIndex;


    // Maximum pages
    WebCrawler crawler(13);


    // =====================================================
    // 2. APPLICATION HEADER
    // =====================================================

    cout << "\n";
    cout << "==================================================\n";
    cout << "       PERSONALIZED INTELLIGENT SEARCH ENGINE\n";
    cout << "==================================================\n";

    cout << "\nInitializing search engine...\n";


    // =====================================================
    // 3. LOAD SEARCH VOCABULARY
    // =====================================================

    vector<string> words =
        dataLoader.loadWords(
            "data/words.txt"
        );

    if (words.empty())
    {
        cout << "\nWarning: "
             << "No search data found.\n";
    }
    else
    {
        cout << "\nSearch vocabulary loaded: "
             << words.size()
             << " words\n";
    }


    // Insert words into Trie
    for (const string& word : words)
    {
        trie.insert(word);
    }


    // =====================================================
    // 4. WEB CRAWLER
    // =====================================================

    cout << "\n==================================================\n";
    cout << "                 WEB CRAWLER\n";
    cout << "==================================================\n";

    cout << "\nStarting web crawler...\n";
    crawler.addSeedURL("computer_science");
    crawler.addSeedURL("data_structures"); 
    crawler.addSeedURL("algorithms");
    crawler.addSeedURL("cpp_programming");
    crawler.addSeedURL("object_oriented_programming");
    crawler.addSeedURL("data_science");
    crawler.addSeedURL("artificial_intelligence");
    crawler.addSeedURL("machine_learning");
    crawler.addSeedURL("internet_protocols");
    crawler.addSeedURL("database_management");
    
    crawler.crawl();


    // =====================================================
    // 5. GET CRAWLED PAGES
    // =====================================================

    vector<WebPage> crawledPages =
        crawler.getPages();

    cout << "\nCrawled pages available: "
         << crawledPages.size()
         << endl;


    // =====================================================
    // 6. BUILD SEARCH INDEX
    // =====================================================

    cout << "\n==================================================\n";
    cout << "                 SEARCH INDEX\n";
    cout << "==================================================\n";

    for (size_t i = 0;
         i < crawledPages.size();
         i++)
    {
        searchIndex.addPage(
            static_cast<int>(
                i + 1
            ),
            crawledPages[i]
        );
    }

    cout << "\nSearch index successfully built for "
         << crawledPages.size()
         << " pages.\n";


    // =====================================================
    // API MODE
    // =====================================================

    if (apiMode)
    {
        ApiServer apiServer(8080);

        apiServer.setRequestHandler(
            [&](const string& query)
            {
                return buildApiSearchResponse(
                    query,
                    trie,
                    rankingEngine,
                    searchHistory,
                    searchIndex
                );
            }
        );

        cout << "\nStarting API mode...\n";

        apiServer.run();

        return 0;
    }


    // =====================================================
    // 7. PAGE SEARCH DEMONSTRATION
    // =====================================================

    cout << "\n==================================================\n";
    cout << "             PAGE SEARCH DEMONSTRATION\n";
    cout << "==================================================\n";

    string testKeyword =
        "computer";

    cout << "\nTest Query: "
         << testKeyword
         << endl;

    vector<int> testResults =
        searchIndex.search(
            testKeyword
        );

    if (testResults.empty())
    {
        cout << "No pages found for this keyword.\n";
    }
    else
    {
        cout << "Matching Pages:\n";

        for (int pageID :
             testResults)
        {
            cout << "  -> Page "
                 << pageID
                 << endl;
        }
    }


    // =====================================================
    // 8. SEARCH INTERFACE
    // =====================================================

    cout << "\n==================================================\n";
    cout << "                SEARCH INTERFACE\n";
    cout << "==================================================\n";

    cout << "\nYou can search the vocabulary "
         << "and crawled pages.\n";

    cout << "Spelling correction is enabled.\n";

    cout << "Search snippets are enabled.\n";

    cout << "Type 'exit' to close the application.\n";


    string query;

    string normalizedQuery;

    string correctedQuery;

    string searchQuery;


    while (true)
    {
        // =================================================
        // GET USER QUERY
        // =================================================

        cout << "\nSearch: ";

        getline(
            cin,
            query
        );


        // =================================================
        // EXIT
        // =================================================

        if (query == "exit")
        {
            break;
        }


        // =================================================
        // NORMALIZE QUERY
        // =================================================

        normalizedQuery =
            normalizeUserQuery(
                query
            );


        // =================================================
        // EMPTY QUERY
        // =================================================

        if (normalizedQuery.empty())
        {
            cout << "Please enter a valid "
                 << "search query.\n";

            continue;
        }


        // =================================================
        // SPELLING CORRECTION
        // =================================================

        correctedQuery =
            correctQuery(
                normalizedQuery,
                trie
            );

        searchQuery =
            correctedQuery;

        bool spellingCorrected =
          hasSpellingCorrection(
         normalizedQuery,
         trie
        );

        bool queryWasCorrected =

         spellingCorrected;

        // =================================================
        // SHOW CORRECTION
        // =================================================

        if (queryWasCorrected)
        {
            cout << "\nDid you mean: "
                 << correctedQuery
                 << "?"
                 << endl;
        }

        correctedQuery =
            correctQuery(
            normalizedQuery,
            trie
        );

        searchQuery =
          correctedQuery;

          
        // =================================================
        // DISPLAY FINAL PROCESSED QUERY
        // =================================================

        cout << "Processed Query: "
             << searchQuery
             << endl;



        // =================================================
        // EXACT SEARCH
        // =================================================

        bool exactMatch =
            allWordsExistInTrie(
                normalizedQuery,
                trie
            );

        bool correctedMatch =
        (
            queryWasCorrected &&
            allWordsExistInTrie(
                correctedQuery,
                trie
            )
        );


        if (exactMatch)
        {
            cout << "\nExact Match: Found\n";
        }
        else if (correctedMatch)
        {
            cout << "\nExact Match: Not Found\n";

            cout << "Corrected Match: Found\n";
        }
        else
        {
            cout << "\nExact Match: Not Found\n";
        }


        // =================================================
        // RECORD USER SEARCH HISTORY
        // =================================================
        //
        // Store the final processed/corrected query so that
        // spelling variants do not split personalization data.
        // =================================================

        searchHistory.recordSearch(
            correctedQuery
        );


        // =================================================
        // AUTOCOMPLETE
        // =================================================

        vector<string> suggestions =
            trie.autocomplete(
                searchQuery
            );


        // =================================================
        // RANK SUGGESTIONS
        // =================================================

        vector<string> rankedSuggestions =
            rankingEngine.rankSuggestions(
                suggestions,
                searchQuery,
                searchHistory
            );

        cout << "\nRanked Suggestions:\n";


        if (rankedSuggestions.empty())
        {
            cout << "No suggestions found.\n";
        }
        else
        {
            for (size_t i = 0;
                 i < rankedSuggestions.size();
                 i++)
            {
                const string& suggestion =
                    rankedSuggestions[i];

                cout << "  "
                     << i + 1
                     << ". "
                     << suggestion
                     << " ["
                     << searchHistory.getFrequency(
                            suggestion
                        )
                     << " searches]"
                     << endl;
            }
        }


        // =================================================
        // SEARCH CRAWLED WEB PAGES
        // =================================================

        vector<SearchResult> pageResults =
            searchIndex.searchPages(
                searchQuery,
                searchHistory
            );


        // =================================================
        // RECORD PAGE SEARCH HISTORY
        // =================================================

        for (const SearchResult& result :
             pageResults)
        {
            searchHistory.recordPageSearch(
                result.getPageID()
            );

            searchHistory.recordQueryPage(
                searchQuery,
                result.getPageID()
            );
        }


        // =================================================
        // DISPLAY WEB PAGE RESULTS
        // =================================================

        cout << "\nMatching Web Pages:\n";


        if (pageResults.empty())
        {
            cout << "No matching pages found.\n";
        }
        else
        {
            for (size_t i = 0;
                 i < pageResults.size();
                 i++)
            {
                cout << "\n";

                cout << "  Result "
                     << i + 1
                     << endl;

                cout << "  Title                  : "
                     << pageResults[i].getTitle()
                     << endl;

                cout << "  URL                    : "
                     << pageResults[i].getURL()
                     << endl;

                cout << "  Relevance Score        : "
                     << pageResults[i].getScore()
                     << endl;

                cout << "  Query Frequency Score  : "
                     << pageResults[i]
                            .getQueryFrequencyScore()
                     << endl;

                cout << "  Page Frequency Score   : "
                     << pageResults[i]
                            .getPageFrequencyScore()
                     << endl;

                cout << "  Query-Page Score       : "
                     << pageResults[i]
                            .getQueryPageScore()
                     << endl;

                cout << "  Recency Bonus          : "
                     << pageResults[i]
                            .getRecencyBonus()
                     << endl;

                cout << "  Personalization        : "
                     << pageResults[i]
                            .getPersonalizationScore()
                     << endl;

                cout << "  Final Score            : "
                     << pageResults[i].getScore()
                        +
                        pageResults[i]
                            .getPersonalizationScore()
                     << endl;


                // =================================================
                // SEARCH SNIPPET
                // =================================================

                string snippet =
                    createSearchSnippet(
                        pageResults[i]
                            .getContent(),
                        searchQuery
                    );

                cout << "  Snippet                : "
                     << snippet
                     << endl;
            }
        }
    }


    // =====================================================
    // 9. SEARCH HISTORY
    // =====================================================

    searchHistory.displayHistory();


    // =====================================================
    // 10. CRAWLED PAGE SUMMARY
    // =====================================================

    cout << "\n==================================================\n";
    cout << "              CRAWLED PAGE SUMMARY\n";
    cout << "==================================================\n";

    cout << "\nTotal pages stored: "
         << crawledPages.size()
         << endl;

    for (size_t i = 0;
         i < crawledPages.size();
         i++)
    {
        cout << "\nPage "
             << i + 1
             << endl;

        cout << "  Title        : "
             << crawledPages[i].getTitle()
             << endl;

        cout << "  URL          : "
             << crawledPages[i].getURL()
             << endl;

        cout << "  Page Searches: "
             << searchHistory.getPageSearchFrequency(
                    static_cast<int>(
                        i + 1
                    )
                )
             << endl;

        cout << "  Content      : "
             << crawledPages[i].getContent()
             << endl;
    }


    // =====================================================
    // 11. PROGRAM TERMINATION
    // =====================================================

    cout << "\n==================================================\n";
    cout << "          SEARCH ENGINE SHUTTING DOWN\n";
    cout << "==================================================\n";

    cout << "\nThank you for using the "
         << "Personalized Intelligent "
         << "Search Engine.\n";

    return 0;
}
