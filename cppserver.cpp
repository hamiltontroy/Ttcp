#include <iostream>
using namespace std;

#include "Ttcp.hpp" //contains declarations
#include "Ttcp.cpp" //contains definitions
using namespace ttcp;

int main(void)
{
    Ttcp
        instance;
    struct ttcp::MessageInfo
        messageInfo;
    const char
        *helloMsg = "Hello Client from Server C++ program.\n";
        
    instance.activate(1111);

    instance.waitForAClient();
    
    messageInfo = instance.receiveMessage(1234); // receives message into a dynamically allocated buffer
    
    instance.printMessage();
    
    send(instance.clientfd(), helloMsg, strlen(helloMsg), 0);
    
    
    
    //conn.printMessage(); // prints a message to stdout, skips unprintable characters. use verbosePrintMessage to print full info about the message.
    //conn.deallocateMessage(); // if a message is not deallocated, a memory leak can occur. works with free();
    //conn.transmitMessage("Hello client! This is Troy!\n"); // shows const char* here, works with std::string as well.

    
    
    return 0;
}