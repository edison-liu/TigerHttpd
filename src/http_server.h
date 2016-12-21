#ifndef __HTTP_SERVER_H__
#define __HTTP_SERVER_H__

#include <string>

namespace TigerHttpd
{

class HttpServer
{
public:
    explicit HttpServer(const std::string& address, const std::string& port,
               const std::string& doc_root);
    ~HttpServer();

    int Start();
    int Stop();
    int Run();

    int Receive();
    int Write();

private:
    int fd_;
    std::string address_;
    std::string port_;
    std::string doc_root_;

    const int max_clients = 1024;
    const int max_buf_size = 4096;
};

} // namespace TigerHttpd

#endif // __HTTP_SERVER_H__
