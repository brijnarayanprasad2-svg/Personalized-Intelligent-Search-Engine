#ifndef APISERVER_H
#define APISERVER_H

#include <winsock2.h>

#include <atomic>
#include <functional>
#include <string>

class ApiServer
{
public:
    using RequestHandler =
        std::function<std::string(const std::string&)>;

    explicit ApiServer(int port = 8080);

    void setRequestHandler(
        RequestHandler handler
    );

    void run();

    void stop();

private:
    int port;

    RequestHandler requestHandler;

    std::atomic<bool> running;

    SOCKET listenSocket;

    void handleClient(
        SOCKET clientSocket
    );

    std::string buildHttpResponse(
        int statusCode,
        const std::string& statusText,
        const std::string& body
    ) const;

    std::string extractRequestTarget(
        const std::string& request
    ) const;

    std::string extractPath(
        const std::string& target
    ) const;

    std::string extractQueryParameter(
        const std::string& target,
        const std::string& parameter
    ) const;

    std::string urlDecode(
        const std::string& value
    ) const;

    bool sendAll(
        SOCKET socket,
        const std::string& data
    ) const;
};

#endif
