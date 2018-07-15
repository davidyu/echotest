#include "UDPSocket.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>

UDPSocket* UDPSocket::Create() {
    #if defined( _WIN64 ) || defined( _WIN32 )
    SOCKET sock = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
    if ( sock != INVALID_SOCKET ) {
        return new UDPSocket( sock );
    } else {
        return nullptr;
    }
    #else
    int sock = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
    if ( sock != -1 ) {
        return new UDPSocket( sock );
    } else {
        return nullptr;
    }
    #endif
}


int UDPSocket::Bind( const SocketAddress& local_addr ) {
    int ret = bind( sock_, local_addr.Get(), local_addr.GetSize() );
    if ( ret < 0 ) {
        #if defined( _WIN64 ) || defined( _WIN32 )
        return WSAGetLastError();
        #else
        return errno;
        #endif
    }
    return ret;
}

int UDPSocket::SendTo( const void* data, int len, const SocketAddress& to_addr ) {
    int bytesSent = sendto( sock_, static_cast<const char*>( data ), len, 0, to_addr.Get(), to_addr.GetSize() );
    if ( bytesSent <= 0 ) {
        #if defined( _WIN64 ) || defined( _WIN32 )
        return WSAGetLastError();
        #else
        return errno;
        #endif
    }
    return bytesSent;
}

int UDPSocket::RecvFrom( void* buf, int len, SocketAddress& from_addr ) {
    socklen_t from_len = from_addr.GetSize();
    int bytesReceived = recvfrom( sock_, static_cast<char*>( buf ), len, 0, from_addr.GetWritableAddr(), &from_len );
    if ( bytesReceived <= 0 ) {
        #if defined( _WIN64 ) || defined( _WIN32 )
        return WSAGetLastError();
        #else
        return errno;
        #endif
    }
    return bytesReceived;
}
