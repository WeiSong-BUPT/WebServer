#pragma once
#include "Macros.h"

class InetAddress;
class Socket
{
private:
    int fd_{-1};
public:
    Socket();
    explicit Socket(int fd);
    ~Socket();
    
    DISALLOW_COPY_AND_MOVE(Socket);

    void Bind(InetAddress *addr);
    void Listen();
    int Accept(InetAddress *addr);

    void Connect(InetAddress *addr);
    void Connect(const char *ip, uint16_t port);
    
    void SetNonBlocking();
    bool IsNonBlocking();
    int GetFd();
};


