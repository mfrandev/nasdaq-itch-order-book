#include <OrderReplace.h>

#include <cstring>
#include <cstdint>

/**
 * Parse the OrderReplace body
 */
std::shared_ptr<OrderReplace> parseOrderReplaceBody(const char* data) {
    static std::shared_ptr<OrderReplace> orderReplace = std::make_shared<OrderReplace>();
    std::size_t offset = 0;
    std::memcpy(&(orderReplace -> originalOrderReferenceNumber), &data[offset], 8); // We know this is an 8 byte int
    offset += 8;
    std::memcpy(&(orderReplace -> newOrderReferenceNumber), &data[offset], 8); // We know this is an 8 byte int
    offset += 8;
    std::memcpy(&(orderReplace -> shares), &data[offset], 4); // We know this is a 4 byte int
    offset += 4;
    std::memcpy(&(orderReplace -> price), &data[offset], 4); // We know this is a 4 byte int
    return orderReplace;
}
