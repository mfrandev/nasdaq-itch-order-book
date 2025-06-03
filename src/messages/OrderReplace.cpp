#include <OrderReplace.h>

#include <endian_utils.h>

/**
 * Parse the OrderReplace body
 */
OrderReplace parseOrderReplaceBody(const char* data) {
    size_t offset = 0;
    uint64_t originalOrderReferenceNumber = toHostEndianUpTo64(&data[offset], 8); // We know this is an 8 byte int
    offset += 8;
    uint64_t newOrderReferenceNumber = toHostEndianUpTo64(&data[offset], 8); // We know this is an 8 byte int
    offset += 8;
    uint32_t shares = toHostEndianUpTo64(&data[offset], 4); // We know this is a 4 byte int
    offset += 4;
    uint32_t price = toHostEndianUpTo64(&data[offset], 4); // We know this is a 4 byte int
    return OrderReplace(originalOrderReferenceNumber, newOrderReferenceNumber, shares, price);
}
