#pragma once
#include <string>
#include "Macros.h"


class Buffer
{
private:
    std::string buf_;
public:
    Buffer() = default;
    ~Buffer() = default;

    DISALLOW_COPY_AND_MOVE(Buffer);

    void Append(const char* str, int size);
    ssize_t Size();
    const char* ToStr();
    void Clear();
    void Getline();
    void SetBuf(const char* buf);
};