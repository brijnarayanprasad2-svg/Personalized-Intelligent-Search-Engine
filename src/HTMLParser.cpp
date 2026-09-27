#include "../include/HTMLParser.h"

#include <algorithm>
#include <cctype>

std::string HTMLParser::extractTitle(
    const std::string& html
) const
{
    std::string lowerHTML = html;

    std::transform(
        lowerHTML.begin(),
        lowerHTML.end(),
        lowerHTML.begin(),
        [](unsigned char ch)
        {
            return std::tolower(ch);
        }
    );

    std::string startTag = "<title>";
    std::string endTag = "</title>";

    size_t start = lowerHTML.find(startTag);

    if (start == std::string::npos)
    {
        return "";
    }

    start += startTag.length();

    size_t end = lowerHTML.find(endTag, start);

    if (end == std::string::npos)
    {
        return "";
    }

    return html.substr(
        start,
        end - start
    );
}

std::string HTMLParser::extractText(
    const std::string& html
) const
{
    std::string text;
    bool insideTag = false;

    for (char ch : html)
    {
        if (ch == '<')
        {
            insideTag = true;
            continue;
        }

        if (ch == '>')
        {
            insideTag = false;
            text += ' ';
            continue;
        }

        if (!insideTag)
        {
            text += ch;
        }
    }

    std::string cleanedText;
    bool previousSpace = false;

    for (char ch : text)
    {
        if (std::isspace(
                static_cast<unsigned char>(ch)))
        {
            if (!previousSpace)
            {
                cleanedText += ' ';
                previousSpace = true;
            }
        }
        else
        {
            cleanedText += ch;
            previousSpace = false;
        }
    }

    return cleanedText;
}
std::vector<std::string> HTMLParser::extractLinks(
    const std::string& html
) const
{
    std::vector<std::string> links;

    std::string lowerHTML = html;

    std::transform(
        lowerHTML.begin(),
        lowerHTML.end(),
        lowerHTML.begin(),
        [](unsigned char ch)
        {
            return std::tolower(ch);
        }
    );

    size_t position = 0;

    while (true)
    {
        size_t anchorStart =
            lowerHTML.find("<a", position);

        if (anchorStart == std::string::npos)
        {
            break;
        }

        size_t hrefStart =
            lowerHTML.find("href", anchorStart);

        if (hrefStart == std::string::npos)
        {
            break;
        }

        size_t equalSign =
            lowerHTML.find("=", hrefStart);

        if (equalSign == std::string::npos)
        {
            break;
        }

        size_t valueStart = equalSign + 1;

        while (
            valueStart < html.length() &&
            std::isspace(
                static_cast<unsigned char>(
                    html[valueStart]
                )
            )
        )
        {
            valueStart++;
        }

        if (
            valueStart >= html.length() ||
            (html[valueStart] != '"' &&
             html[valueStart] != '\'')
        )
        {
            position = equalSign + 1;
            continue;
        }

        char quote = html[valueStart];

        size_t valueEnd =
            html.find(
                quote,
                valueStart + 1
            );

        if (valueEnd == std::string::npos)
        {
            break;
        }

        std::string url =
            html.substr(
                valueStart + 1,
                valueEnd - valueStart - 1
            );

        if (!url.empty())
        {
            links.push_back(url);
        }

        position = valueEnd + 1;
    }

    return links;
}