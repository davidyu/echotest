#pragma once

#include <stdint.h>
#if defined( _WIN64 ) || defined( _WIN32 )
#include <WinSock2.h>
#include <windows.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

class SocketAddress {
public:
    SocketAddress( char const* ip_str, uint16_t port );
    SocketAddress( uint16_t port );

    virtual ~SocketAddress() {}

    const sockaddr * Get()              const   { return &addr_; }
    sockaddr *       GetWritableAddr()          { return &addr_; }
    size_t           GetSize()          const   { return sizeof( sockaddr ); }

private:
    sockaddr addr_;
    sockaddr_in* GetSockAddrIn();
};
