namespace ttcp
{
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
        memset(&vclientAddress, 0, sizeof(vclientAddress));
        memset(&vmessageInfo, 0, sizeof(struct MessageInfo));
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
    
    void Ttcp::deallocateBuffers()
    {
        if(vmessageInfo.bufferStart != NULL)
        {
            free(vmessageInfo.bufferStart);
        }
    }
    
    void Ttcp::cleanup()
    {
        closeAllfds();
        deallocateBuffers();
        zeroOutAllVariables();
    }

    int Ttcp::makeSocket()
    {
        int
            sockfd;
        
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        
        return sockfd;
    }

    void Ttcp::setTcpAddr(struct sockaddr_in &tcpAddr)
    {
        memset(&tcpAddr, 0, sizeof(struct sockaddr_in));
        tcpAddr.sin_family = AF_INET;
        tcpAddr.sin_port = 0; //If sin_port is set to 0, the caller leaves it to the system to assign an available port.
        tcpAddr.sin_addr.s_addr = INADDR_ANY; //bound to all network interfaces on the host.
    }

    void Ttcp::setTcpAddr(struct sockaddr_in &tcpAddr, int portNumber)
    {
        memset(&tcpAddr, 0, sizeof(struct sockaddr_in));
        tcpAddr.sin_family = AF_INET;
        tcpAddr.sin_port = htons((unsigned short)portNumber); //If sin_port is set to 0, the caller leaves it to the system to assign an available port.
        tcpAddr.sin_addr.s_addr = INADDR_ANY; //bound to all network interfaces on the host.
    }

    bool Ttcp::osBindSocket(int &sockfd, struct sockaddr_in &tcpAddr)
    {
        int t;
        t = bind(sockfd, (const struct sockaddr*) &tcpAddr, sizeof(struct sockaddr_in));
        
        //indicates the os failed to allocate the socket
        if(t < 0)
        {
            return false;   
        }
        
        return true;
    }

