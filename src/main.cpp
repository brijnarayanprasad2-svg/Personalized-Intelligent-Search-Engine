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

using namespace std;

int main()
{
    // =========================================================
    // 1. SYSTEM INITIALIZATION
    // =========================================================

    Trie trie;
    SearchData dataLoader;
    SearchHistory searchHistory;
    RankingEngine rankingEngine;
    SearchIndex searchIndex;

    // Maximum pages that can be crawled
    WebCrawler crawler(5);

    // =========================================================
    // 2. APPLICATION HEADER
    // =========================================================

    cout << "\n";
    cout << "==================================================\n";
    cout << "       PERSONALIZED INTELLIGENT SEARCH ENGINE\n";
    cout << "==================================================\n";

    cout << "\nInitializing search engine...\n";

    // =========================================================
    // 3. LOAD SEARCH DATA
    // =========================================================

    vector<string> words =
        dataLoader.loadWords("data/words.txt");

    if (words.empty())
    {
        cout << "\nWarning: No search data found.\n";
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

    // =========================================================
    // 4. WEB CRAWLER
    // =========================================================

    cout << "\n==================================================\n";
    cout << "                 WEB CRAWLER\n";
    cout << "==================================================\n";

    cout << "\nStarting local web crawler...\n";

    // Local HTML pages used as seed pages
    crawler.addSeedURL("page1.html");

    // Start crawling
    crawler.crawl();

    // =========================================================
    // 5. GET CRAWLED PAGES
    // =========================================================

    vector<WebPage> crawledPages =
        crawler.getPages();

    cout << "\nCrawled pages available: "
         << crawledPages.size()
         << endl;

    // =========================================================
    // 6. BUILD SEARCH INDEX
    // =========================================================

    cout << "\n==================================================\n";
    cout << "                 SEARCH INDEX\n";
    cout << "==================================================\n";

    for (size_t i = 0;
         i < crawledPages.size();
         i++)
    {
        searchIndex.addPage(
            static_cast<int>(i + 1),
            crawledPages[i]
        );
    }

    cout << "\nSearch index successfully built for "
         << crawledPages.size()
         << " pages.\n";

    // =========================================================
    // 7. PAGE SEARCH DEMONSTRATION
    // =========================================================

    cout << "\n==================================================\n";
    cout << "             PAGE SEARCH DEMONSTRATION\n";
    cout << "==================================================\n";

    string testKeyword = "computer";

    cout << "\nTest Query: "
         << testKeyword
         << endl;

    vector<int> testResults =
        searchIndex.search(testKeyword);

    if (testResults.empty())
    {
        cout << "No pages found for this keyword.\n";
    }
    else
    {
        cout << "Matching Pages:\n";

        for (int pageID : testResults)
        {
            cout << "  -> Page "
                 << pageID
                 << endl;
        }
    }

    // =========================================================
    // 8. INTERACTIVE SEARCH
    // =========================================================

    cout << "\n==================================================\n";
    cout << "                SEARCH INTERFACE\n";
    cout << "==================================================\n";

    cout << "\nYou can search the vocabulary and crawled pages.\n";
    cout << "Type 'exit' to close the application.\n";

    string query;

    while (true)
    {
        cout << "\nSearch: ";

        getline(cin, query);

        // -----------------------------------------------------
        // EXIT
        // -----------------------------------------------------

        if (query == "exit")
        {
            break;
        }

        // -----------------------------------------------------
        // EMPTY QUERY
        // -----------------------------------------------------

        if (query.empty())
        {
            cout << "Please enter a search query.\n";
            continue;
        }

        // -----------------------------------------------------
        // EXACT WORD SEARCH
        // -----------------------------------------------------

        bool exactMatch =
            trie.search(query);

        if (exactMatch)
        {
            cout << "\nExact Match: Found\n";
        }
        else
        {
            cout << "\nExact Match: Not Found\n";
        }

        // Record every user query for personalization
        searchHistory.recordSearch(query);

        // -----------------------------------------------------
        // AUTOCOMPLETE
        // -----------------------------------------------------

        vector<string> suggestions =
            trie.autocomplete(query);

        // -----------------------------------------------------
        // RANK SUGGESTIONS
        // -----------------------------------------------------

        vector<string> rankedSuggestions =
            rankingEngine.rankSuggestions(
                suggestions,
                query,
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
                            suggestion)
                     << " searches]"
                     << endl;
            }
        }

        // -----------------------------------------------------
        // SEARCH CRAWLED WEB PAGES
        // -----------------------------------------------------

        vector<SearchResult> pageResults =
            searchIndex.searchPages(
                query,
                searchHistory
            );

        // Record page searches
        for (const SearchResult& result : pageResults)
        {
          searchHistory.recordPageSearch(
           result.getPageID()
          );

          searchHistory.recordQueryPage(
          query,
          result.getPageID()
          );
        }

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

                cout << "  Title            : "
                     << pageResults[i].getTitle()
                     << endl;

                cout << "  URL              : "
                     << pageResults[i].getURL()
                     << endl;
                cout << "  Relevance Score        : "
                     << pageResults[i].getScore()
                     << endl;

                cout << "  Query Frequency Score  : "
                     << pageResults[i].getQueryFrequencyScore()
                     << endl;

                cout << "  Page Frequency Score   : "
                     << pageResults[i].getPageFrequencyScore()
                     << endl;

                cout << "  Query-Page Score       : "
                     << pageResults[i].getQueryPageScore()
                     << endl;

                cout << "  Recency Bonus          : "
                     << pageResults[i].getRecencyBonus()
                     << endl;

                cout << "  Personalization        : "
                     << pageResults[i].getPersonalizationScore()
                     << endl;

                cout << "  Final Score            : "
                     << pageResults[i].getScore()
                     + pageResults[i].getPersonalizationScore()
                     << endl;
                     
                cout << "  Content          : "
                     << pageResults[i].getContent()
                     << endl;
            }
        }
    }

    // =========================================================
    // 9. SEARCH HISTORY
    // =========================================================

    searchHistory.displayHistory();

    // =========================================================
    // 10. CRAWLED PAGE SUMMARY
    // =========================================================

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
                    static_cast<int>(i + 1))
             << endl;

        cout << "  Content      : "
             << crawledPages[i].getContent()
             << endl;
    }

    // =========================================================
    // 11. PROGRAM TERMINATION
    // =========================================================

    cout << "\n==================================================\n";
    cout << "          SEARCH ENGINE SHUTTING DOWN\n";
    cout << "==================================================\n";

    cout << "\nThank you for using the "
         << "Personalized Intelligent Search Engine.\n";

    return 0;
}

