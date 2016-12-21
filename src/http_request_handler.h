#ifndef __HTTP_REQUEST_HANDLER_H__
#define __HTTP_REQUEST_HANDLER_H__

#include <string>

namespace TigerHttpd
{

class HttpRequest;
class HttpReply;

class HttpRequestHandler
{
public:
    HttpRequestHandler(const std::string& doc_root);
    ~HttpRequestHandler();

    void HandleRequest(const HttpRequest& request, HttpReply& reply);

private:
    std::string doc_root_;

    bool UrlDecode(const std::string& in, std::string& out);
};

} // namespace TigerHttpd

#endif // __HTTP_REQUEST_HANDLER_H__

