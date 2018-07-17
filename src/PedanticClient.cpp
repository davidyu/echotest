#include "app_constants.h"

#include <stdio.h>
#include <string.h> // do we need this???
#include <chrono> // high-resolution timestamp
#include <thread>

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
    // this needs to happen no matter what, so unless it's been acknowledged we can't continue
    char buf[1000];
    uint16_t seq = 1;   // packet sequence number, starts at 1
    uint16_t acked = 0; // acked up to now
    size_t size = 0;

    buf[0] = (char) PEDANTIC_MESSAGE::JOIN;
    size++;
    buf[1] = seq;
    size += 2;

    if ( socket->SendTo( buf, size, server_addr ) < 0 ) {
        puts("Send failed");
        return 1;
    }

    std::chrono::high_resolution_clock clock;
    std::chrono::high_resolution_clock::time_point start_time = clock.now();
    // while seq 0 not acknowledged, keep sending...we can't continue otherwise
    // give up after 2 seconds
    while ( acked < seq ) {
        // give up after 2 seconds of retrying
        if ( std::chrono::duration_cast<std::chrono::milliseconds>( clock.now() - start_time ) > 2000 ) break;
        if ( socket->RecvFrom( server_reply, 2000, server_addr ) > 0 ) { // TODO MAKE THIS NONBLOCKING
            if ( server_reply[0] == (char) PEDANTIC_MESSAGE::ACK ) {
                acked = server_reply[1];
                break;
            }
        } else {
            std::this_thread::sleep_for( 0.1s ); // resend packets at 10hz so as to not flood server/network
            if ( socket->SendTo( buf, size, server_addr ) < 0 ) {
                puts("Send failed");
                return 1;
            }
        }
    }

    if ( acked < seq ) {
        puts( "Could not connect to server." );
        return 1;
    }

    // keep communicating with server
    // do so on a 60-ish-tick rate
    char server_reply[2000];
    start_time = clock.now();
    while ( true ) {
        if ( std::chrono::duration_cast<std::chrono::milliseconds>( clock.now() - start_time ) < 16 ) continue;

        // Randomize input

        // Send randomized input
    
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
