#include "TtcpServer.hpp"
#include "TtcpServer.cpp"

void sayHi(int sockFd)
{
    const char *hi = "Hi from Troy!\n";
    
    write(sockFd, hi, strlen(hi));
}

int main()
{
    TtcpServer server;

    for(;;)
        sayHi(server.waitForAClient(1111));

    return 0;
}