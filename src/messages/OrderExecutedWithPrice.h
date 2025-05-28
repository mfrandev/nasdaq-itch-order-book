#ifndef NASDAQ_MESSAGES_ORDER_EXECUTED_WITH_PRICE_H_
#define NASDAQ_MESSAGES_ORDER_EXECUTED_WITH_PRICE_H_

#include <cstdint>
#include <stdlib.h>

const size_t PRINTABLE_SIZE = 1;

// Store all other fields for the OrderExecutedWithPrice message here
struct OrderExecutedWithPrice {
    uint64_t orderReferenceNumber;
    uint32_t executedShares;
    uint64_t matchNumber;
    char printable;
    uint32_t executionPrice;
};

OrderExecutedWithPrice* parseOrderExecutedWithPriceBody(const char* data);

#endif // NASDAQ_MESSAGES_ORDER_EXECUTED_WITH_PRICE_H_