#ifndef WEBCRAWLER_H
#define WEBCRAWLER_H

#include "WebPage.h"

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

    vector<WebPage> pages;

    int maxPages;

    // Fetch HTML content from a local page
    string fetchPage(const string& url);

    // Convert HTML into a WebPage object
    WebPage parsePage(
        const string& url,
        const string& html
    );

public:
    // Constructor
    explicit WebCrawler(int limit = 10);

    // Add a starting URL
    void addSeedURL(const string& url);

    // Start BFS-style crawling
    void crawl();

    // Return all crawled pages
    vector<WebPage> getPages() const;
};

#endif