#ifndef NASDAQ_MESSAGES_ORDER_REPLACE_H_
#define NASDAQ_MESSAGES_ORDER_REPLACE_H_

#include <cstdint>

#include <Message.h>

#include <OrderBook.h>

// Class to store the Order Replace message body
class OrderReplace : public Message {

    private: 
        uint64_t originalOrderReferenceNumber;
        uint64_t newOrderReferenceNumber;
        uint32_t shares;
        uint32_t price;

    public:
        OrderReplace(BinaryMessageHeader header, uint64_t originalOrderReferenceNumber, uint64_t newOrderReferenceNumber, uint32_t shares, uint32_t price) :
        Message(std::move(header)),
        originalOrderReferenceNumber(originalOrderReferenceNumber),
        newOrderReferenceNumber(newOrderReferenceNumber),
        shares(shares),
        price(price)
        {}

        void processMessage() const override {
            OrderBook::getInstance().replaceActiveOrder(
                header.getStockLocate(), 
                originalOrderReferenceNumber, 
                newOrderReferenceNumber, 
                shares, 
                price);
        }

        void setOriginalOrderReferenceNumber(uint64_t originalOrderReferenceNumber) { this -> originalOrderReferenceNumber = originalOrderReferenceNumber; }
        void setNewOrderReferenceNumber(uint64_t newOrderReferenceNumber) { this -> newOrderReferenceNumber = newOrderReferenceNumber; }
        void setShares(uint64_t shares) { this -> shares = shares; }
        void setPrice(uint64_t price) { this -> price = price; }

        uint64_t getOriginalOrderReferenceNumber() const { return this -> originalOrderReferenceNumber; }
        uint64_t getNewOrderReferenceNumber() const { return this -> newOrderReferenceNumber; }
        uint32_t getShares() const { return this -> shares; }
        uint32_t getPrice() const { return this -> price; }    
};

// Parse the order replace message body
OrderReplace* parseOrderReplaceBody(BinaryMessageHeader header, const char* data);

#endif // NASDAQ_MESSAGES_ORDER_REPLACE_H_