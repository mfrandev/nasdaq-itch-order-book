#include <OrderDelete.h>

#include <cstring>
#include <cstdint>

/**
 * Parse the OrderDelete body
 */
std::shared_ptr<OrderDelete> parseOrderDeleteBody(const char* data) {
    static std::shared_ptr<OrderDelete> orderDelete = std::make_shared<OrderDelete>();
    std::size_t offset = 0;
    std::memcpy(&(orderDelete -> originalOrderReferenceNumber), &data[offset], 8); // We know this is an 8 byte int
    return orderDelete;
}