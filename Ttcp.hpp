/*Overview:

this code seeks to object orient or abstract away the following
system calls.

<sys/socket.h>
    int     socket(int, int, int);
    int     bind(int, const struct sockaddr *, socklen_t);
    int     listen(int, int);
    int     accept(int, struct sockaddr *restrict, socklen_t *restrict);
    int     connect(int, const struct sockaddr *, socklen_t);
    ssize_t recv(int, void *, size_t, int);
    ssize_t send(int, const void *, size_t, int);
    int     shutdown(int, int);
<unistd.h>
    int close(int);
<netinet/in.h>
    struct sockaddr_in
    {
        short            sin_family;   // e.g. AF_INET
        unsigned short   sin_port;     // e.g. htons(3490)
        struct in_addr   sin_addr;     // see struct in_addr, below
        char             sin_zero[8];  // zero this if you want to
    };

    struct in_addr
    {
        unsigned long s_addr;  // load with inet_aton()
    };
    
<string.h>
    memset(void*, int, int);
*/
#include <sys/socket.h>
#include <unistd.h>
#include <string.h> //memset
#include <netinet/in.h> //struct sockaddr_in

namespace ttcp
{
    struct MessageInfo
    {
        char
            *bufferStart;
        int
            bufferLength;
    };
    
    class Ttcp
    {
        private:

        int
            vlistenerfd, vclientfd;
        struct sockaddr_in 
            vlisteningSocketAddress, vclientAddress;
        struct MessageInfo
            messageInfo;
        
        //Private member functions
        /* //UPDATED zeroOutAllVariables() update as necessary
        zeroOutAllVariables() will make all of the private
        variables set to 0.
        */
        void zeroOutAllVariables();

        /* //UPDATED closeAllfds() update as necessary
        closeAllfds() will close all open file descriptors and terminate
        all open connections
        */
        void closeAllfds();

        /* //Done cleanup()
        cleanup() will simply call closeAllfds() and zeroOutAllVariables()
        */
        void cleanup();

        /* //Done makeListenerfd()
        makeListenerfd() will make a listener fd in a non
        listening state.
        */
        void makeListenerfd();

        /* //Done setListenerAddr()
        setListenerAddr() will set up the listening address.
        */
        void setListenerAddr(); //random port number
        void setListenerAddr(int); //specific port number

        /* //Done osAllocateSocket()
        tells the os to assign the socket an ip address
        */
        bool osAllocateSocket();

        /* //Done turnListenerOn()
        tells the os to set the socket to a position where it is
        able to use an accept() system call to accept connections.
        */
        bool turnListenerOn();

        public:

        Ttcp(); //UPDATED update as necessary

        /*  //Done activate()
        activate() will make a tcp socket already in a listening state,
        then will return the sockfd of the listening socket.
        If there are any open sockets on this object, they will be closed,
        this includes listeners and client sockets.
        */
        int activate(); //random port number
        int activate(int);   //specific port number

        /*  //Done listenerfd()
        listenerfd() will return the sockfd of the listening Socket.
        returns -1 if activate() failed or was not called prior.
        */
        int listenerfd();

        /*  //Done listenerPort()
        listenerPort() will return the port number of the listening socket
        */
        unsigned short listenerPort();

        /* //Done setClientfd()
        setClientfd() will take an integer argument and set the variable
        vclientfd to whatever the parameter is.
        */
        void setClientfd(int);

        /* //Done waitForAClient()
        waitForAClient() will block until a client connects, then will return
        the sockfd of the client. It will also update the private variable vclientAddress.
        */
        int waitForAClient();

        /* //Done clientfd()
        clientSocket() will return the sockfd of the client Socket.
        returns -1 if waitForAClient() failed or was not called prior.
        */
        int clientfd();
        
        /* //Not Done receiveMessage()
        receiveMessage() will receive a message from private variable vclientAddress.
        private variable vclientAddress can only be updated with a waitForAClient() or
        a setClientfd() or a connectTo();
        */
        int receiveMessage();
        
        int connectTo(const char*);
        int connectTo(std::string);
    };
}
