#include <OrderBook.h>

#include <stdlib.h>
#include <cassert>

#include <time_utils.h>

#include <fmt/format.h>

/**
 * Took a memory profile of the OrderBook for the given file, seeing ~0.4% memory usage :)
 */

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
    // assert(!_activeOrdersBook.count(orderReferenceNumber));
    ActiveOrderData* data = (ActiveOrderData*)malloc(sizeof(ActiveOrderData));
    data -> stockLocate = stockLocate;
    data -> numShares = numShares;
    data -> price = price;
    _activeOrdersBook[orderReferenceNumber] = data;
    // assert(_activeOrdersBook.count(orderReferenceNumber));
}

/**
 * This executes an order in full or in part. Orders in part have unique match numbers
 */
uint32_t OrderBook::executeActiveOrder(uint64_t orderReferenceNumber, uint32_t numExecutedShares, uint64_t matchNumber) {
    // assert(_activeOrdersBook.count(orderReferenceNumber));
    ActiveOrderData* data = _activeOrdersBook[orderReferenceNumber];
    uint32_t numOutstandingSharesOnOrder = data -> numShares - numExecutedShares;
    uint32_t price = data -> price;

    if(numOutstandingSharesOnOrder == 0) {
        _activeOrdersBook.erase(orderReferenceNumber);
        free(data);
    } else {
        data -> numShares -= numExecutedShares;
    }
    return price;
}

/**
 * Only difference between this and the function above is execution price
 */
void OrderBook::executeActiveOrderWithPrice(uint64_t orderReferenceNumber, uint32_t numExecutedShares, uint64_t matchNumber, uint32_t price) {
    // assert(_activeOrdersBook.count(orderReferenceNumber));
    ActiveOrderData* data = _activeOrdersBook[orderReferenceNumber];
    uint32_t numOutstandingSharesOnOrder = data -> numShares - numExecutedShares;

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
    // assert(_activeOrdersBook.count(orderReferenceNumber));
    ActiveOrderData* data = _activeOrdersBook[orderReferenceNumber];
    // assert(data -> numShares > numCancelledShares);
    data -> numShares -= numCancelledShares;
}

/**
 * Remove an active order from the book
 */
void OrderBook::deleteActiveOrder(uint64_t orderReferenceNumber) {
    // assert(_activeOrdersBook.count(orderReferenceNumber));
    ActiveOrderData* data = _activeOrdersBook[orderReferenceNumber];
    _activeOrdersBook.erase(orderReferenceNumber);
    free(data);
}

/**
 * Retrieve the listing with the original order reference number, update the struct, and put it back in the book with the new reference number
 */
void OrderBook::replaceActiveOrder(uint64_t originalOrderReferenceNumber, uint64_t newOrderReferenceNumber, uint32_t newNumShares, uint32_t newPrice) {
    // assert(_activeOrdersBook.count(originalOrderReferenceNumber));
    ActiveOrderData* data = _activeOrdersBook[originalOrderReferenceNumber];
    data -> numShares = newNumShares;
    data -> price = newPrice;
    _activeOrdersBook.erase(originalOrderReferenceNumber);
    _activeOrdersBook[newOrderReferenceNumber] = data;
}