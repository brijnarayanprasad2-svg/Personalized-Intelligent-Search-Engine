#include "../include/WebCrawler.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <cctype>

using namespace std;

// --------------------------------------------------
// Remove HTML Tags
// --------------------------------------------------

string removeHTMLTags(const string& text)
{
    string result;
    bool insideTag = false;

    for (char ch : text)
    {
        if (ch == '<')
        {
            insideTag = true;
        }
        else if (ch == '>')
        {
            insideTag = false;
        }
        else if (!insideTag)
        {
            result += ch;
        }
    }

    return result;
}

// --------------------------------------------------
// Normalize Text
// --------------------------------------------------

string normalizeText(const string& text)
{
    string result;
    bool previousWasSpace = false;

    for (char ch : text)
    {
        // Convert uppercase characters to lowercase
        ch = static_cast<char>(
            tolower(static_cast<unsigned char>(ch))
        );

        // Convert multiple whitespace into one space
        if (isspace(static_cast<unsigned char>(ch)))
        {
            if (!previousWasSpace)
            {
                result += ' ';
                previousWasSpace = true;
            }
        }
        else
        {
            result += ch;
            previousWasSpace = false;
        }
    }

    // Remove leading space
    if (!result.empty() &&
        result.front() == ' ')
    {
        result.erase(0, 1);
    }

    // Remove trailing space
    if (!result.empty() &&
        result.back() == ' ')
    {
        result.pop_back();
    }

    return result;
}

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

    // Add URL only if it has not been
    // visited or already added to the queue
    if (visitedURLs.find(url) == visitedURLs.end() &&
        queuedURLs.find(url) == queuedURLs.end())
    {
        urlQueue.push(url);
        queuedURLs.insert(url);
    }
}

// --------------------------------------------------
// Fetch Page
// --------------------------------------------------

string WebCrawler::fetchPage(const string& url)
{
    cout << "\nFetching: "
         << url
         << endl;

    // Create local file path
    string filename = "data/" + url;

    // Add .html if extension is missing
    if (filename.size() < 5 ||
        filename.substr(filename.size() - 5) != ".html")
    {
        filename += ".html";
    }

    ifstream file(filename);

    if (!file.is_open())
    {
        cout << "Could not open: "
             << filename
             << endl;

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

        // Clean and normalize title
        title = removeHTMLTags(title);
        title = normalizeText(title);
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

        // Remove HTML tags
        paragraph = removeHTMLTags(paragraph);

        // Normalize text
        paragraph = normalizeText(paragraph);

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

        // ------------------------------------------
        // Accept only local links
        // ------------------------------------------

        if (!link.empty() &&
            link.find("http://") != 0 &&
            link.find("https://") != 0 &&
            link.find("mailto:") != 0 &&
            link[0] != '#')
        {
            // Add .html if extension is missing
            if (link.size() < 5 ||
                link.substr(link.size() - 5) != ".html")
            {
                link += ".html";
            }

            links.push_back(link);

            cout << "  Link found: "
                 << link
                 << endl;
        }

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

        // URL is no longer waiting in queue
        queuedURLs.erase(currentURL);

        // ------------------------------------------
        // Avoid Duplicate URLs
        // ------------------------------------------

        if (visitedURLs.find(currentURL)
            != visitedURLs.end())
        {
            continue;
        }

        // Mark URL as visited
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

        vector<string> discoveredLinks =
            page.getLinks();

        for (const string& link : discoveredLinks)
        {
            // Add only URLs which are not
            // visited or already queued
            if (visitedURLs.find(link)
                    == visitedURLs.end() &&
                queuedURLs.find(link)
                    == queuedURLs.end())
            {
                urlQueue.push(link);
                queuedURLs.insert(link);
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