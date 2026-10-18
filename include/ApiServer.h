
#ifndef APISERVER_H
#define APISERVER_H

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>

    using SOCKET = int;

    #define INVALID_SOCKET (-1)
    #define SOCKET_ERROR (-1)
    #define closesocket close
    #define SD_BOTH SHUT_RDWR
#endif

#include <atomic>
#include <functional>
#include <string>

class ApiServer
{
public:
    using RequestHandler =
        std::function<std::string(const std::string&)>;

    explicit ApiServer(int port = 8080);

    void setRequestHandler(RequestHandler handler);

    void run();
    void stop();

private:
    int port;
    RequestHandler requestHandler;
    std::atomic<bool> running;
    SOCKET listenSocket;

    void handleClient(SOCKET clientSocket);

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
