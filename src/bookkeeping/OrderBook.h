#ifndef TREXQUANTTAKEHOME_BOOKKEEPING_ORDER_BOOK_H
#define TREXQUANTTAKEHOME_BOOKKEEPING_ORDER_BOOK_H

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

        void addToActiveOrders(uint64_t orderReferenceNumber, uint16_t stockLocate, uint32_t numShares, uint32_t price);
        void executeActiveOrder(uint64_t orderReferenceNumber, uint32_t numExecutedShares, uint64_t matchNumber);
        void executeActiveOrderWithPrice(uint64_t orderReferenceNumber, uint32_t numExecutedShares, uint64_t matchNumber, uint32_t price);
        void cancelActiveOrder(uint64_t orderReferenceNumber, uint32_t numCancelledShares);
        void deleteActiveOrder(uint64_t orderReferenceNumber);
        void replaceActiveOrder(uint64_t originalOrderReferenceNumber, uint64_t newOrderReferenceNumber, uint32_t newNumShares, uint32_t newPrice);

        void trackExecutedTrade(uint64_t matchNumber, uint16_t stockLocate, uint32_t numShares, uint32_t price);
        ExecutedOrderOrTradeData* handleBrokenOrderOrTrade(uint64_t matchNumber);


    private:

        static OrderBook* _instance;

        /**
         * 1. This maps an Order Reference Number to an active order
         * 2. <k,v> mappings in this may get updated or removed throughout the day
         * 3. Using raw pointers again because I am still scared of RAII overhead
         */
        std::unordered_map<uint64_t, ActiveOrderData*> _activeOrdersBook;

        /** 
         * 1. This maps a Match Number to an executed or partially executed order
         * 2. This book is absolutely necessary for retroactively updateing VWAP after a broken trade (only case when this mapping is modified)
         * 3. Using a raw pointer here because freeing any cancelled orders during the broken trade handler is simple enough
         * 4. Entries in this map come from all reporting periods.
         */ 
        std::unordered_map<uint64_t, ExecutedOrderOrTradeData*> _executedOrdersBook;


};

#endif // TREXQUANTTAKEHOME_BOOKKEEPING_ORDER_BOOK_H