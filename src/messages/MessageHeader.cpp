#include <MessageHeader.h>

#include <endian_utils.h>

/**
 * Take a pre-loaded sequence of bytes and parse the ITCH header out of it
 * Use the same portion of memory to store the header, just because
 */
BinaryMessageHeader* parseHeader(const char* data) {
    static BinaryMessageHeader header;
    size_t offset = 0;
    header.messageType = toHostEndianUpTo64(&data[offset], 1);
    offset += 1;
    header.stockLocate = toHostEndianUpTo64(&data[offset], 2);
    offset += 2;
    // header.trackingNumber = (&data[offset], 2); Never use this 
    offset += 2;
    header.timestamp = toHostEndianUpTo64(&data[offset], 6);
    return &header;
}
