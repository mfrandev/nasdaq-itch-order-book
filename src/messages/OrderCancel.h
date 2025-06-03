#ifndef NASDAQ_MESSAGES_ORDER_CANCEL_H_
#define NASDAQ_MESSAGES_ORDER_CANCEL_H_

#include <cstdint>

// Class to store the Order Cancel message body
class OrderCancel {
    private: 
        uint64_t orderReferenceNumber;
        uint32_t cancelledShares;

    public: 
        OrderCancel(uint64_t orderReferenceNumber, uint32_t cancelledShares) :
        orderReferenceNumber(orderReferenceNumber),
        cancelledShares(cancelledShares)
        {}

        void setOrderReferenceNumber(uint64_t orderReferenceNumber) { this -> orderReferenceNumber = orderReferenceNumber; }
        void setCancelledShares(uint32_t cancelledShares) { this -> cancelledShares = cancelledShares; }

        uint64_t getOrderReferenceNumber() const { return this -> orderReferenceNumber; }
        uint32_t getCancelledShares() const { return this -> cancelledShares; }
};

// Parse the order cancel message body
OrderCancel parseOrderCancelBody(const char* data);

#endif // NASDAQ_MESSAGES_ORDER_CANCEL_H_