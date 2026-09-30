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

using namespace std;


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
// QUERY SPELLING CORRECTION
// =========================================================

string correctQuery(
    const string& query,
    const Trie& trie
)
{
    string correctedQuery;
    string word;


    for (size_t i = 0;
         i <= query.length();
         i++)
    {
        // Build current word
        if (i < query.length() &&
            query[i] != ' ')
        {
            word += query[i];
        }
        else
        {
            if (!word.empty())
            {
                string correctedWord =
                    trie.findClosestWord(
                        word
                    );


                // No correction found
                if (correctedWord.empty())
                {
                    correctedWord =
                        word;
                }


                if (!correctedQuery.empty())
                {
                    correctedQuery += ' ';
                }

                correctedQuery +=
                    correctedWord;

                word.clear();
            }
        }
    }

    return correctedQuery;
}


// =========================================================
// GET SEARCH SNIPPET
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


    const size_t MAX_SNIPPET_LENGTH =
        220;


    string lowerContent =
        content;

    string lowerQuery =
        query;


    // Convert content to lowercase
    for (char& ch : lowerContent)
    {
        ch =
            static_cast<char>(
                tolower(
                    static_cast<unsigned char>(
                        ch
                    )
                )
            );
    }


    // Convert query to lowercase
    for (char& ch : lowerQuery)
    {
        ch =
            static_cast<char>(
                tolower(
                    static_cast<unsigned char>(
                        ch
                    )
                )
            );
    }


    // -----------------------------------------------------
    // Try complete query phrase first
    // -----------------------------------------------------

    size_t matchPosition =
        lowerContent.find(
            lowerQuery
        );


    // -----------------------------------------------------
    // If complete phrase isn't found,
    // search individual query words.
    // -----------------------------------------------------

    if (matchPosition ==
        string::npos)
    {
        string queryWord;

        for (size_t i = 0;
             i <= lowerQuery.length();
             i++)
        {
            if (i < lowerQuery.length() &&
                lowerQuery[i] != ' ')
            {
                queryWord +=
                    lowerQuery[i];
            }
            else
            {
                if (!queryWord.empty())
                {
                    matchPosition =
                        lowerContent.find(
                            queryWord
                        );

                    if (matchPosition !=
                        string::npos)
                    {
                        break;
                    }

                    queryWord.clear();
                }
            }
        }
    }


    // -----------------------------------------------------
    // No query match
    // -----------------------------------------------------

    if (matchPosition ==
        string::npos)
    {
        if (content.length() <=
            MAX_SNIPPET_LENGTH)
        {
            return content;
        }

        return content.substr(
                   0,
                   MAX_SNIPPET_LENGTH
               )
               + "...";
    }


    // -----------------------------------------------------
    // Create context around match
    // -----------------------------------------------------

    const size_t CONTEXT_BEFORE =
        70;

    const size_t CONTEXT_AFTER =
        150;


    size_t startPosition = 0;

    if (matchPosition >
        CONTEXT_BEFORE)
    {
        startPosition =
            matchPosition -
            CONTEXT_BEFORE;
    }


    size_t endPosition =
        min(
            content.length(),
            matchPosition +
            lowerQuery.length() +
            CONTEXT_AFTER
        );


    string snippet =
        content.substr(
            startPosition,
            endPosition -
            startPosition
        );


    // -----------------------------------------------------
    // Clean beginning
    // -----------------------------------------------------

    if (startPosition > 0)
    {
        size_t firstSpace =
            snippet.find(' ');

        if (firstSpace !=
            string::npos)
        {
            snippet =
                snippet.substr(
                    firstSpace + 1
                );
        }

        snippet =
            "... " + snippet;
    }


    // -----------------------------------------------------
    // Clean ending
    // -----------------------------------------------------

    if (endPosition <
        content.length())
    {
        snippet += "...";
    }


    // Final safety limit
    if (snippet.length() >
        MAX_SNIPPET_LENGTH)
    {
        snippet =
            snippet.substr(
                0,
                MAX_SNIPPET_LENGTH
            )
            + "...";
    }


    return snippet;
}


// =========================================================
// MAIN
// =========================================================

int main()
{
    // =====================================================
    // 1. SYSTEM INITIALIZATION
    // =====================================================

    Trie trie;

    SearchData dataLoader;

    SearchHistory searchHistory;

    RankingEngine rankingEngine;

    SearchIndex searchIndex;


    // Maximum pages
    WebCrawler crawler(5);


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


    crawler.addSeedURL(
        "https://iana.org/domains/example"
    );


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


        cout << "Processed Query: "
             << normalizedQuery
             << endl;


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


        bool queryWasCorrected =
            (
                correctedQuery !=
                normalizedQuery
            );


        if (queryWasCorrected)
        {
            cout << "\nDid you mean: "
                 << correctedQuery
                 << "?"
                 << endl;
        }


        // =================================================
        // EXACT SEARCH
        // =================================================

        bool exactMatch =
            trie.search(
                normalizedQuery
            );


        bool correctedMatch =
            false;


        if (!exactMatch &&
            queryWasCorrected)
        {
            correctedMatch =
                trie.search(
                    correctedQuery
                );
        }


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
        // IMPORTANT:
        // Store what the user actually typed,
        // not the automatically corrected query.
        // =================================================

        searchHistory.recordSearch(
            normalizedQuery
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