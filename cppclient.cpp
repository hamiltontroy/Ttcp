#include <iostream>
using namespace std;

#include "Ttcp.hpp"
#include "Ttcp.cpp"
using namespace ttcp;

int main()
{
    Ttcp
        instance;
    const char
        *hello_message = "Hello, this is cppclient.cpp\n";
    
    instance.connectTo("127.0.0.1", 1111);
    
    write(instance.clientfd(), hello_message, strlen(hello_message));
    
    instance.receiveMessage();
    
    instance.printMessage();
    
    return 0;
}
