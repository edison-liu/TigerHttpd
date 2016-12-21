#ifndef __MIME_TYPE_H__
#define __MIME_TYPE_H__

#include <string>

namespace TigerHttpd
{

namespace MimeTypes
{

// Convert a file extension into MIME type.
std::string ExtensionToType(const std::string& extension);

} // namespace MimeTypes

} // namespace TigerHttpd

#endif // __MIME_TYPE_H__

