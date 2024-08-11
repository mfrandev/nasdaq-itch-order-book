#include <OrderCancel.h>

#include <endian_utils.h>

/**
 * Parse the OrderCancel body
 */
OrderCancel* parseOrderCancelBody(const char* data) {
    static OrderCancel orderCancel;
    std::size_t offset = 0;
    orderCancel.orderReferenceNumber = toHostEndianUpTo64(&data[offset], 8); // We know this is an 8 byte int
    offset += 8;
    orderCancel.cancelledShares = toHostEndianUpTo64(&data[offset], 4); // We know this is a 4 byte int
    return &orderCancel;
}