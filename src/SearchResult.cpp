#include "../include/SearchResult.h"

using namespace std;

SearchResult::SearchResult()
{
    pageID = 0;
    title = "";
    url = "";
    content = "";
    score = 0.0;
}

SearchResult::SearchResult(
    int id,
    const string& pageTitle,
    const string& pageURL,
    const string& pageContent,
    double relevanceScore)
{
    pageID = id;
    title = pageTitle;
    url = pageURL;
    content = pageContent;
    score = relevanceScore;
}

int SearchResult::getPageID() const
{
    return pageID;
}

string SearchResult::getTitle() const
{
    return title;
}

string SearchResult::getURL() const
{
    return url;
}

string SearchResult::getContent() const
{
    return content;
}

double SearchResult::getScore() const
{
    return score;
}