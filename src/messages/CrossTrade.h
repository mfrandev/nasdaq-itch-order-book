#ifndef NASDAQ_MESSAGES_CROSS_TRADE_H_
#define NASDAQ_MESSAGES_CROSS_TRADE_H_

#include <cstdint>
#include <string>

// Class for parsing the CrossTrade binary message
class CrossTrade {
    private:
        uint64_t shares;
        std::string stock;
        uint32_t crossPrice;
        uint64_t matchNumber;
        char crossType;

    public:
        CrossTrade(uint64_t shares, std::string stock, uint32_t crossPrice, uint64_t matchNumber, char crossType) :
        shares(shares),
        stock(std::move(stock)),
        crossPrice(crossPrice),
        matchNumber(matchNumber),
        crossType(crossType) 
        {}

        void setShares(uint64_t shares) { this -> shares = shares; }
        void setStock(std::string stock) { this -> stock = std::move(stock); }
        void setCrossPrice(uint32_t crossPrice) { this -> crossPrice = crossPrice; }
        void setMatchNumber(uint64_t matchNumber) { this -> matchNumber = matchNumber; }
        void setCrossType(char crossType) { this -> crossType = crossType; }

        uint64_t getShares() const { return this -> shares; }
        std::string getStock() const { return this -> stock; }
        uint32_t getCrossPrice() const { return this -> crossPrice; }
        uint64_t getMatchNumber() const { return this -> matchNumber; }
        char getCrossType() const { return this -> crossType; }
};

CrossTrade parseCrossTradeBody(const char* data);

#endif // NASDAQ_MESSAGES_CROSS_TRADE_H_