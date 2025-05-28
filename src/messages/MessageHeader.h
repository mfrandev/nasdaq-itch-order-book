#ifndef NASDAQ_MESSAGES_MESSAGE_HEADER_H_
#define NASDAQ_MESSAGES_MESSAGE_HEADER_H_

#include <cstdint>

// Format for the first 11 bytes of each message
struct BinaryMessageHeader {
    char messageType;
    uint16_t stockLocate;
    uint16_t trackingNumber;
    uint64_t timestamp; 
};

const int NUMBER_OF_BYTES_FOR_HEADER_CHUNK          = 13;
const int NUMBER_OF_BYTES_OFFSET_FOR_HEADER_CHUNK   = 2;

BinaryMessageHeader* parseHeader(const char* data);

#endif // NASDAQ_MESSAGES_MESSAGE_HEADER_H_