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
#include <arpa/inet.h> // inet_addr()

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
            vmessageInfo;
        
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
        
        /* //UPDATED deallocateBuffers() update as necessary
        deallocateBuffers() will deallocate Buffers.
        */
        void deallocateBuffers();
        
        /* //Done cleanup()
        cleanup() will simply call closeAllfds() and zeroOutAllVariables()
        */
        void cleanup();

        /* //Done makeSocket()
        makeSocket() will make a listener fd in a non
        listening state.
        */
        int makeSocket();

        /* //Done setListenerAddr()
        setListenerAddr() will set up the listening address.
        */
        void setTcpAddr(struct sockaddr_in &); //random port number
        void setTcpAddr(struct sockaddr_in &, int); //specific port number

        /* //Done osBindSocket()
        tells the os to assign the socket an ip address
        */
        bool osBindSocket(int &, struct sockaddr_in &);

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
        Upon being called, the entire class will be reset and cleaned / deallocated,
        and everything will be reset.
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
        
        /* //Done receiveMessage()
        receiveMessage() will receive a message from private variable vclientAddress.
        private variable vclientAddress can only be updated with a waitForAClient() or
        a setClientfd() or a connectTo();
        */
        struct MessageInfo receiveMessage(int); //sets a cap on max bytes it can receive
        struct MessageInfo receiveMessage();    //not safe, can receive unlimited bytes
        
        /* //Done getMessageBuffer()
            returns a pointer to the start of the dynamically allocated message buffer.
        */
        char* getMessageBuffer();
        
        /* //Done getMessageLength()
            returns the length of the dynamically allocated message buffer.
        */
        int getMessageLength();
        
        /* //Not Done connectTo()
            connectTo() will connect to an external ip address.
       */
        bool connectTo(const char*, int); // ip address and port number
        bool connectTo(const char*);
        bool connectTo(std::string);
        
        /* //Done printMessage()
            printMessage() will print out the message received in a buffer.
        */
        void printMessage();
        
        /* //Not Done printVerboseMessage()
            printVerboseMessage() will print out the message received in a buffer.
        */
        void printVerboseMessage();
    };
}
