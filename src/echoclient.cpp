#include "app_constants.h"

#include <stdio.h>
#include <string.h>
// #include <unistd.h> -- do we need this?

#include "SocketAddress.h"
#include "TCPSocket.h"
 
int main(int argc , char *argv[]) {
    char message[1000] , server_reply[2000];
     
    #if defined( _WIN64 ) || defined( _WIN32 )
    WSADATA wsa_data;
    WSAStartup( MAKEWORD( 2, 2 ), &wsa_data );
    #endif

    TCPSocket* socket = TCPSocket::Create();
    if ( socket == nullptr ) {
        perror( "Could not create socket" );
        return 1;
    }
    puts( "Socket created" );
    
    char const * localhost = "127.0.0.1";
    SocketAddress server_addr( localhost, SERVER_PORT );
 
    //Connect to remote server
    if ( socket->Connect( server_addr ) < 0 ) {
        perror( "connect failed. Error" );
        return 1;
    }
     
    puts( "Connected" );
     
    //keep communicating with server
    while ( true ) {
        printf( "Enter message : " );
        if ( scanf( "%s" , message ) == EOF ) break;

        // Send data
        if ( socket->Send( message, strlen( message ) ) < 0 ) {
            puts("Send failed");
            return 1;
        }
         
        // Receive a reply from the server
        if( socket->Recv( server_reply, 2000 ) < 0 ) {
            puts( "recv failed" );
            break;
        }
         
        puts( "Server reply :" );
        puts( server_reply );
    }
     
    delete socket;

    #if defined( _WIN64 ) || defined( _WIN32 )
    WSACleanup();
    #endif

    return 0;
}
