#include "SocketAddress.h"

#include <string.h>

SocketAddress::SocketAddress( uint16_t port ) {
    sockaddr_in* addr = GetSockAddrIn();
    addr->sin_family = AF_INET;
    addr->sin_port = htons( port );
    addr->sin_addr.s_addr = INADDR_ANY;
    memset( &addr->sin_zero, 0, sizeof( addr->sin_zero ) );
}

SocketAddress::SocketAddress( char const* ip_str, uint16_t port ) {
    sockaddr_in* addr = GetSockAddrIn();
    addr->sin_family = AF_INET;
    addr->sin_port = htons( port );
    memset( &addr->sin_zero, 0, sizeof( addr->sin_zero ) );
    #if defined( _WIN64 ) || defined( _WIN32 )
    InetPton( addr->sin_family, ip_str, &addr->sin_addr );
    #else
    inet_pton( addr->sin_family, ip_str, &addr->sin_addr );
    #endif
}

sockaddr_in* SocketAddress::GetSockAddrIn() {
    return reinterpret_cast<sockaddr_in*>( &addr_ );
}
