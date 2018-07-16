#pragma once

#include "SocketAddress.h"

class UDPSocket {
public:
    static UDPSocket* Create();
    virtual          ~UDPSocket() {}

    int               Bind     ( const SocketAddress& local_addr );
    int               SendTo   ( const void* data, int len, const SocketAddress& to_addr );
    int               RecvFrom ( void* buf, int len, SocketAddress& from_addr );

private:
    #if defined( _WIN64 ) || defined( _WIN32 )
    UDPSocket( SOCKET sock )
        : sock_( sock )
    {}

    SOCKET sock_;
    #else
    UDPSocket( int sock )
        : sock_( sock )
    {}

    int sock_;
    #endif
};
