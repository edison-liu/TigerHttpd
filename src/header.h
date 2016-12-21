#ifndef __HEADER_H__
#define __HEADER_H__

#include <string>

namespace TigerHttpd
{

struct Header
{
    std::string name;
    std::string value;
};  

} // namespace TigerHttpd

#endif // __HEADER_H__

