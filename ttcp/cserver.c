/*
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

int make_listening_socket(unsigned short port_number)
{
    int
        socket_fd, test;
    int
        socket_size;
    
    //makes a default tcp socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    if(socket_fd < 0)
    {
        return -1;
    }
    
    socket_size = sizeof(struct sockaddr_in);
    
    memset(&vlistening_socket_address, 0, socket_size);
    
    vlistening_socket_address.sin_family = AF_INET;
    vlistening_socket_address.sin_port = 0; //If sin_port is set to 0, the caller leaves it to the system to assign an available port.
    vlistening_socket_address.sin_addr.s_addr = INADDR_ANY; //bound to all network interfaces on the host.
    
    //assigns the address to the socket
    test = bind(socket_fd, (const struct sockaddr*) &vlistening_socket_address, socket_size);
    
    if(test < 0)
    {
        return -1;
    }
    
    //sets the socket to listen mode.
    test = listen(socket_fd, SOMAXCONN);
    
    if(test < 0)
    {
        return -1;
    }
    
    getsockname(socket_fd, (struct sockaddr *) &vlistening_socket_address, &socket_size);
    
   // vport_number = vlistening_socket_address.sin_port;
    
    return socket_fd;
}

#include <iostream>

int main(void)
{
    int vsfd;
    
    vsfd = make_listening_socket(1111);
    
    return 0;
}