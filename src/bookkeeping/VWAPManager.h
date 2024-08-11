#ifndef TREXQUANTTAKEHOME_BOOKKEEPING_VWAP_MANAGER_H
#define TREXQUANTTAKEHOME_BOOKKEEPING_VWAP_MANAGER_H

#include <cstdint>
#include <unordered_map>
#include <array>
#include <fmt/os.h>

#include <BrokenTradeOrOrderExecution.h>
#include <CrossTrade.h>
#include <OrderExecuted.h>
#include <OrderExecutedWithPrice.h>
#include <StockTradingAction.h>
#include <TradeNonCross.h>
#include <time_utils.h>

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

    uint32_t getTypicalPrice() { return (high + low + close) / 3; }
    uint64_t getPV() { return getTypicalPrice() * volume; }

    explicit VWAPFormula():
        high(0),
        low(UINT32_MAX),
        close(0),
        closingTimestamp(0),
        volume(0) 
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

    static VWAPManager& getInstance();

    // Highest level function call for dumping periodic VWAP to a file
    void outputBrokenTradeAdjustedVWAP();

    void handleBrokenTrade();
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

    // Driver function for executing order and order with price messages
    void _handleOrderExecuted();

    // Merge the two mappings above
    void _mergeRemainingBrokenTradeCandidatesIntoVWAPStatsPerPeriod();
    std::array<fmt::ostream&, NUMBER_OF_PERIODS_PER_DAY> _getFileDescriptors();
};

#endif // TREXQUANTTAKEHOME_BOOKKEEPING_VWAP_MANAGER_H