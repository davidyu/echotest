#pragma once

#include "SocketAddress.h"

class TCPSocket {
public:
    static TCPSocket* Create();
    virtual          ~TCPSocket() {}

private:
    // do not initialize me; use TCPSocket::Create()!
    TCPSocket();
};
