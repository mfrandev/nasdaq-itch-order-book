#ifndef NASDAQ_MESSAGES_ADD_ORDER_MPID_H_
#define NASDAQ_MESSAGES_ADD_ORDER_MPID_H_

#include <string>
#include <cstdint>

// constexpr size_t BUY_SELL_INDICATOR_SIZE = 1; Use the value from AddOrder.h
// constexpr size_t STOCK_SIZE = 8; Use the value from StockTradingAction.h
constexpr size_t ATTRIBUTION_SIZE = 4;

// Store the Add Order MPID message body
class AddOrderMPID {
    private:
        uint64_t orderReferenceNumber;
        char buySellIndicator;
        uint32_t shares;
        std::string stock;
        uint32_t price;
        std::string attribution;
    
    public:
        /**
         * Rule-of-5 compliant
         * Use const std::string& param, since constructor is guaranteed to receive an LValue argument in these cases.
         */
        AddOrderMPID(uint64_t orderReferenceNumber, char buySellIndicator, uint32_t shares, const std::string& stock, uint32_t price, const std::string& attribution) :
            orderReferenceNumber(orderReferenceNumber),
            buySellIndicator(buySellIndicator),
            shares(shares),
            stock(stock),
            price(price),
            attribution(attribution)
            {}

            // Setters
            void setOrderReferenceNumber(uint64_t orderReferenceNumber) { this -> orderReferenceNumber = orderReferenceNumber; }
            void setBuySellIndicator(char buySellIndicator) { this -> buySellIndicator = buySellIndicator; }
            void setShares(uint32_t shares) { this -> shares = shares; }
            void setStock(std::string stock) { this -> stock = std::move(stock); }
            void setPrice(uint32_t price) { this -> price = price; }
            void setAttribution(std::string attribution) { this -> attribution = std::move(attribution); }

            // Getters
            uint64_t getOrderReferenceNumber() const { return this -> orderReferenceNumber; }
            char getBuySellIndicator() const { return this -> buySellIndicator; }
            uint32_t getShares() const { return this -> shares; }
            std::string getStock() const { return this -> stock; }
            uint32_t getPrice() const { return this -> price; }
            std::string getAttribution() const { return this -> attribution; }
};

AddOrderMPID parseAddOrderMPIDBody(const char* data);

#endif // NASDAQ_MESSAGES_ADD_ORDER_MPID_H_