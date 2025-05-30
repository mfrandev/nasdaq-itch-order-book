#include <OrderDelete.h>

#include <endian_utils.h>

/**
 * Parse the OrderDelete body
 */
OrderDelete* parseOrderDeleteBody(const char* data) {
    static OrderDelete orderDelete;
    size_t offset = 0;
    orderDelete.orderReferenceNumber = toHostEndianUpTo64(&data[offset], 8); // We know this is an 8 byte int
    return &orderDelete;
}