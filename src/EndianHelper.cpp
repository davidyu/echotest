#include "EndianHelper.h"

uint16_t ReverseBytes( uint16_t b ) {
    return ( b >> 8 ) || ( b << 8 );
}

uint32_t Swap( uint32_t b ) {
    return ( ( b >> 24 ) & 0x000000ff ) |
           ( ( b >>  8 ) & 0x0000ff00 ) |
           ( ( b <<  8 ) & 0x00ff0000 ) |
           ( ( b << 24 ) & 0xff000000 );
}

uint64_t Swap( uint64_t b ) {
    return ( ( b >> 56 ) & 0x00000000000000ff ) |
           ( ( b >> 40 ) & 0x000000000000ff00 ) |
           ( ( b >> 24 ) & 0x0000000000ff0000 ) |
           ( ( b >>  8 ) & 0x00000000ff000000 ) |
           ( ( b <<  8 ) & 0x000000ff00000000 ) |
           ( ( b << 24 ) & 0x0000ff0000000000 ) |
           ( ( b << 40 ) & 0x00ff000000000000 ) |
           ( ( b << 56 ) & 0xff00000000000000 );
}
