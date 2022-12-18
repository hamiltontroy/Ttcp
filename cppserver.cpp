#include <iostream>
using namespace std;

#include "Ttcp.hpp" //contains declarations
#include "Ttcp.cpp" //contains definitions
using namespace ttcp;

int main(void)
{
    Ttcp
        instance;
    const char
        *helloMsg = "Hello, this is cppserver.cpp.\n";
        
    instance.activate(1111);

    instance.waitForAClient();
    
    instance.receiveMessage(4096); // receives message into a dynamically allocated buffer
    
    instance.printMessage();
    
    send(instance.clientfd(), helloMsg, strlen(helloMsg), 0);
    
    return 0;
}
