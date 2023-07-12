#include "EventLoop.h"
#include "Epoll.h"
#include "Channel.h"
#include "ThreadPool.h"
#include <vector>

EventLoop::EventLoop() {
    epoll_ = new Epoll();
}

EventLoop::~EventLoop() {
    delete epoll_;
}

void EventLoop::Loop() {
    while(!quit_) {
        std::vector<Channel*> chs =  epoll_->Poll();
        for(auto it = chs.begin(); it != chs.end(); it++) {
            (*it)->HandleEvent();
        }
    }
}

void EventLoop::UpdateChannel(Channel* ch) {
    epoll_->UpdateChannel(ch);
}
