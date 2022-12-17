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

class Ttcp
{
    private:
        int
            vlisteningSocket, vclientSocket;
        struct sockaddr_in 
            vlistening_socket_address;
    public:
    
    Ttcp(); //constructor
    /*
        activate() will make a tcp socket already in a listening state,
        then will return the sockfd of the listening socket.
    */
    int activate(); //random port number
    int activate(unsigned short);   //specific port number
    int activate(int);   //specific port number
    
    /*
        listeningSocket() will return the sockfd of the listening Socket.
        returns -1 if activate() failed or was not called prior.
    */
    int listenerfd();
    
    /*
        listenerPort() will return the port number of the listening socket
    */
    unsigned short listenerPort();
    
    /*
        waitForAClient() will block until a client connects, then will return
        the sockfd of the client.
    */
    int waitForAClient();
    
    /*
        clientSocket() will return the sockfd of the client Socket.
        returns -1 if waitForAClient() failed or was not called prior.
    */
    int clientSocket();
};
