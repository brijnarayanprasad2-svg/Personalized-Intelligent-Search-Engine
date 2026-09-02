#include "../include/WebCrawler.h"

#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

// --------------------------------------------------
// Constructor
// --------------------------------------------------

WebCrawler::WebCrawler(int limit)
    : maxPages(limit)
{
    if (maxPages < 1)
        maxPages = 1;
}

// --------------------------------------------------
// Add Seed URL
// --------------------------------------------------

void WebCrawler::addSeedURL(const string& url)
{
    if (url.empty())
        return;

    if (visitedURLs.find(url) == visitedURLs.end())
        urlQueue.push(url);
}

// --------------------------------------------------
// Fetch Page
// --------------------------------------------------

string WebCrawler::fetchPage(const string& url)
{
    cout << "\nFetching: " << url << endl;

    string filename;

    if (url == "page1")
        filename = "data/page1.html";

    else if (url == "page2")
        filename = "data/page2.html";

    else if (url == "page3")
        filename = "data/page3.html";

    else
        return "";

    ifstream file(filename);

    if (!file.is_open())
    {
        cout << "Could not open: "
             << filename << endl;

        return "";
    }

    stringstream buffer;

    buffer << file.rdbuf();

    file.close();

    return buffer.str();
}

// --------------------------------------------------
// Parse HTML Page
// --------------------------------------------------

WebPage WebCrawler::parsePage(
    const string& url,
    const string& html)
{
    string title;
    string content;

    vector<string> links;

    // ----------------------------------------------
    // Extract Title
    // ----------------------------------------------

    size_t titleStart =
        html.find("<title>");

    size_t titleEnd =
        html.find("</title>");

    if (titleStart != string::npos &&
        titleEnd != string::npos &&
        titleEnd > titleStart)
    {
        titleStart += 7;

        title = html.substr(
            titleStart,
            titleEnd - titleStart
        );
    }

    // ----------------------------------------------
    // Extract Paragraph Content
    // ----------------------------------------------

    size_t position = 0;

    while (true)
    {
        size_t paragraphStart =
            html.find("<p>", position);

        if (paragraphStart == string::npos)
            break;

        size_t paragraphEnd =
            html.find("</p>", paragraphStart);

        if (paragraphEnd == string::npos)
            break;

        paragraphStart += 3;

        string paragraph =
            html.substr(
                paragraphStart,
                paragraphEnd - paragraphStart
            );

        if (!paragraph.empty())
        {
            if (!content.empty())
                content += " ";

            content += paragraph;
        }

        position = paragraphEnd + 4;
    }

    // ----------------------------------------------
    // Extract Links
    // ----------------------------------------------

    position = 0;

    while (true)
    {
        size_t hrefStart =
            html.find("href=\"", position);

        if (hrefStart == string::npos)
            break;

        hrefStart += 6;

        size_t hrefEnd =
            html.find("\"", hrefStart);

        if (hrefEnd == string::npos)
            break;

        string link =
            html.substr(
                hrefStart,
                hrefEnd - hrefStart
            );

        if (!link.empty())
            links.push_back(link);

        position = hrefEnd + 1;
    }

    // ----------------------------------------------
    // Create WebPage Object
    // ----------------------------------------------

    return WebPage(
        url,
        title,
        content,
        links
    );
}

// --------------------------------------------------
// Crawl Pages
// --------------------------------------------------

void WebCrawler::crawl()
{
    int count = 0;

    while (!urlQueue.empty() &&
           count < maxPages)
    {
        // ------------------------------------------
        // Get URL from Queue
        // ------------------------------------------

        string currentURL =
            urlQueue.front();

        urlQueue.pop();

        // ------------------------------------------
        // Avoid Duplicate URLs
        // ------------------------------------------

        if (visitedURLs.find(currentURL)
            != visitedURLs.end())
        {
            continue;
        }

        // Mark as visited
        visitedURLs.insert(currentURL);

        // ------------------------------------------
        // Fetch Page
        // ------------------------------------------

        string html =
            fetchPage(currentURL);

        if (html.empty())
        {
            cout << "Failed to fetch page."
                 << endl;

            continue;
        }

        // ------------------------------------------
        // Parse Page
        // ------------------------------------------

        WebPage page =
            parsePage(
                currentURL,
                html
            );

        // ------------------------------------------
        // Store Page
        // ------------------------------------------

        pages.push_back(page);

        // ------------------------------------------
        // Display Page Information
        // ------------------------------------------

        cout << "Title: "
             << page.getTitle()
             << endl;

        cout << "Content: "
             << page.getContent()
             << endl;

        // ------------------------------------------
        // Add Discovered Links to Queue
        // ------------------------------------------

        vector<string> links =
            page.getLinks();

        for (const string& link : links)
        {
            if (visitedURLs.find(link)
                == visitedURLs.end())
            {
                urlQueue.push(link);
            }
        }

        count++;
    }

    // ----------------------------------------------
    // Crawl Summary
    // ----------------------------------------------

    cout << "\n----------------------------------------"
         << endl;

    cout << "Total URLs crawled: "
         << count
         << endl;

    cout << "----------------------------------------"
         << endl;
}

// --------------------------------------------------
// Get Crawled Pages
// --------------------------------------------------

vector<WebPage> WebCrawler::getPages() const
{
    return pages;
}