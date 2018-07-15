#include "app_constants.h"

#include <ctype.h> // isdigit
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#include "SocketAddress.h"
#include "TCPSocket.h"

void * handle_connection( void * client_socket );

int main( int argc , char *argv[] ) {
    // extract server IP address string (argv[1]) into 4 bytes
    char client_message[2000]; // 2000 character limit on client message

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
    SocketAddress client_addr( SERVER_PORT );
    TCPSocket* client_sock;

    while ( ( client_sock = server_socket->Accept( client_addr ) ) ) {
        puts("Connection accepted");
         
        pthread_t echo_thread;
         
        if( pthread_create( &echo_thread, NULL, handle_connection, (void*) client_sock ) < 0 )
        {
            perror("could not create thread");
            return 1;
        }

        puts("Handler assigned");
    }
     
    return 0;
}

void * handle_connection( void *client_sock )
{
    //Get the socket descriptor
    TCPSocket* sock = (TCPSocket*) client_sock;

    if ( client_sock == nullptr ) {
        pthread_exit( client_sock );
    }

    int read_size;
    char *message, client_message[2000];
     
    //Receive a message from client
    while ( ( read_size = sock->Recv( client_message, 2000 ) ) > 0 )
    {
        //Send the message back to client
        sock->Send( client_message, strlen( client_message ) );
    }
     
    if ( read_size == 0 ) {
        puts( "Client disconnected" );
    } else if ( read_size == -1 ) {
        perror( "recv failed" );
    }
         
    // Free the socket pointer
    // free( socket_desc );

    pthread_exit( client_sock );
}
