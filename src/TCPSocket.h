#pragma once

#include "SocketAddress.h"

class TCPSocket {
public:
    static TCPSocket* Create();
    virtual          ~TCPSocket() {} // TODO free me

    int               Bind   ( const SocketAddress& local_addr );
    int               Connect( const SocketAddress& server_addr );
    int               Listen ( int max_backlog );
    TCPSocket *       Accept ( SocketAddress& client_addr );
    int               Send   ( const void* data, int len );
    int               Recv   ( void* buf, int len );
    int               Close  ();

private:
    bool closed_;

    #if defined( _WIN64 ) || defined( _WIN32 )
    TCPSocket( SOCKET sock )
        : sock_( sock )
        , closed_( false )
    {}

    SOCKET sock_;
    #else
    TCPSocket( int sock )
        : sock_( sock )
        , closed_( false )
    {}

    int sock_;
    #endif
};
