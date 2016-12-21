
#include "http_server.h"

#include <iostream>
#include <string>

#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "http_request.h"
#include "http_reply.h"
#include "http_request_parser.h"
#include "http_request_handler.h"

namespace TigerHttpd
{

HttpServer::HttpServer(const std::string& address, const std::string& port,
               const std::string& doc_root)
    : address_(address),
      port_(port),
      doc_root_(doc_root)
{
}

HttpServer::~HttpServer()
{
}

int HttpServer::Start()
{
    fd_ = socket(PF_INET, SOCK_STREAM, 0);
    if (fd_ < 0)
    {
        std::cerr << "Error: Creating socket.\n";
        return -1;
    }

    int enable = 1;
    if (setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) 
    {
        std::cerr << "Error: setsockopt\n";
        close(fd_);
        return -1;
    }

    struct sockaddr_in srv_addr;
    memset(&srv_addr, 0, sizeof(srv_addr));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(atoi(port_.c_str()));
    srv_addr.sin_addr.s_addr = inet_addr(address_.c_str());
    if (bind(fd_, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) < 0)
    {
        std::cerr << "Error: bind to " << address_ << " " << port_ << std::endl;
        close(fd_);
        return -1;
    }

    if (listen(fd_, max_clients) < 0)
    {
        std::cerr << "Error: listen" << std::endl;
        close(fd_);
        return -1;
    }

    std::cout << "Start listening on " << address_ << ":" << port_ << std::endl;

    return 0;
}

int HttpServer::Stop()
{

    return 0;
}

int HttpServer::Run()
{
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_sock, child_pid;

    while (true)
    {
        // Accept a client
        client_sock = accept(fd_, (struct sockaddr *)&client_addr, &client_len);
        if (client_sock < 0)
        {
            std::cerr << "Error: accept\n";
            continue;
        }

        //
        std::cout << "New connection from " 
                  << inet_ntoa(client_addr.sin_addr) 
                  << ":" << client_addr.sin_port << std::endl;

        // Create child process to handle the connection
        if ((child_pid = fork()) < 0)
        {
            std::cerr << "Error: fork\n";
            close(client_sock);
            continue;
        }
        else if (child_pid == 0)
        {
            // Close listen socket
            close(fd_);

            // Receive a Request
            char buf[max_buf_size];
            int len;
            if ((len = read(client_sock, buf, sizeof(buf))) < 0)
            {
                std::cerr << "Error: read\n";
                close(client_sock);
                exit(1);
            }

            std::string str_pkt(buf);
            std::cout << "Received: \n" << str_pkt << std::endl;

            // Parser packet to HttpRequest
            HttpRequest request;
            HttpReply reply;
            HttpRequestParser parser;
            HttpRequestHandler handler(doc_root_);
            HttpRequestParser::ResultType result;

            result = parser.Parser(request, buf, len);
            if (result == HttpRequestParser::GOOD)
            {
                // Handle Reqest and construct Reply
                handler.HandleRequest(request, reply);
            }
            else if (result == HttpRequestParser::BAD)
            {
                // Handle Reqest and construct Reply
                reply = HttpReply::StockReply(HttpReply::bad_request);
            }
            else
            {
                // TODO. Continue read
            }

            // Send Reply to client
            std::string str_buf = reply.ToBuffer();
            len = write(client_sock, str_buf.c_str(), str_buf.length());
            if (len <= 0 or (unsigned int)len != str_buf.length())
            {
                std::cerr << "Error: Expect to writing " << str_buf.length() 
                    <<" bytes, but actually write " << len << std::endl;
            }
            std::cout << "Reply: \n" << str_buf << std::endl;

            close(client_sock);
            // Finished handling of client request. Child process exit.
            exit(0);
        }
        else
        {
            close(client_sock);
            continue;
        }
    }

    return 0;
}

int HttpServer::Receive()
{

    return 0;
}

int HttpServer::Write()
{

    return 0;
}

} // namespace TigerHttpd

