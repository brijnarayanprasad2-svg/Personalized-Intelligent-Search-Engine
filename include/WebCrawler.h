#ifndef WEBCRAWLER_H
#define WEBCRAWLER_H

#include "WebPage.h"
#include "HttpClient.h"

#include <string>
#include <vector>
#include <queue>
#include <unordered_set>

using namespace std;

class WebCrawler
{
private:
    queue<string> urlQueue;

    unordered_set<string> visitedURLs;

    unordered_set<string> queuedURLs;

    vector<WebPage> pages;

    int maxPages;

    // Fetch HTML content from a URL
    string fetchPage(
        const string& url
    );

    // Convert HTML into a WebPage object
    WebPage parsePage(
        const string& url,
        const string& html
    );

    // Remove fragment and trailing slash
    string canonicalizeURL(
        const string& url
    ) const;

    // Check whether URL can be crawled
    bool isCrawlableURL(
        const string& url
    ) const;

public:
    // Constructor
    explicit WebCrawler(
        int limit = 10
    );

    // Add a starting URL
    void addSeedURL(
        const string& url
    );

    // Start BFS-style crawling
    void crawl();

    // Return all crawled pages
    vector<WebPage> getPages() const;
};

#endif