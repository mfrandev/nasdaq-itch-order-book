#ifndef NASDAQ_MESSAGES_ADD_ORDER_H_
#define NASDAQ_MESSAGES_ADD_ORDER_H_

#include <string>
#include <cstdint>

constexpr size_t BUY_SELL_INDICATOR_SIZE = 1;
// constexpr size_t STOCK_SIZE = 8; Use the value from StockTradingAction.h

// Store the Add Order message body
class AddOrder {
    private:
        uint64_t orderReferenceNumber;
        char buySellIndicator;
        uint32_t shares;
        std::string stock;
        uint32_t price;
    
    public:
        /**
         * Rule of 5 compliant
         * Use const std::string& param, since constructor is guaranteed to receive an LValue argument in these cases.
         */
        AddOrder(uint64_t orderReferenceNumber, char buySellIndicator, uint32_t shares, const std::string& stock, uint32_t price) :
        orderReferenceNumber(orderReferenceNumber),
        buySellIndicator(buySellIndicator),
        shares(shares),
        stock(stock),
        price(price)
        {}

        // Setters
        void setOrderReferenceNumber(uint64_t orderReferenceNumber) { this -> orderReferenceNumber = orderReferenceNumber; }
        void setBuySellIndicator(char buySellIndicator) { this -> buySellIndicator = buySellIndicator; }
        void setShares(uint32_t shares) { this -> shares = shares; }
        void setStock(std::string stock) { this -> stock = std::move(stock); }
        void setPrice(uint32_t price) { this -> price = price; }

        // Getters
        uint64_t getOrderReferenceNumber() const { return this -> orderReferenceNumber; }
        char getBuySellIndicator() const { return this -> buySellIndicator; }
        uint32_t getShares() const { return this -> shares; }
        std::string getStock() const { return this -> stock; }
        uint32_t getPrice() const { return this -> price; }
};

AddOrder parseAddOrderBody(const char* data);

#endif // NASDAQ_MESSAGES_ADD_ORDER_H_