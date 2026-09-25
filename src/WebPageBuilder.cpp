#include "../include/WebPageBuilder.h"
#include "../include/HTMLParser.h"

#include <vector>

WebPage WebPageBuilder::buildPage(
    const std::string& url,
    const std::string& html
) const
{
    HTMLParser parser;

    std::string title =
        parser.extractTitle(html);

    std::string content =
        parser.extractText(html);

    std::vector<std::string> links =
        parser.extractLinks(html);

    WebPage page(
        url,
        title,
        content,
        links
    );

    return page;
}