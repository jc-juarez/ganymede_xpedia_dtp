#include <iostream>
#include "gXDataTransmissionServer.hh"

int main()
{
    gX::DataTransmissionServer server("MetadataServer");

    gX::StatusCode status = server.Init();

    if (gX::Status::Failed(status))
    {
        std::cout << "Init failed!" << std::endl;
        return 1;
    }

    std::cout << "Listening" << std::endl;

    server.Run();
}