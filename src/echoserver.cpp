#include "app_constants.h"

#if defined( _WIN64 ) || defined( _WIN32 )
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#else
#include <pthread.h>
#endif

#include <ctype.h> // isdigit
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>

#include "SocketAddress.h"
#include "TCPSocket.h"

void *handle_connection( void * context );

#if defined( _WIN64 ) || defined( _WIN32 )
DWORD WINAPI handle_connection_win32( LPVOID context ) {
    handle_connection( context );
    return 0;
}
#endif

struct handler_thread_info {
    #if defined( _WIN64 ) || defined( _WIN32 )
    HANDLE thread = nullptr;
    #else
    pthread_t thread = nullptr;
    #endif
    TCPSocket * client_socket = nullptr;

    #if defined( _WIN64 ) || defined( _WIN32 )
    handler_thread_info( HANDLE t, TCPSocket * s ): thread( t ), client_socket( s ) {};
    #else
    handler_thread_info( pthread_t t, TCPSocket * s ): thread( t ), client_socket( s ) {};
    #endif
};

int main( int argc , char *argv[] ) {
    #if defined( _WIN64 ) || defined( _WIN32 )
    WSADATA wsa_data;
    WSAStartup( MAKEWORD( 2, 2 ), &wsa_data );
    #endif

    // create socket
    TCPSocket* server_socket = TCPSocket::Create();
    if ( server_socket == nullptr ) {
        perror( "Could not create server socket" );
        return 1;
    }
    puts( "Socket created" );

    SocketAddress server_addr( SERVER_PORT );
     
    // bind
    if ( server_socket->Bind( server_addr ) < 0 ) {
        perror( "bind failed." );
        return 1;
    }

    puts( "bind done" );
     
    //Listen
    server_socket->Listen( 10 ); // maximum of 10 buffered connections
     
    //Accept and incoming connection
    puts( "Waiting for incoming connections..." );

    std::vector<handler_thread_info> thread_infos;
    const int MAX_HANDLER_THREADS = 64;
    thread_infos.reserve( MAX_HANDLER_THREADS );

    SocketAddress client_addr( SERVER_PORT );
    TCPSocket* client_sock;

    while ( ( client_sock = server_socket->Accept( client_addr ) ) ) {
        puts( "Connection accepted" );

        #if defined( _WIN64 ) || defined( _WIN32 )
        DWORD echo_thread_id;
        HANDLE echo_thread_handle = nullptr;
        thread_infos.emplace_back( echo_thread_handle, client_sock );
        echo_thread_handle = CreateThread( 0, 0, handle_connection_win32, &thread_infos.back(), 0, &echo_thread_id );
        if ( echo_thread_handle == nullptr ) {
            perror("could not create thread");
            break;
        } else {
            thread_infos.back().thread = echo_thread_handle; // backfill thread handle
        }
        #else
        pthread_t echo_thread = nullptr;
        thread_infos.emplace_back( echo_thread, client_sock );
        if ( pthread_create( &echo_thread, NULL, handle_connection, (void*) &thread_infos.back() ) < 0 ) {
            perror("could not create thread");
            break;
        } else {
            thread_infos.back().thread = echo_thread; // backfill pthread_t object
        }
        #endif

        puts("Handler assigned");
    }

    delete server_socket;

    #if defined( _WIN64 ) || defined( _WIN32 )
    WSACleanup();
    #endif

    return 0;
}

void * handle_connection( void * context )
{
    // Get the socket descriptor
    struct handler_thread_info* thread_info = (struct handler_thread_info*) context;
    TCPSocket* sock = thread_info->client_socket;

    if ( sock == nullptr ) {
        #if defined( _WIN64 ) || defined( _WIN32 )
        CloseHandle( thread_info->thread );
        #else
        pthread_exit( thread_info->thread );
        #endif
    }

    int read_size;
    char client_message[2000];
     
    //Receive a message from client
    while ( ( read_size = sock->Recv( client_message, 2000 ) ) > 0 )
    {
        // send the message back to client; assume a null-terminated string
        // send bytes equal to the message plus the null-terminator
        sock->Send( client_message, strlen( client_message ) + 1 );
    }
     
    if ( read_size == 0 ) {
        puts( "Client disconnected" );
    } else if ( read_size == -1 ) {
        perror( "recv failed" );
    }
         
    delete sock;

    #if defined( _WIN64 ) || defined( _WIN32 )
    CloseHandle( thread_info->thread );
    #else
    pthread_exit( thread_info->thread );
    #endif

    return nullptr;
}
