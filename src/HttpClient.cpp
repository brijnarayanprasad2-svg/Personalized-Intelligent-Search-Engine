#include <curl/curl.h>

#include "../include/HttpClient.h"

#include <string>
#include <iostream>
size_t writeCallback(
    void* contents,
    size_t size,
    size_t nmemb,
    void* userData
)
{
    size_t totalSize = size * nmemb;

    std::string* output =
        static_cast<std::string*>(userData);

    output->append(
        static_cast<char*>(contents),
        totalSize
    );

    return totalSize;
}

std::string HttpClient::fetchURL(
    const std::string& url
)
{
    CURL* curl = curl_easy_init();

    if (curl == nullptr)
    {
        return "";
    }

    std::string response;

    curl_easy_setopt(
        curl,
        CURLOPT_URL,
        url.c_str()
    );

    curl_easy_setopt(
        curl,
        CURLOPT_WRITEFUNCTION,
        writeCallback
    );

    curl_easy_setopt(
        curl,
        CURLOPT_WRITEDATA,
        &response
    );

    curl_easy_setopt(
        curl,
        CURLOPT_FOLLOWLOCATION,
        1L
    );

    curl_easy_setopt(
        curl,
        CURLOPT_USERAGENT,
        "PersonalizedSearchEngine/1.0"
    );
    CURLcode result =
    curl_easy_perform(curl);

    if (result != CURLE_OK)
    {
      std::cout << "Curl error: "
         << curl_easy_strerror(result)
         << std::endl;

       curl_easy_cleanup(curl);

       return "";
    }

   curl_easy_cleanup(curl);

    return response;
}