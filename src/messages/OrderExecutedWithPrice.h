#ifndef NASDAQ_MESSAGES_ORDER_EXECUTED_WITH_PRICE_H_
#define NASDAQ_MESSAGES_ORDER_EXECUTED_WITH_PRICE_H_

#include <cstdint>
#include <stdlib.h>

// Store all other fields for the OrderExecutedWithPrice message here
struct OrderExecutedWithPrice {

    private:
        uint64_t orderReferenceNumber;
        uint32_t executedShares;
        uint64_t matchNumber;
        char printable;
        uint32_t executionPrice;

    public:
        OrderExecutedWithPrice(uint64_t orderReferenceNumber, uint32_t executedShares, uint64_t matchNumber, char printable, uint32_t executionPrice) :
        orderReferenceNumber(orderReferenceNumber),
        executedShares(executedShares),
        matchNumber(matchNumber),
        printable(printable),
        executionPrice(executionPrice)
        {}

        void getOrderReferenceNumber(uint64_t orderReferenceNumber) { this -> orderReferenceNumber = orderReferenceNumber; }
        void getExecutedShares(uint32_t executedShares) { this -> executedShares = executedShares; }
        void getMatchNumber(uint64_t matchNumber) { this -> matchNumber = matchNumber; }
        void getPrintable(char printable) { this -> printable = printable; }
        void getExecutionPrice(uint32_t executionPrice) { this -> executionPrice = executionPrice; }

        uint64_t getOrderReferenceNumber() const { return this -> orderReferenceNumber; }
        uint32_t getExecutedShares() const { return this -> executedShares; }
        uint64_t getMatchNumber() const { return this -> matchNumber; }
        char getPrintable() const { return this -> printable; }
        uint32_t getExecutionPrice() const { return this -> executionPrice; }
};

OrderExecutedWithPrice parseOrderExecutedWithPriceBody(const char* data);

#endif // NASDAQ_MESSAGES_ORDER_EXECUTED_WITH_PRICE_H_