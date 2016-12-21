
#include "mime_types.h"

namespace TigerHttpd
{

namespace MimeTypes
{

struct mapping
{
  const char* extension;
  const char* mime_type;
} mappings[] =
{
  { "gif", "image/gif" },
  { "htm", "text/html" },
  { "html", "text/html" },
  { "jpg", "image/jpeg" },
  { "png", "image/png" }
};

// Convert a file extension into MIME type.
std::string ExtensionToType(const std::string& extension)
{
    for (mapping m : mappings)
    {
        if (m.extension == extension)
        {
            return m.mime_type;
        }
    }

    return "text/plain";
}

} // namespace MimeTypes

} // namespace TigerHttpd

