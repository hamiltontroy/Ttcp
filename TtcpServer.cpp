// makes a tcp socket in a listening state and sets pvListenerFd
void TtcpServer::setUpSocket(unsigned short portNumber)
{
    struct sockaddr_in
        listenerAddress;

    pvListenerFd = socket(AF_INET, SOCK_STREAM, 0);

    if(pvListenerFd < 0)
    {
        return;
    }

    memset(&listenerAddress, 0, sizeof(struct sockaddr_in));
    
    listenerAddress.sin_family = AF_INET;
    listenerAddress.sin_port = htons(portNumber);
    listenerAddress.sin_addr.s_addr = INADDR_ANY;
    
    //assigns the address to the socket
    if(bind(pvListenerFd, (const struct sockaddr*) &listenerAddress, sizeof(struct sockaddr_in)) == -1)
        return;

    //sets the socket to listen mode.
    if(listen(pvListenerFd, SOMAXCONN) == -1)
        return;
}

void TtcpServer::blockUntilClientConnects()
{
    if(pvListenerFd < 1)
    {
        pvclientFd = -1;
        return;
    }

    pvclientFd = accept(pvListenerFd, NULL, NULL);
}

void TtcpServer::cleanUpClass()
{
    pvbufferedPortNumber = 0;
    
    close(pvclientFd);
    
    close(pvListenerFd);
}

int TtcpServer::waitForAClient(unsigned short portNumber)
{
    if(portNumber == 0)
    {
        cleanUpClass();
        return -1;
    }
    else if(portNumber != pvbufferedPortNumber)
    {
        cleanUpClass();
        setUpSocket(portNumber);
        pvbufferedPortNumber = portNumber;
    }

    blockUntilClientConnects();

    return pvclientFd;
}
