#ifndef TREXQUANTTAKEHOME_MESSAGES_ORDER_REPLACE_H_
#define TREXQUANTTAKEHOME_MESSAGES_ORDER_REPLACE_H_

#include <cstdint>

// Struct to store the Order Replace message body
struct OrderReplace {
    uint64_t originalOrderReferenceNumber;
    uint64_t newOrderReferenceNumber;
    uint32_t shares;
    uint32_t price;
};

// Parse the order replace message body
OrderReplace* parseOrderReplaceBody(const char* data);

#endif // TREXQUANTTAKEHOME_MESSAGES_ORDER_REPLACE_H_