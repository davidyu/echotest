#pragma once

const int PEDANTIC_CLIENT_PORT = 10001;
const int PEDANTIC_SERVER_PORT = 10002;
const int BUFFER_MTU = 1300; // we should not let our buffers exceed this size before sending

enum PEDANTIC_MESSAGE {
    JOIN = 1,
    CLIENT_UPDATE_INPUT,
    SERVER_UPDATE_LOCATION,
    ACK,
    NACK,
    PEDANTIC_MESSAGE_FOOTER_DO_NOT_USE_ME,
};

static_assert( PEDANTIC_MESSAGE_FOOTER_DO_NOT_USE_ME <= 255, "PEDANTIC_MESSAGE is larger than an 8-bit char!" );
