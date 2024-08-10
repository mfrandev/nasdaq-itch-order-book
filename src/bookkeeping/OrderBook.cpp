#include <OrderBook.h>

#include <stdlib.h>
#include <cassert>

#include <time_utils.h>

// Singleton declaration and getter
OrderBook* OrderBook::_instance = nullptr;
OrderBook& OrderBook::getInstance() {
    if(_instance == nullptr) 
        _instance = new OrderBook();
    return *_instance;
}

/**
 * Add the active order data to the book.
 * Struct initialized with "malloc" so it can be freed later
 * This should never be called more than once for a give orderReferenceNumber
 */
void OrderBook::addToActiveOrders(uint64_t orderReferenceNumber, uint16_t stockLocate, uint32_t numShares, uint32_t price) {
    ActiveOrderData* data = (ActiveOrderData*)malloc(sizeof(ActiveOrderData));
    data -> stockLocate = stockLocate;
    data -> numShares = numShares;
    data -> price = price;
    _activeOrdersBook[orderReferenceNumber] = data;
}

/**
 * This executes an order in full or in part. Orders in part have unique match numbers
 */
void OrderBook::executeActiveOrder(uint64_t orderReferenceNumber, uint32_t numExecutedShares, uint64_t matchNumber) {
    ActiveOrderData* data = _activeOrdersBook[orderReferenceNumber];
    uint32_t numOutstandingSharesOnOrder = data -> numShares - numExecutedShares;

    // Save execution info 
    ExecutedOrderOrTradeData* executedOrder = (ExecutedOrderOrTradeData*)(malloc(sizeof(ExecutedOrderOrTradeData)));
    executedOrder -> stockLocate = data -> stockLocate;
    executedOrder -> numShares = numExecutedShares;
    executedOrder -> executionPrice = data -> price;
    executedOrder -> executionPeriod = currentPeriod;
    _executedOrdersBook[matchNumber] = executedOrder;

    if(numOutstandingSharesOnOrder == 0) {
        _activeOrdersBook.erase(orderReferenceNumber);
        free(data);
    } else {
        data -> numShares -= numExecutedShares;
    }
}

/**
 * Only difference between this and the function above is execution price
 */
void OrderBook::executeActiveOrderWithPrice(uint64_t orderReferenceNumber, uint32_t numExecutedShares, uint64_t matchNumber, uint32_t price) {
    ActiveOrderData* data = _activeOrdersBook[orderReferenceNumber];
    uint32_t numOutstandingSharesOnOrder = data -> numShares - numExecutedShares;

    // Save execution info 
    ExecutedOrderOrTradeData* executedOrder = (ExecutedOrderOrTradeData*)(malloc(sizeof(ExecutedOrderOrTradeData)));
    executedOrder -> stockLocate = data -> stockLocate;
    executedOrder -> numShares = numExecutedShares;
    executedOrder -> executionPrice = price;
    executedOrder -> executionPeriod = currentPeriod;
    _executedOrdersBook[matchNumber] = executedOrder;

    if(numOutstandingSharesOnOrder == 0) {
        _activeOrdersBook.erase(orderReferenceNumber);
        free(data);
    } else 
        data -> numShares -= numExecutedShares;
}

/**
 * "This message is sent whenever an order on the book is modified as a result of a partial cancellation."
 * Assuming this is always a partial cancellation. Will not check for share count after cancellation. 
 * Assuming the number of shares on the order is greater than than number of shares being cancelled
 */
void OrderBook::cancelActiveOrder(uint64_t orderReferenceNumber, uint32_t numCancelledShares) {
    ActiveOrderData* data = _activeOrdersBook[orderReferenceNumber];
    assert(data -> numShares > numCancelledShares);
    data -> numShares -= numCancelledShares;
}

/**
 * Remove an active order from the book
 */
void OrderBook::deleteActiveOrder(uint64_t orderReferenceNumber) {
    ActiveOrderData* data = _activeOrdersBook[orderReferenceNumber];
    _activeOrdersBook.erase(orderReferenceNumber);
    free(data);
}

/**
 * Retrieve the listing with the original order reference number, update the struct, and put it back in the book with the new reference number
 */
void OrderBook::replaceActiveOrder(uint64_t originalOrderReferenceNumber, uint64_t newOrderReferenceNumber, uint32_t newNumShares, uint32_t newPrice) {
    ActiveOrderData* data = _activeOrdersBook[originalOrderReferenceNumber];
    data -> numShares = newNumShares;
    data -> price = newPrice;
    _activeOrdersBook.erase(originalOrderReferenceNumber);
    _activeOrdersBook[newOrderReferenceNumber] = data;
}


/**
 * The parameters of this function represet an executed trade that was never registered as an active order
 */
void OrderBook::trackExecutedTrade(uint64_t matchNumber, uint16_t stockLocate, uint32_t numShares, uint32_t price) {
    ExecutedOrderOrTradeData* data = (ExecutedOrderOrTradeData*)(malloc(sizeof(ExecutedOrderOrTradeData)));
    data -> stockLocate = stockLocate;
    data -> numShares = numShares;
    data -> executionPrice = price;
    data -> executionPeriod = currentPeriod;
    _executedOrdersBook[matchNumber] = data;
}

/**
 * Need to look into the executed orders and trades map, retrieve the record, and return it to the VWAPManager
 */
ExecutedOrderOrTradeData* OrderBook::handleBrokenOrderOrTrade(uint64_t matchNumber) {
    return _executedOrdersBook[matchNumber];
}