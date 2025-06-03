#ifndef NASDAQ_MESSAGES_ORDER_EXECUTED_H_
#define NASDAQ_MESSAGES_ORDER_EXECUTED_H_

#include <cstdint>

// Store all other fields for the OrderExecuted message here
class OrderExecuted {

    private: 
        uint64_t orderReferenceNumber;
        uint32_t executedShares;
        uint64_t matchNumber;

    public:
        OrderExecuted(uint64_t orderReferenceNumber, uint32_t executedShares, uint64_t matchNumber) :
        orderReferenceNumber(orderReferenceNumber),
        executedShares(executedShares),
        matchNumber(matchNumber)
        {}

        void setOrderReferenceNumber() { this -> orderReferenceNumber = orderReferenceNumber; }
        void setExecutedShares() { this -> executedShares = executedShares; }
        void setMatchNumber() { this -> matchNumber = matchNumber; }
        
        uint64_t getOrderReferenceNumber() const { return this -> orderReferenceNumber; }
        uint32_t getExecutedShares() const { return this -> executedShares; }
        uint64_t getMatchNumber() const { return this -> matchNumber; }
};

OrderExecuted parseOrderExecutedBody(const char* data);

#endif // NASDAQ_MESSAGES_ORDER_EXECUTED_H_