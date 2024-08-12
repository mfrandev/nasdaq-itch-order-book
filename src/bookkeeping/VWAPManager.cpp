#include <VWAPManager.h>

#include <cassert>

#include <OrderBook.h>
#include <ProcessMessage.h>

extern uint8_t currentPeriod;
extern uint64_t marketClosedTimestamp;

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
    std::array<fmt::ostream, NUMBER_OF_PERIODS_PER_DAY> fileDecriptors = _getFileDescriptors();

    // Perform the broken trade VWAP adjustment
    _mergeRemainingBrokenTradeCandidatesIntoVWAPStatsPerPeriod();
    for(auto& periodicVWAPstatsForOneIssue: _periodicVWAPStatsPerIssue) {
        std::string& currentStockSymbol = _stockLocateToStockSymbol[periodicVWAPstatsForOneIssue.first];

        // Use these two values to calculate VWAP over time
        uint64_t cumulativePVForOneIssue = 0;
        uint64_t cumulativeVolumeForOneIssue = 0;
        // Get cumulative VWAP for this issue
        for(int i = 0; i < NUMBER_OF_PERIODS_PER_DAY; i++) {
            VWAPFormula statsForOnePeriod = periodicVWAPstatsForOneIssue.second[i];
            cumulativePVForOneIssue += statsForOnePeriod.getPV();
            cumulativeVolumeForOneIssue += statsForOnePeriod.volume;
            if(cumulativeVolumeForOneIssue == 0) continue; // VWAP undefined if no volume
            uint64_t vwapThisPeriod = cumulativePVForOneIssue / cumulativeVolumeForOneIssue;
            // This line actually commits the stock symbol and VWAP to the file
            fileDecriptors[i].print("{}: {}\n", currentStockSymbol, _getPriceFromInt(vwapThisPeriod));
        }
    }
}

/**
 * Remove the broken trade from the _brokenTradeCandidate mapping
 */
void VWAPManager::handleBrokenTrade(uint16_t stockLocate, BrokenTradeOrOrderExecution* brokenTradeOrOrderExecution) {
    // If the heuristic didn't catch it, nothing we can do
    if(!_brokenTradeCandidates.count(brokenTradeOrOrderExecution -> matchNumber)) return;
    if(_brokenTradeCandidates[brokenTradeOrOrderExecution -> matchNumber].executionTimestamp > marketClosedTimestamp) return;
    _brokenTradeCandidates.erase(brokenTradeOrOrderExecution -> matchNumber);
}

/**
 * Handle cross trade message
 */
void VWAPManager::handleCrossTrade(uint64_t timestamp, uint16_t stockLocate, CrossTrade* crossTrade) {
    if(crossTrade -> shares == 0) return;
    _handleOrderOrTradeExecuted(stockLocate, crossTrade -> crossPrice, crossTrade -> shares, timestamp, crossTrade -> matchNumber);
}

/**
 * Handle order execute message
 */
void VWAPManager::handleOrderExecuted(uint64_t timestamp, uint16_t stockLocate, OrderExecuted* orderExecuted) {
    uint32_t price = OrderBook::getInstance().executeActiveOrder(orderExecuted -> orderReferenceNumber, orderExecuted -> executedShares, orderExecuted -> matchNumber);
    _handleOrderOrTradeExecuted(stockLocate, price, orderExecuted -> executedShares, timestamp, orderExecuted -> matchNumber);
}

/**
 * Hande order executed with price message
 */
void VWAPManager::handleOrderExecutedWithPrice(uint64_t timestamp, uint16_t stockLocate, OrderExecutedWithPrice* orderExecutedWithPrice) {
    if(orderExecutedWithPrice -> printable == IGNORE_ORDER_EXECUTED_WITH_PRICE_MESSAGE) return;
    OrderBook::getInstance().executeActiveOrderWithPrice(orderExecutedWithPrice -> orderReferenceNumber, orderExecutedWithPrice -> executedShares, orderExecutedWithPrice -> matchNumber, orderExecutedWithPrice -> executionPrice);
    _handleOrderOrTradeExecuted(stockLocate, orderExecutedWithPrice -> executionPrice, orderExecutedWithPrice -> executedShares, timestamp, orderExecutedWithPrice -> matchNumber);
}

/**
 * 
 */
