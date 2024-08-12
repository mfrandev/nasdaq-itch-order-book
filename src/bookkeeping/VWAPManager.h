#ifndef TREXQUANTTAKEHOME_BOOKKEEPING_VWAP_MANAGER_H
#define TREXQUANTTAKEHOME_BOOKKEEPING_VWAP_MANAGER_H

#include <cstdint>
#include <unordered_map>
#include <array>
#include <cmath>
#include <fmt/os.h>
#include <fmt/format.h>

#include <BrokenTradeOrOrderExecution.h>
#include <CrossTrade.h>
#include <OrderExecuted.h>
#include <OrderExecutedWithPrice.h>
#include <StockTradingAction.h>
#include <TradeNonCross.h>

#include <time_utils.h>

const uint32_t TOP_OF_10_PERCENT_BAND = 250000;
const uint32_t TOP_OF_5_PERCENT_BAND  = 500000;

/**
 * This struct contains data for VWAP calculation and derives it.
 * Doesn't feel class-worthy to me for some reason.
 */
struct VWAPFormula {

    uint32_t high;
    uint32_t low;
    uint32_t close;
    uint64_t closingTimestamp;
    uint64_t volume;
    uint32_t lastSalePrice; // Use for evaluating possible erroneous trade

    uint32_t getTypicalPrice() { return std::round((high + low + close) / 3); }
    uint64_t getPV() { return getTypicalPrice() * volume; }

    explicit VWAPFormula():
        high(0),
        low(UINT32_MAX),
        close(0),
        closingTimestamp(0),
        volume(0),
        lastSalePrice(UINT32_MAX) 
    {};
};

/**
 * This struct contains the information we need to adjust VWAP after accounting for broken trades
 */
struct BrokenTradeCandidate {
    uint32_t executionPrice;
    uint64_t executionTimestamp;
    uint64_t executionVolume;
    uint16_t stockLocate;
};

/**
 * This class manages VWAP calculation for each issue
 */
class VWAPManager {
    public:

    void printNumBadTrades() {
        fmt::println("{}",_brokenTradeCandidates.size());
    }

    static VWAPManager& getInstance();

    // Highest level function call for dumping periodic VWAP to a file
    void outputBrokenTradeAdjustedVWAP();

    void handleBrokenTrade(uint16_t stockLocate, BrokenTradeOrOrderExecution* brokenTradeOrOrderExecution);
    void handleCrossTrade(uint64_t timestamp, uint16_t stockLocate, CrossTrade* crossTrade);
    void handleOrderExecuted(uint64_t timestamp, uint16_t stockLocate, OrderExecuted* orderExecuted);
    void handleOrderExecutedWithPrice(uint64_t timestamp, uint16_t stockLocate, OrderExecutedWithPrice* orderExecutedWithPrice);
    void handleStockTradingAction(uint16_t stockLocate, StockTradingAction* stockTradingAction);
    void handleTrade(uint64_t timestamp, uint16_t stockLocate, TradeNonCross* tradeNonCross);

    private:

    static VWAPManager* _instance;

    // Contains a mapping for stockLocate -> stock symbol
    std::unordered_map<uint16_t, std::string> _stockLocateToStockSymbol;

    // Track the VWAP for each issue throughout the day
    std::unordered_map<uint16_t, std::array<VWAPFormula, NUMBER_OF_PERIODS_PER_DAY>> _periodicVWAPStatsPerIssue;

    /**
     * Keep a mapping of possible broken trades based on the NASDAQ guidelines.
     * At the end of System Hours, this mapping will be "merged" with _periodicVWAPStatsPerIssue to get adjusted VWAP
     */
    std::unordered_map<uint64_t, BrokenTradeCandidate> _brokenTradeCandidates;

    // Merge the two mappings above
    void _mergeRemainingBrokenTradeCandidatesIntoVWAPStatsPerPeriod();

    void _handleOrderOrTradeExecuted(uint16_t stockLocate, uint32_t price, uint64_t volume, uint64_t timestamp, uint64_t matchNumber);
    bool _flagOrderOrTradeAsErroneousCandidate(uint32_t price, uint32_t lastPrice);
    std::array<fmt::ostream, NUMBER_OF_PERIODS_PER_DAY> _getFileDescriptors();

    double _getPriceFromInt(uint32_t price);
};

#endif // TREXQUANTTAKEHOME_BOOKKEEPING_VWAP_MANAGER_H