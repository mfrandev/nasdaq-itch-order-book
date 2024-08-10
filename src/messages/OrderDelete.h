#ifndef TREXQUANTTAKEHOME_MESSAGES_ORDER_DELETE_H_
#define TREXQUANTTAKEHOME_MESSAGES_ORDER_DELETE_H_

#include <memory>

// Struct to store the Order Delete message body
struct OrderDelete {
    uint64_t originalOrderReferenceNumber;
};

// Parse the order delete message body
std::shared_ptr<OrderDelete> parseOrderDeleteBody(const char* data);

#endif // TREXQUANTTAKEHOME_MESSAGES_ORDER_DELETE_H_