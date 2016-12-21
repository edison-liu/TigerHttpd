#ifndef __HTTP_REPLY_H__
#define __HTTP_REPLY_H__

#include <string>
#include <vector>

#include "header.h"

namespace TigerHttpd
{

class HttpReply
{
public:

    /// The status of the reply.
    enum Status_Type
    {
        ok = 200,
        created = 201,
        accepted = 202,
        no_content = 204,
        multiple_choices = 300,
        moved_permanently = 301,
        moved_temporarily = 302,
        not_modified = 304,
        bad_request = 400,
        unauthorized = 401,
        forbidden = 403,
        not_found = 404,
        internal_server_error = 500,
        not_implemented = 501,
        bad_gateway = 502,
        service_unavailable = 503
    } status_;

    HttpReply();
    ~HttpReply();

    std::string ToBuffer();

    static HttpReply StockReply(HttpReply::Status_Type status);

    std::vector<Header> headers_;
    std::string content_;
};

} // namespace TigerHttpd

#endif // __HTTP_REPLY_H__
