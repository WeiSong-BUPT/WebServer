#pragma once
#include <arpa/inet.h>
#include "Macros.h"

class InetAddress
{
private:
    struct sockaddr_in addr_ {};
public:

    InetAddress();
    InetAddress(const char* ip, uint16_t port);
    ~InetAddress() = default;

    DISALLOW_COPY_AND_MOVE(InetAddress);

    void SetAddr(sockaddr_in addr);
    sockaddr_in GetAddr();
    const char* GetIp();
    uint16_t GetPort();
};


