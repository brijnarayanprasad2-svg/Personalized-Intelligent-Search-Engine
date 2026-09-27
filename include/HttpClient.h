#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <string>

class HttpClient
{
public:
    std::string fetchURL(const std::string& url);
};

#endif