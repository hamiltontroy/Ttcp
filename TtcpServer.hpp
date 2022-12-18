#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>

class TtcpServer
{
    private:
        unsigned short
            pvbufferedPortNumber = 0;
        int
            pvclientFd = 0, pvListenerFd = 0;
        
        void setUpSocket(unsigned short);
        void blockUntilClientConnects();
        void cleanUpClass();
        
    public:
        int waitForAClient(unsigned short);
};
