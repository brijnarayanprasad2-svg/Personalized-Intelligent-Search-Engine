#ifndef HTMLPARSER_H
#define HTMLPARSER_H

#include <string>
#include <vector>

class HTMLParser
{
public:
    std::string extractTitle(
        const std::string& html
    ) const;

    std::string extractText(
        const std::string& html
    ) const;

    std::vector<std::string> extractLinks(
        const std::string& html
    ) const;
};

#endif