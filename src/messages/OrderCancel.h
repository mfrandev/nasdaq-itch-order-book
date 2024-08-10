#ifndef TREXQUANTTAKEHOME_MESSAGES_ORDER_CANCEL_H_
#define TREXQUANTTAKEHOME_MESSAGES_ORDER_CANCEL_H_

#include <memory>

// Struct to store the Order Cancel message body
struct OrderCancel {
    uint64_t originalOrderReferenceNumber;
    uint32_t cancelledShares;
};

// Parse the order cancel message body
std::shared_ptr<OrderCancel> parseOrderCancelBody(const char* data);

#endif // TREXQUANTTAKEHOME_MESSAGES_ORDER_CANCEL_H_