#include "../include/WebPage.h"

WebPage::WebPage()
    : url(""),
      title(""),
      content("")
{
}

WebPage::WebPage(
    const string& pageURL,
    const string& pageTitle,
    const string& pageContent,
    const vector<string>& pageLinks)
    : url(pageURL),
      title(pageTitle),
      content(pageContent),
      links(pageLinks)
{
}

string WebPage::getURL() const
{
    return url;
}

string WebPage::getTitle() const
{
    return title;
}

string WebPage::getContent() const
{
    return content;
}

vector<string> WebPage::getLinks() const
{
    return links;
}