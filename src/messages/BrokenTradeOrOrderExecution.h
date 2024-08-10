#ifndef TREXQUANTTAKEHOME_MESSAGES_BROKEN_TRADE_OR_ORDER_EXECUTION_H_
#define TREXQUANTTAKEHOME_MESSAGES_BROKEN_TRADE_OR_ORDER_EXECUTION_H_

#include <memory>

// Struct to store the Broken Trade or Order Execution message body
struct BrokenTradeOrOrderExecution {
    uint64_t matchNumber;
};

// Parse the Broken Trade or Order Execution message body
std::shared_ptr<BrokenTradeOrOrderExecution> parseBrokenTradeOrOrderExecutionBody(const char* data);

#endif // TREXQUANTTAKEHOME_MESSAGES_BROKEN_TRADE_OR_ORDER_EXECUTION_H_