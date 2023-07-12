#include "Connection.h"
#include "Socket.h"
#include "Channel.h"
#include "Buffer.h"
#include "util.h"
#include <unistd.h>
#include <string.h>
#include <iostream>

Connection::Connection(EventLoop* loop, Socket* sock) : loop_(loop), sock_(sock) {
    if(loop_ != nullptr) {
        channel_ = new Channel(loop_, sock->GetFd());
        channel_->EnableRead();
        channel_->UseET();
    }
    read_buffer_ = new Buffer();
    send_buffer_ = new Buffer();
    state_ = State::Connected;
}

Connection::~Connection() {
    if (loop_ != nullptr) {
        delete channel_;
    }
    delete sock_;
    delete read_buffer_;
    delete send_buffer_;
}

void Connection::Read() {
    ASSERT(state_ == State::Connected, "connection state is disconnected!");
    read_buffer_->Clear();
    if(sock_->IsNonBlocking()) {
        ReadNonBlocking();
    } else {
        ReadBlocking();
    }
}

void Connection::Write() {
    ASSERT(state_ == State::Connected, "connection state is disconnected!");
    if (sock_->IsNonBlocking()) {
        WriteNonBlocking();
    } else {
        WriteBlocking();
    }
    send_buffer_->Clear();
}

void Connection::ReadNonBlocking() {
    int sockfd = sock_->GetFd();
    char buf[1024];
    while(true){    //由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
        memset(buf, 0, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if(bytes_read > 0){
            read_buffer_->Append(buf, bytes_read);
        } else if(bytes_read == -1 && errno == EINTR){  //客户端正常中断、继续读取
            printf("continue reading\n");
            continue;
        } else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
            break;
        } else if(bytes_read == 0){  //EOF，客户端断开连接
            printf("EOF, client fd %d disconnected\n", sockfd);
            state_ = State::Closed;
            break;
        }  else {
            printf("Other error on client fd %d\n", sockfd);
            state_ = State::Closed;          
            break;
        }
    }
}

void Connection::WriteNonBlocking() {
    int sockfd = sock_->GetFd();
    char buf[send_buffer_->Size()];
    memcpy(buf, send_buffer_->ToStr(), send_buffer_->Size());
    int data_size = send_buffer_->Size();
    int data_left = data_size;
    while (data_left > 0) {
    ssize_t bytes_write = write(sockfd, buf + data_size - data_left, data_left);
    if (bytes_write == -1 && errno == EINTR) {
      printf("continue writing\n");
      continue;
    }
    if (bytes_write == -1 && errno == EAGAIN) {
      break;
    }
    if (bytes_write == -1) {
      printf("Other error on client fd %d\n", sockfd);
      state_ = State::Closed;
      break;
    }
    data_left -= bytes_write;
  }
}

void Connection::SetDeleteConnectionCallback(std::function<void(Socket*)> const &callback) {
    delete_connectioin_callback_ = callback;
}

void Connection::SetOnConnectCallback(std::function<void(Connection*)> const &callback) {
    on_connect_callback_ = callback;
    channel_->SetReadCallback([this]() { on_connect_callback_(this); });
}

Connection::State Connection::GetState() { return state_; }

void Connection::Close() { delete_connectioin_callback_(sock_); }

void Connection::SetSendBuffer(const char *str) { send_buffer_->SetBuf(str); }
Buffer *Connection::GetReadBuffer() { return read_buffer_; }
const char *Connection::ReadBuffer() { return read_buffer_->ToStr(); }
Buffer *Connection::GetSendBuffer() { return send_buffer_; }
const char *Connection::SendBuffer() { return send_buffer_->ToStr(); }
void Connection::GetlineSendBuffer() { send_buffer_->Getline(); }

Socket *Connection::GetSocket() { return sock_; }
