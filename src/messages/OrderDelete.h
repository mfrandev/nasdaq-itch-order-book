#ifndef NASDAQ_MESSAGES_ORDER_DELETE_H_
#define NASDAQ_MESSAGES_ORDER_DELETE_H_

#include <cstdint>

// Class to store the Order Delete message body
class OrderDelete {

    private: 
        uint64_t orderReferenceNumber;
    
    public:
        OrderDelete(uint64_t orderReferenceNumber) :
        orderReferenceNumber(orderReferenceNumber)
        {}

        void setOrderReferenceNumber(uint64_t orderReferenceNumber) { this -> orderReferenceNumber = orderReferenceNumber; }
        uint64_t getOrderReferenceNumber() const { return this -> orderReferenceNumber; }
};

// Parse the order delete message body
OrderDelete parseOrderDeleteBody(const char* data);

#endif // NASDAQ_MESSAGES_ORDER_DELETE_H_