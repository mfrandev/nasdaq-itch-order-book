#include <OrderCancel.h>

#include <cstring>
#include <cstdint>

/**
 * Parse the OrderCancel body
 */
std::shared_ptr<OrderCancel> parseOrderCancelBody(const char* data) {
    static std::shared_ptr<OrderCancel> orderCancel = std::make_shared<OrderCancel>();
    std::size_t offset = 0;
    std::memcpy(&(orderCancel -> originalOrderReferenceNumber), &data[offset], 8); // We know this is an 8 byte int
    offset += 8;
    std::memcpy(&(orderCancel -> cancelledShares), &data[offset], 4); // We know this is a 4 byte int
    return orderCancel;
}