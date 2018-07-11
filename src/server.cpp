#include "app_constants.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

void * handle_connection( void *socket_desc );

int main(int argc , char *argv[]) {
    char client_message[2000]; // 2000 character limit on client message
     
    //Create socket
    int socket_desc = socket( AF_INET, SOCK_STREAM, 0 );
    if ( socket_desc == -1 ) {
        perror("Could not create socket");
        return 1;
    }
    puts("Socket created");

    struct sockaddr_in server, client;
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( SERVER_PORT );
     
    //Bind
    if( bind( socket_desc, (struct sockaddr *)&server , sizeof(server) ) < 0 ) {
        perror( "bind failed. Error" );
        return 1;
    }

    puts( "bind done" );
     
    //Listen
    listen( socket_desc, 3 );
     
    //Accept and incoming connection
    puts( "Waiting for incoming connections..." );
    socklen_t c = sizeof( struct sockaddr_in );
    int client_sock;

    while ( ( client_sock = accept( socket_desc, (struct sockaddr *)&client, &c ) ) ) {
        puts("Connection accepted");
         
        pthread_t echo_thread;
        int * new_sock = (int*) malloc( 1 );
        *new_sock = client_sock;
         
        if( pthread_create( &echo_thread, NULL, handle_connection, (void*) new_sock ) < 0 )
        {
            perror("could not create thread");
            return 1;
        }

        puts("Handler assigned");
    }
     
    int read_size;
    // Receive a message from client
    while ( ( read_size = recv( client_sock, client_message, 2000, 0 ) ) > 0 ) {
        // Echo message back to client
        write( client_sock, client_message , strlen( client_message ) );
    }
     
    if ( read_size == 0 ) {
        puts( "Client disconnected" );
        fflush(stdout);
    } else if ( read_size == -1 ) {
        perror( "recv failed" );
    }
     
    return 0;
}

void * handle_connection( void *socket_desc )
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char *message, client_message[2000];
     
    //Receive a message from client
    while ( ( read_size = recv( sock, client_message, 2000, 0 ) ) > 0 )
    {
        //Send the message back to client
        write( sock, client_message, strlen( client_message ) );
    }
     
    if ( read_size == 0 ) {
        puts( "Client disconnected" );
    } else if ( read_size == -1 ) {
        perror( "recv failed" );
    }
         
    //Free the socket pointer
    free( socket_desc );

    pthread_exit( socket_desc );
}
