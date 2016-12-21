
#include <iostream>
#include <string>

#include "http_server.h"

void usage(int argc, char *argv[])
{
    std::cerr << "Usage: tigerhttpd <address> <port> <doc_root>\n";
    std::cerr << "  For IPv4, try:\n";
    std::cerr << "    " << argv[0] << " 0.0.0.0 80 .\n";
    std::cerr << "  For IPv6, try:\n";
    std::cerr << "    " << argv[0] << " 0::0 80 .\n";
}

int main(int argc, char *argv[])
{
    try
    {
        if (argc !=4)
        {
            usage(argc, argv);
            return -1;
        }

        // Construct the server
        TigerHttpd::HttpServer server(argv[1], argv[2], argv[3]);

        // Start the service
        server.Start();

        // Run the service
        server.Run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
