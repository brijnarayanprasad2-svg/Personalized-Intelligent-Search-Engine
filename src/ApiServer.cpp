#include "../include/ApiServer.h"

#include <ws2tcpip.h>

#include <iostream>
#include <sstream>

ApiServer::ApiServer(int port)
    : port(port),
      running(false),
      listenSocket(INVALID_SOCKET)
{
}


void ApiServer::setRequestHandler(
    RequestHandler handler
)
{
    requestHandler = std::move(handler);
}


void ApiServer::run()
{
    if (running)
    {
        return;
    }

    WSADATA wsaData{};

    int wsaResult =
        WSAStartup(
            MAKEWORD(2, 2),
            &wsaData
        );

    if (wsaResult != 0)
    {
        std::cerr
            << "WSAStartup failed: "
            << wsaResult
            << std::endl;

        return;
    }


    listenSocket =
        socket(
            AF_INET,
            SOCK_STREAM,
            IPPROTO_TCP
        );

    if (listenSocket == INVALID_SOCKET)
    {
        std::cerr
            << "Socket creation failed."
            << std::endl;

        WSACleanup();

        return;
    }


    int option = 1;

    setsockopt(
        listenSocket,
        SOL_SOCKET,
        SO_REUSEADDR,
        reinterpret_cast<const char*>(&option),
        sizeof(option)
    );


    sockaddr_in serverAddress{};

    serverAddress.sin_family =
        AF_INET;

    serverAddress.sin_port =
        htons(
            static_cast<u_short>(port)
        );

    inet_pton(
        AF_INET,
        "127.0.0.1",
        &serverAddress.sin_addr
    );


    int bindResult =
        bind(
            listenSocket,
            reinterpret_cast<sockaddr*>(
                &serverAddress
            ),
            sizeof(serverAddress)
        );

    if (bindResult == SOCKET_ERROR)
    {
        std::cerr
            << "Bind failed on port "
            << port
            << "."
            << std::endl;

        closesocket(listenSocket);

        listenSocket =
            INVALID_SOCKET;

        WSACleanup();

        return;
    }


    int listenResult =
        listen(
            listenSocket,
            SOMAXCONN
        );

    if (listenResult == SOCKET_ERROR)
    {
        std::cerr
            << "Listen failed."
            << std::endl;

        closesocket(listenSocket);

        listenSocket =
            INVALID_SOCKET;

        WSACleanup();

        return;
    }


    running = true;


    std::cout
        << "\n==================================================\n"
        << "                 C++ API SERVER\n"
        << "==================================================\n";

    std::cout
        << "API server running at:\n"
        << "http://127.0.0.1:"
        << port
        << std::endl;


    while (running)
    {
        sockaddr_in clientAddress{};

        int clientAddressLength =
            sizeof(clientAddress);


        SOCKET clientSocket =
            accept(
                listenSocket,
                reinterpret_cast<sockaddr*>(
                    &clientAddress
                ),
                &clientAddressLength
            );


        if (clientSocket == INVALID_SOCKET)
        {
            if (running)
            {
                std::cerr
                    << "Accept failed."
                    << std::endl;
            }

            break;
        }


        handleClient(
            clientSocket
        );
    }


    if (listenSocket != INVALID_SOCKET)
    {
        closesocket(listenSocket);

        listenSocket =
            INVALID_SOCKET;
    }


    running = false;

    WSACleanup();
}


void ApiServer::stop()
{
    running = false;


    if (listenSocket != INVALID_SOCKET)
    {
        shutdown(
            listenSocket,
            SD_BOTH
        );

        closesocket(
            listenSocket
        );

        listenSocket =
            INVALID_SOCKET;
    }
}


void ApiServer::handleClient(
    SOCKET clientSocket
)
{
    char buffer[8192];

    int bytesReceived =
        recv(
            clientSocket,
            buffer,
            sizeof(buffer) - 1,
            0
        );


    if (bytesReceived <= 0)
    {
        closesocket(clientSocket);

        return;
    }


    buffer[bytesReceived] =
        '\0';


    std::string request(
        buffer,
        bytesReceived
    );


    std::string target =
        extractRequestTarget(
            request
        );


    std::string path =
        extractPath(
            target
        );


    // -------------------------------------------------
    // CORS / OPTIONS
    // -------------------------------------------------

    if (request.find("OPTIONS ") == 0)
    {
        std::string response =
            "HTTP/1.1 204 No Content\r\n"
            "Access-Control-Allow-Origin: *\r\n"
            "Access-Control-Allow-Methods: GET, OPTIONS\r\n"
            "Access-Control-Allow-Headers: Content-Type\r\n"
            "Content-Length: 0\r\n"
            "Connection: close\r\n"
            "\r\n";

        sendAll(
            clientSocket,
            response
        );

        closesocket(clientSocket);

        return;
    }


    // -------------------------------------------------
    // HEALTH CHECK
    // -------------------------------------------------

    if (path == "/health")
    {
        std::string body =
            "{\"status\":\"ok\","
            "\"service\":\"Personalized Intelligent Search Engine API\"}";


        std::string response =
            buildHttpResponse(
                200,
                "OK",
                body
            );


        sendAll(
            clientSocket,
            response
        );

        closesocket(clientSocket);

        return;
    }


    // -------------------------------------------------
    // SEARCH
    // -------------------------------------------------

    if (path == "/search")
    {
        std::string query =
            extractQueryParameter(
                target,
                "q"
            );


        if (query.empty())
        {
            std::string body =
                "{\"error\":\"Missing query parameter q\"}";


            std::string response =
                buildHttpResponse(
                    400,
                    "Bad Request",
                    body
                );


            sendAll(
                clientSocket,
                response
            );

            closesocket(clientSocket);

            return;
        }


        if (!requestHandler)
        {
            std::string body =
                "{\"error\":\"Search handler is not configured\"}";


            std::string response =
                buildHttpResponse(
                    503,
                    "Service Unavailable",
                    body
                );


            sendAll(
                clientSocket,
                response
            );

            closesocket(clientSocket);

            return;
        }


        std::string body =
            requestHandler(
                query
            );


        std::string response =
            buildHttpResponse(
                200,
                "OK",
                body
            );


        sendAll(
            clientSocket,
            response
        );

        closesocket(clientSocket);

        return;
    }


    // -------------------------------------------------
    // NOT FOUND
    // -------------------------------------------------

    std::string body =
        "{\"error\":\"Endpoint not found\"}";


    std::string response =
        buildHttpResponse(
            404,
            "Not Found",
            body
        );


    sendAll(
        clientSocket,
        response
    );


    closesocket(clientSocket);
}


