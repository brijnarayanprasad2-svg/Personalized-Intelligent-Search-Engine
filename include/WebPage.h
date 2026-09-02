#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <string>
#include <vector>

using namespace std;

class WebPage
{
private:
    string url;
    string title;
    string content;
    vector<string> links;

public:
    WebPage();

    WebPage(
        const string& url,
        const string& title,
        const string& content,
        const vector<string>& links
    );

    string getURL() const;

    string getTitle() const;

    string getContent() const;

    vector<string> getLinks() const;
};

#endif