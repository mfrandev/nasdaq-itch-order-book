#ifndef TREXQUANTTAKEHOME_MESSAGES_ORDER_EXECUTED_H_
#define TREXQUANTTAKEHOME_MESSAGES_ORDER_EXECUTED_H_

#include <cstdint>
#include <memory>

// Store all other fields for the OrderExecuted message here
struct OrderExecuted {
    uint64_t orderReferenceNumber;
    uint32_t executedShares;
    uint64_t matchNumber;
};

std::shared_ptr<OrderExecuted> parseOrderExecutedBody(const char* data);

#endif // TREXQUANTTAKEHOME_MESSAGES_ORDER_EXECUTED_H_