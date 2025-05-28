#ifndef NASDAQ_BOOKKEEPING_ORDER_BOOK_H
#define NASDAQ_BOOKKEEPING_ORDER_BOOK_H

#include <cstdint>
#include <string>
#include <unordered_map>

/**
 * Book has two mappings:
 * 1. Order Reference Number -> ActiveOrderData
 * 2. Match Number -> ExecutedOrderOrTradeData
 * (Since Orders and Trades can be retroactively broken, must maintain a collection of executed Orders and Trades)
 */
struct ExecutedOrderOrTradeData {
    uint16_t stockLocate;
    uint32_t numShares;
    uint32_t executionPrice;
    uint8_t executionPeriod;
};

struct ActiveOrderData {
    uint16_t stockLocate;
    uint32_t numShares;
    uint32_t price;
};


/**
 * This class is responsible for maintaining the order book
 */
class OrderBook {
    public:

        static OrderBook& getInstance();

        // Functions for maintaining the book
        void addToActiveOrders(uint64_t orderReferenceNumber, uint16_t stockLocate, uint32_t numShares, uint32_t price);
        uint32_t executeActiveOrder(uint64_t orderReferenceNumber, uint32_t numExecutedShares, uint64_t matchNumber);
        void executeActiveOrderWithPrice(uint64_t orderReferenceNumber, uint32_t numExecutedShares, uint64_t matchNumber, uint32_t price);
        void cancelActiveOrder(uint64_t orderReferenceNumber, uint32_t numCancelledShares);
        void deleteActiveOrder(uint64_t orderReferenceNumber);
        void replaceActiveOrder(uint64_t originalOrderReferenceNumber, uint64_t newOrderReferenceNumber, uint32_t newNumShares, uint32_t newPrice);
        
    private:

        static OrderBook* _instance;

        /**
         * 1. This maps an Order Reference Number to an active order
         * 2. <k,v> mappings in this may get updated or removed throughout the day
         * 3. Using raw pointers again because I am still scared of RAII overhead
         */
        std::unordered_map<uint64_t, ActiveOrderData*> _activeOrdersBook;

};

#endif // NASDAQ_BOOKKEEPING_ORDER_BOOK_H