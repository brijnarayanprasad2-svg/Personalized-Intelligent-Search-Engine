#ifndef SEARCHRESULT_H
#define SEARCHRESULT_H

#include <string>

using namespace std;

class SearchResult
{
private:
    int pageID;
    string title;
    string url;
    string content;
    double score;

public:
    SearchResult();

    SearchResult(
        int id,
        const string& pageTitle,
        const string& pageURL,
        const string& pageContent,
        double relevanceScore
    );

    int getPageID() const;

    string getTitle() const;

    string getURL() const;

    string getContent() const;

    double getScore() const;
};

#endif