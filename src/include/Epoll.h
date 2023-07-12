#pragma once
#include "Macros.h"
#include <sys/epoll.h>
#include <vector>

class Channel;
class Epoll
{
private:
    int epfd_{1};
    struct epoll_event *events_{nullptr};
public:
    Epoll();
    ~Epoll();

    DISALLOW_COPY_AND_MOVE(Epoll);

    void UpdateChannel(Channel* channel);
    void DeleteChannel(Channel* channel);

    std::vector<Channel*> Poll(int timeout = -1);
};

