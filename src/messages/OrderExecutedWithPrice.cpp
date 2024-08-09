#include <OrderExecutedWithPrice.h>

#include <cstring>

#include <utils/endian_utils.h>

/**
 * Parse the orderExecutedWithPrice message body
 */
std::shared_ptr<OrderExecutedWithPrice> parseOrderExecutedWithPriceBody(const char* data) {
    static std::shared_ptr<OrderExecutedWithPrice> orderExecutedWithPrice = std::make_shared<OrderExecutedWithPrice>();
    std::size_t offset = 0;
    orderExecutedWithPrice -> orderReferenceNumber = toHostEndianUpTo64(&data[offset], 8); // We know this is 8 bytes
    offset += 8;
    orderExecutedWithPrice -> executedShares = toHostEndianUpTo64(&data[offset], 4); // We know this is 4 bytes
    offset += 4;
    orderExecutedWithPrice -> matchNumber = toHostEndianUpTo64(&data[offset], 8); // We know this is 8 bytes
    offset += 8;
    std::memcpy(&(orderExecutedWithPrice -> printable), &data[offset], PRINTABLE_SIZE);
    offset += PRINTABLE_SIZE;
    orderExecutedWithPrice -> executionPrice = toHostEndianUpTo64(&data[offset], 4); // We know this is 4 bytes
    return orderExecutedWithPrice;
}
