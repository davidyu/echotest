#include "app_constants.h"

#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write

int main(int argc , char *argv[])
{
    char client_message[2000];
     
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
    if( bind( socket_desc, (struct sockaddr *)&server , sizeof(server) ) < 0 )
    {
        perror( "bind failed. Error" );
        return 1;
    }

    puts( "bind done" );
     
    //Listen
    listen( socket_desc, 3 );
     
    //Accept and incoming connection
    puts( "Waiting for incoming connections..." );
    socklen_t c = sizeof( struct sockaddr_in );
     
    //accept connection from an incoming client
    int client_sock = accept( socket_desc, (struct sockaddr *)&client, &c );
    if ( client_sock < 0 ) {
        perror( "accept failed; could not accept connection from client" );
        return 1;
    }
    puts( "Connection accepted" );
     
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
