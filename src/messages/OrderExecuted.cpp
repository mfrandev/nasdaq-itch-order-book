#include <OrderExecuted.h>

#include <endian_utils.h>

OrderExecuted* parseOrderExecutedBody(const char* data) {
    static OrderExecuted orderExecuted;
    std::size_t offset = 0;
    orderExecuted.orderReferenceNumber = toHostEndianUpTo64(&data[offset], 8); // We know this is 8 bytes
    offset += 8;
    orderExecuted.executedShares = toHostEndianUpTo64(&data[offset], 4); // We know this is 4 bytes
    offset += 4;
    orderExecuted.matchNumber = toHostEndianUpTo64(&data[offset], 8); // We know this is 8 bytes
    return &orderExecuted;
}
