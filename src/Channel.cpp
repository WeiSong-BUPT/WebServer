#include "Channel.h"
#include "EventLoop.h"
#include <unistd.h>
#include <sys/epoll.h>

Channel::Channel(EventLoop *loop, int fd) 
    : loop_(loop), fd_(fd), listen_events_(0), ready_events_(0), in_epoll_(false) {}

Channel::~Channel(){
    if(fd_ != -1){
        close(fd_);
        fd_ = -1;
    }
}

void Channel::HandleEvent() {
    if(ready_events_ & (EPOLLIN | EPOLLPRI)){
        read_callback_();
    }
    if(ready_events_ & (EPOLLOUT)){
        write_callback_();
    }
}

void Channel::EnableRead() {
    listen_events_ |= EPOLLIN | EPOLLPRI;
    loop_->UpdateChannel(this);
}

void Channel::UseET(){
    listen_events_ |= EPOLLET;
    loop_->UpdateChannel(this);
}

int Channel::GetFd(){
    return fd_;
}

uint32_t Channel::GetListenEvents(){
    return listen_events_;
}
uint32_t Channel::GetReadyEvents(){
    return ready_events_;
}

bool Channel::GetInEpoll(){
    return in_epoll_;
}

void Channel::SetInEpoll(bool in){
    in_epoll_ = in;
}

void Channel::SetReadyEvents(uint32_t _ev){
    ready_events_ = _ev;
}
void Channel::SetReadCallback(std::function<void()> const &callback){
    read_callback_ = callback;
}
