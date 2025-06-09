#ifndef NASDAQ_MESSAGES_ORDER_DELETE_H_
#define NASDAQ_MESSAGES_ORDER_DELETE_H_

#include <cstdint>

#include <Message.h>

#include <OrderBook.h>

// Class to store the Order Delete message body
class OrderDelete : public Message {

    private: 
        uint64_t orderReferenceNumber;
    
    public:
        OrderDelete(BinaryMessageHeader header, uint64_t orderReferenceNumber) :
        Message(std::move(header)),
        orderReferenceNumber(orderReferenceNumber)
        {}

        void processMessage() const override { OrderBook::getInstance().deleteActiveOrder(orderReferenceNumber); }

        void setOrderReferenceNumber(uint64_t orderReferenceNumber) { this -> orderReferenceNumber = orderReferenceNumber; }
        uint64_t getOrderReferenceNumber() const { return this -> orderReferenceNumber; }
};

// Parse the order delete message body
OrderDelete* parseOrderDeleteBody(BinaryMessageHeader header, const char* data);

#endif // NASDAQ_MESSAGES_ORDER_DELETE_H_