#ifndef NASDAQ_MESSAGES_ORDER_EXECUTED_H_
#define NASDAQ_MESSAGES_ORDER_EXECUTED_H_

#include <cstdint>

// Store all other fields for the OrderExecuted message here
struct OrderExecuted {
    uint64_t orderReferenceNumber;
    uint32_t executedShares;
    uint64_t matchNumber;
};

OrderExecuted* parseOrderExecutedBody(const char* data);

#endif // NASDAQ_MESSAGES_ORDER_EXECUTED_H_