    bool Ttcp::turnListenerOn()
    {
        //indicates the os failed to set the socket to listening
        if(listen(vlistenerfd, SOMAXCONN) < 0) 
        {
            return false;
        }
        
        return true;
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
        vlistenerfd = makeSocket();
        
        if(vlistenerfd < 0)
        {
            cleanup();
            return -1;
        }
        
        setTcpAddr(vlisteningSocketAddress);
        
        //assigns the address to the socket
        if(osBindSocket(vlistenerfd, vlisteningSocketAddress) == false)
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
        vlistenerfd = makeSocket();
        
        if(vlistenerfd < 0)
        {
            cleanup();
            return -1;
        }
        
        setTcpAddr(vlisteningSocketAddress, portNumber);
        
        //assigns the address to the socket
        if(osBindSocket(vlistenerfd, vlisteningSocketAddress) == false)
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

    struct MessageInfo Ttcp::receiveMessage(int maxBytes)
    {
        const int
            PAGE_SIZE = 4096; //this can be changed based on implementation
        char
            *bufferStart, *temporaryPointer;
        int
            memoryAllocationCount = 0, totalCharsReceived = 0, temporaryCharsReceivedCount;
        
        //allocate an initial buffer
        bufferStart = (char*)malloc(PAGE_SIZE);
        if(bufferStart == NULL)
        {
            vmessageInfo.bufferStart = NULL;
            vmessageInfo.bufferLength = 0;
            
            return vmessageInfo;
        }
        
        memoryAllocationCount++;
        
        while(totalCharsReceived < maxBytes)
        {
            //recv into buffer
            temporaryCharsReceivedCount = recv(vclientfd, bufferStart + totalCharsReceived, PAGE_SIZE, 0);
            
            //check for recv() errors
            if(temporaryCharsReceivedCount < 1)
            {
                free(bufferStart);
                
                vmessageInfo.bufferStart = NULL;
                vmessageInfo.bufferLength = 0;
            
                return vmessageInfo;
            }
            
            totalCharsReceived += temporaryCharsReceivedCount;
            temporaryCharsReceivedCount = 0;
            
            //check if buffer needs to be resized
            if(totalCharsReceived == PAGE_SIZE * memoryAllocationCount)
            {
                memoryAllocationCount++;
                
                //resize buffer to a larger size to read more chars
                temporaryPointer = (char*)realloc(bufferStart, PAGE_SIZE * memoryAllocationCount);
                if(temporaryPointer == NULL)
                {
                    free(bufferStart);
                    vmessageInfo.bufferStart = NULL;
                    vmessageInfo.bufferLength = 0;
            
                    return vmessageInfo;
                }
                
                bufferStart = temporaryPointer;
                temporaryPointer = NULL;
            }
            else
            {   //resize buffer to a smaller size
                temporaryPointer = (char*)realloc(bufferStart, totalCharsReceived);
                if(temporaryPointer == NULL)
                {
                    free(bufferStart);
                    vmessageInfo.bufferStart = NULL;
                    vmessageInfo.bufferLength = 0;
            
                    return vmessageInfo;
                }
                
                //successfully exit
                bufferStart = temporaryPointer;
                temporaryPointer = NULL;
                break;
            }
        }
        
        vmessageInfo.bufferStart = bufferStart;
        vmessageInfo.bufferLength = totalCharsReceived;
        
        return vmessageInfo;
    }

    struct MessageInfo Ttcp::receiveMessage()
    {
        const size_t
            PAGE_SIZE = 4096; //this can be changed based on implementation
        char
            *bufferStart, *temporaryPointer;
        size_t
            memoryAllocationCount = 0, totalCharsReceived = 0, temporaryCharsReceivedCount;
        
        //allocate an initial buffer
        bufferStart = (char*)malloc(PAGE_SIZE);
        if(bufferStart == NULL)
        {
            vmessageInfo.bufferStart = NULL;
            vmessageInfo.bufferLength = 0;
            
            return vmessageInfo;
        }
        
        memoryAllocationCount++;
        
        while(1)
        {
            //recv into buffer
            temporaryCharsReceivedCount = recv(vclientfd, bufferStart + totalCharsReceived, PAGE_SIZE, 0);
            
            //check for recv() errors
            if(temporaryCharsReceivedCount < 1)
            {
                free(bufferStart);
                
                vmessageInfo.bufferStart = NULL;
                vmessageInfo.bufferLength = 0;
            
                return vmessageInfo;
            }
            
            totalCharsReceived += temporaryCharsReceivedCount;
            temporaryCharsReceivedCount = 0;
            
            //check if buffer needs to be resized
            if(totalCharsReceived == PAGE_SIZE * memoryAllocationCount)
            {
                memoryAllocationCount++;
                
                //resize buffer to a larger size to read more chars
                temporaryPointer = (char*)realloc(bufferStart, PAGE_SIZE * memoryAllocationCount);
                if(temporaryPointer == NULL)
                {
                    free(bufferStart);
                    vmessageInfo.bufferStart = NULL;
                    vmessageInfo.bufferLength = 0;
            
                    return vmessageInfo;
                }
                
                bufferStart = temporaryPointer;
                temporaryPointer = NULL;
            }
            else
            {   //resize buffer to a smaller size
                temporaryPointer = (char*)realloc(bufferStart, totalCharsReceived);
                if(temporaryPointer == NULL)
                {
                    free(bufferStart);
                    vmessageInfo.bufferStart = NULL;
                    vmessageInfo.bufferLength = 0;
            
                    return vmessageInfo;
                }
                
                //successfully exit
                bufferStart = temporaryPointer;
                temporaryPointer = NULL;
                break;
            }
        }
        
        vmessageInfo.bufferStart = bufferStart;
        vmessageInfo.bufferLength = totalCharsReceived;
        
        return vmessageInfo;
    }
    
    char* Ttcp::getMessageBuffer()
    {
        return vmessageInfo.bufferStart;
    }
    
    int Ttcp::getMessageLength()
    {
        return vmessageInfo.bufferLength;
    }
    
    
    bool Ttcp::connectTo(const char* ipAddrStr, int portNumber)
    {            
        vclientfd = makeSocket();
        
        if(vclientfd < 0)
        {
            return false;
        }
        
        setTcpAddr(vclientAddress, portNumber);
        
        size_t 
            addressSize = sizeof(struct sockaddr);
            
        if(connect(vclientfd, (const struct sockaddr*) &vclientAddress, addressSize) != 0)
        {
            return false;
        }
        
        return true; //successfully connected
    }

    void Ttcp::printMessage()
    {
        for(int i = 0; i < vmessageInfo.bufferLength; i++)
        {		
            putchar(*(vmessageInfo.bufferStart + i));
        }
        putchar('\0');
    }
    
    // end
}
// end