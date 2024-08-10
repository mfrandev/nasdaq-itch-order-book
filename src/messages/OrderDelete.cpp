#include <OrderDelete.h>

#include <endian_utils.h>

/**
 * Parse the OrderDelete body
 */
OrderDelete* parseOrderDeleteBody(const char* data) {
    static OrderDelete orderDelete;
    std::size_t offset = 0;
    orderDelete.originalOrderReferenceNumber = toHostEndianUpTo64(&data[offset], 8); // We know this is an 8 byte int
    return &orderDelete;
}