void VWAPManager::handleStockTradingAction(uint16_t stockLocate, StockTradingAction* stockTradingAction) {
    if(!_stockLocateToStockSymbol.count(stockLocate))
        _stockLocateToStockSymbol[stockLocate] = stockTradingAction -> stock;
    if(stockTradingAction -> tradingState != STOCK_IS_TRADING) return;
    if(_periodicVWAPStatsPerIssue.count(stockLocate)) return;

    //Create the default vwap and locate-to-symbol mappings for this issue
    VWAPFormula defaultFormula;
    std::array<VWAPFormula, NUMBER_OF_PERIODS_PER_DAY> periodicData;
    periodicData.fill(defaultFormula);
    _periodicVWAPStatsPerIssue[stockLocate] = periodicData;
}

/**
 * Handle a normal trade message
 */
void VWAPManager::handleTrade(uint64_t timestamp, uint16_t stockLocate, TradeNonCross* tradeNonCross) {
    _handleOrderOrTradeExecuted(stockLocate, tradeNonCross -> price, tradeNonCross -> shares, timestamp, tradeNonCross -> matchNumber);
}

// ======================== Private Functions ========================

/**
 * Handle all orders and trades
 */
void VWAPManager::_handleOrderOrTradeExecuted(uint16_t stockLocate, uint32_t price, uint64_t volume, uint64_t timestamp, uint64_t matchNumber) {
    // Check if this order should flagged for erroneous trade
    // assert(_periodicVWAPStatsPerIssue.count(stockLocate));
    if(!_periodicVWAPStatsPerIssue.count(stockLocate)) return;    
    VWAPFormula* data = &(_periodicVWAPStatsPerIssue[stockLocate][currentPeriod]);
    if(data -> lastSalePrice != UINT32_MAX && _flagOrderOrTradeAsErroneousCandidate(price, data -> lastSalePrice)) {
        // If not erroneous, this will eventually get merged into the trade statistics 
        _brokenTradeCandidates[matchNumber] = {price, timestamp, volume, stockLocate};
    } else {
        if(price > data -> high)
            data -> high = price;
        else if(price < data -> low)
            data -> low = price;
        data -> volume += volume;
        data -> close = price;
        data -> closingTimestamp = timestamp;
    }
    data -> lastSalePrice = price;
}

/**
 * "Nasdaq does not normally break trades that are between the Reference Price and up to but not including the Numerical Threshold."
 * https://www.nasdaqtrader.com/Trader.aspx?id=ClearlyErroneous#:~:text=The%20terms%20of%20a%20transaction,or%20identification%20of%20the%20security 
 * Using their values as a heuristic to 
 */
bool VWAPManager::_flagOrderOrTradeAsErroneousCandidate(uint32_t price, uint32_t lastPrice) {
    double lowerBound = 0;
    double upperBound = 0;

    // Nasdaq advises these are the cutoff points
    if(price < TOP_OF_10_PERCENT_BAND) {
        lowerBound = 0.9;
        upperBound = 1.1;
    } else if(price < TOP_OF_5_PERCENT_BAND) {
        lowerBound = 0.95;
        upperBound = 1.05;
    } else {
        lowerBound = 0.97;
        upperBound = 1.03;
    }
    uint32_t floor = lastPrice * lowerBound;
    uint32_t ceiling = lastPrice * upperBound;
    return price <= floor || price >= ceiling;
}

/**
 * At the end of system hours, any non-broken trades should be accounted for
 */
void VWAPManager::_mergeRemainingBrokenTradeCandidatesIntoVWAPStatsPerPeriod() {
    for(auto& mergeCandidate: _brokenTradeCandidates) {
        uint8_t period = getCurrentPeriodFromTimestamp(mergeCandidate.second.executionTimestamp);
        // assert(_periodicVWAPStatsPerIssue.count(mergeCandidate.second.stockLocate));
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

std::array<fmt::ostream, NUMBER_OF_PERIODS_PER_DAY> VWAPManager::_getFileDescriptors() {
    std::array<fmt::ostream, NUMBER_OF_PERIODS_PER_DAY> fileDescriptors = {
        fmt::output_file("./../LiveOutput/vwap_1.txt"),
        fmt::output_file("./../LiveOutput/vwap_2.txt"),
        fmt::output_file("./../LiveOutput/vwap_3.txt"),
        fmt::output_file("./../LiveOutput/vwap_4.txt"),
        fmt::output_file("./../LiveOutput/vwap_5.txt"),
        fmt::output_file("./../LiveOutput/vwap_6.txt"),
        fmt::output_file("./../LiveOutput/vwap_7.txt")
    };

    return fileDescriptors;
}


double VWAPManager::_getPriceFromInt(uint32_t price) {
    return static_cast<double>(static_cast<double>(price) / 10000);
}
