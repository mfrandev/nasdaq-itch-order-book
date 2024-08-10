#include <OrderReplace.h>

#include <cstring>
#include <cstdint>

#include <endian_utils.h>

/**
 * Parse the OrderReplace body
 */
std::shared_ptr<OrderReplace> parseOrderReplaceBody(const char* data) {
    static std::shared_ptr<OrderReplace> orderReplace = std::make_shared<OrderReplace>();
    std::size_t offset = 0;
    orderReplace -> originalOrderReferenceNumber = toHostEndianUpTo64(&data[offset], 8); // We know this is an 8 byte int
    offset += 8;
    orderReplace -> newOrderReferenceNumber = toHostEndianUpTo64(&data[offset], 8); // We know this is an 8 byte int
    offset += 8;
    orderReplace -> shares = toHostEndianUpTo64(&data[offset], 4); // We know this is a 4 byte int
    offset += 4;
    orderReplace -> price = toHostEndianUpTo64(&data[offset], 4); // We know this is a 4 byte int
    return orderReplace;
}
