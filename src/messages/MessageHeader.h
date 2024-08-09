#ifndef TREXQUANTTAKEHOME_MESSAGES_MESSAGE_HEADER_H_
#define TREXQUANTTAKEHOME_MESSAGES_MESSAGE_HEADER_H_

#include <memory>
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

std::shared_ptr<BinaryMessageHeader> parseHeader(const char* data);

#endif // TREXQUANTTAKEHOME_MESSAGES_MESSAGE_HEADER_H_