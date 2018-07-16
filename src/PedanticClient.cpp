#include "app_constants.h"

#include <stdio.h> //printf
#include <string.h>    //strlen
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
#include <unistd.h>

#include "SocketAddress.h"
#include "UDPSocket.h"
 
int main(int argc , char *argv[]) {
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

    //keep communicating with server
    while ( true ) {
        // 
        //
        /*
        char * data[1000];
        size_t size = 1000;

        if ( socket->SendTo( data, size, server_addr ) < 0 ) {
            puts("Send failed");
            return 1;
        }
         
        // Receive a reply from the server
        if ( socket->RecvFrom( server_reply, 2000, server_addr ) < 0 ) {
            puts( "recv failed" );
            break;
        }
         
        puts( "Server reply :" );
        puts( server_reply );
        */
    }
     
    delete socket;
    return 0;
}
