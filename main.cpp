#include <iostream>

#include "Ttcp.hpp" //contains declarations
#include "Ttcp.cpp" //contains definitions

/*

*/

void safe_string_print(char* string_start, int string_length)
{
    for(int i = 0; i < string_length; i++)
    {		
        putchar(*(string_start + i));
    }
    putchar('\0');
}

void verbose_safe_string_print(char* string_start, int string_length)
{
    printf("Printing %d characters to stdout.\n{\n", string_length);
    
    safe_string_print(string_start, string_length);
    
    printf("\n}End of %d character print to stdout.\n", string_length);
}

int main(void)
{
    Ttcp
        server, conn;
    struct MessageInfo
        messageInfo;
        
    server.activate(1111);

    conn.setClientfd(server.waitForAClient()); // takes an int fildes.
    messageInfo = conn.receiveMessage(1234); // receives message into a dynamically allocated buffer
    //conn.printMessage(); // prints a message to stdout, skips unprintable characters. use verbosePrintMessage to print full info about the message.
    //conn.deallocateMessage(); // if a message is not deallocated, a memory leak can occur. works with free();
    //conn.transmitMessage("Hello client! This is Troy!\n"); // shows const char* here, works with std::string as well.

    verbose_safe_string_print(messageInfo.bufferStart, messageInfo.bufferLength);
    
    return 0;
}