#ifndef NASDAQ_MESSAGES_BROKEN_TRADE_OR_ORDER_EXECUTION_H_
#define NASDAQ_MESSAGES_BROKEN_TRADE_OR_ORDER_EXECUTION_H_

#include <cstdint>

// Class to store the Broken Trade or Order Execution message body
class BrokenTradeOrOrderExecution {
    private:
        uint64_t matchNumber;
    public:
        BrokenTradeOrOrderExecution(uint64_t matchNumber) :
        matchNumber(matchNumber)
        {}

        void setMatchNumber(uint64_t matchNumber) { this -> matchNumber = matchNumber; }
        uint64_t getMatchNumber() const { return this -> matchNumber; } 
};

// Parse the Broken Trade or Order Execution message body
BrokenTradeOrOrderExecution parseBrokenTradeOrOrderExecutionBody(const char* data);

#endif // NASDAQ_MESSAGES_BROKEN_TRADE_OR_ORDER_EXECUTION_H_