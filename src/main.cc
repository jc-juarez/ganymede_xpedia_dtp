#include <iostream>
#include "gXDataTransmissionServer.hh"

gX::StatusCode
PrintRequest(std::string p_Request)
{
    std::cout << "PrintRequest execution." << std::endl;
    std::cout << "MESSAGE: " << p_Request.c_str() << std::endl;
    
    return gX::Status::Success;
}

int main()
{
    gX::DataTransmissionServer server("MetadataServer");

    gX::DataTransmissionServerConfiguration configuration;

    configuration.m_PacketTagResolverTable = {
        {0, gX::EndpointType(std::bind(&PrintRequest, std::placeholders::_1))}
    };

    gX::StatusCode status = server.Init(&configuration);

    if (gX::Status::Failed(status))
    {
        std::cout << "Init failed!" << std::endl;
        return 1;
    }

    std::cout << "Listening" << std::endl;

    server.Run();
}