#ifndef NASDAQ_MESSAGES_ORDER_CANCEL_H_
#define NASDAQ_MESSAGES_ORDER_CANCEL_H_

#include <cstdint>

// Struct to store the Order Cancel message body
struct OrderCancel {
    uint64_t orderReferenceNumber;
    uint32_t cancelledShares;
};

// Parse the order cancel message body
OrderCancel* parseOrderCancelBody(const char* data);

#endif // NASDAQ_MESSAGES_ORDER_CANCEL_H_