#ifndef TREXQUANTTAKEHOME_MESSAGES_ORDER_EXECUTED_WITH_PRICE_H_
#define TREXQUANTTAKEHOME_MESSAGES_ORDER_EXECUTED_WITH_PRICE_H_

#include <cstdint>

const std::size_t PRINTABLE_SIZE = 1;

// Store all other fields for the OrderExecutedWithPrice message here
struct OrderExecutedWithPrice {
    uint64_t orderReferenceNumber;
    uint32_t executedShares;
    uint64_t matchNumber;
    char printable;
    uint32_t executionPrice;
};

OrderExecutedWithPrice* parseOrderExecutedWithPriceBody(const char* data);

#endif // TREXQUANTTAKEHOME_MESSAGES_ORDER_EXECUTED_WITH_PRICE_H_