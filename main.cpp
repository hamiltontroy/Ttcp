#include <iostream>

#include "Ttcp.hpp" //contains declarations
#include "Ttcp.cpp" //contains definitions

/*

*/

int main(void)
{
    ttcp::Ttcp
        server, conn;
    
    server.activate(1111);

    conn.setClientfd(server.waitForAClient()); // takes an int fildes.
    //conn.receiveMessage(); // receives message into a dynamically allocated buffer
    //conn.printMessage(); // prints a message to stdout, skips unprintable characters. use verbosePrintMessage to print full info about the message.
    //conn.deallocateMessage(); // if a message is not deallocated, a memory leak can occur. works with free();
    //conn.transmitMessage("Hello client! This is Troy!\n"); // shows const char* here, works with std::string as well.

    
    return 0;
}