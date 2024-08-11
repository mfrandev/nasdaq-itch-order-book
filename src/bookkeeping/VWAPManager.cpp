#include <VWAPManager.h>

#include <cassert>

#include <OrderBook.h>

// Singleton declaration and getter
VWAPManager* VWAPManager::_instance = nullptr;
VWAPManager& VWAPManager::getInstance() {
    if(_instance == nullptr) 
        _instance = new VWAPManager();
    return *_instance;
}

const char IGNORE_ORDER_EXECUTED_WITH_PRICE_MESSAGE = 'N'; // Non-printable and we should ignore 
const char STOCK_IS_TRADING                         = 'T';

/**
 * Output the broken trade adjusted periodic VWAP to a set of files
 */
void VWAPManager::outputBrokenTradeAdjustedVWAP() {

    // Get our output objects. Cursory Google search implies this is RAII, so no need to close explcitly
    // std::array<fmt::ostream&, NUMBER_OF_PERIODS_PER_DAY> fileDecriptors = _getFileDescriptors();

    // Perform the broken trade VWAP adjustment
    _mergeRemainingBrokenTradeCandidatesIntoVWAPStatsPerPeriod();
    for(auto& periodicVWAPstatsForOneIssue: _periodicVWAPStatsPerIssue) {
        // std::string& currentStockSymbol = _stockLocateToStockSymbol[periodicVWAPstatsForOneIssue.first];

        // Use these two values to calculate VWAP over time
        uint32_t cumulativePVForOneIssue = 0;
        uint64_t cumulativeVolumeForOneIssue = 0;

        // Get cumulative VWAP for this issue
        for(int i = 0; i < NUMBER_OF_PERIODS_PER_DAY; i++) {
            VWAPFormula statsForOnePeriod = periodicVWAPstatsForOneIssue.second[i];
            cumulativePVForOneIssue += statsForOnePeriod.getPV();
            cumulativeVolumeForOneIssue += statsForOnePeriod.volume;
            uint64_t vwapThisPeriod = cumulativePVForOneIssue / cumulativeVolumeForOneIssue;
            
            // This line actually commits the stock symbol and VWAP to the file
            // fileDecriptors[i].print("{}, {}", currentStockSymbol, vwapThisPeriod);
        }
    }
}

/**
 * 
 */
void VWAPManager::handleBrokenTrade() {

}

/**
 * 
 */
void VWAPManager::handleCrossTrade(uint64_t timestamp, uint16_t stockLocate, CrossTrade* crossTrade) {

}

/**
 * 
 */
void VWAPManager::handleOrderExecuted(uint64_t timestamp, uint16_t stockLocate, OrderExecuted* orderExecuted) {
    uint32_t price = OrderBook::getInstance().executeActiveOrder(orderExecuted -> orderReferenceNumber, orderExecuted -> executedShares, orderExecuted -> matchNumber);
}

/**
 * 
 */
void VWAPManager::handleOrderExecutedWithPrice(uint64_t timestamp, uint16_t stockLocate, OrderExecutedWithPrice* orderExecutedWithPrice) {
    if(orderExecutedWithPrice -> printable == IGNORE_ORDER_EXECUTED_WITH_PRICE_MESSAGE) return;
    OrderBook::getInstance().executeActiveOrderWithPrice(orderExecutedWithPrice -> orderReferenceNumber, orderExecutedWithPrice -> executedShares, orderExecutedWithPrice -> matchNumber, orderExecutedWithPrice -> executionPrice);
}

/**
 * 
 */
void VWAPManager::handleStockTradingAction(uint16_t stockLocate, StockTradingAction* stockTradingAction) {
    if(stockTradingAction -> tradingState != STOCK_IS_TRADING) return;
    if(_periodicVWAPStatsPerIssue.count(stockLocate)) return;

    //Create the default vwap and locate-to-symbol mappings for this issue
    VWAPFormula defaultFormula;
    std::array<VWAPFormula, NUMBER_OF_PERIODS_PER_DAY> periodicData;
    periodicData.fill(defaultFormula);
    _periodicVWAPStatsPerIssue[stockLocate] = periodicData;
    _stockLocateToStockSymbol[stockLocate] = stockTradingAction -> stock;
}

/**
 * 
 */
void VWAPManager::handleTrade(uint64_t timestamp, uint16_t stockLocate, TradeNonCross* tradeNonCross) {

}

// ======================== Private Functions ========================

/**
 * At the end of system hours, any entries remaining in the 
 */
void VWAPManager::_mergeRemainingBrokenTradeCandidatesIntoVWAPStatsPerPeriod() {
    for(auto& mergeCandidate: _brokenTradeCandidates) {
        uint8_t period = getCurrentPeriodFromTimestamp(mergeCandidate.second.executionTimestamp);
        assert(_periodicVWAPStatsPerIssue.count(mergeCandidate.second.stockLocate));
        VWAPFormula dataToUpdate = _periodicVWAPStatsPerIssue[mergeCandidate.second.stockLocate][period];

        // Perform the merge
        if(mergeCandidate.second.executionPrice < dataToUpdate.low)
            dataToUpdate.low = mergeCandidate.second.executionPrice;
        else if(mergeCandidate.second.executionPrice > dataToUpdate.high)
            dataToUpdate.high = mergeCandidate.second.executionPrice;
        dataToUpdate.volume += mergeCandidate.second.executionVolume;
        // No need to worry about the closing stats if the merge candidate is not later in the period than the currently tracked last trade
        if(mergeCandidate.second.executionTimestamp < dataToUpdate.closingTimestamp) 
            continue;
        dataToUpdate.closingTimestamp = mergeCandidate.second.executionTimestamp;
        dataToUpdate.close = mergeCandidate.second.executionPrice;
    }

    // Free the memory from _brokenTradeCandidates after the merge is complete
    std::unordered_map<uint64_t, BrokenTradeCandidate> temp;
    temp.swap(_brokenTradeCandidates);
}   
