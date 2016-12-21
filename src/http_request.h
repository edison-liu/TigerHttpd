#ifndef __HTTP_REQUEST_H__
#define __HTTP_REQUEST_H__

#include <string>
#include <vector>

#include "header.h"

namespace TigerHttpd
{

struct HttpRequest
{
    std::string method_;
    std::string uri_;
    int         http_ver_major_;
    int         http_ver_minor_;
    std::vector<Header> headers_;

};

} // namespace TigerHttpd

#endif // __HTTP_REQUEST_H__
