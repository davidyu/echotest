#pragma once

#include <stdint.h>

// there's generally no need to use these;
// prefer htons/htonl or ntohs/ntohl
uint16_t ReverseBytes( uint16_t b );
uint32_t ReverseBytes( uint32_t b );
uint64_t ReverseBytes( uint64_t b );
