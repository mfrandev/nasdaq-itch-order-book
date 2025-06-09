#ifndef NASDAQ_MESSAGES_BROKEN_TRADE_OR_ORDER_EXECUTION_H_
#define NASDAQ_MESSAGES_BROKEN_TRADE_OR_ORDER_EXECUTION_H_

#include <cstdint>

#include <Message.h>

#include <VWAPManager.h>

// Class to store the Broken Trade or Order Execution message body
class BrokenTradeOrOrderExecution : public Message {
    private:
        uint64_t matchNumber;
    public:
        BrokenTradeOrOrderExecution(BinaryMessageHeader header, uint64_t matchNumber) :
        Message(std::move(header)),
        matchNumber(matchNumber)
        {}

        void processMessage() const override { VWAPManager::getInstance().handleBrokenTrade(header.getStockLocate(), matchNumber); }

        void setMatchNumber(uint64_t matchNumber) { this -> matchNumber = matchNumber; }
        uint64_t getMatchNumber() const { return this -> matchNumber; } 
};

// Parse the Broken Trade or Order Execution message body
BrokenTradeOrOrderExecution* parseBrokenTradeOrOrderExecutionBody(BinaryMessageHeader header, const char* data);

#endif // NASDAQ_MESSAGES_BROKEN_TRADE_OR_ORDER_EXECUTION_H_