std::string ApiServer::buildHttpResponse(
    int statusCode,
    const std::string& statusText,
    const std::string& body
) const
{
    std::ostringstream response;


    response
        << "HTTP/1.1 "
        << statusCode
        << " "
        << statusText
        << "\r\n";


    response
        << "Content-Type: application/json; "
        << "charset=utf-8\r\n";


    response
        << "Access-Control-Allow-Origin: *\r\n";


    response
        << "Access-Control-Allow-Methods: "
        << "GET, OPTIONS\r\n";


    response
        << "Access-Control-Allow-Headers: "
        << "Content-Type\r\n";


    response
        << "Connection: close\r\n";


    response
        << "Content-Length: "
        << body.size()
        << "\r\n";


    response
        << "\r\n";


    response
        << body;


    return response.str();
}


std::string ApiServer::extractRequestTarget(
    const std::string& request
) const
{
    std::istringstream stream(
        request
    );


    std::string method;
    std::string target;
    std::string version;


    stream
        >> method
        >> target
        >> version;


    return target;
}


std::string ApiServer::extractPath(
    const std::string& target
) const
{
    size_t queryPosition =
        target.find('?');


    if (queryPosition == std::string::npos)
    {
        return target;
    }


    return target.substr(
        0,
        queryPosition
    );
}


std::string ApiServer::extractQueryParameter(
    const std::string& target,
    const std::string& parameter
) const
{
    size_t questionPosition =
        target.find('?');


    if (questionPosition == std::string::npos)
    {
        return "";
    }


    std::string queryString =
        target.substr(
            questionPosition + 1
        );


    std::stringstream stream(
        queryString
    );


    std::string part;


    while (std::getline(
        stream,
        part,
        '&'
    ))
    {
        size_t equalsPosition =
            part.find('=');


        if (equalsPosition == std::string::npos)
        {
            continue;
        }


        std::string key =
            part.substr(
                0,
                equalsPosition
            );


        std::string value =
            part.substr(
                equalsPosition + 1
            );


        if (key == parameter)
        {
            return urlDecode(
                value
            );
        }
    }


    return "";
}


std::string ApiServer::urlDecode(
    const std::string& value
) const
{
    std::string result;

    result.reserve(
        value.length()
    );


    for (size_t i = 0;
         i < value.length();
         i++)
    {
        char ch =
            value[i];


        if (ch == '+')
        {
            result += ' ';
        }
        else if (
            ch == '%' &&
            i + 2 < value.length()
        )
        {
            auto hexValue =
                [](char c) -> int
            {
                if (c >= '0' && c <= '9')
                {
                    return c - '0';
                }

                if (c >= 'A' && c <= 'F')
                {
                    return c - 'A' + 10;
                }

                if (c >= 'a' && c <= 'f')
                {
                    return c - 'a' + 10;
                }

                return -1;
            };


            int high =
                hexValue(
                    value[i + 1]
                );


            int low =
                hexValue(
                    value[i + 2]
                );


            if (high >= 0 &&
                low >= 0)
            {
                result +=
                    static_cast<char>(
                        high * 16 + low
                    );

                i += 2;
            }
            else
            {
                result += ch;
            }
        }
        else
        {
            result += ch;
        }
    }


    return result;
}


bool ApiServer::sendAll(
    SOCKET socket,
    const std::string& data
) const
{
    size_t totalSent = 0;


    while (totalSent < data.size())
    {
        int bytesSent =
            send(
                socket,
                data.data() + totalSent,
                static_cast<int>(
                    data.size() - totalSent
                ),
                0
            );


        if (bytesSent == SOCKET_ERROR)
        {
            return false;
        }


        totalSent +=
            static_cast<size_t>(
                bytesSent
            );
    }


    return true;
}
