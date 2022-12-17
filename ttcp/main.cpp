#include <iostream>

#include "Ttcp.hpp" //contains declarations
#include "Ttcp.cpp" //contains definitions

int main(void)
{
    Ttcp
        server;
    
    server.activate();
    
    std::cout << server.listenerPort() << std::endl;
    
    server.activate(111111);
    
    std::cout << server.listenerPort() << std::endl;
    
    return 0;
}