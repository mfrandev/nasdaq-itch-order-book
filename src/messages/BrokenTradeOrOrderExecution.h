#ifndef NASDAQ_MESSAGES_BROKEN_TRADE_OR_ORDER_EXECUTION_H_
#define NASDAQ_MESSAGES_BROKEN_TRADE_OR_ORDER_EXECUTION_H_

#include <cstdint>

// Struct to store the Broken Trade or Order Execution message body
struct BrokenTradeOrOrderExecution {
    uint64_t matchNumber;
};

// Parse the Broken Trade or Order Execution message body
BrokenTradeOrOrderExecution* parseBrokenTradeOrOrderExecutionBody(const char* data);

#endif // NASDAQ_MESSAGES_BROKEN_TRADE_OR_ORDER_EXECUTION_H_