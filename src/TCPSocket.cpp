#include "TCPSocket.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>

TCPSocket* TCPSocket::Create() {
    #if defined( _WIN64 ) || defined( _WIN32 )
    SOCKET sock = socket( AF_INET, SOCK_STREAM, 0 );
    if ( sock != INVALID_SOCKET ) {
        return new TCPSocket( sock );
    } else {
        return nullptr;
    }
    #else
    int sock = socket( AF_INET, SOCK_STREAM, 0 );
    if ( sock != -1 ) {
        return new TCPSocket( sock );
    } else {
        return nullptr;
    }
    #endif
}


int TCPSocket::Bind( const SocketAddress& local_addr ) {
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

int TCPSocket::Connect( const SocketAddress& server_addr ) {
    int ret = connect( sock_, server_addr.Get(), sizeof( struct sockaddr ) );
    if ( ret < 0 ) {
        #if defined( _WIN64 ) || defined( _WIN32 )
        return WSAGetLastError();
        #else
        return errno;
        #endif
    }

    return ret;
}

int TCPSocket::Listen( int max_backlog ) {
    int ret = listen( sock_, max_backlog );
    if ( ret < 0 ) {
        #if defined( _WIN64 ) || defined( _WIN32 )
        return WSAGetLastError();
        #else
        return errno;
        #endif
    }

    return ret;
}

TCPSocket * TCPSocket::Accept( SocketAddress& client_addr ) {
    #if defined( _WIN64 ) || defined( _WIN32 )
    size_t len = client_addr.GetSize();
    SOCKET clientSock = accept( sock_, client_addr.GetWritableAddr(), &len );
    if ( clientSock != INVALID_SOCKET ) {
        return new TCPSocket( clientSock );
    } else {
        return nullptr;
    }
    #else
    socklen_t len = client_addr.GetSize();
    int clientSock = accept( sock_, client_addr.GetWritableAddr(), &len );
    if ( clientSock != -1 ) {
        return new TCPSocket( clientSock );
    } else {
        return nullptr;
    }
    #endif
}

int TCPSocket::Send( const void* data, int len ) {
    int bytesSent = send( sock_, static_cast<const char*>( data ), len, 0 );
    if ( bytesSent <= 0 ) {
        #if defined( _WIN64 ) || defined( _WIN32 )
        return WSAGetLastError();
        #else
        return errno;
        #endif
    }
    return bytesSent;
}

int TCPSocket::Recv( void* buf, int len ) {
    int bytesReceived = recv( sock_, static_cast<char*>( buf ), len, 0 );
    if ( bytesReceived <= 0 ) {
        #if defined( _WIN64 ) || defined( _WIN32 )
        return WSAGetLastError();
        #else
        return errno;
        #endif
    }
    return bytesReceived;
}

int TCPSocket::Close() {
    close( sock_ );
    closed_ = true;
}
