//vport_number = vlistening_socket_address.sin_port;
Ttcp::Ttcp(): 
    vlisteningSocket(-1), vclientSocket(-1)
{
    
}

int Ttcp::activate()
{
    int
        socket_fd, test;
    socklen_t
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
        close(socket_fd);
        return -1;
    }
    
    //sets the socket to listen mode.
    test = listen(socket_fd, SOMAXCONN);
    
    if(test < 0)
    {
        close(socket_fd);
        return -1;
    }
    
    vlisteningSocket = socket_fd;
    
    getsockname(socket_fd, (struct sockaddr *) &vlistening_socket_address, &socket_size);
    
    return socket_fd;
}

int Ttcp::activate(unsigned short port_number)
{
    int
        socket_fd, test;
    socklen_t
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
    vlistening_socket_address.sin_port = htons(port_number); //If sin_port is set to 0, the caller leaves it to the system to assign an available port.
    vlistening_socket_address.sin_addr.s_addr = INADDR_ANY; //bound to all network interfaces on the host.
    
    //assigns the address to the socket
    test = bind(socket_fd, (const struct sockaddr*) &vlistening_socket_address, socket_size);
    
    if(test < 0)
    {
        close(socket_fd);
        return -1;
    }
    
    //sets the socket to listen mode.
    test = listen(socket_fd, SOMAXCONN);
    
    if(test < 0)
    {
        close(socket_fd);
        return -1;
    }
    
    vlisteningSocket = socket_fd;
    
    getsockname(socket_fd, (struct sockaddr *) &vlistening_socket_address, &socket_size);
    
    return socket_fd;
}

int Ttcp::activate(int port_number)
{
    if(port_number < 0 || port_number > 65535)
    {
        return -1;
    }
    
    int
        socket_fd, test;
    socklen_t
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
    vlistening_socket_address.sin_port = htons((unsigned short)port_number); //If sin_port is set to 0, the caller leaves it to the system to assign an available port.
    vlistening_socket_address.sin_addr.s_addr = INADDR_ANY; //bound to all network interfaces on the host.
    
    //assigns the address to the socket
    test = bind(socket_fd, (const struct sockaddr*) &vlistening_socket_address, socket_size);
    
    if(test < 0)
    {
        close(socket_fd);
        return -1;
    }
    
    //sets the socket to listen mode.
    test = listen(socket_fd, SOMAXCONN);
    
    if(test < 0)
    {
        close(socket_fd);
        return -1;
    }
    
    vlisteningSocket = socket_fd;
    
    getsockname(socket_fd, (struct sockaddr *) &vlistening_socket_address, &socket_size);
    
    return socket_fd;
}

int Ttcp::listenerfd()
{
    return vlisteningSocket;
}

unsigned short Ttcp::listenerPort()
{
    return ntohs(vlistening_socket_address.sin_port);
}

// end