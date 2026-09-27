#include "../include/WebCrawler.h"
#include "../include/HttpClient.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <unordered_set>
#include <vector>

using namespace std;


// ==================================================
// Helper: Remove HTML Tags
// ==================================================

string removeHTMLTags(const string& text)
{
    string result;
    result.reserve(text.size());

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


// ==================================================
// Helper: Normalize Text
// ==================================================

string normalizeText(const string& text)
{
    string result;
    result.reserve(text.size());

    bool previousWasSpace = false;

    for (char ch : text)
    {
        unsigned char c =
            static_cast<unsigned char>(ch);

        if (isspace(c))
        {
            if (!previousWasSpace)
            {
                result += ' ';
                previousWasSpace = true;
            }
        }
        else
        {
            result +=
                static_cast<char>(
                    tolower(c)
                );

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


// ==================================================
// Helper: Check HTTP / HTTPS URL
// ==================================================

bool isHTTPURL(const string& url)
{
    return url.find("http://") == 0 ||
           url.find("https://") == 0;
}


// ==================================================
// Helper: Canonicalize URL
// ==================================================

string WebCrawler::canonicalizeURL(
    const string& url) const
{
    string result = url;

    // Remove fragment
    size_t fragmentPosition =
        result.find('#');

    if (fragmentPosition != string::npos)
    {
        result.erase(fragmentPosition);
    }

    // Remove trailing slash
    if (result.length() > 8 &&
        result.back() == '/')
    {
        result.pop_back();
    }

    return result;
}


// ==================================================
// Helper: Check Crawlable URL
// ==================================================

bool WebCrawler::isCrawlableURL(
    const string& url
) const
{
    if (!isHTTPURL(url))
    {
        return false;
    }

    string lowerURL = url;

    for (char& ch : lowerURL)
    {
        ch = static_cast<char>(
            tolower(
                static_cast<unsigned char>(ch)
            )
        );
    }

    // Reject unsupported protocols
    if (lowerURL.find("rsync://") == 0 ||
        lowerURL.find("ftp://") == 0 ||
        lowerURL.find("mailto:") == 0 ||
        lowerURL.find("javascript:") == 0)
    {
        return false;
    }

    size_t queryPosition =
        lowerURL.find('?');

    if (queryPosition != string::npos)
    {
        lowerURL.erase(queryPosition);
    }

    size_t fragmentPosition =
        lowerURL.find('#');

    if (fragmentPosition != string::npos)
    {
        lowerURL.erase(fragmentPosition);
    }

    const vector<string> blockedExtensions =
    {
        ".css",
        ".js",
        ".ico",
        ".png",
        ".jpg",
        ".jpeg",
        ".gif",
        ".svg",
        ".webp",
        ".pdf",
        ".zip",
        ".rar",
        ".mp3",
        ".mp4",
        ".avi",
        ".mov"
    };

    for (const string& extension :
         blockedExtensions)
    {
        if (lowerURL.size() >= extension.size() &&
            lowerURL.compare(
                lowerURL.size() - extension.size(),
                extension.size(),
                extension
            ) == 0)
        {
            return false;
        }
    }
    size_t protocolMarker =
    lowerURL.find("://", 8);

    if (protocolMarker != string::npos)
    {
      return false;
    }

    return true;
}


// ==================================================
// Helper: Get Domain
// ==================================================

string getDomain(const string& url)
{
    size_t protocolEnd =
        url.find("://");

    if (protocolEnd == string::npos)
    {
        return "";
    }

    size_t domainStart =
        protocolEnd + 3;

    size_t domainEnd =
        url.find(
            '/',
            domainStart
        );

    string domain;

    if (domainEnd == string::npos)
    {
        domain =
            url.substr(domainStart);
    }
    else
    {
        domain =
            url.substr(
                domainStart,
                domainEnd - domainStart
            );
    }

    // Remove port
    size_t portPosition =
        domain.find(':');

    if (portPosition != string::npos)
    {
        domain.erase(portPosition);
    }

    // Lowercase domain
    for (char& ch : domain)
    {
        ch =
            static_cast<char>(
                tolower(
                    static_cast<unsigned char>(ch)
                )
            );
    }

    return domain;
}


// ==================================================
// Helper: Same Domain Check
// ==================================================

bool isSameDomain(
    const string& baseURL,
    const string& link)
{
    string baseDomain =
        getDomain(baseURL);

    string linkDomain =
        getDomain(link);

    if (baseDomain.empty() ||
        linkDomain.empty())
    {
        return false;
    }

    return baseDomain == linkDomain;
}


// ==================================================
// Helper: Resolve Relative URL
// ==================================================

string resolveURL(
    const string& baseURL,
    const string& link)
{
    // Already absolute HTTP/HTTPS URL
    if (isHTTPURL(link))
    {
        return link;
    }

    // Relative URLs only work with HTTP pages
    if (!isHTTPURL(baseURL))
    {
        return link;
    }

    size_t protocolEnd =
        baseURL.find("://");

    if (protocolEnd == string::npos)
    {
        return link;
    }

    size_t hostStart =
        protocolEnd + 3;

    size_t pathStart =
        baseURL.find(
            '/',
            hostStart
        );

    string baseURLRoot;

    if (pathStart == string::npos)
    {
        baseURLRoot =
            baseURL;
    }
    else
    {
        baseURLRoot =
            baseURL.substr(
                0,
                pathStart
            );
    }

    // Root-relative URL
    // Example: /about

    if (!link.empty() &&
        link[0] == '/')
    {
        return baseURLRoot + link;
    }

    // Relative URL
    // Example: about

    size_t lastSlash =
        baseURL.rfind('/');

    if (lastSlash != string::npos &&
        lastSlash >= hostStart)
    {
        return
            baseURL.substr(
                0,
                lastSlash + 1
            )
            + link;
    }

    return baseURLRoot + "/" + link;
}


// ==================================================
// Constructor
// ==================================================

WebCrawler::WebCrawler(int limit)
    : maxPages(limit)
{
    if (maxPages < 1)
    {
        maxPages = 1;
    }
}


// ==================================================
// Add Seed URL
// ==================================================

void WebCrawler::addSeedURL(
    const string& url)
{
    if (url.empty())
    {
        return;
    }

    string cleanURL =
        canonicalizeURL(url);

    // Reject unknown protocols
    if (!isHTTPURL(cleanURL) &&
        cleanURL.find("://") != string::npos)
    {
        cout << "Rejected URL: "
             << cleanURL
             << endl;

        return;
    }

    // HTTP resource validation
    if (isHTTPURL(cleanURL) &&
        !isCrawlableURL(cleanURL))
    {
        cout << "Rejected URL: "
             << cleanURL
             << endl;

        return;
    }

    // Avoid duplicate seed
    if (visitedURLs.find(cleanURL) !=
        visitedURLs.end())
    {
        return;
    }

    if (queuedURLs.find(cleanURL) !=
        queuedURLs.end())
    {
        return;
    }

    urlQueue.push(cleanURL);
    queuedURLs.insert(cleanURL);
}


// ==================================================
// Fetch Page
// ==================================================

string WebCrawler::fetchPage(
    const string& url)
{
    cout << "\nFetching: "
         << url
         << endl;

    // ------------------------------------------------
    // HTTP / HTTPS
    // ------------------------------------------------

    if (isHTTPURL(url))
    {
        HttpClient client;

        string html =
            client.fetchURL(url);

        if (html.empty())
        {
            cout << "HTTP fetch failed."
                 << endl;
        }

        return html;
    }


    // ------------------------------------------------
    // Local HTML File
    // ------------------------------------------------

    string filename =
        "data/" + url;

    if (filename.size() < 5 ||
        filename.substr(
            filename.size() - 5
        ) != ".html")
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

    return buffer.str();
}


// ==================================================
// Parse HTML Page
// ==================================================

WebPage WebCrawler::parsePage(
    const string& url,
    const string& html)
{
    string title;
    string content;

    vector<string> links;

    unordered_set<string> uniqueLinks;


    // =================================================
    // Convert HTML to lowercase for searching
    // =================================================

    string lowerHTML = html;

    transform(
        lowerHTML.begin(),
        lowerHTML.end(),
        lowerHTML.begin(),
        [](unsigned char ch)
        {
            return static_cast<char>(
                tolower(ch)
            );
        }
    );


    // =================================================
    // Extract Title
    // =================================================

    size_t titleStart =
        lowerHTML.find("<title>");

    if (titleStart != string::npos)
    {
        size_t titleEnd =
            lowerHTML.find(
                "</title>",
                titleStart + 7
            );

        if (titleEnd != string::npos)
        {
            titleStart += 7;

            title =
                html.substr(
                    titleStart,
                    titleEnd - titleStart
                );

            title =
                normalizeText(
                    removeHTMLTags(title)
                );
        }
    }


    // =================================================
    // Extract Paragraph Content
    // =================================================

    size_t position = 0;

    while (true)
    {
        size_t paragraphStart =
            lowerHTML.find(
                "<p>",
                position
            );

        if (paragraphStart ==
            string::npos)
        {
            break;
        }

        size_t paragraphEnd =
            lowerHTML.find(
                "</p>",
                paragraphStart + 3
            );

        if (paragraphEnd ==
            string::npos)
        {
            break;
        }

        paragraphStart += 3;

        string paragraph =
            html.substr(
                paragraphStart,
                paragraphEnd -
                paragraphStart
            );

        paragraph =
            normalizeText(
                removeHTMLTags(
                    paragraph
                )
            );

        if (!paragraph.empty())
        {
            if (!content.empty())
            {
                content += " ";
            }

            content += paragraph;
        }

        position =
            paragraphEnd + 4;
    }


    // =================================================
    // Extract Links
    // =================================================

    position = 0;

    while (true)
    {
        size_t hrefStart =
            lowerHTML.find(
                "href=",
                position
            );

        if (hrefStart ==
            string::npos)
        {
            break;
        }

        size_t valueStart =
            hrefStart + 5;


        // ------------------------------------------------
        // Skip whitespace
        // ------------------------------------------------

        while (
            valueStart < html.length() &&
            isspace(
                static_cast<unsigned char>(
                    html[valueStart]
                )
            )
        )
        {
            valueStart++;
        }

        if (valueStart >= html.length())
        {
            break;
        }


        // ------------------------------------------------
        // Check quote
        // ------------------------------------------------

        char quote =
            html[valueStart];

        if (quote != '"' &&
            quote != '\'')
        {
            position =
                valueStart + 1;

            continue;
        }


        // ------------------------------------------------
        // Find closing quote
        // ------------------------------------------------

        size_t valueEnd =
            html.find(
                quote,
                valueStart + 1
            );

        if (valueEnd ==
            string::npos)
        {
            break;
        }


        string link =
            html.substr(
                valueStart + 1,
                valueEnd -
                valueStart -
                1
            );


        // ------------------------------------------------
        // Remove unwanted links
        // ------------------------------------------------

        if (
            link.empty() ||
            link[0] == '#' ||
            link.find("javascript:") == 0 ||
            link.find("mailto:") == 0 ||
            link.find("tel:") == 0 ||
            link.find("data:") == 0
        )
        {
            position =
                valueEnd + 1;

            continue;
        }


        // ------------------------------------------------
        // Local project link
        // ------------------------------------------------

        if (!isHTTPURL(url))
        {
            if (link.find("://") !=
                string::npos)
            {
                cout << "  Rejected link: "
                     << link
                     << endl;

                position =
                    valueEnd + 1;

                continue;
            }

            if (link.find('.') ==
                string::npos)
            {
                link += ".html";
            }

            if (uniqueLinks.insert(link).second)
            {
                links.push_back(link);

                cout << "  Link found: "
                     << link
                     << endl;
            }

            position =
                valueEnd + 1;

            continue;
        }


        // ------------------------------------------------
        // HTTP / HTTPS link
        // ------------------------------------------------

        string resolvedLink =
            resolveURL(
                url,
                link
            );


        // ------------------------------------------------
        // Canonicalize
        // ------------------------------------------------

        resolvedLink =
            canonicalizeURL(
                resolvedLink
            );


        // ------------------------------------------------
        // Validate
        // ------------------------------------------------

        if (!isHTTPURL(resolvedLink))
        {
            cout << "  Rejected link: "
                 << link
                 << endl;

            position =
                valueEnd + 1;

            continue;
        }

        if (!isCrawlableURL(resolvedLink))
        {
            cout << "  Rejected resource: "
                 << resolvedLink
                 << endl;

            position =
                valueEnd + 1;

            continue;
        }


        // ------------------------------------------------
        // Same-domain check
        // ------------------------------------------------

        if (!isSameDomain(
                url,
                resolvedLink))
        {
            cout << "  Rejected external domain: "
                 << resolvedLink
                 << endl;

            position =
                valueEnd + 1;

            continue;
        }


        // ------------------------------------------------
        // Remove duplicate links
        // ------------------------------------------------

        if (uniqueLinks.insert(
                resolvedLink
            ).second)
        {
            links.push_back(
                resolvedLink
            );

            cout << "  Link found: "
                 << resolvedLink
                 << endl;
        }


        position =
            valueEnd + 1;
    }


    // =================================================
    // Create WebPage
    // =================================================

    return WebPage(
        url,
        title,
        content,
        links
    );
}


// ==================================================
// Crawl Pages - BFS
// ==================================================
// ==================================================
// Crawl Pages - Controlled BFS
// ==================================================

void WebCrawler::crawl()
{
    int count = 0;

    // Keep only a controlled number of pending URLs.
    // This prevents unnecessary queue growth.
    const int MAX_PENDING_URLS =
        maxPages * 2;

    while (!urlQueue.empty() &&
           count < maxPages)
    {
        // ------------------------------------------------
        // Get next URL from BFS queue
        // ------------------------------------------------

        string currentURL =
            urlQueue.front();

        urlQueue.pop();

        queuedURLs.erase(
            currentURL
        );


        // ------------------------------------------------
        // Avoid duplicate URL
        // ------------------------------------------------

        if (visitedURLs.find(currentURL) !=
            visitedURLs.end())
        {
            continue;
        }


        // ------------------------------------------------
        // Validate current URL
        // ------------------------------------------------

        if (isHTTPURL(currentURL) &&
            !isCrawlableURL(currentURL))
        {
            cout << "Rejected URL: "
                 << currentURL
                 << endl;

            continue;
        }


        // ------------------------------------------------
        // Mark as visited
        // ------------------------------------------------

        visitedURLs.insert(
            currentURL
        );


        // ------------------------------------------------
        // Fetch page
        // ------------------------------------------------

        string html =
            fetchPage(currentURL);

        if (html.empty())
        {
            cout << "Failed to fetch page."
                 << endl;

            continue;
        }


        // ------------------------------------------------
        // Parse page
        // ------------------------------------------------

        WebPage page =
            parsePage(
                currentURL,
                html
            );


        // ------------------------------------------------
        // Store page
        // ------------------------------------------------

        pages.push_back(page);


        // ------------------------------------------------
        // Display page information
        // ------------------------------------------------

        cout << "Title: "
             << page.getTitle()
             << endl;

        cout << "Content: "
             << page.getContent()
             << endl;


        // ------------------------------------------------
        // Process discovered links
        // ------------------------------------------------

        vector<string> discoveredLinks =
            page.getLinks();

        for (const string& link :
             discoveredLinks)
        {
            if (link.empty())
            {
                continue;
            }


            // ------------------------------------------------
            // Canonicalize URL
            // ------------------------------------------------

            string canonicalURL =
                canonicalizeURL(link);

            if (canonicalURL.empty())
            {
                continue;
            }


            // ------------------------------------------------
            // Validate URL
            // ------------------------------------------------

            bool validLink = true;

            if (isHTTPURL(canonicalURL))
            {
                validLink =
                    isCrawlableURL(
                        canonicalURL
                    );
            }
            else if (canonicalURL.find("://") !=
                     string::npos)
            {
                validLink = false;
            }


            if (!validLink)
            {
                cout << "  Rejected link: "
                     << canonicalURL
                     << endl;

                continue;
            }


            // ------------------------------------------------
            // Same-domain filtering
            // ------------------------------------------------

            if (isHTTPURL(canonicalURL) &&
                isHTTPURL(currentURL) &&
                !isSameDomain(
                    currentURL,
                    canonicalURL
                ))
            {
                cout << "  Rejected external domain: "
                     << canonicalURL
                     << endl;

                continue;
            }


            // ------------------------------------------------
            // Duplicate URL prevention
            // ------------------------------------------------

            if (visitedURLs.find(canonicalURL) !=
                    visitedURLs.end() ||
                queuedURLs.find(canonicalURL) !=
                    queuedURLs.end())
            {
                continue;
            }


            // ------------------------------------------------
            // Control BFS queue size
            // ------------------------------------------------

            if (static_cast<int>(
                    urlQueue.size()
                ) >= MAX_PENDING_URLS)
            {
                continue;
            }


            // ------------------------------------------------
            // Add URL to BFS queue
            // ------------------------------------------------

            cout << "  Link queued: "
                 << canonicalURL
                 << endl;

            urlQueue.push(
                canonicalURL
            );

            queuedURLs.insert(
                canonicalURL
            );
        }


        count++;
    }


    // ==================================================
    // Crawl Summary
    // ==================================================

    cout << "\n----------------------------------------"
         << endl;

    cout << "Total URLs crawled: "
         << count
         << endl;

    cout << "Pending URLs remaining: "
         << urlQueue.size()
         << endl;

    cout << "----------------------------------------"
         << endl;
}

// ==================================================
// Get Crawled Pages
// ==================================================

vector<WebPage> WebCrawler::getPages() const
{
    return pages;
}