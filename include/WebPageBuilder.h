#ifndef WEBPAGEBUILDER_H
#define WEBPAGEBUILDER_H

#include <string>

#include "WebPage.h"

class WebPageBuilder
{
public:
    WebPage buildPage(
        const std::string& url,
        const std::string& html
    ) const;
};

#endif