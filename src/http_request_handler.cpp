
#include "http_request_handler.h"

#include <fstream>
#include <sstream>
#include <string>

#include "http_request.h"
#include "http_reply.h"
#include "mime_types.h"

namespace TigerHttpd
{

HttpRequestHandler::HttpRequestHandler(const std::string& doc_root)
  : doc_root_(doc_root)
{
}

HttpRequestHandler::~HttpRequestHandler()
{
}

void HttpRequestHandler::HandleRequest(const HttpRequest& request, HttpReply& reply)
{
    // Decode URL to path
    std::string request_path;
    if (!UrlDecode(request.uri_, request_path))
    {
        reply = HttpReply::StockReply(HttpReply::bad_request);
        return;
    }

    if (request_path.empty() or request_path[0] != '/' 
        or request_path.find("..") != std::string::npos)
    {
        reply = HttpReply::StockReply(HttpReply::bad_request);
        return;
    }

    if (request_path[request_path.size() - 1] == '/')
    {
        request_path += "index.html";
    }

    // determine the file extension
    std::size_t last_slash_pos = request_path.find_last_of("/");
    std::size_t last_dot_pos = request_path.find_last_of(".");
    std::string extension;
    if (last_dot_pos != std::string::npos and last_dot_pos > last_slash_pos)
    {
        extension = request_path.substr(last_dot_pos + 1);
    }

    // Open the file to send back
    std::string full_path = doc_root_ + request_path;
    std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
    if (!is)
    {
        reply = HttpReply::StockReply(HttpReply::bad_request);
        return;
    }

    // Fill out the reply to be sent to the client
    reply.status_ = HttpReply::ok;
    char buf[512];
    while (is.read(buf, sizeof(buf)).gcount() > 0)
    {
        reply.content_.append(buf, is.gcount());
    }
    reply.headers_.resize(2);
    reply.headers_[0].name = "Content-Length";
    reply.headers_[0].value = std::to_string(reply.content_.size());
    reply.headers_[1].name = "Content-Type";
    reply.headers_[1].value = MimeTypes::ExtensionToType(extension);

}

bool HttpRequestHandler::UrlDecode(const std::string& in, std::string& out)
{
    out.clear();
    out.reserve(in.size());
    for (std::size_t i = 0; i < in.size(); ++i)
    {
        if (in[i] == '%')
        {
            if (i + 3 <= in.size())
            {
                int value = 0;
                std::istringstream is(in.substr(i + 1, 2));
                if (is >> std::hex >> value)
                {
                    out += static_cast<char>(value);
                    i += 2;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        else if (in[i] == '+')
        {
            out += ' ';
        }
        else
        {
            out += in[i];
        }
    }
    return true;
}

} // namespace TigerHttpd

