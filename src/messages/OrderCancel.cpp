#include <OrderCancel.h>

#include <cstring>
#include <cstdint>

#include <endian_utils.h>

/**
 * Parse the OrderCancel body
 */
std::shared_ptr<OrderCancel> parseOrderCancelBody(const char* data) {
    static std::shared_ptr<OrderCancel> orderCancel = std::make_shared<OrderCancel>();
    std::size_t offset = 0;
    orderCancel -> originalOrderReferenceNumber = toHostEndianUpTo64(&data[offset], 8); // We know this is an 8 byte int
    offset += 8;
    orderCancel -> cancelledShares = toHostEndianUpTo64(&data[offset], 4); // We know this is a 4 byte int
    return orderCancel;
}