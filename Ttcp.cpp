using namespace ttcp;

//vportNumber = vlisteningSocketAddress.sin_port;
Ttcp::Ttcp(): 
    vlistenerfd(-1), vclientfd(-1)
{
    memset(&vlisteningSocketAddress, 0, sizeof(vlisteningSocketAddress));
}

void Ttcp::zeroOutAllVariables()
{
    /*
    this is an important function and as such will be updated with the variables it
    manipulates as more variables arise.
    It currently covers:
        vlistenerfd, vclientfd, vlisteningSocketAddress
    */
    memset(&vlistenerfd, 0, sizeof(vlistenerfd));
    memset(&vclientfd, 0, sizeof(vclientfd));
    memset(&vlisteningSocketAddress, 0, sizeof(vlisteningSocketAddress));
}

void Ttcp::closeAllfds()
{
    /*
    this is an important function and as such will be updated with the variables it
    manipulates as more variables arise.
    It currently covers:
        vlistenerfd, vclientfd.
    */
    if(vlistenerfd != 0) close(vlistenerfd);
    
    if(vclientfd != 0) close(vclientfd);
}

void Ttcp::cleanup()
{
    closeAllfds();
    zeroOutAllVariables();
}

void Ttcp::makeListenerfd()
{
    vlistenerfd = socket(AF_INET, SOCK_STREAM, 0);
}

void Ttcp::setListenerAddr()
{
    memset(&vlisteningSocketAddress, 0, sizeof(struct sockaddr_in));
    vlisteningSocketAddress.sin_family = AF_INET;
    vlisteningSocketAddress.sin_port = 0; //If sin_port is set to 0, the caller leaves it to the system to assign an available port.
    vlisteningSocketAddress.sin_addr.s_addr = INADDR_ANY; //bound to all network interfaces on the host.
}

void Ttcp::setListenerAddr(int portNumber)
{
    memset(&vlisteningSocketAddress, 0, sizeof(struct sockaddr_in));
    vlisteningSocketAddress.sin_family = AF_INET;
    vlisteningSocketAddress.sin_port = htons((unsigned short)portNumber); //If sin_port is set to 0, the caller leaves it to the system to assign an available port.
    vlisteningSocketAddress.sin_addr.s_addr = INADDR_ANY; //bound to all network interfaces on the host.
}

bool Ttcp::osAllocateSocket()
{
    int t;
    t = bind(vlistenerfd, (const struct sockaddr*) &vlisteningSocketAddress, sizeof(struct sockaddr_in));
    
    //indicates the os failed to allocate the socket
    if(t < 0) return false;
    else return true;
}

bool Ttcp::turnListenerOn()
{
    int t;
    t = listen(vlistenerfd, SOMAXCONN);
    
    //indicates the os failed to set the socket to listening
    if(t < 0) return false;
    else return true;
}

int Ttcp::activate()
{    
    /*
        assures that there wont be multiple simultaneous
        sockets created via this object.
    */
    if(vlistenerfd != 0 || vclientfd != 0)
    {
        cleanup();
    }
    
    //makes a default tcp socket
    makeListenerfd();
    
    if(vlistenerfd < 0)
    {
        cleanup();
        return -1;
    }
    
    setListenerAddr();
    
    //assigns the address to the socket
    if(osAllocateSocket() == false)
    {
        cleanup();
        return -1;
    }
    
    //sets the socket to listen mode.
    if(turnListenerOn() == false)
    {
        cleanup();
        return -1;
    }
    
    socklen_t
        addressSize = sizeof(struct sockaddr_in);
    
    getsockname(vlistenerfd, (struct sockaddr *) &vlisteningSocketAddress, &addressSize);
    
    return vlistenerfd;
}

int Ttcp::activate(int portNumber)
{    
    /*
        assures that there wont be multiple simultaneous
        sockets created via this object.
    */
    if(vlistenerfd != 0 || vclientfd != 0)
    {
        cleanup();
    }
    
    if(portNumber < 0 || portNumber > 65535)
        return -1;
    
    //makes a default tcp socket
    makeListenerfd();
    
    if(vlistenerfd < 0)
    {
        cleanup();
        return -1;
    }
    
    setListenerAddr(portNumber);
    
    //assigns the address to the socket
    if(osAllocateSocket() == false)
    {
        cleanup();
        return -1;
    }
    
    //sets the socket to listen mode.
    if(turnListenerOn() == false)
    {
        cleanup();
        return -1;
    }
    
    socklen_t
        addressSize = sizeof(struct sockaddr_in);
    
    getsockname(vlistenerfd, (struct sockaddr *) &vlisteningSocketAddress, &addressSize);
    
    return vlistenerfd;
}

int Ttcp::listenerfd()
{
    return vlistenerfd;
}

unsigned short Ttcp::listenerPort()
{
    return ntohs(vlisteningSocketAddress.sin_port);
}

void Ttcp::setClientfd(int clientSocket)
{
    vclientfd = clientSocket;
}

int Ttcp::waitForAClient()
{
    if(vlistenerfd < 1)
    {
        return -1;
    }
    
    socklen_t
        addressSize = sizeof(struct sockaddr_in);
    
    vclientfd = accept(vlistenerfd, (struct sockaddr *) &vclientAddress, &addressSize);
    
    return vclientfd;
}

int Ttcp::clientfd()
{
    return vclientfd;
}

int Ttcp::receiveMessage()
{
    return 0;
}

// end