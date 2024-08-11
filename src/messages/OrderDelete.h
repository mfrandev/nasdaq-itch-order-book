#ifndef TREXQUANTTAKEHOME_MESSAGES_ORDER_DELETE_H_
#define TREXQUANTTAKEHOME_MESSAGES_ORDER_DELETE_H_

#include <cstdint>

// Struct to store the Order Delete message body
struct OrderDelete {
    uint64_t orderReferenceNumber;
};

// Parse the order delete message body
OrderDelete* parseOrderDeleteBody(const char* data);

#endif // TREXQUANTTAKEHOME_MESSAGES_ORDER_DELETE_H_