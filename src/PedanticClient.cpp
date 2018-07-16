#include "app_constants.h"

#include <stdio.h> //printf
#include <string.h>    //strlen

#include "SocketAddress.h"
#include "UDPSocket.h"
#include "PedanticCommon.h"
 
int main(int argc , char *argv[]) {
    #if defined( _WIN64 ) || defined( _WIN32 )
    WSADATA wsa_data;
    WSAStartup( MAKEWORD( 2, 2 ), &wsa_data );
    #endif

    UDPSocket* socket = UDPSocket::Create();
    if ( socket == nullptr ) {
        perror( "Could not create socket" );
        return 1;
    }
    puts( "Socket created" );
    
    char const * localhost = "127.0.0.1";
    SocketAddress server_addr( localhost, PEDANTIC_SERVER_PORT );
 
    SocketAddress client_addr( PEDANTIC_CLIENT_PORT );

    // bind
    if ( socket->Bind( client_addr ) < 0 ) {
        perror( "bind failed." );
        return 1;
    }

    puts( "bind done" );

    // tell server to create entity with position xyz

    // keep communicating with server
    char server_reply[2000];
    while ( true ) {
        char buf[1000];
        size_t size = 0;

        buf[0] = (char) PEDANTIC_MESSAGE::JOIN;
        size++;

        if ( socket->SendTo( buf, size, server_addr ) < 0 ) {
            puts("Send failed");
            return 1;
        }
         
        // Receive a reply from the server
        if ( socket->RecvFrom( server_reply, 2000, server_addr ) > 0 ) {
            puts( "Server reply :" );
            puts( server_reply );
        } else {
            puts( "recv failed" );
            break;
        }
    }
     
    delete socket;

    #if defined( _WIN64 ) || defined( _WIN32 )
    WSACleanup();
    #endif

    return 0;
}
