#pragma once
#include <functional>
#include "Macros.h"

class Epoll;
class Channel;
class EventLoop
{
private:
    Epoll *epoll_{nullptr};
    bool quit_{false};
public:
    EventLoop();
    ~EventLoop();

    DISALLOW_COPY_AND_MOVE(EventLoop);

    void Loop();
    void UpdateChannel(Channel* ch);
};


