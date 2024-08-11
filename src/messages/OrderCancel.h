#ifndef TREXQUANTTAKEHOME_MESSAGES_ORDER_CANCEL_H_
#define TREXQUANTTAKEHOME_MESSAGES_ORDER_CANCEL_H_

#include <cstdint>

// Struct to store the Order Cancel message body
struct OrderCancel {
    uint64_t orderReferenceNumber;
    uint32_t cancelledShares;
};

// Parse the order cancel message body
OrderCancel* parseOrderCancelBody(const char* data);

#endif // TREXQUANTTAKEHOME_MESSAGES_ORDER_CANCEL_H_