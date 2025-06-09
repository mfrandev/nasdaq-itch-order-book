#ifndef NASDAQ_MESSAGES_ORDER_CANCEL_H_
#define NASDAQ_MESSAGES_ORDER_CANCEL_H_

#include <cstdint>

#include <Message.h>
#include <OrderBook.h>

// Class to store the Order Cancel message body
class OrderCancel : public Message {
    private: 
        uint64_t orderReferenceNumber;
        uint32_t cancelledShares;

    public: 
        OrderCancel(BinaryMessageHeader header, uint64_t orderReferenceNumber, uint32_t cancelledShares) :
        Message(std::move(header)),
        orderReferenceNumber(orderReferenceNumber),
        cancelledShares(cancelledShares)
        {}

        bool processMessage() const override { OrderBook::getInstance().cancelActiveOrder(orderReferenceNumber, cancelledShares); return true; } 

        void setOrderReferenceNumber(uint64_t orderReferenceNumber) { this -> orderReferenceNumber = orderReferenceNumber; }
        void setCancelledShares(uint32_t cancelledShares) { this -> cancelledShares = cancelledShares; }

        uint64_t getOrderReferenceNumber() const { return this -> orderReferenceNumber; }
        uint32_t getCancelledShares() const { return this -> cancelledShares; }
};

// Parse the order cancel message body
OrderCancel* parseOrderCancelBody(BinaryMessageHeader header, const char* data);

#endif // NASDAQ_MESSAGES_ORDER_CANCEL_H_