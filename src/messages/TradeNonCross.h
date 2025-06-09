#ifndef NASDAQ_MESSAGES_TRADE_NON_CROSS_H_
#define NASDAQ_MESSAGES_TRADE_NON_CROSS_H_

#include <cstdint>
#include <string>

#include <Message.h>
#include <time_utils.h>

#include <VWAPManager.h>

// constexpr size_t BUY_SELL_INDICATOR_SIZE = 1; Initialized in AddOrder.h
// constexpr size_t STOCK_SIZE           = 8; Initialized in StockTradingAction.h
// For parsing a Trade (non-cross) message
class TradeNonCross : public Message {

    private:
        uint64_t orderReferenceNumber;
        char buySellIndicator;
        uint32_t shares;
        std::string stock;
        uint32_t price;
        uint64_t matchNumber;

    public:

        /**
         * Rule of 5 compliant
         */
        TradeNonCross(BinaryMessageHeader header, uint64_t orderReferenceNumber, char buySellIndicator, uint32_t shares, std::string stock, uint32_t price, uint64_t matchNumber) :
        Message(std::move(header)),
        orderReferenceNumber(orderReferenceNumber),
        buySellIndicator(buySellIndicator),
        shares(shares),
        stock(std::move(stock)),
        price(price),
        matchNumber(matchNumber)
        {}

        bool processMessage() const override {
            if(isAfterHours(header.getTimestamp())) return false;
            VWAPManager::getInstance().handleTrade(
                header.getTimestamp(), 
                header.getStockLocate(), 
                price, 
                shares, 
                matchNumber);
            return true;
        }

        void setOrderReferenceNumber(uint64_t orderReferenceNumber) { this -> orderReferenceNumber = orderReferenceNumber; }
        void setBuySellIndicator(char buySellIndicator) { this -> buySellIndicator = buySellIndicator; }
        void setShares(uint32_t shares) { this -> shares = shares; }
        void setStock(std::string stock) { this -> stock = std::move(stock); }
        void setPrice(uint32_t price) { this -> price = price; }
        void setMatchNumber(uint64_t matchNumber) { this -> matchNumber = matchNumber; }

        uint64_t getOrderReferenceNumber() const { return this -> orderReferenceNumber; }
        char getBuySellIndicator() const { return this -> buySellIndicator; }
        uint32_t getShares() const { return this -> shares; }
        std::string getStock() const { return this -> stock; }
        uint32_t getPrice() const { return this -> price; }
        uint64_t getMatchNumber() const { return this -> matchNumber; }
};

TradeNonCross* parseTradeNonCrossBody(BinaryMessageHeader header, const char* data);

#endif // NASDAQ_MESSAGES_TRADE_NON_CROSS_H_