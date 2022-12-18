
#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
/*
int connect
(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
*/

int main(void)
{
    int
        socket_fd;
    struct sockaddr_in 
        address_buf;
    char
        message_buffer[1024] = "\0", *hello_message = "Hello Server From Client\n";
    size_t
        address_size = sizeof(struct sockaddr);
    
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    bzero(&address_buf, address_size);
 
    address_buf.sin_family = AF_INET;
    address_buf.sin_port = htons(1111);
    address_buf.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    connect(socket_fd, (const struct sockaddr*) &address_buf, address_size);
    
    write(socket_fd, hello_message, strlen(hello_message));
    read(socket_fd, message_buffer, sizeof(message_buffer));
    
    printf("%s\n", message_buffer);
    
    return 0;
}